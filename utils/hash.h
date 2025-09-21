#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2020 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

namespace vml
{
	namespace hash
	{

		// -----------------------------------------------------------
		// RS hash function

		static unsigned int RSHash(char* str, unsigned int len)
		{
			unsigned int b = 378551;
			unsigned int a = 63689;
			unsigned int hash = 0;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash = hash * a + (*str);
				a = a * b;
			}

			return hash;
		}

		// -----------------------------------------------------------
		// JS hash function

		static unsigned int JSHash(char* str, unsigned int len)
		{
			unsigned int hash = 1315423911;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash ^= ((hash << 5) + (*str) + (hash >> 2));
			}

			return hash;
		}
		/*
		// -----------------------------------------------------------
		// P. J. Weinberger hash function 

		static unsigned int PJWHash(char* str, unsigned int len)
		{
			const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
			const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
			const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
			const unsigned int HighBits=(unsigned int)(0xFFFFFFFF) << ((unsigned int)(BitsInUnsignedInt - OneEighth));
			unsigned int hash = 0;
			unsigned int test = 0;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash = (hash << OneEighth) + (*str);

				if ((test = hash & HighBits) != 0)
				{
					hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
				}
			}

			return hash;
		}
		*/
		/*
		// -----------------------------------------------------------
		// ELF hash function 

		static unsigned int ELFHash(char* str, unsigned int len)
		{
			unsigned int hash = 0;
			unsigned int x = 0;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash = (hash << 4) + (*str);
				if ((x = hash & 0xF0000000L) != 0)
				{
					hash ^= (x >> 24);
				}
				hash &= ~x;
			}

			return hash;
		}
		*/
		// -----------------------------------------------------------
		// BKDR hash function 

		static unsigned int BKDRHash(char* str, unsigned int len)
		{
			unsigned int seed = 131; // 31 131 1313 13131 131313 etc.. 
			unsigned int hash = 0;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash = (hash * seed) + (*str);
			}

			return hash;
		}

		// -----------------------------------------------------------
		// SDBM hash function 

		static unsigned int SDBMHash(char* str, unsigned int len)
		{
			unsigned int hash = 0;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash = (*str) + (hash << 6) + (hash << 16) - hash;
			}

			return hash;
		}

		// -----------------------------------------------------------
		// DJB hash function 

		static unsigned int DJBHash(char* str, unsigned int len)
		{
			unsigned int hash = 5381;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash = ((hash << 5) + hash) + (*str);
			}

			return hash;
		}

		// -----------------------------------------------------------
		// DEK hash function 

		static unsigned int DEKHash(char* str, unsigned int len)
		{
			unsigned int hash = len;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
			}

			return hash;
		}

		// -----------------------------------------------------------
		// BP hash function 

		static unsigned int BPHash(char* str, unsigned int len)
		{
			unsigned int hash = 0;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash = hash << 7 ^ (*str);
			}

			return hash;
		}

		// -----------------------------------------------------------
		// End Of FNV hash function 

		static unsigned int FNVHash(char* str, unsigned int len)
		{
			const unsigned int fnv_prime = 0x811C9DC5;
			unsigned int hash = 0;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash *= fnv_prime;
				hash ^= (*str);
			}

			return hash;
		}

		// -----------------------------------------------------------
		// AP hash function

		static unsigned int APHash(char* str, unsigned int len)
		{
			unsigned int hash = 0xAAAAAAAA;
			unsigned int i = 0;

			for (i = 0; i < len; str++, i++)
			{
				hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
					(~((hash << 11) + ((*str) ^ (hash >> 5))));
			}

			return hash;
		}

	} // end of hash namespace

} // end of vml namespace
