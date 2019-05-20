/*@H
* File: phys2d_shared.h
* Author: Jesse Calvert
* Created: October 9, 2017, 12:23
* Last modified: November 27, 2017, 12:38
*/

#pragma once

#include <stdarg.h>

inline u32
SafeTruncateUInt64(u64 Value)
{
    // TODO: defines for maximum values, like UInt32Max
    Assert(Value <= 0xFFFFFFFF);
    u32 Result = (u32)Value;
    return Result;
}

inline u16
SafeTruncateToU16(u32 Value)
{
    // TODO: defines for maximum values, like UInt32Max
    Assert(Value <= 0xFFFF);
    u16 Result = (u16)Value;
    return Result;
}

#define DLIST_INIT(Sentinel) (Sentinel)->Next = (Sentinel)->Prev = (Sentinel)

#define DLIST_INSERT(Sentinel, Node) \
(Node)->Next = (Sentinel)->Next; \
(Node)->Prev = (Sentinel); \
(Node)->Next->Prev = (Node); \
(Node)->Prev->Next = (Node)

#define DLIST_REMOVE(Node) \
(Node)->Prev->Next = (Node)->Next; \
(Node)->Next->Prev = (Node)->Prev; \
(Node)->Next = (Node)->Prev = 0

inline b32
IsPowerOf2(u32 Value)
{
	b32 Result = (Value & (Value - 1)) == 0;
	return Result;
}

#include "phys2d_intrinsics.h"
#include "phys2d_math.h"
#include "phys2d_memory.h"

struct string
{
	u32 Length;
	char *Text;
};

internal u32
CStyleStringLength(char *Text)
{
	u32 Length = 0;
	while(*Text)
	{
		++Text;
		++Length;
	}
	return Length;
}

inline string
String(char *CStyleString)
{
	string Result = {};
	Result.Text = CStyleString;
	Result.Length = CStyleStringLength(CStyleString);
	return Result;
}

#if 0
inline char *
MakeCString(memory_region *Region, string String)
{
	char *Result = PushArray(Region, String.Length + 1, char);
	for(u32 Index = 0;
		Index < String.Length;
		++Index)
	{
		Result[Index] = String.Text[Index];
	}

	Result[String.Length] = 0;
	return Result;
}
#endif

inline b32
StringCompare(string A, string B)
{
	b32 Result = (A.Length == B.Length);
	for(u32 Index = 0;
		Result && (Index < A.Length);
		++Index)
	{
		Result = (A.Text[Index] == B.Text[Index]);
	}

	return Result;
}

inline b32
StringCompare(string A, char *B)
{
	b32 Result = StringCompare(A, String(B));
	return Result;
}

inline b32
StringStartsWith(string String, string Test)
{
	b32 Result = (String.Length >= Test.Length);
	for(u32 Index = 0;
		Result && (Index < Test.Length);
		++Index)
	{
		Result = (String.Text[Index] == Test.Text[Index]);
	}

	return Result;
}

inline b32
StringStartsWith(string A, char *Test)
{
	b32 Result = StringStartsWith(A, String(Test));
	return Result;
}

internal u32
StringHexToU32(char *CStyleString)
{
	u32 Result = 0;
	char *Scan = CStyleString;

	if((Scan[0] == '0') && ((Scan[1] == 'x') || (Scan[1] == 'X')))
	{
		Scan += 2;
	}

	while(*Scan)
	{
		if((*Scan >= '0') &&
		   (*Scan <= '9'))
		{
			Result *= 16;
			Result += (*Scan - '0');
			++Scan;
		}
		else if((*Scan >= 'A') &&
		        (*Scan <= 'F'))
		{
			Result *= 16;
			Result += (*Scan - 'A');
			++Scan;
		}
		else if((*Scan >= 'a') &&
		   		(*Scan <= 'f'))
		{
			Result *= 16;
			Result += (*Scan - 'a');
			++Scan;
		}
		else
		{
			break;
		}
	}

	return Result;
}

internal s32
StringToS32(char *CStyleString)
{
	s32 Result = 0;
	b32 IsNegative = false;
	char *Scan = CStyleString;
	if(*Scan == '-')
	{
		IsNegative = true;
	}

	while(*Scan)
	{
		if((*Scan >= '0') &&
		   (*Scan <= '9'))
		{
			Result *= 10;
			Result += (*Scan - '0');
			++Scan;
		}
		else
		{
			break;
		}
	}

	Result = IsNegative ? -Result : Result;
	return Result;
}

internal s32
StringToS32(string String)
{
	s32 Result = StringToS32(String.Text);
	return Result;
}

struct format_builder
{
	char *Dest;
	umm Size;
};

inline void
OutChar(format_builder *FormatBuilder, char Char)
{
	*FormatBuilder->Dest++ = Char;
	++FormatBuilder->Size;
}

internal void
OutPositiveIntegerBackwards_(format_builder *FB, u32 Integer, u32 MinimumWidth, u32 Base, char *Digits)
{
	Assert(Integer >= 0);
	u32 Length = 0;
	if(Integer)
	{
		while(Integer)
		{
			char Char = Digits[Integer % Base];
			Integer /= Base;
			OutChar(FB, Char);
			++Length;
		}
	}
	else
	{
		OutChar(FB, Digits[0]);
		++Length;
	}

	if(Length < MinimumWidth)
	{
		for(u32 Index = 0;
			Index < (MinimumWidth - Length);
			++Index)
		{
			OutChar(FB, '0');
		}
	}
}

internal void
OutInteger(format_builder *FB, s32 Integer, char Prepend, char PadChar, u32 MinimumWidth, b32 LeftJustified)
{
	u32 Base = 10;
	char *DigitsUppercase = "0123456789ABCDEF";

	char TempBuffer[256];
	format_builder TempFB = {TempBuffer, 0};
	if(Integer < 0)
	{
		Integer = -Integer;
		Prepend = '-';
	}

	OutPositiveIntegerBackwards_(&TempFB, Integer, 0, Base, DigitsUppercase);
	if(Prepend)
	{
		OutChar(&TempFB, Prepend);
	}

	if((TempFB.Size < MinimumWidth) && !LeftJustified)
	{
		for(u32 Index = 0;
			Index < (MinimumWidth - TempFB.Size);
			++Index)
		{
			OutChar(FB, PadChar);
		}
	}
	for(s32 Index = (s32)(TempFB.Size - 1);
		Index >= 0;
		--Index)
	{
		char Char = TempBuffer[Index];
		OutChar(FB, Char);
	}
	if((TempFB.Size < MinimumWidth) && LeftJustified)
	{
		for(u32 Index = 0;
			Index < (MinimumWidth - TempFB.Size);
			++Index)
		{
			OutChar(FB, PadChar);
		}
	}
}

internal void
OutUnsignedInteger(format_builder *FB, u32 Integer, char Prepend, char PadChar, u32 MinimumWidth, b32 LeftJustified, u32 Base, b32 Uppercase = false)
{
	Assert((Base == 10) || (Base == 16));
	char *DigitsLowercase = "0123456789abcdef";
	char *DigitsUppercase = "0123456789ABCDEF";

	char *Digits = DigitsLowercase;
	if(Uppercase)
	{
		Digits = DigitsLowercase;
	}

	char TempBuffer[256];
	format_builder TempFB = {TempBuffer, 0};

	OutPositiveIntegerBackwards_(&TempFB, Integer, 0, Base, Digits);
	if(Prepend)
	{
		OutChar(&TempFB, Prepend);
	}

	if((TempFB.Size < MinimumWidth) && !LeftJustified)
	{
		for(u32 Index = 0;
			Index < (MinimumWidth - TempFB.Size);
			++Index)
		{
			OutChar(FB, PadChar);
		}
	}
	for(s32 Index = (s32)(TempFB.Size - 1);
		Index >= 0;
		--Index)
	{
		char Char = TempBuffer[Index];
		OutChar(FB, Char);
	}
	if((TempFB.Size < MinimumWidth) && LeftJustified)
	{
		for(u32 Index = 0;
			Index < (MinimumWidth - TempFB.Size);
			++Index)
		{
			OutChar(FB, PadChar);
		}
	}
}

internal void
OutFloat(format_builder *FB, r64 Float, char Prepend, char PadChar, u32 MinimumWidth, b32 LeftJustified, u32 Precision)
{
	u32 Base = 10;
	char *Digits = "0123456789ABCDEF";

	char TempBuffer[256];
	format_builder TempFB = {TempBuffer, 0};
	if(Float < 0)
	{
		Float = -Float;
		Prepend = '-';
	}

	s32 IntegerPart = (s32)Float;
	r64 FractionalPart = Float - (r64)IntegerPart;
	for(u32 PrecisionIndex = 0;
		PrecisionIndex < Precision;
		++PrecisionIndex)
	{
		FractionalPart *= 10.0;
	}
	s32 FractionalPartInt = (s32)FractionalPart;
	OutPositiveIntegerBackwards_(&TempFB, FractionalPartInt, Precision, Base, Digits);

	if(Precision)
	{
		OutChar(&TempFB, '.');
	}

	OutPositiveIntegerBackwards_(&TempFB, IntegerPart, 0, Base, Digits);

	if(Prepend)
	{
		OutChar(&TempFB, Prepend);
	}

	if((TempFB.Size < MinimumWidth) && !LeftJustified)
	{
		for(u32 Index = 0;
			Index < (MinimumWidth - TempFB.Size);
			++Index)
		{
			OutChar(FB, PadChar);
		}
	}
	for(s32 Index = (s32)(TempFB.Size - 1);
		Index >= 0;
		--Index)
	{
		char Char = TempBuffer[Index];
		OutChar(FB, Char);
	}
	if((TempFB.Size < MinimumWidth) && LeftJustified)
	{
		for(u32 Index = 0;
			Index < (MinimumWidth - TempFB.Size);
			++Index)
		{
			OutChar(FB, PadChar);
		}
	}
}

inline b32
IsDecimalNumber(char Char)
{
	b32 Result = ((Char >= '0') && (Char <= '9'));
	return Result;
}

inline b32
IsHexNumber(char Char)
{
	b32 Result = IsDecimalNumber(Char) ||
		((Char >= 'a') && (Char <= 'f')) ||
		((Char >= 'A') && (Char <= 'F'));
	return Result;
}

inline b32
IsAlpha(char Char)
{
	b32 Result = (((Char >= 'a') && (Char <= 'z')) ||
				  ((Char >= 'A') && (Char <= 'Z')));
	return Result;
}

internal string
FormatStringList(char *Buffer, umm BufferSize, char *FormatInit, va_list ArgList)
{
	// TODO: Support the [length] specifier

	format_builder FB = {};
	FB.Dest = Buffer;

	char *Format = FormatInit;

	while(*Format && (FB.Size < BufferSize-1))
	{
		if(*Format == '%')
		{
			++Format;

			b32 LeftJustified = false;
			char PrependChar = 0;
			char PadChar = ' ';
			u32 MinimumFieldWidth = 0;
			u32 Precision = 6;
			b32 ReadingOptions = true;
			while(ReadingOptions)
			{
				switch(*Format)
				{
					case '-': {LeftJustified = true; ++Format;} break;
					case '+': {PrependChar = '+'; ++Format;} break;
					case ' ': {PrependChar = ' '; ++Format;} break;
					case '0': {PadChar = '0'; ++Format;} break;
					default:
					{
						ReadingOptions = false;
					}
				}
			}

			if(*Format == '*')
			{
				MinimumFieldWidth = va_arg(ArgList, s32);
				++Format;
			}
			else
			{
				Assert(MinimumFieldWidth == 0);
				MinimumFieldWidth = StringToS32(Format);
				while(IsDecimalNumber(*Format)) {++Format;}
			}
			Assert(MinimumFieldWidth >= 0);

			if(*Format == '.')
			{
				++Format;
				if(*Format == '*')
				{
					Precision = va_arg(ArgList, s32);
					++Format;
				}
				else
				{
					Precision = StringToS32(Format);
					while(IsDecimalNumber(*Format)) {++Format;}
				}
			}
			Assert(Precision >= 0);

			switch(*Format)
			{
				case 'f':
				{
					r64 Float = va_arg(ArgList, r64);
					OutFloat(&FB, Float, PrependChar, PadChar, MinimumFieldWidth, LeftJustified, Precision);
				} break;

				case 'd':
				{
					s32 Integer = va_arg(ArgList, s32);
					OutInteger(&FB, Integer, PrependChar, PadChar, MinimumFieldWidth, LeftJustified);
				} break;

				case 'u':
				{
					s32 Integer = va_arg(ArgList, u32);
					OutUnsignedInteger(&FB, Integer, PrependChar, PadChar, MinimumFieldWidth, LeftJustified, 10);
				} break;

				case 'x':
				{
					s32 Integer = va_arg(ArgList, u32);
					OutUnsignedInteger(&FB, Integer, PrependChar, PadChar, MinimumFieldWidth, LeftJustified, 16, false);
				} break;

				case 'X':
				{
					s32 Integer = va_arg(ArgList, u32);
					OutUnsignedInteger(&FB, Integer, PrependChar, PadChar, MinimumFieldWidth, LeftJustified, 16, true);
				} break;

				case 's':
				{
					string String = va_arg(ArgList, string);
					if((String.Length < MinimumFieldWidth) && !LeftJustified)
					{
						for(u32 PadIndex = 0;
							PadIndex < MinimumFieldWidth - String.Length;
							++PadIndex)
						{
							OutChar(&FB, PadChar);
						}
					}
					for(u32 CharIndex = 0;
						CharIndex < String.Length;
						++CharIndex)
					{
						char Character = String.Text[CharIndex];
						OutChar(&FB, Character);
					}
					if((String.Length < MinimumFieldWidth) && LeftJustified)
					{
						for(u32 PadIndex = 0;
							PadIndex < MinimumFieldWidth - String.Length;
							++PadIndex)
						{
							OutChar(&FB, PadChar);
						}
					}
				} break;

				InvalidDefaultCase;
			}

			Format++;
		}
		else
		{
			OutChar(&FB, *Format++);
		}
	}

	OutChar(&FB, 0);

	// TODO: Store the new string in a new buffer. (Global memory_region?)
	string Result = {};
	Result.Text = Buffer;
	Result.Length = SafeTruncateUInt64(FB.Size) - 1;
	return Result;
}

internal string
FormatString(char *Buffer, umm BufferSize, char *Format, ...)
{
	// TODO: Is there a safe way to check that there are the right amout of
	//	arguments, and they are the right type?

	va_list ArgList;
	va_start(ArgList, Format);
	string Result = FormatStringList(Buffer, BufferSize, Format, ArgList);
	va_end(ArgList);

	return Result;
}
