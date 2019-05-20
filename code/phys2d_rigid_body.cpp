/*@H
* File: phys2d_rigid_body.cpp
* Author: Jesse Calvert
* Created: ember 6, 2017
* Last modified: November 27, 2017, 14:47
*/

internal rectangle2
GetWorldAABB(rigid_body *Body, v2 Margin = V2(0.0f, 0.0f))
{
	rectangle2 Result = {};
	shape *Shape = &Body->Shape;
	switch(Shape->Type)
	{
		case Shape_Circle:
		{
			circle *Circle = &Shape->Circle;
			v2 HalfDim = V2(Circle->Radius, Circle->Radius);
			v2 Center = LocalPointToWorldPoint(Body, Circle->Center);

			Result = Rectangle2(Center - HalfDim, Center + HalfDim);
		} break;

		case Shape_ConvexPolygon:
		{
			v2 Min = V2(Real32Maximum, Real32Maximum);
			v2 Max = V2(Real32Minimum, Real32Minimum);
			for(u32 VertexIndex = 0;
				VertexIndex < Shape->Polygon.VertexCount;
				++VertexIndex)
			{
				v2 Vertex = Shape->Polygon.Vertices[VertexIndex];
				Vertex = LocalPointToWorldPoint(Body, Vertex);
				if(Vertex.x < Min.x)
				{
					Min.x = Vertex.x;
				}
				if(Vertex.y < Min.y)
				{
					Min.y = Vertex.y;
				}
				if(Vertex.x > Max.x)
				{
					Max.x = Vertex.x;
				}
				if(Vertex.y > Max.y)
				{
					Max.y = Vertex.y;
				}
			}

			Result = Rectangle2(Min, Max);
		} break;

		InvalidDefaultCase;
	}

	Result = Rectangle2(Result.Min - Margin, Result.Max + Margin);
	return Result;
}

internal void
AddPotentialCollision(physics_state *State, u32 IndexA, u32 IndexB)
{
	index_pair Pair = {IndexA, IndexB};

	u32 Collideable = true;
	for(u32 UncollideableIndex = 0;
		Collideable && (UncollideableIndex < State->UncollideableCount);
		++UncollideableIndex)
	{
		index_pair *Uncollideable = State->Uncollideables + UncollideableIndex;
		if(*Uncollideable == Pair)
		{
			Collideable = false;
		}
	}

	if(Collideable)
	{
		bucket *CurrentBucket = 0;
		if(State->BucketCount > 0)
		{
			CurrentBucket = State->PotentialCollisionBuckets + (State->BucketCount - 1);
		}

		if(!CurrentBucket || CurrentBucket->PotentialCollisionCount == BUCKET_SIZE)
		{
			Assert(State->BucketCount < ArrayCount(State->PotentialCollisionBuckets));
			CurrentBucket = State->PotentialCollisionBuckets + State->BucketCount++;
			CurrentBucket->PotentialCollisions = PushArray(&State->TempRegion, BUCKET_SIZE, index_pair);
			CurrentBucket->PotentialCollisionCount = 0;
		}

		CurrentBucket->PotentialCollisions[CurrentBucket->PotentialCollisionCount++] = Pair;
	}
}

inline v2
Support(shape *Shape, v2 Direction)
{
	v2 Result = {};

	switch(Shape->Type)
	{
		case Shape_Circle:
		{
			circle *Circle = &Shape->Circle;
			v2 DirectionNormal = Normalize(Direction);
			Result = Circle->Center + Circle->Radius*DirectionNormal;
		} break;

		case Shape_ConvexPolygon:
		{
			f32 LargestInnerProduct = Real32Minimum;
			for(u32 VertexIndex = 0;
				VertexIndex < Shape->Polygon.VertexCount;
				++VertexIndex)
			{
				v2 Vertex = Shape->Polygon.Vertices[VertexIndex];
				f32 InnerProduct = Inner(Vertex, Direction);
				if(InnerProduct > LargestInnerProduct)
				{
					LargestInnerProduct = InnerProduct;
					Result = Vertex;
				}
			}
		} break;

		InvalidDefaultCase;
	}

	return Result;
}

internal minkowski_difference_support
MinkowskiDifferenceSupport(rigid_body *BodyA, rigid_body *BodyB, v2 Direction)
{
	minkowski_difference_support Result = {};

	shape *ShapeA = &BodyA->Shape;
	shape *ShapeB = &BodyB->Shape;

	v2 LocalDirectionA = WorldVectorToLocalVector(BodyA, Direction);
	v2 LocalDirectionB = WorldVectorToLocalVector(BodyB, -Direction);

	v2 LocalSupportA = Support(ShapeA, LocalDirectionA);
	v2 LocalSupportB = Support(ShapeB, LocalDirectionB);

	Result.SupportA = LocalPointToWorldPoint(BodyA, LocalSupportA);
	Result.SupportB = LocalPointToWorldPoint(BodyB, LocalSupportB);
	Result.Support = Result.SupportA - Result.SupportB;

	return Result;
}

internal b32
SimplexIsNotDegenerate(minkowski_simplex *Simplex)
{
	b32 Result = true;
	for(u32 IndexA = 0;
		Result && (IndexA < Simplex->Size);
		++IndexA)
	{
		for(u32 IndexB = IndexA + 1;
			Result && (IndexB < Simplex->Size);
			++IndexB)
		{
			v2 A = Simplex->Points[IndexA].Support;
			v2 B = Simplex->Points[IndexB].Support;
			if(A == B)
			{
				Result = false;
			}
		}
	}

	return Result;
}

internal v2
ReduceSimplexAndFindDirectionToOrigin(minkowski_simplex *Simplex)
{
	Assert(SimplexIsNotDegenerate(Simplex));

	// NOTE: the last point in the simplex is assumed to be the one
	// added in the direction of the origin.
	v2 Result = {};

	switch(Simplex->Size)
	{
		case 1:
		{
			Result = -Simplex->Points[0].Support;
			if(Result == V2(0.0f, 0.0f))
			{
				Simplex->ContainsOrigin = true;
			}
		} break;

		case 2:
		{
			v2 A = Simplex->Points[0].Support;
			v2 B = Simplex->Points[1].Support;

			v2 AB = B - A;
			v2 BO = -B;
			if(SameDirection(BO, AB))
			{
				// NOTE: [B]
				Simplex->Size = 1;
				Simplex->Points[0] = Simplex->Points[1];
				Result = BO;
			}
			else
			{
				v2 PerpAB = Perp(AB);
				f32 InnerProduct = Inner(BO, PerpAB);
				if(InnerProduct == 0.0f)
				{
					// NOTE: Origin in on line
					Simplex->ContainsOrigin = true;
				}
				else if(InnerProduct > 0.0f)
				{
					// NOTE: [A, B]
					Result = PerpAB;
				}
				else
				{
					// NOTE: [B, A]
					minkowski_difference_support Temp = Simplex->Points[0];
					Simplex->Points[0] = Simplex->Points[1];
					Simplex->Points[1] = Temp;
					Result = -PerpAB;
				}
			}
		} break;

		case 3:
		{
			v2 A = Simplex->Points[0].Support;
			v2 B = Simplex->Points[1].Support;
			v2 C = Simplex->Points[2].Support;

			v2 CO = -C;
			v2 BC = C - B;
			v2 AC = C - A;
			v2 PerpCB = Perp(-BC);
			v2 PerpAC = Perp(AC);

			if(SameDirection(CO, PerpCB))
			{
				if(SameDirection(CO, -BC))
				{
					// NOTE: [C, B]
					Simplex->Size = 2;
					Simplex->Points[0] = Simplex->Points[2];
					Result = PerpCB;
				}
				else if(SameDirection(CO, -AC))
				{
					// NOTE: [A, C]
					Simplex->Size = 2;
					Simplex->Points[1] = Simplex->Points[2];
					Result = PerpAC;
				}
				else
				{
					// NOTE: [C]
					Simplex->Size = 1;
					Simplex->Points[0] = Simplex->Points[2];
					Result = CO;
				}
			}
			else if(SameDirection(CO, PerpAC))
			{
				if(SameDirection(CO, -AC))
				{
					// NOTE: [A, C]
					Simplex->Size = 2;
					Simplex->Points[1] = Simplex->Points[2];
					Result = PerpAC;
				}
				else
				{
					// NOTE: [C]
					Simplex->Size = 1;
					Simplex->Points[0] = Simplex->Points[2];
					Result = CO;
				}
			}
			else
			{
				// NOTE: Intersection
				Simplex->ContainsOrigin = true;
			}
		} break;

		InvalidDefaultCase;
	}

	return Result;
}

internal minkowski_difference_support
SimplexGetClosestPoint(minkowski_simplex *Simplex, v2 Point)
{
	minkowski_difference_support Result = {};

	switch(Simplex->Size)
	{
		case 1:
		{
			Result = Simplex->Points[0];
		} break;

		case 2:
		{
			f32 t = SegmentClosestPointBarycentric(Point, Simplex->Points[0].Support, Simplex->Points[1].Support);
			Result = Lerp(Simplex->Points[0], t, Simplex->Points[1]);
		} break;

		InvalidDefaultCase;
	}

	return Result;
}

internal minkowski_simplex
GJK(rigid_body *BodyA, rigid_body *BodyB)
{
	TIMED_FUNCTION();

	minkowski_simplex Simplex = {};
	v2 InitialDirection = BodyB->WorldCentroid - BodyA->WorldCentroid;
	if(LengthSq(InitialDirection) == 0.0f)
	{
		InitialDirection = V2(1.0f, 0.0f);
	}
	minkowski_difference_support MinkowskiSupport = MinkowskiDifferenceSupport(BodyA, BodyB, InitialDirection);
	Simplex.Points[Simplex.Size++] = MinkowskiSupport;

	while(1)
	{
		v2 Direction = ReduceSimplexAndFindDirectionToOrigin(&Simplex);
		Assert(SimplexIsNotDegenerate(&Simplex));
		if(Simplex.ContainsOrigin)
		{
			// NOTE: Intersection.
			break;
		}

		minkowski_difference_support NewSupport = MinkowskiDifferenceSupport(BodyA, BodyB, Direction);
		f32 InnerProduct = Inner(Direction, NewSupport.Support);
		if(InnerProduct < 0.0f)
		{
			// NOTE: No intersection.
			Simplex.ContainsOrigin = false;
			break;
		}
		else if(InnerProduct == 0.0f)
		{
			if(NewSupport.Support == Simplex.Points[Simplex.Size].Support)
			{
				// NOTE: Point is not new, No intersection.
				Simplex.ContainsOrigin = false;
				break;
			}
		}

		Assert(Simplex.Size < ArrayCount(Simplex.Points));
		Simplex.Points[Simplex.Size++] = NewSupport;
	}

	return Simplex;
}

internal b32
RigidBodyContainsPoint(rigid_body *Body, v2 Point)
{
	rigid_body Dummy = {};
	Dummy.Shape.Type = Shape_Circle;
	Dummy.P = Dummy.WorldCentroid = Point;
	Dummy.Rotation = RotationMat2(0.0f);
	UpdateRotation(&Dummy);

	minkowski_simplex GJKResult = GJK(Body, &Dummy);
	return GJKResult.ContainsOrigin;
}

internal contact_data
ExpandingPolygonAlgorithm(memory_region *Region,
	rigid_body *BodyA, rigid_body *BodyB,
	minkowski_simplex *Simplex)
{
	TIMED_FUNCTION();

	contact_data Result = {};
	Assert(SimplexIsNotDegenerate(Simplex));

	switch(Simplex->Size)
	{
		case 1:
		{
			f32 BestDistanceSq = 0.0f;
			minkowski_difference_support BestSupport = {};
			v2 SearchDirections[] =
			{
				{1.0f, 0.0f},
				{0.0f, 1.0f},
				{-1.0f, 0.0f},
				{0.0f, -1.0f},
			};
			for(u32 Index = 0;
				Index < ArrayCount(SearchDirections);
				++Index)
			{
				v2 SearchDirection = SearchDirections[Index];
				minkowski_difference_support Support = MinkowskiDifferenceSupport(BodyA, BodyB, SearchDirection);
				f32 SupportLengthSq = LengthSq(Support.Support - Simplex->Points[0].Support);
				if(SupportLengthSq > BestDistanceSq)
				{
					BestDistanceSq = SupportLengthSq;
					BestSupport = Support;
				}
			}

			Assert(BestDistanceSq > 0.0f);
			Simplex->Points[Simplex->Size++] = BestSupport;
			Assert(SimplexIsNotDegenerate(Simplex));
		} // NOTE: Intentional fallthrough

		case 2:
		{
			v2 A = Simplex->Points[0].Support;
			v2 B = Simplex->Points[1].Support;
			v2 NormalToLine = Perp(B - A);

			minkowski_difference_support Support0 = MinkowskiDifferenceSupport(BodyA, BodyB, NormalToLine);
			minkowski_difference_support Support1 = MinkowskiDifferenceSupport(BodyA, BodyB, -NormalToLine);

			f32 DistanceSqToLine0 = DistanceToLineSq(Support0.Support, A, B);
			f32 DistanceSqToLine1 = DistanceToLineSq(Support1.Support, A, B);

			if(DistanceSqToLine0 > DistanceSqToLine1)
			{
				Assert(DistanceSqToLine0 > 0.0f);
				Simplex->Points[Simplex->Size++] = Support0;
			}
			else
			{
				Assert(DistanceSqToLine1 > 0.0f);
				Simplex->Points[Simplex->Size++] = Simplex->Points[1];
				Simplex->Points[1] = Support1;
			}
			Assert(SimplexIsNotDegenerate(Simplex));
		} // NOTE: Intentional fallthrough

		case 3:
		{
			expanding_polygon Polygon = {};
			DLIST_INIT(&Polygon.Sentinel);

			expanding_polygon_vertex *LastVertex = &Polygon.Sentinel;
			for(u32 Index = 0;
				Index < Simplex->Size;
				++Index)
			{
				minkowski_difference_support Point = Simplex->Points[Index];
				expanding_polygon_vertex *Vertex = PushStruct(Region, expanding_polygon_vertex);
				Vertex->Vertex = Point;
				DLIST_INSERT(LastVertex, Vertex);
				LastVertex = Vertex;
				++Polygon.VertexCount;
			}

			f32 Epsilon = 0.0001f;
			f32 EpsilonSq = Square(0.0001f);

			while(1)
			{
				expanding_polygon_vertex *ClosestLineStart = 0;
				f32 ClosestDistanceSq = Real32Maximum;
				v2 BestNormal = {};
				v2 BestStart = {};
				v2 BestEnd = {};
				for(expanding_polygon_vertex *Vertex = Polygon.Sentinel.Next;
					Vertex != &Polygon.Sentinel;
					Vertex = Vertex->Next)
				{
					minkowski_difference_support LineStart = Vertex->Vertex;
					minkowski_difference_support LineEnd = GetNextVertexData(&Polygon, Vertex);

					v2 Line = LineStart.Support - LineEnd.Support;
					v2 Normal = Perp(Line);
					Assert(Inner(LineStart.Support, Normal) > -Epsilon);

					v2 ClosestPoint = SegmentClosestPoint(V2(0.0f, 0.0f), LineStart.Support, LineEnd.Support);
					f32 DistanceSqToLine = LengthSq(ClosestPoint);
					if(DistanceSqToLine < ClosestDistanceSq)
					{
						ClosestDistanceSq = DistanceSqToLine;
						ClosestLineStart = Vertex;
						BestNormal = Normal;
						BestStart = LineStart.Support;
						BestEnd = LineEnd.Support;
					}
				}

				minkowski_difference_support NewPoint = MinkowskiDifferenceSupport(BodyA, BodyB, BestNormal);
				v2 ClosestPointOnOldLineToNewPoint = SegmentClosestPoint(NewPoint.Support, BestStart, BestEnd);
				f32 NewPointDistanceSq = LengthSq(ClosestPointOnOldLineToNewPoint - NewPoint.Support);
				if(NewPointDistanceSq > EpsilonSq)
				{
					expanding_polygon_vertex *FirstKeptVertex = 0;
					expanding_polygon_vertex *Vertex = GetPrevVertex(&Polygon, ClosestLineStart);
					while(!FirstKeptVertex)
					{
						minkowski_difference_support LineStart = Vertex->Vertex;
						minkowski_difference_support LineEnd = GetNextVertexData(&Polygon, Vertex);
						v2 Line = LineStart.Support - LineEnd.Support;
						v2 Normal = Perp(Line);
						if(Inner(Normal, NewPoint.Support - LineStart.Support) < 0.0f)
						{
							FirstKeptVertex = GetNextVertex(&Polygon, Vertex);
						}
						else
						{
							Vertex = GetPrevVertex(&Polygon, Vertex);
						}
					}

					expanding_polygon_vertex *OnePastLastToRemove = 0;
					Vertex = GetNextVertex(&Polygon, ClosestLineStart);
					while(!OnePastLastToRemove)
					{
						minkowski_difference_support LineStart = Vertex->Vertex;
						minkowski_difference_support LineEnd = GetNextVertexData(&Polygon, Vertex);
						v2 Line = LineStart.Support - LineEnd.Support;
						v2 Normal = Perp(Line);
						if(Inner(Normal, NewPoint.Support - LineStart.Support) < 0.0f)
						{
							OnePastLastToRemove = Vertex;
						}
						else
						{
							Vertex = GetNextVertex(&Polygon, Vertex);
						}
					}

					Assert(OnePastLastToRemove != FirstKeptVertex);

					expanding_polygon_vertex *ToRemove = GetNextVertex(&Polygon, FirstKeptVertex);
					while(ToRemove != OnePastLastToRemove)
					{
						expanding_polygon_vertex *Temp = ToRemove;
						ToRemove = GetNextVertex(&Polygon, Temp);
						DLIST_REMOVE(Temp);
						Temp->Next = Polygon.FirstFree;
						Polygon.FirstFree = Temp;
						--Polygon.VertexCount;
					}

					expanding_polygon_vertex *NewVertex = Polygon.FirstFree;
					if(NewVertex)
					{
						Polygon.FirstFree = NewVertex->Next;
						ZeroStruct(*NewVertex);
					}
					else
					{
						NewVertex = PushStruct(Region, expanding_polygon_vertex);
					}
					NewVertex->Vertex = NewPoint;
					++Polygon.VertexCount;

					DLIST_INSERT(FirstKeptVertex, NewVertex);
				}
				else
				{
					minkowski_difference_support LineStart = ClosestLineStart->Vertex;
					minkowski_difference_support LineEnd = GetNextVertexData(&Polygon, ClosestLineStart);

					r32 t = SegmentClosestPointBarycentric(V2(0.0f, 0.0f), LineStart.Support, LineEnd.Support);
					minkowski_difference_support ClosestMinkowskiPoint = Lerp(LineStart, t, LineEnd);

					Result.WorldContactPointA = ClosestMinkowskiPoint.SupportA;
					Result.WorldContactPointB = ClosestMinkowskiPoint.SupportB;

					Result.LocalContactPointA = WorldPointToLocalPoint(BodyA, Result.WorldContactPointA);
					Result.LocalContactPointB = WorldPointToLocalPoint(BodyB, Result.WorldContactPointB);

					Result.Normal = Normalize(BestNormal);
					v2 AtoB = BodyB->WorldCentroid - BodyA->WorldCentroid;
					if(!SameDirection(AtoB, Result.Normal))
					{
						Result.Normal = -Result.Normal;
					}
					Result.Penetration = Length(ClosestMinkowskiPoint.Support);
					break;
				}
			}
		} break;

		InvalidDefaultCase;
	}

	return Result;
}

inline u32
HashContactManifold(index_pair *Pair)
{
	u32 Result = (13*Pair->IndexA + 83*Pair->IndexB + 237) % ArrayCount(((physics_state*)0)->ContactManifoldHashTable);
	return Result;
}

internal contact_manifold *
PhysicsGetOrCreateContactManifold(physics_state *State, index_pair *Pair)
{
	u32 HashValue = HashContactManifold(Pair);
	contact_manifold *Result = State->ContactManifoldHashTable[HashValue];
	if(Result)
	{
		while(Result && (Result->Pair != *Pair))
		{
			Result = Result->NextInHash;
		}
	}

	if(!Result)
	{
		Result = State->ManifoldFreeList;
		if(Result)
		{
			State->ManifoldFreeList = Result->NextInHash;
			ZeroStruct(*Result);
		}
		else
		{
			Result = PushStruct(&State->Region, contact_manifold);
		}

		Result->Pair = *Pair;
		Result->NextInHash = State->ContactManifoldHashTable[HashValue];
		State->ContactManifoldHashTable[HashValue] = Result;
		DLIST_INSERT(&State->ManifoldSentinel, Result);
	}

	return Result;
}

internal void
RemoveManifold(physics_state *State, contact_manifold *Manifold)
{
	index_pair *Pair = &Manifold->Pair;
	u32 HashValue = HashContactManifold(Pair);
	contact_manifold *ManifoldToRemove = State->ContactManifoldHashTable[HashValue];
	contact_manifold *PreviousInHash = 0;
	if(ManifoldToRemove)
	{
		while(ManifoldToRemove && (ManifoldToRemove->Pair != *Pair))
		{
			PreviousInHash = ManifoldToRemove;
			ManifoldToRemove = ManifoldToRemove->NextInHash;
		}
	}

	Assert(ManifoldToRemove);
	Assert(ManifoldToRemove == Manifold);
	if(PreviousInHash)
	{
		PreviousInHash->NextInHash = ManifoldToRemove->NextInHash;
	}
	else
	{
		State->ContactManifoldHashTable[HashValue] = ManifoldToRemove->NextInHash;
	}

	DLIST_REMOVE(ManifoldToRemove);
	ManifoldToRemove->NextInHash = State->ManifoldFreeList;
	State->ManifoldFreeList = ManifoldToRemove;
}

internal contact_manifold *
ManifoldCheckContacts(physics_state *State, contact_manifold *Manifold)
{
	contact_manifold *NextManifold = Manifold->Next;

	// TODO: How to determine this?
	f32 PersistentThreshold = PERSISTENT_THRESHOLD;
	f32 PersistentThresholdSq = Square(PersistentThreshold);

	rigid_body *BodyA = State->RigidBodies + Manifold->Pair.IndexA;
	rigid_body *BodyB = State->RigidBodies + Manifold->Pair.IndexB;

	for(u32 Index = 0;
		Index < Manifold->ContactCount;
		++Index)
	{
		contact_data *Contact = Manifold->Contacts + Index;

		v2 LocalToWorldA = LocalPointToWorldPoint(BodyA, Contact->LocalContactPointA);
		v2 LocalToWorldB = LocalPointToWorldPoint(BodyB, Contact->LocalContactPointB);

		v2 WorldContactDistance = Contact->WorldContactPointB - Contact->WorldContactPointA;
		v2 LocalChangeA = Contact->WorldContactPointA - LocalToWorldA;
		v2 LocalChangeB = Contact->WorldContactPointB - LocalToWorldB;

		b32 StillPenetrating = !SameDirection(WorldContactDistance, Contact->Normal);
		b32 CloseEnoughA = (LengthSq(LocalChangeA) < PersistentThresholdSq);
		b32 CloseEnoughB = (LengthSq(LocalChangeB) < PersistentThresholdSq);

		if(StillPenetrating && CloseEnoughA && CloseEnoughB)
		{
			Contact->Persistent = true;
			// NOTE: Warm start.
			f32 WarmStartCoeff = 0.75f;
			Contact->NormalImpulseSum *= WarmStartCoeff;
			ApplyImpulse(BodyA, -Contact->NormalImpulseSum*Contact->Normal, Contact->WorldContactPointA);
			ApplyImpulse(BodyB,  Contact->NormalImpulseSum*Contact->Normal, Contact->WorldContactPointB);

			v2 Tangent = Perp(Contact->Normal);
			Contact->TangentImpulseSum *= WarmStartCoeff;
			ApplyImpulse(BodyA, -Contact->TangentImpulseSum*Tangent, Contact->WorldContactPointA);
			ApplyImpulse(BodyB,  Contact->TangentImpulseSum*Tangent, Contact->WorldContactPointB);
		}
		else
		{
			switch(Manifold->ContactCount)
			{
				case 1:
				{
					RemoveManifold(State, Manifold);
				} break;

				case 2:
				{
					if(Index == 1)
					{
						--Manifold->ContactCount;
					}
					else
					{
						--Manifold->ContactCount;
						Manifold->Contacts[0] = Manifold->Contacts[1];
						// TODO: Hack to make the for loop go to the newly copied ContactData.
						--Index;
					}
				} break;

				InvalidDefaultCase;
			}
		}
	}

	return NextManifold;
}

internal b32
ContactDataIsDifferent(contact_data *ContactA, contact_data *ContactB)
{
	// TODO: How to determine this?
	f32 PersistentThreshold = PERSISTENT_THRESHOLD;
	f32 PersistentThresholdSq = Square(PersistentThreshold);

	v2 ChangeInWorldPosA = ContactA->WorldContactPointA - ContactB->WorldContactPointA;
	v2 ChangeInWorldPosB = ContactA->WorldContactPointB - ContactB->WorldContactPointB;

	b32 FarEnoughA = (LengthSq(ChangeInWorldPosA) > PersistentThresholdSq);
	b32 FarEnoughB = (LengthSq(ChangeInWorldPosB) > PersistentThresholdSq);

	return (FarEnoughA && FarEnoughB);
}

internal void
AddContact(contact_manifold *Manifold, contact_data *NewContact)
{
	b32 IsNewContact = true;
	for(u32 Index = 0;
		IsNewContact && (Index < Manifold->ContactCount);
		++Index)
	{
		contact_data *OldContact = Manifold->Contacts + Index;
		if(!ContactDataIsDifferent(NewContact, OldContact))
		{
			IsNewContact = false;
		}
	}

	if(IsNewContact)
	{
		if(Manifold->ContactCount < 2)
		{
			Manifold->Contacts[Manifold->ContactCount++] = *NewContact;
		}
		else
		{
			contact_data *Contacts[3] =
			{
				NewContact,
				Manifold->Contacts + 0,
				Manifold->Contacts + 1,
			};

			f32 MaxPenetration = 0.0f;
			contact_data *FirstContact = 0;
			for(u32 Index = 0;
				Index < ArrayCount(Contacts);
				++Index)
			{
				contact_data *Contact = Contacts[Index];
				if(Contact->Penetration > MaxPenetration)
				{
					MaxPenetration = Contact->Penetration;
					FirstContact = Contact;
				}
			}

			Manifold->Contacts[0] = *FirstContact;

			f32 FurthestDistanceSq = 0.0f;
			contact_data *SecondContact = 0;
			for(u32 Index = 0;
				Index < ArrayCount(Contacts);
				++Index)
			{
				contact_data *Contact = Contacts[Index];
				f32 DistanceSq = LengthSq(Contact->LocalContactPointA - FirstContact->LocalContactPointA);
				if(DistanceSq > FurthestDistanceSq)
				{
					FurthestDistanceSq = DistanceSq;
					SecondContact = Contact;
				}
			}

			Assert(FirstContact != SecondContact);
			Manifold->Contacts[1] = *SecondContact;
		}
	}
}

internal void
SolveJointConstraints(rigid_body *BodyA, rigid_body *BodyB, joint *Joint, f32 dt)
{
	v2 WorldAnchorA = LocalPointToWorldPoint(BodyA, Joint->LocalAnchorA);
	v2 WorldAnchorB = LocalPointToWorldPoint(BodyB, Joint->LocalAnchorB);
	v2 AnchorSeparation = WorldAnchorB - WorldAnchorA;

	r32 AnchorSeparationLengthSq = LengthSq(AnchorSeparation);
	if(AnchorSeparationLengthSq > 0.0f)
	{
		v2 WorldRadiusA = WorldAnchorA - BodyA->WorldCentroid;
		v2 WorldRadiusB = WorldAnchorB - BodyB->WorldCentroid;

		v2 WorldRadiusPerpA = Perp(WorldRadiusA);
		v2 WorldRadiusPerpB = Perp(WorldRadiusB);

		v2 RelativeVelocity = BodyB->dP - BodyA->dP +
			BodyB->AngularVelocity*WorldRadiusPerpB -
			BodyA->AngularVelocity*WorldRadiusPerpA;

		f32 AnchorSeparationLength = SquareRoot(AnchorSeparationLengthSq);
		v2 AnchorSeparationNormal = (1.0f/AnchorSeparationLength)*AnchorSeparation;

		// TODO: How to determine the constants?
		f32 BaumgarteTerm = 0.1f/dt;
		f32 BaumgarteSlop = 0.01f;
		f32 RelativeAnchorVelocity = Inner(RelativeVelocity, AnchorSeparationNormal);
		f32 Bias = BaumgarteTerm*Maximum(AnchorSeparationLength - BaumgarteSlop, 0.0f);

		f32 Constraint = RelativeAnchorVelocity + Bias;
		f32 EffectiveMass = 1.0f/(BodyA->InvMass + BodyB->InvMass +
			BodyA->InvMomentOfInertia*Square(Inner(WorldRadiusPerpA, AnchorSeparationNormal)) +
			BodyB->InvMomentOfInertia*Square(Inner(WorldRadiusPerpB, AnchorSeparationNormal)));

		f32 LangrangianMultiplier = -EffectiveMass*Constraint;

		ApplyImpulse(BodyA, -LangrangianMultiplier*AnchorSeparationNormal, WorldAnchorA);
		ApplyImpulse(BodyB,  LangrangianMultiplier*AnchorSeparationNormal, WorldAnchorB);
	}
}

internal void
SolveContactConstraints(rigid_body *BodyA, rigid_body *BodyB, contact_data *Contact, f32 dt)
{
	if(Contact->Penetration > 0.0f)
	{
#if 0
		v2 WorldRadiusA = LocalVectorToWorldVector(BodyA, Contact->LocalContactPointA - BodyA->LocalCentroid);
		v2 WorldRadiusB = LocalVectorToWorldVector(BodyB, Contact->LocalContactPointB - BodyB->LocalCentroid);
#else
		v2 WorldRadiusA = Contact->WorldContactPointA - BodyA->WorldCentroid;
		v2 WorldRadiusB = Contact->WorldContactPointB - BodyB->WorldCentroid;
#endif

		v2 WorldRadiusPerpA = Perp(WorldRadiusA);
		v2 WorldRadiusPerpB = Perp(WorldRadiusB);

		v2 RelativeVelocity = BodyB->dP - BodyA->dP +
			BodyB->AngularVelocity*WorldRadiusPerpB -
			BodyA->AngularVelocity*WorldRadiusPerpA;

		// TODO: How to determine the constants?
		f32 RestitutionCoeff = 0.5f;
		f32 RestitutionSlop = 0.5f;
		f32 BaumgarteTerm = -0.1f/dt;
		f32 BaumgarteSlop = 0.01f;
		f32 RelativeVelocityNormal = Inner(RelativeVelocity, Contact->Normal);
		f32 ContactBias = BaumgarteTerm*Maximum(Contact->Penetration - BaumgarteSlop, 0.0f) +
			RestitutionCoeff*Maximum(RelativeVelocityNormal - RestitutionSlop, 0.0f);

		f32 ContactConstraint = RelativeVelocityNormal + ContactBias;
		f32 EffectiveMassNormal = 1.0f/(BodyA->InvMass + BodyB->InvMass +
			BodyA->InvMomentOfInertia*Square(Inner(WorldRadiusPerpA, Contact->Normal)) +
			BodyB->InvMomentOfInertia*Square(Inner(WorldRadiusPerpB, Contact->Normal)));

		f32 NormalLangrangianMultiplier = -EffectiveMassNormal*ContactConstraint;
		f32 PrevNormalImpulse = Contact->NormalImpulseSum;
		Contact->NormalImpulseSum = Maximum(Contact->NormalImpulseSum + NormalLangrangianMultiplier, 0.0f);
		NormalLangrangianMultiplier = (Contact->NormalImpulseSum - PrevNormalImpulse);

		ApplyImpulse(BodyA, -NormalLangrangianMultiplier*Contact->Normal, Contact->WorldContactPointA);
		ApplyImpulse(BodyB,  NormalLangrangianMultiplier*Contact->Normal, Contact->WorldContactPointB);

		// TODO: How to determine the constants?
		f32 FrictionCoeff = 1.5f;
		v2 Tangent = Perp(Contact->Normal);
		f32 RelativeVelocityTangent = Inner(RelativeVelocity, Tangent);
		f32 FrictionBias = 0.0f;

		f32 FrictionConstraint = RelativeVelocityTangent + FrictionBias;
		f32 EffectiveMassTangent = 1.0f/(BodyA->InvMass + BodyB->InvMass +
			BodyA->InvMomentOfInertia*Square(Inner(WorldRadiusPerpA, Tangent)) +
			BodyB->InvMomentOfInertia*Square(Inner(WorldRadiusPerpB, Tangent)));

		f32 TangentLangrangianMultiplier = -EffectiveMassTangent*FrictionConstraint;
		f32 MaximumFrictionImpulse = FrictionCoeff*Contact->NormalImpulseSum;
		f32 PrevTangentImpulse = Contact->TangentImpulseSum;
		Contact->TangentImpulseSum = Clamp(Contact->TangentImpulseSum + TangentLangrangianMultiplier,
			-MaximumFrictionImpulse, MaximumFrictionImpulse);
		TangentLangrangianMultiplier = (Contact->TangentImpulseSum - PrevTangentImpulse);

		ApplyImpulse(BodyA, -TangentLangrangianMultiplier*Tangent, Contact->WorldContactPointA);
		ApplyImpulse(BodyB,  TangentLangrangianMultiplier*Tangent, Contact->WorldContactPointB);
	}
}

internal aabb_tree_node *
AABBCreateNewNode(physics_state *State)
{
	aabb_tree_node *Result = State->AABBNodeFreeList;
	if(Result)
	{
		State->AABBNodeFreeList = Result->NextInFreeList;
		ZeroStruct(*Result);
	}
	else
	{
		Result = PushStruct(&State->Region, aabb_tree_node);
	}
	return Result;
}

internal aabb_tree_node *
AABBChooseBestBranch(physics_state *State, aabb_tree_node *Branch,
	aabb_tree_node *NewLeaf)
{
	aabb_tree_node *Result = 0;
	f32 LeftCost = Perimeter(Combine(NewLeaf->AABB, Branch->Left->AABB));
	f32 RightCost = Perimeter(Combine(NewLeaf->AABB, Branch->Right->AABB));
	if(LeftCost < RightCost)
	{
		Result = Branch->Left;
	}
	else
	{
		Result = Branch->Right;
	}

	return Result;
}

internal void
AABBNodeRotate(physics_state *State, aabb_tree_node *Node, b32 RotateLeft)
{
	TIMED_FUNCTION();
	aabb_tree_node *NewParent = RotateLeft ? Node->Right : Node->Left;
	b32 NewChildLeft = NewParent->Left->Height < NewParent->Right->Height;
	aabb_tree_node *NewChild = NewChildLeft ? NewParent->Left : NewParent->Right;
	aabb_tree_node *GrandParent = Node->Parent;

	if(GrandParent)
	{
		if(GrandParent->Left == Node)
		{
			GrandParent->Left = NewParent;
		}
		else
		{
			GrandParent->Right = NewParent;
		}
	}
	else
	{
		State->AABBRoot = NewParent;
	}

	NewParent->Parent = GrandParent;
	if(NewChildLeft)
	{
		NewParent->Left = Node;
	}
	else
	{
		NewParent->Right = Node;
	}
	if(RotateLeft)
	{
		Node->Right = NewChild;
	}
	else
	{
		Node->Left = NewChild;
	}
	Node->Parent = NewParent;
	NewChild->Parent = Node;
}

internal void
AABBSyncHierarchy(physics_state *State, aabb_tree_node *Node)
{
	TIMED_FUNCTION();
	while(Node)
	{
		Assert(!NodeIsLeaf(Node));
		if(Node->Left->Height > (Node->Right->Height + 1))
		{
			AABBNodeRotate(State, Node, false);
		}
		else if(Node->Right->Height > (Node->Left->Height + 1))
		{
			AABBNodeRotate(State, Node, true);
		}

		Node->AABB = Combine(Node->Left->AABB, Node->Right->AABB);
		Node->Height = 1 + Maximum(Node->Left->Height, Node->Right->Height);
		Node = Node->Parent;
	}
}

internal aabb_tree_node *
AABBInsert(physics_state *State, rigid_body *Body)
{
	TIMED_FUNCTION();
	rectangle2 AABB = GetWorldAABB(Body, V2(0.1f, 0.1f));
	aabb_tree_node *NewLeaf = AABBCreateNewNode(State);
	NewLeaf->AABB = AABB;
	NewLeaf->BodyIndex = Body->Index;

	aabb_tree_node *Sibling = State->AABBRoot;
	if(Sibling)
	{
		while(!NodeIsLeaf(Sibling))
		{
			Sibling = AABBChooseBestBranch(State, Sibling, NewLeaf);
		}

		aabb_tree_node *NewBranch = AABBCreateNewNode(State);
		NewBranch->Parent = Sibling->Parent;
		NewBranch->Left = NewLeaf;
		NewBranch->Right = Sibling;
		if(NewBranch->Parent)
		{
			if(NewBranch->Parent->Left == Sibling)
			{
				NewBranch->Parent->Left = NewBranch;
			}
			else
			{
				NewBranch->Parent->Right = NewBranch;
			}
		}
		else
		{
			State->AABBRoot = NewBranch;
		}

		NewLeaf->Parent = NewBranch;
		Sibling->Parent = NewBranch;

		AABBSyncHierarchy(State, NewBranch);
	}
	else
	{
		State->AABBRoot = NewLeaf;
	}

	Body->AABBNode = NewLeaf;
	return NewLeaf;
}

internal void
AABBRemove(physics_state *State, rigid_body *Body)
{
	TIMED_FUNCTION();
	aabb_tree_node *NodeToRemove = Body->AABBNode;
	aabb_tree_node *Parent = NodeToRemove->Parent;
	if(Parent)
	{
		aabb_tree_node *Sibling = ((Parent->Left == NodeToRemove) ? Parent->Right : Parent->Left);
		Assert(Sibling);

		aabb_tree_node *GrandParent = Parent->Parent;
		if(GrandParent)
		{
			if(GrandParent->Left == Parent)
			{
				GrandParent->Left = Sibling;
			}
			else
			{
				GrandParent->Right = Sibling;
			}

			Sibling->Parent = GrandParent;
			AABBSyncHierarchy(State, GrandParent);
		}
		else
		{
			State->AABBRoot = Sibling;
			Sibling->Parent = 0;
		}

		Parent->NextInFreeList = State->AABBNodeFreeList;
		State->AABBNodeFreeList = Parent;
	}
	else
	{
		State->AABBRoot = 0;
	}

	NodeToRemove->NextInFreeList = State->AABBNodeFreeList;
	State->AABBNodeFreeList = NodeToRemove;
	Body->AABBNode = 0;
}

internal void
AABBUpdate(physics_state *State, rigid_body *Body)
{
	TIMED_FUNCTION();
	rectangle2 TightAABB = GetWorldAABB(Body);
	aabb_tree_node *Node = Body->AABBNode;
	b32 NeedsUpdate = !Contains(Node->AABB, TightAABB);
	if(NeedsUpdate)
	{
		AABBRemove(State, Body);
		AABBInsert(State, Body);
	}
}

internal void
AABBQueryRecursive(physics_state *State, rectangle2 AABB,
	aabb_tree_node *Node, aabb_query_result *Result)
{
	if(RectangleIntersect(AABB, Node->AABB))
	{
		if(NodeIsLeaf(Node))
		{
			Result->BodyIndices[Result->Count++] = Node->BodyIndex;
		}
		else
		{
			AABBQueryRecursive(State, AABB, Node->Left, Result);
			AABBQueryRecursive(State, AABB, Node->Right, Result);
		}
	}
}

internal aabb_query_result
AABBQuery(physics_state *State, rectangle2 AABB)
{
	TIMED_FUNCTION();
	aabb_query_result Result = {};
	u32 MaxHits = State->RigidBodyCount;
	Result.BodyIndices = PushArray(&State->TempRegion, MaxHits, u32);

	AABBQueryRecursive(State, AABB, State->AABBRoot, &Result);
	return Result;
}

internal rigid_body *
AABBQueryPoint(physics_state *State, v2 Point)
{
	rigid_body *Result = 0;
	temporary_memory TempMem = BeginTemporaryMemory(&State->TempRegion);
	aabb_query_result QueryResult = AABBQuery(State, Rectangle2(Point, Point));
	for(u32 ResultIndex = 0;
		ResultIndex < QueryResult.Count;
		++ResultIndex)
	{
		rigid_body *TestBody = State->RigidBodies + QueryResult.BodyIndices[ResultIndex];
		if(RigidBodyContainsPoint(TestBody, Point))
		{
			Result = TestBody;
			break;
		}
	}

	EndTemporaryMemory(&TempMem);
	return Result;
}

internal rigid_body *
AddRigidBody(physics_state *Physics, entity *Entity, v2 P, v2 LocalCentroid,
	f32 RotationAngle, f32 InvMass, f32 InvMomentOfInertia, shape Shape, b32 Collideable)
{
	Assert(Physics->RigidBodyCount < ArrayCount(Physics->RigidBodies));
	u32 BodyIndex = Physics->RigidBodyCount++;
	rigid_body *RigidBody = Physics->RigidBodies + BodyIndex;
	RigidBody->Index = BodyIndex;
	RigidBody->Entity = Entity;

	RigidBody->Collideable = Collideable;
	RigidBody->InvMass = InvMass;
	RigidBody->InvMomentOfInertia = InvMomentOfInertia;
	RigidBody->P = P;
	RigidBody->Rotation = RotationMat2(RotationAngle);
	RigidBody->Shape = Shape;

	UpdateRotation(RigidBody);
	UpdateWorldCentroidFromPosition(RigidBody);
	AABBInsert(Physics, RigidBody);

	RigidBody->OldP = RigidBody->P;
	RigidBody->OldRotation = RigidBody->Rotation;
	RigidBody->OldInvRotation = RigidBody->InvRotation;

	return RigidBody;
}

internal joint *
AddRevoluteJoint(physics_state *Physics, rigid_body *BodyA, rigid_body *BodyB, v2 LocalAnchorA, v2 LocalAnchorB, b32 Collideable)
{
	joint *Result = Physics->Joints + Physics->JointCount++;
	Result->Type = Joint_Revolute;
	Result->Pair.IndexA = BodyA->Index;
	Result->Pair.IndexB = BodyB->Index;
	Result->LocalAnchorA = LocalAnchorA;
	Result->LocalAnchorB = LocalAnchorB;
	Result->Collideable = Collideable;

	if(!Result->Collideable)
	{
		index_pair *Pair = Physics->Uncollideables + Physics->UncollideableCount++;
		*Pair = Result->Pair;
	}

	return Result;
}

internal joint *
AddRevoluteJoint(physics_state *Physics, entity *EntityA, entity *EntityB, v2 LocalAnchorA, v2 LocalAnchorB, b32 Collideable)
{
	rigid_body *BodyA = EntityA->RigidBody;
	rigid_body *BodyB = EntityB->RigidBody;
	joint *Result = AddRevoluteJoint(Physics, BodyA, BodyB, LocalAnchorA, LocalAnchorB, Collideable);
	return Result;
}

inline void
MoveBody(physics_state *State, rigid_body *Body, v2 P)
{
	Body->P = P;
	UpdateWorldCentroidFromPosition(Body);
	AABBUpdate(State, Body);
}

internal void
DEBUGDisplayNodeRegionRecursive(physics_state *State, render_group *Group, aabb_tree_node *Node,	u32 Depth = 0)
{
	b32 IsLeaf = NodeIsLeaf(Node);
	v4 Color = IsLeaf ? V4(0.1f, 0.1f, 0.8f, 1.0f) : V4(0.8f, 0.1f, 0.1f, 1.0f);
	PushRectangleOutline(Group, Node->AABB.Min, Node->AABB.Max, Color);

	if(!IsLeaf)
	{
		DEBUGDisplayNodeRegionRecursive(State, Group, Node->Left,  Depth + 1);
		DEBUGDisplayNodeRegionRecursive(State, Group, Node->Right, Depth + 1);
	}
}

internal void
DEBUGDisplayTreeNodeRegionRecursive(physics_state *State, render_group *Group, aabb_tree_node *Node,	u32 Depth, v2 ParentDiagramP, v2 TotalDimension, b32 IsLeft = false)
{
	b32 IsLeaf = NodeIsLeaf(Node);
	v2 NodeDiagramP = ParentDiagramP;
	f32 TotalDiagramWidth = TotalDimension.x;
	f32 TotalDiagramHeight = TotalDimension.y;
	f32 Height = TotalDiagramHeight/(1 << (Depth));
	f32 Width = TotalDiagramWidth/(1 << (Depth+1));
	v2 NodeDim = 0.1f*Width*V2(1.0f, 1.0f);
	if(Depth > 0)
	{
		NodeDiagramP = ParentDiagramP + V2(IsLeft ? -Width : Width, -Height);
	}

	PushLine(Group, ParentDiagramP, NodeDiagramP, 1.0f, V4(0.3f, 0.3f, 0.3f, 1.0f));
	PushQuad(Group, NodeDiagramP, NodeDim, RotationMat2(0.0f), V4(0.8f, 0.2f, 0.2f, 1));

	if(!IsLeaf)
	{
		DEBUGDisplayTreeNodeRegionRecursive(State, Group, Node->Left,  Depth + 1, NodeDiagramP, TotalDimension, true);
		DEBUGDisplayTreeNodeRegionRecursive(State, Group, Node->Right, Depth + 1, NodeDiagramP, TotalDimension, false);
	}
}

internal void
DEBUGDisplayAABBTreeDiagram(physics_state *State, render_group *Group, v2 Min, v2 Max)
{
	v2 Dimension = Max - Min;
	v2 RootP = V2(Min.x + 0.5f*Dimension.x, Max.y);

	f32 BorderWidth = 2.0f;
	PushRectangleOutline(Group, Min, Max, V4(0.75f, 0.75f, 0.75f, 0.75f), BorderWidth);

	TemporaryClipRectChange(Group, V2ToV2i(Min), V2ToV2i(Max));
	PushQuad(Group, Min, Max, V4(0.1f, 0.1f, 0.1f, 0.5f));

	DEBUGDisplayTreeNodeRegionRecursive(State, Group, State->AABBRoot, 0, RootP, Dimension);
}

internal void
DEBUGDisplayAABBTreeRegions(physics_state *State, render_group *Group)
{
	if(Global_DisplayAABB)
	{
		DEBUGDisplayNodeRegionRecursive(State, Group, State->AABBRoot);
	}
}

internal void
PhysicsIntegrate(physics_state *State, f32 dt)
{
	{DEBUG_DATA_BLOCK("RigidBody");
		rigid_body *FirstBody = State->RigidBodies + 1;
		DEBUG_VALUE(FirstBody);
	}

	for(u32 RigidBodyIndex = 0;
		RigidBodyIndex < State->RigidBodyCount;
		++RigidBodyIndex)
	{
		rigid_body *Body = State->RigidBodies + RigidBodyIndex;

		if(Body->InvMass && Body->InvMomentOfInertia)
		{
			Body->OldP = Body->P;
			Body->OldRotation = Body->Rotation;
			Body->OldInvRotation = Body->InvRotation;

			Body->WorldCentroid += Body->dP*dt;
			f32 RotationAngle = Body->AngularVelocity*dt;
			Body->Rotation = RotationMat2(RotationAngle)*Body->Rotation;

			UpdateRotation(Body);
			UpdatePositionFromWorldCentroid(Body);

			AABBUpdate(State, Body);
		}
	}
}

internal void
PhysicsFindAndResolveContacts(physics_state *State, f32 dt)
{
	TIMED_FUNCTION();

	CheckMemory(&State->TempRegion);
	temporary_memory TempMem = BeginTemporaryMemory(&State->TempRegion);

	{
		TIMED_BLOCK("Broadphase");
		// NOTE: Broadphase
		State->BucketCount = 0;
		for(u32 RigidBodyIndex = 0;
			RigidBodyIndex < State->RigidBodyCount;
			++RigidBodyIndex)
		{
			rigid_body *Body = State->RigidBodies + RigidBodyIndex;
			if(Body->Collideable)
			{
#if 0
				// NOTE: n squared brute force
				for(u32 TestRigidBodyIndex = RigidBodyIndex + 1;
					TestRigidBodyIndex < State->RigidBodyCount;
					++TestRigidBodyIndex)
				{
					rigid_body *TestBody = State->RigidBodies + TestRigidBodyIndex;
					rectangle2 BodyAABB = GetWorldAABB(Body, V2(0.1f, 0.1f));
					rectangle2 TestBodyAABB = GetWorldAABB(TestBody, V2(0.1f, 0.1f));
					if(RectangleIntersect(BodyAABB, TestBodyAABB))
					{
						AddPotentialCollision(State, RigidBodyIndex, TestRigidBodyIndex);
					}
				}
#else
				// NOTE: nlogn-ish AABB Query.
				rectangle2 BodyAABB = GetWorldAABB(Body);
				aabb_query_result Query = AABBQuery(State, BodyAABB);
				for(u32 TestIndex = 0;
					TestIndex < Query.Count;
					++TestIndex)
				{
					u32 TestBodyIndex = Query.BodyIndices[TestIndex];
					rigid_body *TestBody = State->RigidBodies + TestBodyIndex;
					if(TestBody->Collideable)
					{
						if(TestBodyIndex != RigidBodyIndex)
						{
							AddPotentialCollision(State, RigidBodyIndex, TestBodyIndex);
						}
					}
				}
#endif
			}
		}
	}

	{
		TIMED_BLOCK("Build Contacts");
		// NOTE: Contacts
		for(contact_manifold *Manifold = State->ManifoldSentinel.Next;
			Manifold != &State->ManifoldSentinel;
			)
		{
			Manifold = ManifoldCheckContacts(State, Manifold);
		}

		for(u32 BucketIndex = 0;
			BucketIndex < State->BucketCount;
			++BucketIndex)
		{
			bucket *Bucket = State->PotentialCollisionBuckets + BucketIndex;
			for(u32 PairIndex = 0;
				PairIndex < Bucket->PotentialCollisionCount;
				++PairIndex)
			{
				index_pair *Pair = Bucket->PotentialCollisions + PairIndex;
				rigid_body *BodyA = State->RigidBodies + Pair->IndexA;
				rigid_body *BodyB = State->RigidBodies + Pair->IndexB;
				contact_manifold *Manifold = PhysicsGetOrCreateContactManifold(State, Pair);

				minkowski_simplex GJKResult = GJK(BodyA, BodyB);
				if(GJKResult.ContainsOrigin)
				{
					contact_data Contact = ExpandingPolygonAlgorithm(&State->TempRegion,
						BodyA, BodyB, &GJKResult);
					AddContact(Manifold, &Contact);
				}
				else
				{
#if 0
					// NOTE: Make contact_data for potential future collision
					minkowski_difference_support ClosestPoint = SimplexGetClosestPoint(&GJKResult, V2(0.0f, 0.0f));
					contact_data Contact = {};
					Contact.WorldContactPointA = ClosestPoint.SupportA;
					Contact.WorldContactPointB = ClosestPoint.SupportB;

					Contact.LocalContactPointA = WorldPointToLocalPoint(BodyA, Contact.WorldContactPointA);
					Contact.LocalContactPointB = WorldPointToLocalPoint(BodyB, Contact.WorldContactPointB);

					v2 ContactDistanceVector = Contact.WorldContactPointB - Contact.WorldContactPointA;
					Contact.Penetration = -Length(ContactDistanceVector);
					Contact.Normal = (-1.0f/Contact.Penetration)*ContactDistanceVector;

					AddContact(Manifold, &Contact);
#endif
				}
			}
		}
	}

	{
		TIMED_BLOCK("Resolve Contacts");
		// NOTE: Resolve
		u32 IterationCount = 32;
		for(u32 Iteration = 0;
			Iteration < IterationCount;
			++Iteration)
		{
			for(contact_manifold *Manifold = State->ManifoldSentinel.Next;
				Manifold != &State->ManifoldSentinel;
				Manifold = Manifold->Next)
			{
				rigid_body *BodyA = State->RigidBodies + Manifold->Pair.IndexA;
				rigid_body *BodyB = State->RigidBodies + Manifold->Pair.IndexB;

				for(u32 Index = 0;
					Index < Manifold->ContactCount;
					++Index)
				{
					contact_data *Contact = Manifold->Contacts + Index;
					SolveContactConstraints(BodyA, BodyB, Contact, dt);
				}
			}

			for(u32 JointIndex = 0;
				JointIndex < State->JointCount;
				++JointIndex)
			{
				joint *Joint = State->Joints + JointIndex;
				rigid_body *BodyA = State->RigidBodies + Joint->Pair.IndexA;
				rigid_body *BodyB = State->RigidBodies + Joint->Pair.IndexB;

				SolveJointConstraints(BodyA, BodyB, Joint, dt);
			}
		}
	}

	EndTemporaryMemory(&TempMem);
}

internal void
DEBUGDisplayContactInfo(physics_state *State, render_group *Group)
{
	if(Global_DisplayContacts)
	{
		for(contact_manifold *Manifold = State->ManifoldSentinel.Next;
			Manifold != &State->ManifoldSentinel;
			Manifold = Manifold->Next)
		{
			contact_data *LastContact = 0;
			for(u32 Index = 0;
				Index < Manifold->ContactCount;
				++Index)
			{
				contact_data *Contact = Manifold->Contacts + Index;

				PushPoint(Group, Contact->WorldContactPointA);
				PushPoint(Group, Contact->WorldContactPointB, V4(1.0f, 0.0f, 0.0f, 0.5f));

				if(Global_DisplayContactNormals)
				{
					f32 Scale = 1.0f;
					PushLine(Group, Contact->WorldContactPointA, Contact->WorldContactPointA + Scale*Contact->Normal, 0.05f, V4(1.0f, 1.0f, 0.0f, 0.5f));
					PushLine(Group, Contact->WorldContactPointB, Contact->WorldContactPointB + Scale*Contact->Normal, 0.05f, V4(0.0f, 1.0f, 1.0f, 0.5f));
				}

				if(Global_DisplayImpulses)
				{
					f32 Scale = 0.5f;
					v2 Tangent = Perp(Contact->Normal);
					v2 ImpulseVector = Scale*(Contact->NormalImpulseSum*Contact->Normal + Contact->TangentImpulseSum*Tangent);
					PushLine(Group, Contact->WorldContactPointA, Contact->WorldContactPointA + -ImpulseVector, 0.05f, V4(1.0f, 1.0f, 0.0f, 0.5f));
					PushLine(Group, Contact->WorldContactPointB, Contact->WorldContactPointB +  ImpulseVector, 0.05f, V4(0.0f, 1.0f, 1.0f, 0.5f));
				}

				if(LastContact)
				{
					contact_data *ContactData0 = LastContact;
					contact_data *ContactData1 = Contact;
					PushLine(Group, ContactData0->WorldContactPointA, ContactData1->WorldContactPointA, 0.05f, V4(0.5f, 0.5f, 0.5f, 0.25f));
					PushLine(Group, ContactData0->WorldContactPointB, ContactData1->WorldContactPointB, 0.05f, V4(0.5f, 0.5f, 0.5f, 0.25f));
				}

				LastContact = Contact;
			}
		}

		if(Global_DisplayJoints)
		{
			for(u32 JointIndex = 0;
				JointIndex < State->JointCount;
				++JointIndex)
			{
				joint *Joint = State->Joints + JointIndex;
				rigid_body *BodyA = State->RigidBodies + Joint->Pair.IndexA;
				rigid_body *BodyB = State->RigidBodies + Joint->Pair.IndexB;
				v2 WorldAnchorA = LocalPointToWorldPoint(BodyA, Joint->LocalAnchorA);
				v2 WorldAnchorB = LocalPointToWorldPoint(BodyB, Joint->LocalAnchorB);

				PushLine(Group, WorldAnchorA, BodyA->WorldCentroid, 0.05f, V4(0.1f, 0.1f, 0.1f, 0.5f));
				PushLine(Group, WorldAnchorB, BodyB->WorldCentroid, 0.05f, V4(0.1f, 0.1f, 0.1f, 0.5f));

				PushPoint(Group, WorldAnchorA);
				PushPoint(Group, WorldAnchorB, V4(1.0f, 0.0f, 0.0f, 0.5f));
			}
		}
	}
}

internal physics_state *
InitializePhysics(render_group *DEBUGRenderGroup,
	camera *ScreenCamera, camera *WorldCamera)
{
	physics_state *Result = BootstrapPushStruct(physics_state, Region);
	DLIST_INIT(&Result->ManifoldSentinel);

	return Result;
}
