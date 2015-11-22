#pragma once

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long ulong;

typedef enum { qfalse=0, qtrue } qboolean;
#define	qboolean	int		//don't want strict type checking on the qboolean

#if defined (_MSC_VER) && (_MSC_VER >= 1600)

	#include <stdint.h>

	// vsnprintf is ISO/IEC 9899:1999
	// abstracting this to make it portable
	int Q_vsnprintf( char *str, size_t size, const char *format, va_list args );

#elif defined (_MSC_VER)

	#include <io.h>

	typedef signed __int64 int64_t;
	typedef signed __int32 int32_t;
	typedef signed __int16 int16_t;
	typedef signed __int8  int8_t;
	typedef unsigned __int64 uint64_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int16 uint16_t;
	typedef unsigned __int8  uint8_t;

	// vsnprintf is ISO/IEC 9899:1999
	// abstracting this to make it portable
	int Q_vsnprintf( char *str, size_t size, const char *format, va_list args );
#else // not using MSVC

	#if !defined(__STDC_LIMIT_MACROS)
	#define __STDC_LIMIT_MACROS
	#endif
	#include <stdint.h>

	#define Q_vsnprintf vsnprintf

#endif