#pragma once

	// Unsigned base types.
	
	typedef unsigned char 		byte;		// 8-bit  unsigned.
	typedef unsigned char 		uint8;		// 8-bit  unsigned.
	typedef unsigned short int	uint16;		// 16-bit unsigned.
	typedef unsigned int		uint32;		// 32-bit unsigned.
	typedef unsigned long long	uint64;		// 64-bit unsigned.

	// Signed base types.
	typedef	signed char			int8;		// 8-bit  signed.
	typedef signed short int	int16;		// 16-bit signed.
	typedef signed int	 		int32;		// 32-bit signed.
	typedef signed long long	int64;		// 64-bit signed.
	
	typedef float				float32;    // 32-bit float
	typedef double				float64;    // 64-bit float

	#if BS_COMPILER == BS_COMPILER_MSVC
		typedef unsigned __int64 uint64;
		typedef __int64 int64;
	#else
		typedef unsigned long long UINT64;
		typedef long long INT64;
	#endif
		
