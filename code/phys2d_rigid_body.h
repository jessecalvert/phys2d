/*@H
* File: phys2d_rigid_body.h
* Author: Jesse Calvert
* Created: October 6, 2017, 16:48
* Last modified: November 27, 2017, 14:13
*/

#pragma once
#include "phys2d_math.h"
#include "phys2d_memory.h"

struct minkowski_difference_support
{
	v2 SupportA;
	v2 SupportB;
	v2 Support;
};
inline minkowski_difference_support
Lerp(minkowski_difference_support A, f32 t, minkowski_difference_support B)
{
	minkowski_difference_support Result = {};
	Result.SupportA = Lerp(A.SupportA, t, B.SupportA);
	Result.SupportB = Lerp(A.SupportB, t, B.SupportB);
	Result.Support = Lerp(A.Support, t, B.Support);
	return Result;
}

struct minkowski_simplex
{
	u32 Size;
	minkowski_difference_support Points[3];

	b32 ContainsOrigin;
};

struct expanding_polygon_vertex
{
	expanding_polygon_vertex *Next;
	expanding_polygon_vertex *Prev;

	minkowski_difference_support Vertex;
};

struct expanding_polygon
{
	u32 VertexCount;

	expanding_polygon_vertex Sentinel;
	expanding_polygon_vertex *FirstFree;
};
inline expanding_polygon_vertex *
GetNextVertex(expanding_polygon *Polygon, expanding_polygon_vertex *Vertex)
{
	expanding_polygon_vertex *NextVertex = Vertex->Next;
	if(NextVertex == &Polygon->Sentinel)
	{
		NextVertex = NextVertex->Next;
	}
	return NextVertex;
}
inline expanding_polygon_vertex *
GetPrevVertex(expanding_polygon *Polygon, expanding_polygon_vertex *Vertex)
{
	expanding_polygon_vertex *PrevVertex = Vertex->Prev;
	if(PrevVertex == &Polygon->Sentinel)
	{
		PrevVertex = PrevVertex->Prev;
	}
	return PrevVertex;
}

inline minkowski_difference_support
GetNextVertexData(expanding_polygon *Polygon, expanding_polygon_vertex *Vertex)
{
	expanding_polygon_vertex *NextVertex = GetNextVertex(Polygon, Vertex);
	minkowski_difference_support Result = NextVertex->Vertex;
	return Result;
}

struct circle
{
	v2 Center;
	f32 Radius;
};

struct pill
{
	v2 P0, P1;
	f32 Radius;
};

// TODO: QuickHull for convex_polygon
struct convex_polygon
{
	u32 VertexCount;
	v2 *Vertices;
};

enum shape_type
{
	Shape_Circle,
	Shape_Pill,
	Shape_ConvexPolygon,

	Shape_Count,
};
struct shape
{
	shape_type Type;
	union
	{
		circle Circle;
		pill Pill;
		convex_polygon Polygon;
	};
};

struct rigid_body
{
	u32 Index;
	struct entity *Entity;

	b32 Collideable;

	f32 InvMass;
	f32 InvMomentOfInertia;

	v2 WorldCentroid;
	v2 LocalCentroid;

	v2 P;
	mat2 Rotation;
	mat2 InvRotation;
	v2 OldP;
	mat2 OldRotation;
	mat2 OldInvRotation;

	v2 dP;
	f32 AngularVelocity;

	// TODO: Add multiple colliders for one rigid_body
	shape Shape;

	struct aabb_tree_node *AABBNode;
};

struct index_pair
{
	u32 IndexA;
	u32 IndexB;
};
inline b32
operator==(index_pair A, index_pair B)
{
	b32 Result = (((A.IndexA == B.IndexA) && (A.IndexB == B.IndexB)) ||
		((A.IndexA == B.IndexB) && (A.IndexB == B.IndexA)));
	return Result;
}
inline b32
operator!=(index_pair A, index_pair B)
{
	b32 Result = !(A == B);
	return Result;
}

#define PERSISTENT_THRESHOLD 0.01f
struct contact_data
{
	v2 WorldContactPointA;
	v2 WorldContactPointB;

	v2 LocalContactPointA;
	v2 LocalContactPointB;

	v2 Normal; // NOTE: Points from A to B.
	f32 Penetration;

	f32 NormalImpulseSum;
	f32 TangentImpulseSum;

	b32 Persistent;
};

struct contact_manifold
{
	contact_manifold *Next;
	contact_manifold *Prev;
	contact_manifold *NextInHash;

	index_pair Pair;

	u32 ContactCount;
	contact_data Contacts[2];
};

enum joint_type
{
	Joint_Revolute,
	Joint_Prismatic,

	Joint_Count,
};
struct joint
{
	joint_type Type;
	index_pair Pair;

	// NOTE: Joint_Revolute
	v2 LocalAnchorA;
	v2 LocalAnchorB;

	b32 Collideable;
};

#define BUCKET_SIZE 64
struct bucket
{
	u32 PotentialCollisionCount;
	index_pair *PotentialCollisions;
};

struct aabb_query_result
{
	u32 Count;
	u32 *BodyIndices;
};

struct aabb_tree_node
{
	// TODO: These pointers could be indicies
	aabb_tree_node *Parent;
	union
	{
		aabb_tree_node *Left;
		aabb_tree_node *NextInFreeList;
	};
	union
	{
		aabb_tree_node *Right;
		u32 BodyIndex;
	};

	u32 Height;
	rectangle2 AABB;
};
inline b32
NodeIsLeaf(aabb_tree_node *Node)
{
	b32 Result = (Node->Left == 0);
	return Result;
}

#define MAX_RIGID_BODY_COUNT 1024
#define MAX_BUCKETS	64
struct physics_state
{
	memory_region Region;
	memory_region TempRegion;

	u32 RigidBodyCount;
	rigid_body RigidBodies[MAX_RIGID_BODY_COUNT];

	u32 BucketCount;
	bucket PotentialCollisionBuckets[MAX_BUCKETS];

	contact_manifold *ContactManifoldHashTable[1024];
	contact_manifold *ManifoldFreeList;
	contact_manifold ManifoldSentinel;

	u32 JointCount;
	joint Joints[128];

	u32 UncollideableCount;
	index_pair Uncollideables[128];

	// TODO: Keep the tree balanced.
	aabb_tree_node *AABBRoot;
	aabb_tree_node *AABBNodeFreeList;
};

inline void
UpdateRotation(rigid_body *Body)
{
	Body->Rotation = ReOrthonormalize(Body->Rotation);
	Body->InvRotation = Transpose(Body->Rotation);
}

inline void
UpdatePositionFromWorldCentroid(rigid_body *Body)
{
	Body->P = Body->WorldCentroid + Body->Rotation*(-Body->LocalCentroid);
}

inline void
UpdateWorldCentroidFromPosition(rigid_body *Body)
{
	Body->WorldCentroid = Body->P + Body->Rotation*Body->LocalCentroid;
}

inline v2
LocalPointToWorldPoint(rigid_body *Body, v2 Point)
{
	v2 Result = Body->Rotation*Point + Body->P;
	return Result;
}

inline v2
WorldPointToLocalPoint(rigid_body *Body, v2 Point)
{
	v2 Result = Body->InvRotation*(Point - Body->P);
	return Result;
}

inline v2
LocalVectorToWorldVector(rigid_body *Body, v2 Vector)
{
	v2 Result = Body->Rotation*Vector;
	return Result;
}

inline v2
WorldVectorToLocalVector(rigid_body *Body, v2 Vector)
{
	v2 Result = Body->InvRotation*Vector;
	return Result;
}

inline void
ApplyImpulse(rigid_body *Body, v2 Impulse, v2 Location)
{
	if(Body->InvMass)
	{
		Body->dP += Body->InvMass*Impulse;

		f32 TorqueImpulse = Inner(Perp(Location - Body->WorldCentroid), Impulse);
		Body->AngularVelocity += Body->InvMomentOfInertia*TorqueImpulse;
	}
}

inline void
ApplyTorque(rigid_body *Body, f32 Torque)
{
	if(Body->InvMass)
	{
		Body->AngularVelocity += Body->InvMomentOfInertia*Torque;
	}
}
