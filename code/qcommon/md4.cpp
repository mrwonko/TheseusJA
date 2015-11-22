/*
mdfour.c

An implementation of MD4 designed for use in the samba SMB
authentication protocol

Copyright (C) 1997-1998  Andrew Tridgell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to:

Free Software Foundation, Inc.
59 Temple Place - Suite 330
Boston, MA  02111-1307, USA

$Id: mdfour.c,v 1.1 2002/08/23 22:03:27 abster Exp $
*/

#include "md4.h"

#include <array>
#include <cassert>
#include <algorithm>
#include <functional>

using uint128 = std::array< std::uint32_t, 4 >;
using uint512 = std::array< std::uint32_t, 16 >;

static uint128& operator+=( uint128& lhs, const uint128& rhs )
{
	std::transform( lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), std::plus< std::uint32_t >() );
	return lhs;
}

struct mdfour_context
{
	uint128 value = { {
		0x67452301,
		0xefcdab89,
		0x98badcfe,
		0x10325476
	} };
	uint64_t numBytes = 0;
};


/* NOTE: This code makes no attempt to be fast!

It assumes that an int is at least 32 bits long
*/

#define F(X,Y,Z) (((X)&(Y)) | ((~(X))&(Z)))
#define G(X,Y,Z) (((X)&(Y)) | ((X)&(Z)) | ((Y)&(Z)))
#define H(X,Y,Z) ((X)^(Y)^(Z))
#define lshift(x,s) (((x)<<(s)) | ((x)>>(32-(s))))

#define ROUND1(a,b,c,d,k,s) a = lshift(a + F(b,c,d) + input[k], s)
#define ROUND2(a,b,c,d,k,s) a = lshift(a + G(b,c,d) + input[k] + 0x5A827999,s)
#define ROUND3(a,b,c,d,k,s) a = lshift(a + H(b,c,d) + input[k] + 0x6ED9EBA1,s)

/* this applies md4 to 64 byte chunks */
static void mdfour64( const uint512& input, mdfour_context& ctx )
{
	uint128 value = ctx.value;
	uint32_t& A = value[ 0 ];
	uint32_t& B = value[ 1 ];
	uint32_t& C = value[ 2 ];
	uint32_t& D = value[ 3 ];

	ROUND1( A, B, C, D, 0, 3 );  ROUND1( D, A, B, C, 1, 7 );
	ROUND1( C, D, A, B, 2, 11 );  ROUND1( B, C, D, A, 3, 19 );
	ROUND1( A, B, C, D, 4, 3 );  ROUND1( D, A, B, C, 5, 7 );
	ROUND1( C, D, A, B, 6, 11 );  ROUND1( B, C, D, A, 7, 19 );
	ROUND1( A, B, C, D, 8, 3 );  ROUND1( D, A, B, C, 9, 7 );
	ROUND1( C, D, A, B, 10, 11 );  ROUND1( B, C, D, A, 11, 19 );
	ROUND1( A, B, C, D, 12, 3 );  ROUND1( D, A, B, C, 13, 7 );
	ROUND1( C, D, A, B, 14, 11 );  ROUND1( B, C, D, A, 15, 19 );

	ROUND2( A, B, C, D, 0, 3 );  ROUND2( D, A, B, C, 4, 5 );
	ROUND2( C, D, A, B, 8, 9 );  ROUND2( B, C, D, A, 12, 13 );
	ROUND2( A, B, C, D, 1, 3 );  ROUND2( D, A, B, C, 5, 5 );
	ROUND2( C, D, A, B, 9, 9 );  ROUND2( B, C, D, A, 13, 13 );
	ROUND2( A, B, C, D, 2, 3 );  ROUND2( D, A, B, C, 6, 5 );
	ROUND2( C, D, A, B, 10, 9 );  ROUND2( B, C, D, A, 14, 13 );
	ROUND2( A, B, C, D, 3, 3 );  ROUND2( D, A, B, C, 7, 5 );
	ROUND2( C, D, A, B, 11, 9 );  ROUND2( B, C, D, A, 15, 13 );

	ROUND3( A, B, C, D, 0, 3 );  ROUND3( D, A, B, C, 8, 9 );
	ROUND3( C, D, A, B, 4, 11 );  ROUND3( B, C, D, A, 12, 15 );
	ROUND3( A, B, C, D, 2, 3 );  ROUND3( D, A, B, C, 10, 9 );
	ROUND3( C, D, A, B, 6, 11 );  ROUND3( B, C, D, A, 14, 15 );
	ROUND3( A, B, C, D, 1, 3 );  ROUND3( D, A, B, C, 9, 9 );
	ROUND3( C, D, A, B, 5, 11 );  ROUND3( B, C, D, A, 13, 15 );
	ROUND3( A, B, C, D, 3, 3 );  ROUND3( D, A, B, C, 11, 9 );
	ROUND3( C, D, A, B, 7, 11 );  ROUND3( B, C, D, A, 15, 15 );

	ctx.value += value;
}

static uint512 copy64( const gsl::array_view< const byte >& input )
{
	assert( input.size() >= 64 );
	uint512 result;
	auto it = input.begin();
	for( uint32_t& val : result )
	{
		val = 0;
		for( unsigned int i = 0; i < 4; ++i )
		{
			val |= ( *it++ ) << ( i * 8 );
		}
	}
	return result;
}

static void copy8( gsl::array_view< byte > out, const uint64_t x )
{
	assert( out.size() >= 8 );
	for( unsigned int i = 0; i < 8; ++i )
	{
		out[ i ] = ( x >> ( 8 * i ) ) & 0xFF;
	}
}

static void mdfour_tail( const gsl::array_view< const byte >& input, mdfour_context& ctx )
{
	assert( input.size() < 64 );

	ctx.numBytes += input.size();
	std::uint64_t numBits = ctx.numBytes * 8;

	std::array< byte, 64 > buf{}; // 0-initialized
	std::copy( input.begin(), input.end(), buf.begin() );
	
	// add a 1 bit, then fill with 0 bits until there are 64 bit remaining (for bit count)
	buf[ input.size() ] = 0x80;

	// does the bit count still fit in this 512 bit block?
	const bool needExtraBlock = input.size() + 1 + 8 > 64;

	if( !needExtraBlock )
	{
		copy8( { buf.data() + ( buf.size() - 8 ), buf.data() + buf.size() }, numBits );
	}
	uint512 data = copy64( buf );
	mdfour64( data, ctx );
	if( needExtraBlock )
	{
		data = {};
		data[ 14 ] = numBits & 0xFFFFFFFF;
		data[ 15 ] = ( numBits >> 32 ) & 0xFFFFFFFF;
		mdfour64( data, ctx );
	}
}

static uint128 mdfour( gsl::array_view< const byte > input )
{
	mdfour_context ctx;
	
	if( input.size() == 0 )
	{
		// this happens twice for size 0; but can't change that or savegame compat might break.
		mdfour_tail( input, ctx );
	}

	while( input.size() >= 64 )
	{
		uint512 data = copy64( input );
		mdfour64( data, ctx );
		ctx.numBytes += 64;
		input = { input.begin() + 64, input.end() };
	}
	mdfour_tail( input, ctx );
	return ctx.value;
}

//===================================================================

std::uint32_t Com::BlockChecksum( const gsl::array_view< const byte >& buffer )
{
	uint128 digest = mdfour( buffer );

	return digest[0] ^ digest[1] ^ digest[2] ^ digest[3];
}
