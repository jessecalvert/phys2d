/*@H
* File: phys2d_intrinsics.h
* Author: Jesse Calvert
* Created: October 12, 2017, 11:44
* Last modified: October 12, 2017, 11:44
*/

#pragma once

#ifndef PHYS2D_INTRINSICS_H

#include <math.h>

inline int32
SignOf(int32 Value)
{
    int32 Result = (Value >= 0) ? 1 : -1;
    return Result;
}

inline real32
SignOf(real32 Value)
{
    real32 Result = (Value >= 0) ? 1.0f : -1.0f;
    return Result;
}

inline r32
Square(r32 Value)
{
    r32 Result = Value*Value;
    return Result;
}

inline r32
Pow(r32 Base, r32 Exp)
{
    r32 Result = powf(Base, Exp);
    return Result;
}

inline real32
SquareRoot(real32 Real32)
{
    real32 Result = sqrtf(Real32);
    return Result;
}

inline uint32
RotateLeft(uint32 Value, int32 Amount)
{
#if COMPILER_MSVC
    uint32 Result = _rotl(Value, Amount);
#else
    //TODO: actually port this to other compiler platforms
    Amount &= 31;
    uint32 Result = ((Value << Amount) | (Value >> (32 - Amount)));
#endif
    return Result;
}

inline uint32
RotateRight(uint32 Value, int32 Amount)
{
#if COMPILER_MSVC
    uint32 Result = _rotr(Value, Amount);
#else
    // TODO: actually port this to other compiler platforms
    Amount &= 31;
    uint32 Result = ((Value >> Amount) | (Value << (32 - Amount)));
#endif
    return Result;
}

inline real32
AbsoluteValue(real32 Real32)
{
    real32 Result = (r32) fabs(Real32);
    return Result;
}

inline int32
RoundReal32ToInt32(real32 Value)
{
    int32 Result = (int32) roundf(Value);
    // TODO: intrinsic?
    return Result;
}

inline uint32
RoundReal32ToUInt32(real32 Value)
{
    uint32 Result = (uint32)roundf(Value);
    // TODO: intrinsic?
    return Result;
}

inline int32
FloorReal32ToInt32(real32 Value)
{
    int32 Result = (int32)floorf(Value);
    return Result;
}

inline int32
CeilReal32ToInt32(real32 Value)
{
    int32 Result = (int32)ceilf(Value);
    return Result;
}

inline int32
TruncateReal32ToInt32(real32 Value)
{
    int32 Result = (int32) (Value);
    return Result;
}

inline real32
Sin(real32 Angle)
{
    real32 Result = sinf(Angle);
    return Result;
}

inline real32
Cos(real32 Angle)
{
    real32 Result = cosf(Angle);
    return Result;
}

inline real32
Tan(real32 Angle)
{
    real32 Result = tanf(Angle);
    return Result;
}

inline real32
Arccos(r32 Angle)
{
    real32 Result = (r32) acos(Angle);
    return Result;
}

inline real32
ATan2(real32 Y, real32 X)
{
    real32 Result = (r32) atan2(Y, X);
    return Result;
}

inline r32
LogBase2(r32 A)
{
    r32 Result = log2f(A);
    return Result;
}

inline r32
LogBase10(r32 A)
{
    r32 Result = log10f(A);
    return Result;
}

struct bit_scan_result
{
    bool32 Found;
    uint32 Index;
};

inline bit_scan_result
FindLeastSignificantSetBit(uint32 Value)
{
    bit_scan_result Result = {};

#if COMPILER_MSVC
    Result.Found = _BitScanForward((unsigned long *)&Result.Index, Value);
#else
    for (uint32 Test = 0; Test < 32; ++Test)
    {
        if(Value & (1 << Test))
        {
            Result.Index = Test;
            Result.Found = true;
            break;
        }
    }
#endif

    return Result;
}

#define PHYS2D_INTRINSICS_H
#endif
