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

//-----------------------------------------------------------
// to do :
//

/*
static void Toggle(unsigned char& bitflag, unsigned char bit)
static void SetMask(unsigned char& bitflag, unsigned char mask)
static unsigned char SetBitsExcept(unsigned char& bitflag, unsigned char mask)
static unsigned short int ClearBitsExcept(unsigned short int& bitflag, unsigned short int mask)
static int PosFromBitValue(int bitvalue)
static unsigned char PopCount(unsigned char& bitflag)
static unsigned char ZeroPopCount(unsigned char& bitflag)
static int ExtractLSB(unsigned char* bitflag)
static void SetMask(unsigned char& bitflag, unsigned char mask)
*/


namespace vml
{
	namespace bits
	{
			
			class BitArray 
			{
			
				// --------------------------------------------------------------
				// bit proxy class 

				class BitProxy
				{

					public:

						BitProxy(BitArray& ba, size_t bit) : m_ba(ba), m_bit(bit) {}

						operator bool()
						{
							return ((m_ba.Compressed[m_bit >> 3] & (1 << (m_bit % 8))) != 0);
						}

						BitProxy& operator = (bool v)
						{
							if (v) m_ba.Compressed[m_bit >> 3] |= 1 << (m_bit % 8);
							else m_ba.Compressed[m_bit >> 3] &= ~(1 << (m_bit % 8));
							return *this;
						}

						bool operator ~()
						{
							if (((m_ba.Compressed[m_bit >> 3] & (1 << (m_bit % 8))) != 0))	return false;
							return true;
						}

						BitArray& m_ba;
						size_t m_bit;
				};
		
				//-----------------------------------------------------------
				// private data

				unsigned char *Compressed;			// compressed bit flag array
				size_t		   Elements;			// number of elements
				size_t		   Bytes;				// occupancy in bytes
		
			public:

				//-----------------------------------------------------------
				// sets bit 'i' to true

				void SetToTrue(const size_t i)
				{
					Compressed[i >> 3] |= 1 << (i % 8);
				}

				//-----------------------------------------------------------
				// sets bit 'i' to true

				void SetToFalse(const size_t i)
				{
					Compressed[i >> 3] &= ~(1 << (i % 8));
				}

				//-----------------------------------------------------------
				// gets bit at 'i' position

				bool Get(const size_t i) 
				{
					return ((Compressed[i >> 3] & (1 << (i % 8))) != 0);
				}

				//-----------------------------------------------------------
				// clear all

				void Clear()
				{
					memset(Compressed, 0, sizeof(unsigned char)*Bytes);
				}

				// -----------------------------------------------------------
				// flips all bits in the flag array

				void Flip()
				{
					for (size_t i = 0; i < Bytes; ++i) 
						Compressed[i] = ~Compressed[i];
				}

				// -----------------------------------------------------------
				// test if any bit is set

				bool IsAnyBitSet()
				{
					for (size_t i = 0; i < Bytes; ++i)
						if (Compressed[i] != 0) return true;
					return false;
				}

				// -----------------------------------------------------------
				// test if all bits are set

				bool AreAllBitsSet()
				{
					for (size_t i = 0; i < Elements; ++i)
						if (!((Compressed[i >> 3] & (1 << (i % 8))) != 0))
							return false;
					return true;
				}

				// -----------------------------------------------------------
				// test if all bits are set

				bool AreAllBitsClear()
				{
					for (size_t i = 0; i < Elements; ++i)
						if (((Compressed[i >> 3] & (1 << (i % 8))) != 0))
							return false;
					return true;
				}

				// -----------------------------------------------------------
				// converts bit flag array in a string

				const std::string ConvertToString()
				{
					std::string text;
					text.resize(Elements);
					for (size_t i = 0; i < Elements; ++i)
					{
						if (Compressed[i >> 3] & (1 << (i % 8)))
							text[i] = '1';
						else text[i] = '0';
					}
					return text;
				}
		
				// -------------------------------------------------------------
				// converts a string into a bit flag array

				void ConvertFromString(const std::string &text)
				{
					Elements = text.size();
					Bytes = (int)ceil((float)Elements / 8.0f);
					if (Compressed) delete Compressed;
					Compressed = new unsigned char[Bytes];
					memset(Compressed, 0, Bytes * sizeof(unsigned char));
					for (size_t i = 0; i < Elements; ++i)
					{
						switch (text.at(i))
						{
							case '1': Compressed[i >> 3] |= (1 << (i % 8)); break;
							case '0': Compressed[i >> 3] &= ~(1 << (i % 8)); break;
							default: 
										// error 
							break;
						}
					}
				}
		
				//-----------------------------------------------------------
				// getters

				size_t GetElementsCount() const { return Elements; }
				size_t GetBytesCount() const { return Bytes; }
				const unsigned char *GetBitFlagArray() const { return Compressed; }

				//-----------------------------------------------------------
				// bitwise operations

				// ----------------------------------------------------------
				// negate all the array

				BitArray operator ~()
				{
					if (Compressed)
					{
						BitArray c;
						c.Elements = Elements;
						c.Bytes = Bytes;
						c.Compressed = new unsigned char[c.Bytes];
						for (size_t i = 0; i < c.Bytes; ++i)
							c.Compressed[i] = ~Compressed[i];
						return c;
					}
					else
					{
						throw;

					}
				}

				// ----------------------------------------------------------
				// compound and assignement

				BitArray operator&= (const BitArray& b)
				{
					if (Bytes != b.GetBytesCount())
					{
						throw;
					}

					if (Compressed && b.Compressed)
					{
						for (size_t i = 0; i < Bytes; ++i)
							Compressed[i] &= b.Compressed[i];
						return *this;
					}
					else
					{
						throw;
					}

				}

				// ----------------------------------------------------------
				// compound or assignement

				BitArray operator|= (const BitArray& b)
				{
					if (Bytes != b.GetBytesCount())
					{
						throw;
					}

					if (Compressed && b.Compressed)
					{
						for (size_t i = 0; i < Bytes; ++i)
							Compressed[i] |= b.Compressed[i];
						return *this;
					}
					else
					{
						throw;
					}
				}
				// ----------------------------------------------------------
				// compound xor assignement

				BitArray operator^= (const BitArray& b)
				{
					if (Bytes != b.GetBytesCount())
					{
						throw;
					}

					if (Compressed && b.Compressed)
					{
						for (size_t i = 0; i < Bytes; ++i)
							Compressed[i] ^= b.Compressed[i];
						return *this;
					}
					else
					{
						throw;
					}

				}

				// ----------------------------------------------------------
				// compound not assignement
				//
				// * there is no operator for this function *

				friend BitArray operator& (const BitArray& a, const BitArray& b);
				friend BitArray operator| (const BitArray& a, const BitArray& b);
				friend BitArray operator^ (const BitArray& a, const BitArray& b);
				friend bool operator== (const BitArray& a, const BitArray& b);
				friend bool operator!= (const BitArray& a, const BitArray& b);
				friend BitArray operator& (const BitArray& a, const BitArray& b);

				// -----------------------------------------------------------
				// proxy [] access operator both for reading and writing
				// the compressed bit array

				BitProxy operator [](size_t i)
				{
					return BitProxy(*this, i);
				}

				//-----------------------------------------------------------
				// ctor / dtor

				// construct with a given number of elements

				BitArray(size_t elements)
				{
					if (elements == 0)	
					{ 
						// throw error
					}

					Elements = elements;
					Bytes = (int)ceil((float)Elements / 8.0f);
					Compressed = new unsigned char[Bytes];
					memset(Compressed, 0, sizeof(unsigned char)*Bytes);
				}

				// copy constructor

				BitArray(const BitArray &b)
				{
					Elements = b.Elements;
					Bytes = b.Bytes;
					if (Bytes) Compressed = new unsigned char[Bytes];
					else Compressed = nullptr;
					if ( Compressed ) memcpy(Compressed, b.Compressed, sizeof(unsigned char)*Bytes);
				}

				// assignement operator

				BitArray &operator=(const BitArray &b)
				{
					if (this != &b)
					{
						Elements = b.Elements;
						Bytes = b.Bytes;
						if (Compressed) { delete Compressed; Compressed = nullptr; }
						if (Bytes) Compressed = new unsigned char[Bytes];
						else Compressed = nullptr;
						if (Compressed) memcpy(Compressed, b.Compressed, sizeof(unsigned char)*Bytes);
					}

					return *this;
				}
		
				// parameterless constructor

				BitArray()
				{
					Compressed = nullptr;
					Elements = 0;
					Bytes = 0;
				}

				// construct from a string

				BitArray(const std::string &text)
				{
					ConvertFromString(text);
				}

				// destructor

				~BitArray()
				{
					if (Compressed) delete Compressed;
				}
		
			};

			////////////////////////////////////////////////////////////////////////////////////
			// and operation

			static BitArray operator& (const BitArray& a, const BitArray& b)
			{
				if (a.GetBytesCount() != b.GetBytesCount()) 
				{ 
					// throw error
				}

				BitArray c;
				c.Elements = a.Elements;
				c.Bytes = a.Bytes;
				c.Compressed = new unsigned char[c.Bytes];
				for (size_t i = 0; i < c.Bytes; ++i)
					c.Compressed[i] = a.Compressed[i] & b.Compressed[i];
				return c;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// or operation

			static BitArray operator| (const BitArray& a, const BitArray& b)
			{
				BitArray c;
				if (a.GetBytesCount() != b.GetBytesCount())
				{
					c.Elements = a.Elements;
					c.Bytes = a.Bytes;
					c.Compressed = new unsigned char[c.Bytes];
					for (size_t i = 0; i < c.Bytes; ++i)
						c.Compressed[i] = a.Compressed[i] | b.Compressed[i];
				}
				return c;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// xor operation

			static BitArray operator^ (const BitArray& a, const BitArray& b)
			{
				if (a.GetBytesCount() != b.GetBytesCount()) 
				{
							// throw error
				}

				BitArray c;
				c.Elements = a.Elements;
				c.Bytes = a.Bytes;
				c.Compressed = new unsigned char[c.Bytes];
				for (size_t i = 0; i < c.Bytes; ++i)
					c.Compressed[i] = a.Compressed[i] ^ b.Compressed[i];
				return c;
			}

			//////////////////////////////////////////////////////////////////////

			static bool operator==(const BitArray& a, const BitArray& b)
			{
				if (a.Compressed && b.Compressed)
				{
					if (a.Bytes != b.Bytes)
						return false;
					for (size_t i = 0; i < a.Bytes; ++i)
						if (a.Compressed[i] != b.Compressed[i]) return false;
					return true;
				}
				return false;
			}
			
			///////////////////////////////////////////////////////////////////////

			static bool operator!=(const BitArray& a, const BitArray& b)
			{
				if (a.Compressed && b.Compressed )
				{
					if (a.Bytes != b.Bytes)
						return true;
					for (size_t i = 0; i < a.Bytes; ++i)
						if (a.Compressed[i] == b.Compressed[i]) return false;
					return true;
				}
				return false;
			}

	} // end of namespace bits
	
} // end of namespace vml
