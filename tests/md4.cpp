#include <cstdint>
#include <cstring>
#include <random>
#include <vector>
#include <array>

#include "../code/qcommon/md4.h"

/**
I've rewritten the MD4 code to be nicer; however that is of little use if it is not correct.
*/


using byte = unsigned char;

#include <boost/test/unit_test.hpp>

namespace original
{
	typedef struct mdfour_s
	{
		uint32_t A, B, C, D;
		uint32_t totalN;
	} mdfour_ctx;

	static  mdfour_ctx *m;

	#define F(X,Y,Z) (((X)&(Y)) | ((~(X))&(Z)))
	#define G(X,Y,Z) (((X)&(Y)) | ((X)&(Z)) | ((Y)&(Z)))
	#define H(X,Y,Z) ((X)^(Y)^(Z))
	#define lshift(x,s) (((x)<<(s)) | ((x)>>(32-(s))))

	#define ROUND1(a,b,c,d,k,s) a = lshift(a + F(b,c,d) + X[k], s)
	#define ROUND2(a,b,c,d,k,s) a = lshift(a + G(b,c,d) + X[k] + 0x5A827999,s)
	#define ROUND3(a,b,c,d,k,s) a = lshift(a + H(b,c,d) + X[k] + 0x6ED9EBA1,s)

	/* this applies md4 to 64 byte chunks */
	static void mdfour64( uint32_t *M )
	{
		int j;
		uint32_t AA, BB, CC, DD;
		uint32_t X[ 16 ];
		uint32_t A, B, C, D;

		for( j = 0; j<16; j++ )
			X[ j ] = M[ j ];

		A = m->A; B = m->B; C = m->C; D = m->D;
		AA = A; BB = B; CC = C; DD = D;

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

		A += AA; B += BB; C += CC; D += DD;

		for( j = 0; j<16; j++ )
			X[ j ] = 0;

		m->A = A; m->B = B; m->C = C; m->D = D;
	}

	static void copy64( uint32_t *M, const byte *in )
	{
		int i;

		for( i = 0; i<16; i++ )
			M[ i ] = ( in[ i * 4 + 3 ] << 24 ) | ( in[ i * 4 + 2 ] << 16 ) |
			( in[ i * 4 + 1 ] << 8 ) | ( in[ i * 4 + 0 ] << 0 );
	}

	static void copy4( byte *out, uint32_t x )
	{
		out[ 0 ] = x & 0xFF;
		out[ 1 ] = ( x >> 8 ) & 0xFF;
		out[ 2 ] = ( x >> 16 ) & 0xFF;
		out[ 3 ] = ( x >> 24 ) & 0xFF;
	}

	static void mdfour_begin( mdfour_ctx *md )
	{
		md->A = 0x67452301;
		md->B = 0xefcdab89;
		md->C = 0x98badcfe;
		md->D = 0x10325476;
		md->totalN = 0;
	}


	static void mdfour_tail( const byte *in, int n )
	{
		byte buf[ 128 ];
		uint32_t M[ 16 ];
		uint32_t b;

		m->totalN += n;

		b = m->totalN * 8;

		std::memset( buf, 0, 128 );
		if( n ) std::memcpy( buf, in, n );
		buf[ n ] = 0x80;

		if( n <= 55 )
		{
			copy4( buf + 56, b );
			copy64( M, buf );
			mdfour64( M );
		}
		else
		{
			copy4( buf + 120, b );
			copy64( M, buf );
			mdfour64( M );
			copy64( M, buf + 64 );
			mdfour64( M );
		}
	}

	static void mdfour_update( mdfour_ctx *md, const byte *in, int n )
	{
		uint32_t M[ 16 ];

		m = md;

		if( n == 0 ) mdfour_tail( in, n );

		while( n >= 64 )
		{
			copy64( M, in );
			mdfour64( M );
			in += 64;
			n -= 64;
			m->totalN += 64;
		}

		mdfour_tail( in, n );
	}


	static void mdfour_result( mdfour_ctx *md, byte *out )
	{
		m = md;

		copy4( out, m->A );
		copy4( out + 4, m->B );
		copy4( out + 8, m->C );
		copy4( out + 12, m->D );
	}

	static void mdfour( byte *out, const byte *in, int n )
	{
		mdfour_ctx md;
		mdfour_begin( &md );
		mdfour_update( &md, in, n );
		mdfour_result( &md, out );
	}

	//===================================================================

	std::uint32_t Com_BlockChecksum( const byte *buffer, int length )
	{
		std::uint32_t digest[ 4 ];

		mdfour( reinterpret_cast< byte* >( digest ), buffer, length );

		return digest[ 0 ] ^ digest[ 1 ] ^ digest[ 2 ] ^ digest[ 3 ];
	}
}

static void randomData( std::vector< byte >& output, std::size_t length )
{
	static std::mt19937 engine;
	std::uniform_int_distribution< int > distribution( std::numeric_limits< byte >::min(), std::numeric_limits< byte >::max() );
	output.clear();
	output.reserve( length );
	for( std::size_t i = 0; i < length; ++i )
	{
		output.push_back( distribution( engine ) );
	}
}

BOOST_AUTO_TEST_SUITE( md4 )

BOOST_AUTO_TEST_CASE( blockChecksum )
{
	const int max = 1024;
	std::vector< byte > data;
	data.reserve( max );
	for( int length = 0; length <= max; ++length )
	{
		randomData( data, length );

		BOOST_CHECK_EQUAL( original::Com_BlockChecksum( data.data(), data.size() ), Com::BlockChecksum( data ) );
	}
}

BOOST_AUTO_TEST_SUITE_END() // md4
