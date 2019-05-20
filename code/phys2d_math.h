/*@H
* File: phys2d_math.h
* Author: Jesse Calvert
* Created: October 9, 2017, 16:28
* Last modified: October 22, 2017, 17:26
*/

#pragma once
#include "phys2d_intrinsics.h"

//
// NOTE: Scalar operations
//

inline r32
Clamp(r32 Value, r32 Min, r32 Max)
{
	r32 Result = Value;
	if(Value < Min)
	{
		Result = Min;
	}
	if(Value > Max)
	{
		Result = Max;
	}
	return Result;
}

inline r32
Clamp01(r32 Value)
{
	r32 Result = Clamp(Value, 0.0f, 1.0f);
	return Result;
}

inline r32
Lerp(r32 A, r32 t, r32 B)
{
	r32 Result = A + t*(B - A);
	return Result;
}

//
// NOTE: v2 operations
//

inline v2
V2(r32 x, r32 y)
{
	v2 Result = {x, y};
	return Result;
}

inline v2
operator-(v2 A)
{
	v2 Result = V2(-A.x, -A.y);
	return Result;
}

inline v2
operator-(v2 A, v2 B)
{
	v2 Result = V2(A.x - B.x, A.y - B.y);
	return Result;
}

inline v2
operator+(v2 A, v2 B)
{
	v2 Result = V2(A.x + B.x, A.y + B.y);
	return Result;
}

inline v2 &
operator+=(v2 &A, v2 B)
{
	A = A + B;
	return A;
}

inline v2
operator*(r32 Scale, v2 Vector)
{
	v2 Result = V2(Scale*Vector.x, Scale*Vector.y);
	return Result;
}

inline v2
operator*(v2 Vector, r32 Scale)
{
	v2 Result = Scale*Vector;
	return Result;
}

inline b32
operator==(v2 A, v2 B)
{
	b32 Result = ((A.x == B.x) &&
	              (A.y == B.y));
	return Result;
}

inline v2
Hadamard(v2 A, v2 B)
{
	v2 Result = {A.x*B.x, A.y*B.y};
	return Result;
}

inline f32
Inner(v2 A, v2 B)
{
	f32 Result = A.x*B.x + A.y*B.y;
	return Result;
}

inline v2
Perp(v2 A)
{
	v2 Result = {-A.y, A.x};
	return Result;
}

inline v2
Lerp(v2 A, f32 t, v2 B)
{
	v2 Result = {A.x + t*(B.x - A.x), A.y + t*(B.y - A.y)};
	return Result;
}

inline f32
LengthSq(v2 A)
{
	f32 Result = Inner(A, A);
	return Result;
}

inline f32
Length(v2 A)
{
	f32 Result = SquareRoot(LengthSq(A));
	return Result;
}

inline v2
Normalize(v2 A)
{
	f32 LengthOfA = Length(A);
	v2 Result = (1.0f/LengthOfA)*A;
	return Result;
}

inline b32
SameDirection(v2 A, v2 B)
{
	b32 Result = (Inner(A, B) > 0.0f);
	return Result;
}

inline r32
DistanceToLine(v2 TestPoint, v2 LineUnit)
{
	r32 Result = Length(TestPoint - Inner(TestPoint, LineUnit)*LineUnit);
	return Result;
}

inline r32
DistanceToLineSq(v2 TestPoint, v2 LineStart, v2 LineEnd)
{
	v2 Line = LineEnd - LineStart;
	r32 Result = (Square(Inner(TestPoint, Perp(Line))))/(LengthSq(Line));
	return Result;
}

inline r32
DistanceToLine(v2 TestPoint, v2 LineStart, v2 LineEnd)
{
	r32 Result = SquareRoot(DistanceToLineSq(TestPoint, LineStart, LineEnd));
	return Result;
}

inline f32
SegmentClosestPointBarycentric(v2 TestPoint, v2 Head, v2 Tail)
{
	v2 B = Tail;
	v2 A = Head;
	v2 AB = B - A;
	v2 BP = TestPoint - B;
	v2 AP = TestPoint - A;

	f32 Result = 0.0f;
	if(SameDirection(AB, AP))
	{
		if(SameDirection(BP, AB))
		{
			// NOTE: [B]
			Result = 1.0f;
		}
		else
		{
			// NOTE: [A, B]
			Result = Inner(TestPoint - A, AB) / LengthSq(AB);
		}
	}
	else
	{
		// NOTE: [A]
		Result = 0.0f;
	}
	return Result;
}

inline v2
SegmentClosestPoint(v2 TestPoint, v2 Head, v2 Tail)
{
	f32 t = SegmentClosestPointBarycentric(TestPoint, Head, Tail);
	v2 Result = Lerp(Head, t, Tail);
	return Result;
}

//
// NOTE: v2i operations
//

inline v2i
V2i(s32 x, s32 y)
{
	v2i Result = {x, y};
	return Result;
}

inline v2i
V2ToV2i(v2 A)
{
	v2i Result = {(s32)A.x, (s32)A.y};
	return Result;
}

inline v2
V2iToV2(v2i A)
{
	v2 Result = {(f32)A.x, (f32)A.y};
	return Result;
}

inline v2
operator*(r32 Scale, v2i A)
{
	v2 Result = {Scale*A.x, Scale*A.y};
	return Result;
}

inline v2i
operator*(s32 Scale, v2i A)
{
	v2i Result = {Scale*A.x, Scale*A.y};
	return Result;
}

inline v2i
operator-(v2i B)
{
	v2i Result = {-B.x, -B.y};
	return Result;
}

inline v2i
operator-(v2i A, v2i B)
{
	v2i Result = {A.x - B.x, A.y - B.y};
	return Result;
}

inline v2i
operator+(v2i A, v2i B)
{
	v2i Result = {A.x + B.x, A.y + B.y};
	return Result;
}

inline b32
operator==(v2i A, v2i B)
{
	b32 Result = ((A.x == B.x) &&
	              (A.y == B.y));
	return Result;
}

inline b32
operator!=(v2i A, v2i B)
{
	b32 Result = !(A == B);
	return Result;
}

//
// NOTE: v3i operations
//

inline v3i
V3i(s32 x, s32 y, s32 z)
{
	v3i Result = {x, y, z};
	return Result;
}

inline v3i
operator+(v3i A, v3i B)
{
	v3i Result = {A.x + B.x, A.y + B.y, A.z + B.z};
	return Result;
}

inline v3i
operator-(v3i A, v3i B)
{
	v3i Result = {A.x - B.x, A.y - B.y, A.z - B.z};
	return Result;
}

inline b32
operator==(v3i A, v3i B)
{
	b32 Result = ((A.x == B.x) && (A.y == B.y) && (A.z == B.z));
	return Result;
}

inline b32
operator!=(v3i A, v3i B)
{
	b32 Result = !(A == B);
	return Result;
}

//
// NOTE: v3 operations
//

inline v3
V3(r32 x, r32 y, r32 z)
{
	v3 Result = {x, y, z};
	return Result;
}

inline v3
V3(v2 xy, r32 z)
{
	v3 Result = {xy.x, xy.y, z};
	return Result;
}

inline v3
V3(r32 x, v2 yz)
{
	v3 Result = {x, yz.x, yz.y};
	return Result;
}

inline v3
operator+(v3 A, v3 B)
{
	v3 Result = {A.x + B.x, A.y + B.y, A.z + B.z};
	return Result;
}

inline v3
operator*(r32 Scalar, v3 Vector)
{
	v3 Result = {Scalar*Vector.x, Scalar*Vector.y, Scalar*Vector.z};
	return Result;
}

inline v3
operator*(v3 Vector, r32 Scalar)
{
	v3 Result = Scalar*Vector;
	return Result;
}

inline v3
operator-(v3 A, v3 B)
{
	v3 Result = A + (-1.0f)*B;
	return Result;
}

inline v3
operator-(v3 A)
{
	v3 Result = V3(-A.x, -A.y, -A.z);
	return Result;
}

inline v3 &
operator+=(v3 &A, v3 B)
{
	A = A + B;
	return A;
}

inline b32
operator==(v3 A, v3 B)
{
	b32 Result = ((A.x == B.x) &&
	              (A.y == B.y) &&
	              (A.z == B.z));
	return Result;
}

inline b32
operator!=(v3 A, v3 B)
{
	b32 Result = !(A == B);
	return Result;
}

inline r32
Inner(v3 A, v3 B)
{
	r32 Result = A.x*B.x + A.y*B.y + A.z*B.z;
	return Result;
}

inline v3
Lerp(v3 A, r32 t, v3 B)
{
	v3 Result = V3(Lerp(A.x, t, B.x),
	               Lerp(A.y, t, B.y),
	               Lerp(A.z, t, B.z));
	return Result;
}

inline r32
LengthSq(v3 A)
{
	r32 Result = Inner(A,A);
	return Result;
}

inline r32
Length(v3 A)
{
	r32 Result = SquareRoot(LengthSq(A));
	return Result;
}

inline r32
Distance(v3 A, v3 B)
{
	r32 Result = Length(B - A);
	return Result;
}

inline v3
Normalize(v3 A)
{
	r32 ALength = Length(A);
	Assert(ALength != 0.0f);
	v3 Result = (1.0f/ALength)*A;
	return Result;
}

inline r32
DistanceToLine(v3 TestPoint, v3 LineUnit)
{
	r32 Result = Length(TestPoint - Inner(TestPoint, LineUnit)*LineUnit);
	return Result;
}

inline r32
DistanceToLine(v3 TestPoint, v3 LineStart, v3 LineEnd)
{
	v3 LineUnit = Normalize(LineEnd - LineStart);
	r32 Result = DistanceToLine(TestPoint - LineStart, LineUnit);
	return Result;
}

inline r32
SignedDistanceToPlane(v3 TestPoint, v3 UnitNormal)
{
	r32 Result = Inner(TestPoint, UnitNormal);
	return Result;
}

inline r32
DistanceToPlane(v3 TestPoint, v3 UnitNormal)
{
	r32 Result = AbsoluteValue(SignedDistanceToPlane(TestPoint, UnitNormal));
	return Result;
}

inline b32
IsUnit(v3 A)
{
	r32 Epsilon = 0.001f;
	r32 ALengthSq = LengthSq(A);
	b32 Result = ((ALengthSq < (1.0f + Square(Epsilon))) &&
	              (ALengthSq > (1.0f - Square(Epsilon))));
	return Result;
}

inline v3
NormalizeOr(v3 A, v3 Or)
{
	r32 ALength = Length(A);
	v3 Result = Or;
	if(ALength != 0.0f)
	{
		Result = (1.0f/ALength)*A;
	}
	return Result;
}

inline v3
NOZ(v3 A)
{
	r32 ALength = Length(A);
	v3 Result = {};
	if(ALength != 0.0f)
	{
		Result = (1.0f/ALength)*A;
	}
	return Result;
}

inline v3
Cross(v3 U, v3 V)
{
	v3 Result = V3(U.y*V.z - U.z*V.y,
	               U.z*V.x - U.x*V.z,
	               U.x*V.y - U.y*V.x);
	return Result;
}

inline b32
SameDirection(v3 A, v3 B)
{
	b32 Result = (Inner(A, B) > 0.0f);
	return Result;
}

inline v3
Hadamard(v3 A, v3 B)
{
	v3 Result = {A.x*B.x, A.y*B.y, A.z*B.z};
	return Result;
}

inline b32
AreParallel(v3 A, v3 B)
{
	r32 Epsilon = 0.00001f;
	b32 Result = (LengthSq(Cross(A, B)) < Epsilon);
	return Result;
}

inline v3
SegmentClosestPoint(v3 TestPoint, v3 Head, v3 Tail)
{
	v3 B = Tail;
	v3 A = Head;
	v3 AB = B - A;
	v3 BP = TestPoint - B;
	v3 AP = TestPoint - A;

	v3 Result = {};
	if(SameDirection(AB, AP))
	{
		if(SameDirection(BP, AB))
		{
			// NOTE: [B]
			Result = B;
		}
		else
		{
			// NOTE: [A, B]
			v3 LineUnit = Normalize(AB);
			Result = Inner(TestPoint - A, LineUnit)*LineUnit;
		}
	}
	else
	{
		// NOTE: [A]
		Result = A;
	}
	return Result;
}

inline r32
DistanceToSegment(v3 TestPoint, v3 Head, v3 Tail)
{
	v3 ClosestPoint = SegmentClosestPoint(TestPoint, Head, Tail);
	r32 Result = Length(TestPoint - ClosestPoint);
	return Result;
}

//
// NOTE: v4 operations
//

inline v4
V4(r32 x, r32 y, r32 z, r32 w)
{
	v4 Result = {x, y, z, w};
	return Result;
}

inline v4
V4(v2 V, r32 z, r32 w)
{
	v4 Result = V4(V.x, V.y, z, w);
	return Result;
}

inline v4
V4(v3 V, r32 w)
{
	v4 Result = V4(V.x, V.y, V.z, w);
	return Result;
}

inline b32
operator==(v4 A, v4 B)
{
	b32 Result = ((A.x == B.x) &&
	              (A.y == B.y) &&
	              (A.z == B.z) &&
	              (A.w == B.w));
	return Result;
}

inline v4
operator+(v4 A, v4 B)
{
	v4 Result = {A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w};
	return Result;
}

inline v4
operator*(r32 Scalar, v4 Vector)
{
	v4 Result = {Scalar*Vector.x, Scalar*Vector.y, Scalar*Vector.z, Scalar*Vector.w};
	return Result;
}

inline v4
operator*(v4 Vector, r32 Scalar)
{
	v4 Result = Scalar*Vector;
	return Result;
}

inline v4
operator-(v4 A, v4 B)
{
	v4 Result = A + (-1.0f)*B;
	return Result;
}

inline v4
operator-(v4 A)
{
	v4 Result = V4(-A.x, -A.y, -A.z, -A.w);
	return Result;
}

inline r32
Inner(v4 A, v4 B)
{
	r32 Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
	return Result;
}

//
// NOTE: rectangle2 operations
//

inline rectangle2
Rectangle2(v2 Min, v2 Max)
{
	Assert(Min.x <= Max.x);
	Assert(Min.y <= Max.y);
	rectangle2 Result = {Min, Max};
	return Result;
}

inline rectangle2
Rectangle2CenterDim(v2 Center, v2 Dim)
{
	v2 Min = Center - 0.5f*Dim;
	v2 Max = Center + 0.5f*Dim;
	rectangle2 Result = Rectangle2(Min, Max);
	return Result;
}

inline rectangle2
operator+(v2 Offset, rectangle2 Rect)
{
	rectangle2 Result = Rect;
	Result.Min += Offset;
	Result.Max += Offset;
	return Result;
}

inline rectangle2
operator+(rectangle2 Rect, v2 Offset)
{
	rectangle2 Result = Offset + Rect;
	return Result;
}

inline v2
Dim(rectangle2 Rect)
{
	v2 Result = Rect.Max - Rect.Min;
	return Result;
}

inline v2
Center(rectangle2 Rect)
{
	v2 Result = 0.5f*(Rect.Max - Rect.Min);
	return Result;
}

inline b32
RectangleIntersect(rectangle2 A, rectangle2 B)
{
	b32 Result = !(((A.Max.x < B.Min.x) || (B.Max.x < A.Min.x)) ||
	               ((A.Max.y < B.Min.y) || (B.Max.y < A.Min.y)));
	return Result;
}

inline b32
InsideRectangle(rectangle2 Rect, v2 P)
{
	b32 Result = (((P.x >= Rect.Min.x) && (P.x <= Rect.Max.x)) &&
	              ((P.y >= Rect.Min.y) && (P.y <= Rect.Max.y)));
	return Result;
}

inline b32
Contains(rectangle2 A, rectangle2 B)
{
	b32 Result = (InsideRectangle(A, B.Min) && InsideRectangle(A, B.Max));
	return Result;
}

inline rectangle2
Combine(rectangle2 A, rectangle2 B)
{
	rectangle2 Result = A;
	for(u32 DimensionIndex = 0;
		DimensionIndex < 2;
		++DimensionIndex)
	{
		if(B.Min.E[DimensionIndex] < Result.Min.E[DimensionIndex])
		{
			Result.Min.E[DimensionIndex] = B.Min.E[DimensionIndex];
		}
		if(B.Max.E[DimensionIndex] > Result.Max.E[DimensionIndex])
		{
			Result.Max.E[DimensionIndex] = B.Max.E[DimensionIndex];
		}
	}

	return Result;
}

inline f32
Perimeter(rectangle2 A)
{
	v2 Dim = A.Max - A.Min;
	f32 Result = 2.0f*(Dim.x + Dim.y);
	return Result;
}

inline rectangle2
ClipTo(rectangle2 Rect, rectangle2 Clip)
{
	rectangle2 Result = Rect;
	Result.Min.x = Maximum(Result.Min.x, Clip.Min.x);
	Result.Min.y = Maximum(Result.Min.y, Clip.Min.y);
	Result.Max.x = Minimum(Result.Max.x, Clip.Max.x);
	Result.Max.y = Minimum(Result.Max.y, Clip.Max.y);
	return Result;
}

//
// NOTE: rectangle3 operations
//

inline rectangle3
Rectangle3(v3 Min, v3 Max)
{
	Assert(Min.x <= Max.x);
	Assert(Min.y <= Max.y);
	Assert(Min.z <= Max.z);
	rectangle3 Result = {Min, Max};
	return Result;
}

inline rectangle3
Rectangle3CenterDim(v3 Center, v3 Dim)
{
	v3 Min = Center - 0.5f*Dim;
	v3 Max = Center + 0.5f*Dim;
	rectangle3 Result = Rectangle3(Min, Max);
	return Result;
}

inline rectangle3
operator+(v3 Offset, rectangle3 Rect)
{
	rectangle3 Result = Rect;
	Result.Min += Offset;
	Result.Max += Offset;
	return Result;
}

inline rectangle3
operator+(rectangle3 Rect, v3 Offset)
{
	rectangle3 Result = Offset + Rect;
	return Result;
}

inline v3
Dim(rectangle3 Rect)
{
	v3 Result = Rect.Max - Rect.Min;
	return Result;
}

inline v3
Center(rectangle3 Rect)
{
	v3 Result = 0.5f*(Rect.Max - Rect.Min);
	return Result;
}

inline b32
RectangleIntersect(rectangle3 A, rectangle3 B)
{
	b32 Result = !(((A.Max.x < B.Min.x) || (B.Max.x < A.Min.x)) ||
	               ((A.Max.y < B.Min.y) || (B.Max.y < A.Min.y)) ||
	               ((A.Max.z < B.Min.z) || (B.Max.z < A.Min.z)));
	return Result;
}

inline b32
InsideRectangle(rectangle3 Rect, v3 P)
{
	b32 Result = (((P.x >= Rect.Min.x) && (P.x <= Rect.Max.x)) &&
	              ((P.y >= Rect.Min.y) && (P.y <= Rect.Max.y)) &&
	              ((P.z >= Rect.Min.z) && (P.z <= Rect.Max.z)));
	return Result;
}

inline r32
SurfaceArea(rectangle3 Rect)
{
	v3 Dimension = Dim(Rect);
	r32 Result = 2.0f * (Dimension.x*Dimension.y + Dimension.x*Dimension.z + Dimension.y*Dimension.z);
	return Result;
}

inline rectangle3
Combine(rectangle3 A, rectangle3 B)
{
	rectangle3 Result = A;
	for(u32 DimensionIndex = 0;
		DimensionIndex < 3;
		++DimensionIndex)
	{
		if(B.Min.E[DimensionIndex] < Result.Min.E[DimensionIndex])
		{
			Result.Min.E[DimensionIndex] = B.Min.E[DimensionIndex];
		}
		if(B.Max.E[DimensionIndex] > Result.Max.E[DimensionIndex])
		{
			Result.Max.E[DimensionIndex] = B.Max.E[DimensionIndex];
		}
	}

	return Result;
}

//
// NOTE: mat2 operations
//

inline mat2
Mat2(v2 C0, v2 C1)
{
	mat2 Result = {};
	Result.C[0] = C0;
	Result.C[1] = C1;
	return Result;
}

inline mat2
IdentityMat2()
{
	mat2 Result =
	{
		1, 0,
		0, 1,
	};
	return Result;
}

inline mat2
operator+(mat2 A, mat2 B)
{
	mat2 Result = Mat2(A.C[0] + B.C[0],
	                   A.C[1] + B.C[1]);
	return Result;
}

inline v2
Row(mat2 A, u32 RowIndex)
{
	Assert(RowIndex < 2);
	v2 Result = V2(A.M[RowIndex], A.M[RowIndex + 2]);
	return Result;
}

inline mat2
operator*(mat2 A, mat2 B)
{
	mat2 Result = {};
	for(u32 Y = 0;
	    Y < 2;
	    ++Y)
	{
		for(u32 X = 0;
		    X < 2;
		    ++X)
		{
			u32 Index = Y + 2*X;
			Result.M[Index] = Inner(B.C[X], Row(A, Y));
		}
	}

	return Result;
}

inline mat2
Inverse(mat2 A)
{
	f32 Determinant = A.M[0]*A.M[3] - A.M[1]*A.M[2];
	Assert(Determinant != 0.0f);
	f32 InvDeterminant = 1.0f/Determinant;
	mat2 Result =
	{
		InvDeterminant*A.M[3], InvDeterminant*(-A.M[2]),
		InvDeterminant*(-A.M[1]), InvDeterminant*A.M[0],
	};
	return Result;
}

inline mat2
Transpose(mat2 A)
{
	mat2 Result =
	{
		A.M[0], A.M[2],
		A.M[1], A.M[3],
	};
	return Result;
}

inline v2
operator*(mat2 A, v2 V)
{
	v2 Result = V2(Inner(Row(A, 0), V),
	               Inner(Row(A, 1), V));
	return Result;
}

inline mat2
RotationMat2(f32 Rotation)
{
	mat2 Result =
	{
		Cos(Rotation), Sin(Rotation),
		-Sin(Rotation), Cos(Rotation),
	};
	return Result;
}

inline f32
RotationAngleFromMat2(mat2 RotationMat)
{
	f32 Result = Arccos(RotationMat.M[0]);
	if(RotationMat.M[1] < 0.0f)
	{
		Result = 2.0f*Pi32 - Result;
	}
	return Result;
}

inline mat2
ReOrthonormalize(mat2 A)
{
	v2 C0 = Normalize(A.C[0]);
	v2 C1 = Perp(C0);
	mat2 Result = Mat2(C0, C1);
	return Result;
}

inline mat2
Lerp(mat2 A, f32 t, mat2 B)
{
	mat2 Result = {};
	Result.C[0] = Lerp(A.C[0], t, B.C[0]);
	Result.C[1] = Lerp(A.C[1], t, B.C[1]);
	return Result;
}

//
// NOTE: mat3 operations
//

inline mat3
Mat3(v3 C0, v3 C1, v3 C2)
{
	mat3 Result = {};
	Result.C[0] = C0;
	Result.C[1] = C1;
	Result.C[2] = C2;
	return Result;
}

inline mat3
IdentityMat3()
{
	mat3 Result =
	{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
	};
	return Result;
}

inline mat3
operator+(mat3 A, mat3 B)
{
	mat3 Result = Mat3(A.C[0] + B.C[0],
	                   A.C[1] + B.C[1],
	                   A.C[2] + B.C[2]);
	return Result;
}

inline v3
Row(mat3 A, u32 RowIndex)
{
	Assert(RowIndex < 3);
	v3 Result = V3(A.M[RowIndex], A.M[RowIndex + 3], A.M[RowIndex + 6]);
	return Result;
}

inline mat3
operator*(mat3 A, mat3 B)
{
	mat3 Result = {};
	for(u32 Y = 0;
	    Y < 3;
	    ++Y)
	{
		for(u32 X = 0;
		    X < 3;
		    ++X)
		{
			u32 Index = Y + 3*X;
			Result.M[Index] = Inner(B.C[X], Row(A, Y));
		}
	}

	return Result;
}

inline v3
operator*(mat3 A, v3 V)
{
	v3 Result = V3(Inner(Row(A, 0), V),
	               Inner(Row(A, 1), V),
	               Inner(Row(A, 2), V));
	return Result;
}

inline mat3
RotationMat3(r32 Pitch, r32 Yaw)
{
	mat3 PitchRotation =
	{
		1, 0, 0,
		0, Cos(Pitch), Sin(Pitch),
		0, -Sin(Pitch), Cos(Pitch),
	};
	mat3 YawRotation =
	{
		Cos(Yaw), 0, -Sin(Yaw),
		0, 1, 0,
		Sin(Yaw), 0, Cos(Yaw),
	};
	mat3 Result = YawRotation*PitchRotation;
	return Result;
}

//
// NOTE: mat4 operations
//

inline mat4
IdentityMat4()
{
	mat4 Result =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	return Result;
}

inline v4
Row(mat4 A, u32 RowIndex)
{
	Assert(RowIndex < 4);
	v4 Result = V4(A.M[RowIndex], A.M[RowIndex + 4], A.M[RowIndex + 8], A.M[RowIndex + 12]);
	return Result;
}

inline mat4
operator*(mat4 A, mat4 B)
{
	TIMED_BLOCK("mat4*mat4");

	mat4 Result = {};
	for(u32 Y = 0;
	    Y < 4;
	    ++Y)
	{
		for(u32 X = 0;
		    X < 4;
		    ++X)
		{
			u32 Index = Y + 4*X;
			Result.M[Index] = Inner(B.C[X], Row(A, Y));
		}
	}

	return Result;
}

inline v4
operator*(mat4 A, v4 V)
{
	TIMED_BLOCK("mat4*v4");

	v4 Result = V4(Inner(Row(A, 0), V),
	               Inner(Row(A, 1), V),
	               Inner(Row(A, 2), V),
	               Inner(Row(A, 3), V));
	return Result;
}

inline mat4
RotationMat4(r32 Roll)
{
	mat4 Result =
	{
		Cos(Roll), Sin(Roll), 0, 0,
		-Sin(Roll), Cos(Roll), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	return Result;
}

inline mat4
RotationMat4(mat2 M)
{
	mat4 Result =
	{
		M.M[0], M.M[1], 0, 0,
		M.M[2], M.M[3], 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	return Result;
}

inline mat4
RotationMat4(r32 Pitch, r32 Yaw)
{
	mat4 PitchRotation =
	{
		1, 0, 0, 0,
		0, Cos(Pitch), Sin(Pitch), 0,
		0, -Sin(Pitch), Cos(Pitch), 0,
		0, 0, 0, 1,
	};
	mat4 YawRotation =
	{
		Cos(Yaw), 0, -Sin(Yaw), 0,
		0, 1, 0, 0,
		Sin(Yaw), 0, Cos(Yaw), 0,
		0, 0, 0, 1,
	};
	mat4 Result = PitchRotation*YawRotation;
	return Result;
}

inline mat4
TranslationMat4(v2 V)
{
	mat4 Result =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		V.x, V.y, 0, 1,
	};
	return Result;
}

inline mat4
TranslationMat4(v3 V)
{
	mat4 Result =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		V.x, V.y, V.z, 1,
	};
	return Result;
}

inline mat4
ScaleMat4(r32 Scale)
{
	mat4 Result =
	{
		Scale, 0, 0, 0,
		0, Scale, 0, 0,
		0, 0, Scale, 0,
		0, 0, 0, 1,
	};
	return Result;
}

inline mat4
ScaleMat4(v2 Scale)
{
	mat4 Result =
	{
		Scale.x, 0, 0, 0,
		0, Scale.y, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	return Result;
}

inline mat4
ScaleMat4(v3 Scale)
{
	mat4 Result =
	{
		Scale.x, 0, 0, 0,
		0, Scale.y, 0, 0,
		0, 0, Scale.z, 0,
		0, 0, 0, 1,
	};
	return Result;
}

inline mat4
TransformationMat4(v3 P, mat2 Rotation, v2 Scale, v2 Offset)
{
	v2 A = Rotation*Hadamard(Scale, Offset);
	mat4 Result =
	{
		Scale.x*Rotation.M[0], Scale.x*Rotation.M[1], 0, 0,
		Scale.y*Rotation.M[2], Scale.y*Rotation.M[3], 0, 0,
		0, 0, 1, 0,
		A.x + P.x, A.y + P.y, P.z, 1,
	};

	return Result;
}

inline mat4
TransformationMat4(v2 P, mat2 Rotation, v2 Scale, v2 Offset)
{
	mat4 Result = TransformationMat4(V3(P, 0.0f), Rotation, Scale, Offset);
	return Result;
}

//
// NOTE: quaternion operations
//

inline quaternion
Q(r32 r, r32 i, r32 j, r32 k)
{
	quaternion Result = {r, i, j, k};
	return Result;
}

inline quaternion
Q(r32 r, v3 V)
{
	quaternion Result = Q(r, V.x, V.y, V.z);
	return Result;
}

inline quaternion
operator+(quaternion A, quaternion B)
{
	quaternion Result = Q(A.r + B.r,
	                      A.i + B.i,
	                      A.j + B.j,
	                      A.k + B.k);
	return Result;
}

inline quaternion
operator*(quaternion A, quaternion B)
{
	quaternion Result = Q(A.r*B.r - A.i*B.i - A.j*B.j - A.k*B.k,
	                      A.r*B.i + A.i*B.r + A.j*B.k - A.k*B.j,
	                      A.r*B.j + A.j*B.r + A.k*B.i - A.i*B.k,
	                      A.r*B.k + A.k*B.r + A.i*B.j - A.j*B.i);
	return Result;
}

inline quaternion
operator*(r32 A, quaternion B)
{
	quaternion Result = Q(A*B.r, A*B.i, A*B.j, A*B.k);
	return Result;
}

inline r32
LengthSq(quaternion A)
{
	r32 Result = (A.r*A.r + A.i*A.i + A.j*A.j + A.k*A.k);
	return Result;
}

inline r32
Length(quaternion A)
{
	r32 Result = SquareRoot(LengthSq(A));
	return Result;
}

inline b32
IsUnit(quaternion A)
{
	r32 Epsilon = 0.001f;
	b32 Result = ((LengthSq(A) < (1.0f + Square(Epsilon))) &&
	              (LengthSq(A) > (1.0f - Square(Epsilon))));
	return Result;
}

inline quaternion
Normalize(quaternion A)
{
	r32 LengthA = Length(A);
	quaternion Result = Q(A.r/LengthA,
	                      A.i/LengthA,
	                      A.j/LengthA,
	                      A.k/LengthA);
	return Result;
}

inline quaternion
NormalizeIfNeeded(quaternion A)
{
	quaternion Result = IsUnit(A) ? A : Normalize(A);
	return Result;
}

inline quaternion
Conjugate(quaternion A)
{
	quaternion Result = Q(A.r, -A.i, -A.j, -A.k);
	return Result;
}

inline quaternion
Inverse(quaternion A)
{
	quaternion Result = {};
	if(IsUnit(A))
	{
		Result = Conjugate(A);
	}
	else
	{
		Result = (1.0f/LengthSq(A))*Conjugate(A);
	}
	return Result;
}

inline quaternion
RotationQuaternion(v3 Axis, r32 Angle)
{
	quaternion Result = {1, 0, 0, 0};
	if((Angle != 0) &&
	   (LengthSq(Axis) > 0))
	{
		Assert(IsUnit(Axis));
		r32 HalfAngle = 0.5f*Angle;
		r32 SinHalfAngle = Sin(HalfAngle);
		Result = Q(Cos(HalfAngle),
	                Axis.x*SinHalfAngle,
	                Axis.y*SinHalfAngle,
		            Axis.z*SinHalfAngle);
	}

	return Result;
}

inline mat4
RotationMat4(quaternion Q)
{
	Assert(IsUnit(Q));
	r32 TwoRI = 2.0f*Q.r*Q.i;
	r32 TwoRJ = 2.0f*Q.r*Q.j;
	r32 TwoRK = 2.0f*Q.r*Q.k;
	r32 TwoIJ = 2.0f*Q.i*Q.j;
	r32 TwoIK = 2.0f*Q.i*Q.k;
	r32 TwoJK = 2.0f*Q.j*Q.k;
	mat4 Result =
	{
		1.0f - 2.0f*Square(Q.j) - 2.0f*Square(Q.k), TwoIJ + TwoRK, TwoIK - TwoRJ, 0,
		TwoIJ - TwoRK, 1.0f - 2.0f*Square(Q.i) - 2.0f*Square(Q.k), TwoJK + TwoRI, 0,
		TwoIK + TwoRJ, TwoJK - TwoRI, 1.0f - 2.0f*Square(Q.i) - 2.0f*Square(Q.j), 0,
		0, 0, 0, 1
	};
	return Result;
}

inline mat3
RotationMat3(quaternion Q)
{
	Assert(IsUnit(Q));
	r32 TwoRI = 2.0f*Q.r*Q.i;
	r32 TwoRJ = 2.0f*Q.r*Q.j;
	r32 TwoRK = 2.0f*Q.r*Q.k;
	r32 TwoIJ = 2.0f*Q.i*Q.j;
	r32 TwoIK = 2.0f*Q.i*Q.k;
	r32 TwoJK = 2.0f*Q.j*Q.k;
	mat3 Result =
	{
		1.0f - 2.0f*Square(Q.j) - 2.0f*Square(Q.k), TwoIJ + TwoRK, TwoIK - TwoRJ,
		TwoIJ - TwoRK, 1.0f - 2.0f*Square(Q.i) - 2.0f*Square(Q.k), TwoJK + TwoRI,
		TwoIK + TwoRJ, TwoJK - TwoRI, 1.0f - 2.0f*Square(Q.i) - 2.0f*Square(Q.j),
	};
	return Result;
}

inline v3
RotateBy(v3 V, quaternion Rotation)
{
	v3 Result = (Rotation*Q(0.0f, V)*Conjugate(Rotation)).xyz;
	return Result;
}

inline quaternion
Lerp(quaternion A, r32 t, quaternion B)
{
	// NOTE: This is a non-constant speed lerp!
	quaternion Result = Normalize(Q(Lerp(A.r, t, B.r),
	                                Lerp(A.i, t, B.i),
	                                Lerp(A.j, t, B.j),
	                                Lerp(A.k, t, B.k)));
	return Result;
}

inline void
GetPointList(v3 *OutData, rectangle3 Rect, quaternion Rotation, v3 Offset = V3(0,0,0))
{
	v3 HalfDim = 0.5f*Dim(Rect);
	v3 RectCenter = Center(Rect);
	*(OutData + 0) = RotateBy(HalfDim, Rotation) + RectCenter;
	*(OutData + 1) = RotateBy(V3(-HalfDim.x, HalfDim.y, HalfDim.z), Rotation) + RectCenter;
	*(OutData + 2) = RotateBy(V3(HalfDim.x, -HalfDim.y, HalfDim.z), Rotation) + RectCenter;
	*(OutData + 3) = RotateBy(V3(HalfDim.x, HalfDim.y, -HalfDim.z), Rotation) + RectCenter;
	*(OutData + 4) = RotateBy(V3(-HalfDim.x, -HalfDim.y, HalfDim.z), Rotation) + RectCenter;
	*(OutData + 5) = RotateBy(V3(HalfDim.x, -HalfDim.y, -HalfDim.z), Rotation) + RectCenter;
	*(OutData + 6) = RotateBy(V3(-HalfDim.x, HalfDim.y, -HalfDim.z), Rotation) + RectCenter;
	*(OutData + 7) = RotateBy(V3(-HalfDim.x, -HalfDim.y, -HalfDim.z), Rotation) + RectCenter;
}

inline quaternion
GetRotationComponent(quaternion A, v3 Axis)
{
	quaternion Result = Q(1,0,0,0);

	r32 RotationAngle = 2.0f*Arccos(A.r);
	if(RotationAngle != 0.0f)
	{
		r32 InvHalfSinAngle = 1.0f/Sin(0.5f*RotationAngle);
		v3 RotationAxis = V3(A.i*InvHalfSinAngle, A.j*InvHalfSinAngle, A.k*InvHalfSinAngle);
		v3 NewRotationAxis = NOZ(Inner(RotationAxis, Axis)*Axis);
		if(LengthSq(NewRotationAxis) == 0.0f)
		{
			Result = A;
		}
		else
		{
			r32 NewRotationAngle = RotationAngle*Inner(RotationAxis, Axis);
			Result = RotationQuaternion(Axis, NewRotationAngle);
		}
	}

	return Result;
}

//
// NOTE: sphere operations
//

inline sphere
Sphere(v3 Center, r32 Radius)
{
	sphere Result = {Center, Radius};
	return Result;
}

inline v3
SphereRadiusVector(r32 Radius, r32 Phi, r32 Theta)
{
	v3 Result = Radius*V3(Sin(Phi)*Cos(Theta), -Cos(Phi), Sin(Phi)*Sin(Theta));
	return Result;
}
