#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
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

///////////////////////////////
// 3x3 matrix class

namespace vml
{
	namespace math
	{

		template <typename T>
		class mat3x3
		{

			private:

				T Data[9];

			public:

				// --------------------------------------------------------------------
				// access operators

				__forceinline T& operator[](const size_t i) 
				{
					return Data[i];
				}

				__forceinline const T& operator[](const size_t i)  const
				{
					return Data[i];
				}

				__forceinline const T& operator()(const size_t i, const size_t j) const
				{
					return Data[i + j * 3];
				}

				// --------------------------------------------------------------
				// casting to pointer

				__forceinline operator T* () const
				{
					return (T*)Data;
				}

				__forceinline operator const T* () const
				{
					return (const T*)Data;
				}

				// --------------------------------------------------------------
				// artimetic operators

				__forceinline const mat3x3 operator -()
				{
					return mat3x3(-Data[0], -Data[1], -Data[2],
								  -Data[3], -Data[4], -Data[5],
								  -Data[6], -Data[7], -Data[8]);
				}

				__forceinline const mat3x3 operator +()
				{
					return *this;
				}

				__forceinline const mat3x3& operator +=(const mat3x3& M)
				{
					Data[0] += M.Data[0];	Data[1] += M.Data[1];	Data[2] += M.Data[2];
					Data[3] += M.Data[3];	Data[4] += M.Data[4];	Data[5] += M.Data[5];
					Data[6] += M.Data[6];	Data[7] += M.Data[7];	Data[8] += M.Data[8];
					return *this;
				}

				__forceinline const mat3x3& operator +=(const T s)
				{
					Data[0] += s; Data[1] += s;	Data[2] += s;
					Data[3] += s; Data[4] += s;	Data[5] += s;
					Data[6] += s; Data[7] += s;	Data[8] += s;
					return *this;
				}

				__forceinline const mat3x3& operator -=(const mat3x3& M)
				{
					Data[0] -= M.Data[0];	Data[1] -= M.Data[1];	Data[2] -= M.Data[2];
					Data[3] -= M.Data[3];	Data[4] -= M.Data[4];	Data[5] -= M.Data[5];
					Data[6] -= M.Data[6];	Data[7] -= M.Data[7];	Data[8] -= M.Data[8];
					return *this;
				}

				__forceinline const mat3x3& operator -=(const T s)
				{
					Data[0] -= s; Data[1] -= s;	Data[2] -= s;
					Data[3] -= s; Data[4] -= s;	Data[5] -= s;
					Data[6] -= s; Data[7] -= s;	Data[8] -= s;
					return *this;
				}

				__forceinline const mat3x3& operator *=(const mat3x3& M)
				{
					T  t[9] = { Data[0] * M.Data[0] + Data[1] * M.Data[3] + Data[2] * M.Data[6],
							    Data[0] * M.Data[1] + Data[1] * M.Data[4] + Data[2] * M.Data[7],
							    Data[0] * M.Data[2] + Data[1] * M.Data[5] + Data[2] * M.Data[8],
								Data[3] * M.Data[0] + Data[4] * M.Data[3] + Data[5] * M.Data[6],
								Data[3] * M.Data[1] + Data[4] * M.Data[4] + Data[5] * M.Data[7],
								Data[3] * M.Data[2] + Data[4] * M.Data[5] + Data[5] * M.Data[8],
								Data[6] * M.Data[0] + Data[7] * M.Data[3] + Data[8] * M.Data[6],
								Data[6] * M.Data[1] + Data[7] * M.Data[4] + Data[8] * M.Data[7],
								Data[6] * M.Data[2] + Data[7] * M.Data[5] + Data[8] * M.Data[8] };
				
					Data[0] = t[0];
					Data[1] = t[1];
					Data[2] = t[2];
					Data[3] = t[3];
					Data[4] = t[4];
					Data[5] = t[5];
					Data[6] = t[6];
					Data[7] = t[7];
					Data[8] = t[8];
					
					return *this;
				}

				__forceinline const mat3x3& operator *=(const T s)
				{
					Data[0] *= s; Data[1] *= s; Data[2] *= s;
					Data[3] *= s; Data[4] *= s; Data[5] *= s;
					Data[6] *= s; Data[7] *= s; Data[8] *= s;
					return *this;
				}

				__forceinline const mat3x3& operator /=(const mat3x3& R)
				{
					T det = (R.Data[0] * R.Data[4] * R.Data[8] - R.Data[0] * R.Data[5] * R.Data[7]) +
							(R.Data[1] * R.Data[5] * R.Data[6] - R.Data[1] * R.Data[3] * R.Data[8]) +
							(R.Data[2] * R.Data[3] * R.Data[7] - R.Data[2] * R.Data[4] * R.Data[6]);

					// cannot invert singular R.Dataatrix

					if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
						vml::os::Error("Matrix3x3 : Determinant is null");

					T m00 = R.Data[4] * R.Data[8] - R.Data[5] * R.Data[7];
					T m01 = R.Data[2] * R.Data[7] - R.Data[1] * R.Data[8];
					T m02 = R.Data[1] * R.Data[5] - R.Data[2] * R.Data[4];
					T m10 = R.Data[5] * R.Data[6] - R.Data[3] * R.Data[8];
					T m11 = R.Data[0] * R.Data[8] - R.Data[2] * R.Data[6];
					T m12 = R.Data[2] * R.Data[3] - R.Data[0] * R.Data[5];
					T m20 = R.Data[3] * R.Data[7] - R.Data[6] * R.Data[4];
					T m21 = R.Data[1] * R.Data[6] - R.Data[0] * R.Data[7];
					T m22 = R.Data[0] * R.Data[4] - R.Data[1] * R.Data[3];

					det = T(1) / det;

					m00 *= det; m01 *= det; m02 *= det;
					m10 *= det; m11 *= det; m12 *= det;
					m20 *= det; m21 *= det; m22 *= det;

					T  t[9] = { Data[0] * m00 + Data[1] * m10 + Data[2] * m20,
								Data[0] * m01 + Data[1] * m11 + Data[2] * m21,
								Data[0] * m02 + Data[1] * m12 + Data[2] * m22,
								Data[3] * m00 + Data[4] * m10 + Data[5] * m20,
								Data[3] * m01 + Data[4] * m11 + Data[5] * m21,
								Data[3] * m02 + Data[4] * m12 + Data[5] * m22,
								Data[6] * m00 + Data[7] * m10 + Data[8] * m20,
								Data[6] * m01 + Data[7] * m11 + Data[8] * m21,
								Data[6] * m02 + Data[7] * m12 + Data[8] * m22 };

					Data[0] = t[0];
					Data[1] = t[1];
					Data[2] = t[2];
					Data[3] = t[3];
					Data[4] = t[4];
					Data[5] = t[5];
					Data[6] = t[6];
					Data[7] = t[7];
					Data[8] = t[8];

					return *this;
				}

				__forceinline const mat3x3& operator /=(const T s)
				{
					double invs = T(1) / T(s);
					if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
						vml::os::Error("Matrix3x3 : scalar is null");
					Data[0] *= invs; Data[1] *= invs; Data[2] *= invs;
					Data[3] *= invs; Data[4] *= invs; Data[5] *= invs;
					Data[6] *= invs; Data[7] *= invs; Data[8] *= invs;
					return *this;
				}

				// --------------------------------------------------------------
				// comparison operators

				__forceinline bool operator== (const mat3x3& M) const
				{
					return memcmp(Data, M.Data, 9 * sizeof(T)) == 0;
				}

				__forceinline bool operator!= (const mat3x3& M) const
				{
					return memcmp(Data, M.Data, 9 * sizeof(T)) != 0;
				}

				__forceinline bool operator<= (const mat3x3& M) const
				{
					return memcmp(Data, M.Data, 9 * sizeof(T)) <= 0;
				}

				__forceinline bool operator>= (const mat3x3& M) const
				{
					return memcmp(Data, M.Data, 9 * sizeof(T)) >= 0;
				}

				__forceinline bool operator> (const mat3x3& M) const
				{
					return memcmp(Data, M.Data, 9 * sizeof(T)) > 0;
				}

				__forceinline bool operator< (const mat3x3& M) const
				{
					return memcmp(Data, M.Data, 9 * sizeof(T)) < 0;
				}

				// --------------------------------------------------------------------
				// query functions

				__forceinline const T* GetData() const
				{
					return Data;
				}

				__forceinline T* GetData()
				{
					return Data;
				}
		
				// index at i,j coords

				__forceinline int Index(const int i, const int j)
				{
					return  i + j * 3;
				}
			
				// --------------------------------------------------------------
				// ctor / dtor

				mat3x3()
				{
					Data[0] = T(1); Data[1] = T(0); Data[2] = T(0);
					Data[3] = T(0); Data[4] = T(1); Data[5] = T(0);
					Data[6] = T(0); Data[7] = T(0); Data[8] = T(1);
				}

				mat3x3(T m11, T m12, T m13,
					   T m21, T m22, T m23,
					   T m31, T m32, T m33)
				{
					Data[0] = m11; Data[1] = m12; Data[2] = m13;
					Data[3] = m21; Data[4] = m22; Data[5] = m23;
					Data[6] = m31; Data[7] = m32; Data[8] = m33;
				}

				// ---------------------------------------------------------------
				// assignment operator
				// if you assign different order matrices
				// the dest matrix will take the source's order

				const mat3x3& operator =(const mat3x3& M)
				{
					if (&M == this)
						return *this;
					Data[0] = M.Data[0]; Data[1] = M.Data[1]; Data[2] = M.Data[2];
					Data[3] = M.Data[3]; Data[4] = M.Data[4]; Data[5] = M.Data[5];
					Data[6] = M.Data[6]; Data[7] = M.Data[7]; Data[8] = M.Data[8];
					return *this;
				}

				// ---------------------------------------------------------------
				// copy constructor
				// if you assign different order matrices
				// the dest matrix will take the source's order
				
				mat3x3(const mat3x3& M)
				{
					Data[0] = M.Data[0]; Data[1] = M.Data[1]; Data[2] = M.Data[2];
					Data[3] = M.Data[3]; Data[4] = M.Data[4]; Data[5] = M.Data[5];
					Data[6] = M.Data[6]; Data[7] = M.Data[7]; Data[8] = M.Data[8];
				}
				
				~mat3x3()
				{
				}

			};

			/////////////////////////////////////////////////////////////////////////////////
			// friend functions

			template <typename T>
			__forceinline const mat3x3<T> operator +(const mat3x3<T>& R, const mat3x3<T>& L)
			{
				return mat3x3<T>(L[0] + R[0], L[1] + R[1], L[2] + R[2],
								 L[3] + R[3], L[4] + R[4], L[5] + R[5],
								 L[6] + R[6], L[7] + R[7], L[8] + R[8]);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator +(const T a, const mat3x3<T>& R)
			{
				return mat3x3<T>(a + R[0], a + R[1], a + R[2],
								 a + R[3], a + R[4], a + R[5],
								 a + R[6], a + R[7], a + R[8]);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator +( const mat3x3<T>& R, const T a )
			{
				return mat3x3<T>(a + R[0], a + R[1], a + R[2],
								 a + R[3], a + R[4], a + R[5],
								 a + R[6], a + R[7], a + R[8]);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator -(const mat3x3<T>& R, const mat3x3<T>& L)
			{
				return mat3x3<T>(L[0] - R[0], L[1] - R[1], L[2] - R[2],
								 L[3] - R[3], L[4] - R[4], L[5] - R[5],
								 L[6] - R[6], L[7] - R[7], L[8] - R[8]);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator -(const T a, const mat3x3<T>& R)
			{
				return mat3x3<T>(a - R[0], a - R[1], a - R[2],
								 a - R[3], a - R[4], a - R[5],
								 a - R[6], a - R[7], a - R[8]);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator -(const mat3x3<T>& R, const T a)
			{
				return mat3x3<T>(a - R[0], a - R[1], a - R[2],
								 a - R[3], a - R[4], a - R[5],
								 a - R[6], a - R[7], a - R[8]);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator *(const mat3x3<T>& R, const T s)
			{
				return mat3x3<T>(R[0] * s, R[1] * s, R[2] * s,
								 R[3] * s, R[4] * s, R[5] * s,
								 R[6] * s, R[7] * s, R[8] * s);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator *(const T s, const mat3x3<T>& R)
			{
				return mat3x3<T>(R[0] * s, R[1] * s, R[2] * s,
								 R[3] * s, R[4] * s, R[5] * s,
								 R[6] * s, R[7] * s, R[8] * s);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator *(const mat3x3<T>& L, const mat3x3<T>& R)
			{
				return mat3x3<T>(L[0] * R[0] + L[1] * R[3] + L[2] * R[6],
								 L[0] * R[1] + L[1] * R[4] + L[2] * R[7],
								 L[0] * R[2] + L[1] * R[5] + L[2] * R[8],
								 L[3] * R[0] + L[4] * R[3] + L[5] * R[6],
								 L[3] * R[1] + L[4] * R[4] + L[5] * R[7],
								 L[3] * R[2] + L[4] * R[5] + L[5] * R[8],
								 L[6] * R[0] + L[7] * R[3] + L[8] * R[6],
								 L[6] * R[1] + L[7] * R[4] + L[8] * R[7],
								 L[6] * R[2] + L[7] * R[5] + L[8] * R[8]);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator /(const mat3x3<T>& R, const T s)
			{
				if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
					vml::os::Error("Mat2x2 : scalar is null");
				T invs = T(1) / s;
				return mat3x3<T>(R[0] * invs, R[1] * invs, R[2] * invs,
								 R[3] * invs, R[4] * invs, R[5] * invs,
								 R[6] * invs, R[7] * invs, R[8] * invs);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator /(const T s, const mat3x3<T>& R)
			{
				T det = R[0] * R[4] * R[8] - R[0] * R[5] * R[7] +
						R[1] * R[5] * R[6] - R[1] * R[3] * R[8] +
						R[2] * R[3] * R[7] - R[2] * R[4] * R[6];

				// cannot invert singular matrix

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Error("Matrix3x3 : Determinant is null");

				T m00 = R[0], m01 = R[3], m02 = R[6];
				T m10 = R[1], m11 = R[4], m12 = R[7];
				T m20 = R[2], m21 = R[5], m22 = R[8];

				T  t[9] = { m11 * m22 - m21 * m12,
							m20 * m12 - m10 * m22,
							m10 * m21 - m20 * m11,
							m21 * m02 - m01 * m22,
							m00 * m22 - m20 * m02,
							m20 * m01 - m00 * m21,
							m01 * m12 - m02 * m11,
							m10 * m02 - m00 * m12,
							m00 * m11 - m10 * m01 };

				det = s / det;

				return mat3x3<T>(t[0] * det, t[1] * det, t[2] * det,
								 t[3] * det, t[4] * det, t[5] * det,
								 t[6] * det, t[7] * det, t[8] * det);
			}

			template <typename T>
			__forceinline const mat3x3<T> operator /(const mat3x3<T>& R, const mat3x3<T>& L)
			{
				T det = L[0] * L[4] * L[8] - L[0] * L[5] * L[7] +
						L[1] * L[5] * L[6] - L[1] * L[3] * L[8] +
						L[2] * L[3] * L[7] - L[2] * L[4] * L[6];

				// cannot invert singular matrix

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Error("Matrix3x3 : Determinant is null");

				T m00 = L[0], m01 = L[3], m02 = L[6];
				T m10 = L[1], m11 = L[4], m12 = L[7];
				T m20 = L[2], m21 = L[5], m22 = L[8];

				T  t[9] = { m11 * m22 - m21 * m12,
							m20 * m12 - m10 * m22,
							m10 * m21 - m20 * m11,
							m21 * m02 - m01 * m22,
							m00 * m22 - m20 * m02,
							m20 * m01 - m00 * m21,
							m01 * m12 - m02 * m11,
							m10 * m02 - m00 * m12,
							m00 * m11 - m10 * m01 };

				det = 1.0f / det;

				t[0] *= det; t[1] *= det; t[2] *= det;
				t[3] *= det; t[4] *= det; t[5] *= det;
				t[6] *= det; t[7] *= det; t[8] *= det;

				return mat3x3<T>(R[0] * t[0] + R[1] * t[3] + R[2] * t[6],
								 R[0] * t[1] + R[1] * t[4] + R[2] * t[7],
								 R[0] * t[2] + R[1] * t[5] + R[2] * t[8],
								 R[3] * t[0] + R[4] * t[3] + R[5] * t[6],
								 R[3] * t[1] + R[4] * t[4] + R[5] * t[7],
								 R[3] * t[2] + R[4] * t[5] + R[5] * t[8],
								 R[6] * t[0] + R[7] * t[3] + R[8] * t[6],
								 R[6] * t[1] + R[7] * t[4] + R[8] * t[7],
								 R[6] * t[2] + R[7] * t[5] + R[8] * t[8]);
			}


			///////////////////////////////////////////////////////////////////////
			// vector operations

			// row vector by a matrix

			template <typename T>
			__forceinline const vec3<T> operator *(const vec3<T>& L, const mat3x3<T>& R)
			{
				return vec3<T>(L[0] * R[0] + L[1] * R[3] + L[2] * R[6],
							   L[0] * R[1] + L[1] * R[4] + L[2] * R[7],
							   L[0] * R[2] + L[1] * R[5] + L[2] * R[8]);
			}

			template <typename T>
			__forceinline const vec3<T> operator *(const mat3x3<T>& R, const vec3<T>& L )
			{
				return vec3<T>(L[0] * R[0] + L[1] * R[3] + L[2] * R[6],
							   L[0] * R[1] + L[1] * R[4] + L[2] * R[7],
							   L[0] * R[2] + L[1] * R[5] + L[2] * R[8]);
			}
			
			template <typename T >
			__forceinline std::ostream& operator<< (std::ostream& output, const vml::math::mat3x3<T>& M)
			{
					std::ostringstream os;
			//		os << "ciao" << "\n";
					return os;

					/*
					std::string strbuffer[9];
					size_t strlength[9] = { 0,0,0,0,0,0,0,0 };
					
					int maxlenght = 0;

					for (size_t j = 0; j < 3; ++j)
					{
						for (size_t i = 0; i < 3; ++i)
						{
							int index = i + j * 3;
							std::ostringstream os;
							os << M[index];
							int lenght = os.str().length();
							strbuffer[index] = os.str();
							strlength[index] = lenght;
							if (lenght > maxlenght) maxlenght = lenght;
						}
					}

					maxlenght *= 2;
					maxlenght += 1;

					std::ostringstream os;

					for (size_t j = 0; j < 3; ++j)
					{
						for (size_t i = 0; i < 3; ++i)
						{
							int index = i + j * 3;

							os << strbuffer[index] << std::setw(maxlenght - strlength[index]);
						}

						os << "\n";
					}

					return os;
					*/
			}
			
	} // end of namespace math

} // end of namesapce vml


///////////////////////////////////////////////////////////////////////////////////
// predefined types

namespace vml
{
	namespace math
	{

		typedef mat3x3<byte>	mat3x3b;	// 8-bit  unsigned.
		typedef mat3x3<uint8>	mat3x3ui8;	// 8-bit  unsigned.
		typedef mat3x3<uint16>	mat3x3ui16;	// 16-bit unsigned.
		typedef mat3x3<uint32>  mat3x3ui32;	// 32-bit unsigned.
		typedef mat3x3<uint64>  mat3x3ui64;	// 64-bit unsigned.
		typedef mat3x3<int8>	mat3x3i8;	// 8-bit  signed.
		typedef mat3x3<int16>	mat3x3i16;	// 16-bit signed.
		typedef mat3x3<int32>	mat3x3i32;	// 32-bit signed.
		typedef mat3x3<int64>	mat3x3i64;	// 64-bit signed.
		typedef mat3x3<float32> mat3x3f;	// 32-bit float
		typedef mat3x3<float64> mat3x3d;	// 64-bit float

	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// matrix3x3 utilities

namespace vml
{
	namespace math
	{
		namespace mat3x3utils
		{
			// --------------------------------------------------------
			// metrics functions

			template <typename T >
			__forceinline const T Determinant(const mat3x3<T>& R)
			{
				return (R[0] * R[4] * R[8] +
						R[1] * R[5] * R[6] +
						R[2] * R[3] * R[7] -
						R[2] * R[4] * R[6] -
						R[5] * R[7] * R[0] -
						R[8] * R[1] * R[3]);
			}

			// matrix norm
			template <typename T >
			__forceinline const T Norm(const mat3x3<T>& R)
			{
				return sqrtf(R[0] * R[0] + R[1] * R[1] + R[2] * R[2] +
							 R[3] * R[3] + R[4] * R[4] + R[5] * R[5] +
							 R[6] * R[6] + R[7] * R[7] + R[8] * R[8]);
			}

			// ---------------------------------------------------------------
			// computes the trace for anxn matrix
			// the sum of all elelemnts of the main diagonal

			template <typename T >
			__forceinline const T Trace(const mat3x3<T>& R)
			{
				return  R[0] + R[4] + R[8];
			}

			// ---------------------------------------------------------------
			// computes the rank for a 3x3 matrix

			template <typename T >
			__forceinline int Rank(const mat3x3<T>& R)
			{
				if (R[0] * R[4] * R[8] +
					R[1] * R[5] * R[6] +
					R[2] * R[3] * R[7] -
					R[2] * R[4] * R[6] -
					R[5] * R[7] * R[0] -
					R[8] * R[1] * R[3] != 0.0f) return 3;

				if (R[4] * R[8] - R[5] * R[7] != 0.0f) return 2;
				if (R[3] * R[8] - R[5] * R[6] != 0.0f) return 2;
				if (R[3] * R[7] - R[4] * R[6] != 0.0f) return 2;
				if (R[1] * R[8] - R[2] * R[7] != 0.0f) return 2;
				if (R[0] * R[8] - R[2] * R[6] != 0.0f) return 2;
				if (R[0] * R[7] - R[1] * R[6] != 0.0f) return 2;
				if (R[1] * R[5] - R[2] * R[4] != 0.0f) return 2;
				if (R[0] * R[5] - R[2] * R[3] != 0.0f) return 2;
				if (R[0] * R[4] - R[1] * R[3] != 0.0f) return 2;

				if (R[0] != 0.0f) return 1;
				if (R[1] != 0.0f) return 1;
				if (R[2] != 0.0f) return 1;
				if (R[3] != 0.0f) return 1;
				if (R[4] != 0.0f) return 1;
				if (R[5] != 0.0f) return 1;
				if (R[6] != 0.0f) return 1;
				if (R[7] != 0.0f) return 1;
				if (R[8] != 0.0f) return 1;

				return 0;
			}

			// compute normalized distance from maximum eleement

			template <typename T>
			__forceinline mat3x3<T> NormalizedDistance(const mat3x3<T>& R)
			{
				T invmax = R[0];

				if (R[1] > invmax) invmax = R[1];
				if (R[2] > invmax) invmax = R[2];
				if (R[3] > invmax) invmax = R[3];
				if (R[4] > invmax) invmax = R[4];
				if (R[5] > invmax) invmax = R[5];
				if (R[6] > invmax) invmax = R[6];
				if (R[7] > invmax) invmax = R[7];
				if (R[8] > invmax) invmax = R[8];

				if (invmax > -vml::math::EPSILON && invmax < vml::math::EPSILON)
					vml::os::Error("Mat3x3 : max is null");

				invmax = T(1) / invmax;

				return mat3x3<T>(R[0] * invmax, R[1] * invmax, R[2] * invmax,
								 R[3] * invmax, R[4] * invmax, R[5] * invmax,
								 R[6] * invmax, R[7] * invmax, R[8] * invmax);
			}

			// ---------------------------------------------------------------
			// matrix inversion

			template <typename T >
			__forceinline mat3x3<T> Invert(const mat3x3<T>& R)
			{
				T det = R[0] * R[4] * R[8] - R[0] * R[5] * R[7] +
						R[1] * R[5] * R[6] - R[1] * R[3] * R[8] +
						R[2] * R[3] * R[7] - R[2] * R[4] * R[6];

				// cannot invert singular matrix

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Error("Mat3x3 : Determinant is null");

				T m00 = R[0], m01 = R[3], m02 = R[6];
				T m10 = R[1], m11 = R[4], m12 = R[7];
				T m20 = R[2], m21 = R[5], m22 = R[8];

				R[0] = m11 * m22 - m21 * m12;
				R[1] = m20 * m12 - m10 * m22;
				R[2] = m10 * m21 - m20 * m11;
				R[3] = m21 * m02 - m01 * m22;
				R[4] = m00 * m22 - m20 * m02;
				R[5] = m20 * m01 - m00 * m21;
				R[6] = m01 * m12 - m02 * m11;
				R[7] = m10 * m02 - m00 * m12;
				R[8] = m00 * m11 - m10 * m01;

				det = 1.0f / det;

				return mat3x3<T> ( R[0] * det, R[1] *det, R[2] * det,
								   R[3] * det, R[4] *det, R[5] * det,
								   R[6] * det, R[7] *det, R[8] * det );

			};

			// --------------------------------------------------------
			// transposes a nxn matrix
			
			template <typename T >
			__forceinline mat3x3<T> Transpose(const mat3x3<T>& R)
			{
				return mat3x3<T>(R[0], R[3], R[6],
								 R[1], R[4], R[7],
								 R[2], R[5], R[8]);
			}

			// --------------------------------------------------------
			// zero a nxn matrix

			template <typename T>
			__forceinline mat3x3<T> Zero()
			{
				return mat3x3<T>(T(0), T(0), T(0), 
					             T(0), T(0), T(0), 
					             T(0), T(0), T(0));
			}

			// --------------------------------------------------------
			// matrix identity

			template <typename T>
			__forceinline mat3x3<T> Identity()
			{
				return mat3x3<T>(T(1), T(0), T(0),
								 T(0), T(1), T(0),
								 T(0), T(0), T(1));
			}

			// --------------------------------------------------------
			// initialise matrix with random values

			template <typename T>
			__forceinline mat3x3<T> Random(const int min, const int max)
			{
				return mat3x3<T>(T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))),
								 T(min + (T(rand()) / (RAND_MAX + 1) * (max - min + 1))));
			}

			// --------------------------------------------------------
			// initialise matrix with diagonal value

			template <typename T>
			__forceinline mat3x3<T> Diagonal(const T value)
			{
				return mat3x3<T>(T(value), T(0),    T(0), 
					             T(0),     T(value),T(0),
								 T(0),     T(0),    T(value));
			}

			// --------------------------------------------------------
 		    // initialise matrix with a upper triangular values

			template <typename T>
			__forceinline mat3x3<T> UpperTriangular(const T value)
			{
				return mat3x3<T>(T(value), T(value), T(value),
								 T(0)    , T(value), T(value),
								 T(0),     T(0),     T(value));
			}

			// --------------------------------------------------------
		    // initialise matrix with a upper triangular values

			template <typename T>
			__forceinline mat3x3<T> LowerTriangular(const T value)
			{
				return mat3x3<T>(T(value), T(value), T(value),
								 T(value), T(value), T(0),
								 T(value), T(0),     T(0));
			}

			// --------------------------------------------------------
			// compute the absolute value of a matrix

			template <typename T>
			__forceinline mat3x3<T> Abs(const mat3x3<T>& R)
			{
				return mat3x3<T> (fabs(R[0]),fabs(R[1]),fabs(R[2]),
								  fabs(R[3]),fabs(R[4]),fabs(R[5]),
								  fabs(R[6]),fabs(R[7]),fabs(R[8]));
			}

			// --------------------------------------------------------
			// flat matrix value in the the [0..1] range

			template <typename T>
			__forceinline mat3x3<T> Flat(const mat3x3<T>& R)
			{
				T min = (double)R[0];
				if (R[1] < min)min = R[1];
				if (R[2] < min)min = R[2];
				if (R[3] < min)min = R[3];
				if (R[4] < min)min = R[4];
				if (R[5] < min)min = R[5];
				if (R[6] < min)min = R[6];
				if (R[7] < min)min = R[7];
				if (R[8] < min)min = R[8];
				T max = (double)R[0];
				if (R[1] > max)max = R[1];
				if (R[2] > max)max = R[2];
				if (R[3] > max)max = R[3];
				if (R[4] > max)max = R[4];
				if (R[5] > max)max = R[5];
				if (R[6] > max)max = R[6];
				if (R[7] > max)max = R[7];
				if (R[8] > max)max = R[8];
				T invdenum = max - min;
				if (invdenum > -vml::math::EPSILON && invdenum < vml::math::EPSILON)
					vml::os::Error("Mat3x3 : Determinant is null");
				invdenum = 1.0f / invdenum;
				return mat3x3<T>((R[0] - min) * invdenum, (R[1] - min) * invdenum, (R[2] - min) * invdenum,
								 (R[3] - min) * invdenum, (R[4] - min) * invdenum, (R[5] - min) * invdenum,
								 (R[6] - min) * invdenum, (R[7] - min) * invdenum, (R[8] - min) * invdenum);
			}

			//-------------------------------------------------------------------------------
			// helper functions to compute cofactors
			// which is a lower order matrix obtaind eliminating the a row 
			// and b column

			template <typename T>
			const mat2x2<T> GetCoFactor(const mat3x3<T>& R,const size_t a, const size_t b, bool evalsign)
			{
				if (a > 2)
					vml::os::Error("Mat3x3 : i is out of range");
				if (b > 2)
					vml::os::Error("Mat3x3 : j is out of range");
				if (a == 0 && b == 0 && !evalsign) return mat2x2<T>( R[4],  R[5],  R[7],  R[8]);
				if (a == 0 && b == 0 &&  evalsign) return mat2x2<T>( R[4],  R[5],  R[7],  R[8]);
				if (a == 1 && b == 0 && !evalsign) return mat2x2<T>( R[3],  R[5],  R[6],  R[8]);
				if (a == 1 && b == 0 &&  evalsign) return mat2x2<T>(-R[3], -R[5], -R[6], -R[8]);
				if (a == 2 && b == 0 && !evalsign) return mat2x2<T>( R[3],  R[4],  R[6],  R[7]);
				if (a == 2 && b == 0 &&  evalsign) return mat2x2<T>( R[3],  R[4],  R[6],  R[7]);
				if (a == 0 && b == 1 && !evalsign) return mat2x2<T>( R[1],  R[2],  R[7],  R[8]);
				if (a == 0 && b == 1 &&  evalsign) return mat2x2<T>(-R[1], -R[2], -R[7], -R[8]);
				if (a == 1 && b == 1 && !evalsign) return mat2x2<T>( R[0],  R[2],  R[6],  R[8]);
				if (a == 1 && b == 1 &&  evalsign) return mat2x2<T>( R[0],  R[2],  R[6],  R[8]);
				if (a == 2 && b == 1 && !evalsign) return mat2x2<T>( R[0],  R[1],  R[6],  R[7]);
				if (a == 2 && b == 1 &&  evalsign) return mat2x2<T>(-R[0], -R[1], -R[6], -R[7]);
				if (a == 0 && b == 2 && !evalsign) return mat2x2<T>( R[1],  R[2],  R[4],  R[5]);
				if (a == 0 && b == 2 &&  evalsign) return mat2x2<T>( R[1],  R[2],  R[4],  R[5]);
				if (a == 1 && b == 2 && !evalsign) return mat2x2<T>( R[0],  R[2],  R[3],  R[5]);
				if (a == 1 && b == 2 &&  evalsign) return mat2x2<T>(-R[0], -R[2], -R[3], -R[5]);
				if (a == 2 && b == 2 && !evalsign) return mat2x2<T>( R[0],  R[1],  R[3],  R[4]);
				if (a == 2 && b == 2 &&  evalsign) return mat2x2<T>( R[0],  R[1],  R[3],  R[4]);
				return mat2x2<T>(0,0,0,0); 
			}

			// ---------------------------------------------------------------
			// minors of a nxn matrix

			template <typename T>
			const std::vector<mat2x2<T>> GetCoFactors(const mat3x3<T>& R)
			{
				std::vector <mat2x2<T>> cofactors;
				cofactors.emplace_back(GetCoFactor(R,0, 0, true));
				cofactors.emplace_back(GetCoFactor(R,1, 0, true));
				cofactors.emplace_back(GetCoFactor(R,2, 0, true));
				cofactors.emplace_back(GetCoFactor(R,0, 1, true));
				cofactors.emplace_back(GetCoFactor(R,1, 1, true));
				cofactors.emplace_back(GetCoFactor(R,2, 1, true));
				cofactors.emplace_back(GetCoFactor(R,0, 2, true));
				cofactors.emplace_back(GetCoFactor(R,1, 2, true));
				cofactors.emplace_back(GetCoFactor(R,2, 2, true));
				return cofactors;
			}

			//-------------------------------------------------------------------------------
			// helper functions to compute cofactors
			// which is a lower order matrix obtaind eliminating the a row 
			// and b column

			template <typename T>
			const mat2x2<T> GetAdjoint(const mat3x3<T>& R,const size_t a, const size_t b)
			{
				if (a > 2)
					vml::os::Error("Mat3x3 : i is out of range");
				if (b > 2)
					vml::os::Error("Mat3x3 : j is out of range");
				if (a == 2 && b == 2) return  mat2x2<T>( R[4],  R[5],  R[7],  R[8]);
				if (a == 1 && b == 2) return  mat2x2<T>(-R[3], -R[5], -R[6], -R[8]);
				if (a == 0 && b == 2) return  mat2x2<T>( R[3],  R[4],  R[6],  R[7]);
				if (a == 2 && b == 1) return  mat2x2<T>(-R[1], -R[2], -R[7], -R[8]);
				if (a == 1 && b == 1) return  mat2x2<T>( R[0],  R[2],  R[6],  R[8]);
				if (a == 0 && b == 1) return  mat2x2<T>(-R[0], -R[1], -R[6], -R[7]);
				if (a == 2 && b == 0) return  mat2x2<T>( R[1],  R[2],  R[4],  R[5]);
				if (a == 1 && b == 0) return  mat2x2<T>(-R[0], -R[2], -R[3], -R[5]);
				if (a == 0 && b == 0) return  mat2x2<T>( R[0],  R[1],  R[3],  R[4]);
				return mat2x2<T>(0,0,0,0);
			}

			// ---------------------------------------------------------------
			// minors of a nxn matrix

			template <typename T>
			const std::vector<mat2x2<T>> GetAdjoints(const mat3x3<T>& R)
			{
				std::vector <mat2x2<T>> cofactors;
				cofactors.emplace_back(GetAdjoint(R,0, 0));
				cofactors.emplace_back(GetAdjoint(R,1, 0));
				cofactors.emplace_back(GetAdjoint(R,2, 0));
				cofactors.emplace_back(GetAdjoint(R,0, 1));
				cofactors.emplace_back(GetAdjoint(R,1, 1));
				cofactors.emplace_back(GetAdjoint(R,2, 1));
				cofactors.emplace_back(GetAdjoint(R,0, 2));
				cofactors.emplace_back(GetAdjoint(R,1, 2));
				cofactors.emplace_back(GetAdjoint(R,2, 2));
				return cofactors;
			}

			// --------------------------------------------------------
			// calculate the condition number
			// which is the norm multiplied
			// byt the norm of the matrix
			// inverse

			template <typename T>
			__forceinline T ConditionNumber(const mat3x3<T>& R)
			{
				T det = R[0] * R[4] * R[8] - R[0] * R[5] * R[7] +
						R[1] * R[5] * R[6] - R[1] * R[3] * R[8] +
						R[2] * R[3] * R[7] - R[2] * R[4] * R[6];

				// cannot invert singular matrix

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Error("Mat3x3 : Determinant is null");

				T  t[9] = { R[4] * R[8] - R[5] * R[7],
							R[2] * R[7] - R[1] * R[8],
							R[1] * R[5] - R[2] * R[4],
							R[5] * R[6] - R[3] * R[8],
							R[0] * R[8] - R[2] * R[6],
							R[2] * R[3] - R[0] * R[5],
							R[3] * R[7] - R[6] * R[4],
							R[1] * R[6] - R[0] * R[7],
							R[0] * R[4] - R[1] * R[3] };

				det = 1.0f / det;

				float n0 = R[0] * R[0] + R[1] * R[1] + R[2] * R[2] +
						   R[3] * R[3] + R[4] * R[4] + R[5] * R[5] +
						   R[6] * R[6] + R[7] * R[7] + R[8] * R[8];

				float n1 = (t[0] * t[0] + t[1] * t[1] + t[2] * t[2] +
							t[3] * t[3] + t[4] * t[4] + t[5] * t[5] +
							t[6] * t[6] + t[7] * t[7] + t[8] * t[8]) * det * det;

				return sqrtf(n0 * n1);
			}

			// --------------------------------------------------------
			// create a submatrix given an nxn matrix

			template <typename T>
			__forceinline T SubMatrix(const mat3x3<T>& R, const size_t i, const size_t j)
			{
				return CoFactor(R, i, j, false);
			}

			// --------------------------------------------------------
			// create a distance matrix given an 2x2 matrix

			template <typename T>
			__forceinline T DistanceFromMatrix(const mat3x3<T>& R, const mat3x3<T>& M)
			{
				T t[9] = { R[0] - M[0], R[1] - M[1], R[2] - M[2],
					       R[3] - M[3], R[4] - M[4], R[5] - M[5],
						   R[6] - M[6] ,R[7] - M[7] ,R[8] - M[8] };
				return sqrtf(t[0] * t[0] + t[1] * t[1] + t[2] * t[2] + 
					         t[3] * t[3] + t[4] * t[4] + t[5] * t[5]+ 
					         t[6] * t[6] + t[7] * t[7] + t[8] * t[8]);
			}

			// --------------------------------------------------------
			// create a normalized distance matrix given an 2x2 matrix

			template <typename T>
			__forceinline T NormalizedDistanceFromMatrix(const mat3x3<T>& R, const mat3x3<T>& M)
			{
				T t[9] = { R[0] - M[0], R[1] - M[1], R[2] - M[2],
						   R[3] - M[3], R[4] - M[4], R[5] - M[5],
						   R[6] - M[6] ,R[7] - M[7] ,R[8] - M[8] };
				T max = t[0];
				if (t[1] > max)max = t[1];
				if (t[2] > max)max = t[2];
				if (t[3] > max)max = t[3];
				if (t[4] > max)max = t[4];
				if (t[5] > max)max = t[5];
				if (t[6] > max)max = t[6];
				if (t[7] > max)max = t[7];
				if (t[8] > max)max = t[8];
				return sqrtf(t[0] * t[0] + t[1] * t[1] + t[2] * t[2] +
							 t[3] * t[3] + t[4] * t[4] + t[5] * t[5] +
							 t[6] * t[6] + t[7] * t[7] + t[8] * t[8]) / max;
			}

			// ---------------------------------------------------------------
			// make matrix simmetric using the realtion m=p+ transposed of p
			
			template <typename T>
			__forceinline mat3x3<T> MakeSimmetric(const mat3x3<T>& R)
			{
				T a = R[0], b = R[1], c = R[2],
				  d = R[3], e = R[4], f = R[5],
				  g = R[6], h = R[7], i = R[8];
				return mat3x3<T> (a + a, b + d, c + g,
								  d + b, e + e, f + h,
								  g + c, h + f, i + i);
			}

			// --------------------------------------------------------
			// normalize matrix

			template <typename T>
			__forceinline mat3x3<T> Normalize(const vml::math::mat3x3<T>& R)
			{
				T det =R[0] * R[4] * R[8] + R[1] * R[5] * R[6] + R[2] * R[3] * R[7] - R[2] * R[4] * R[6] - R[5] * R[7] * R[0] - R[8] * R[1] * R[3];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Error("Mat3x3 : max is null");
				det = T(1) / det;
				return mat3x3<T>(R[0] * det, R[1] * det, R[2] * det, R[3] * det, R[4] * det, R[5] * det, R[6] * det, R[7] * det, R[8] * det);
			}
		
			// --------------------------------------------------------
			// extracting colums as vector

			template <typename T>
			__forceinline vec3<T> GetColumn(const vml::math::mat3x3<T> &R,const size_t i)
			{
				if (i > 2)
					vml::os::Error("Mat3x3 : index i out of range");
				if (i == 0) return vec3<T>(R[0], R[3], R[6]);
				if (i == 1) return vec3<T>(R[1], R[4], R[7]);
				if (i == 2) return vec3<T>(R[2], R[5], R[8]);
				return vec3<T>(T(0), T(0),T(0));
			}
		
			// --------------------------------------------------------
			// extracting row as vector

			template <typename T>
			__forceinline vec3<T> GetRow(const vml::math::mat3x3<T>& R, const size_t i)
			{
				if (i > 2)
					vml::os::Error("Mat3x3 : index i out of range");
				if (i == 0) return vec3<T>(R[0], R[1], R[2]);
				if (i == 1) return vec3<T>(R[3], R[4], R[5]);
				if (i == 2) return vec3<T>(R[6], R[7], R[8]);
				return vec3<T>(T(0), T(0), T(0));
			}

			// --------------------------------------------------------
			// compute the minimum in the matrix

			template <typename T>
			__forceinline T GetMin(const vml::math::mat3x3<T>& R)
			{
				T min = R[0];
				if (R[1] < min)min = R[1];
				if (R[2] < min)min = R[2];
				if (R[3] < min)min = R[3];
				if (R[4] < min)min = R[4];
				if (R[5] < min)min = R[5];
				if (R[6] < min)min = R[6];
				if (R[7] < min)min = R[7];
				if (R[8] < min)min = R[8];
				return min;
			}

			// --------------------------------------------------------
			// compute the minimum in the matrix

			template <typename T>
			__forceinline T GetAbsMin(const vml::math::mat3x3<T>& R)
			{
				T min = fabs(R[0]);
				if (fabs(R[1]) < min)min = fabs(R[1]);
				if (fabs(R[2]) < min)min = fabs(R[2]);
				if (fabs(R[3]) < min)min = fabs(R[3]);
				if (fabs(R[4]) < min)min = fabs(R[4]);
				if (fabs(R[5]) < min)min = fabs(R[5]);
				if (fabs(R[6]) < min)min = fabs(R[6]);
				if (fabs(R[7]) < min)min = fabs(R[7]);
				if (fabs(R[8]) < min)min = fabs(R[8]);
				return min;
			}

			// --------------------------------------------------------
			// compute the minimum in the matrix

			template <typename T>
			__forceinline T GetMax(const vml::math::mat3x3<T>& R)
			{
				T min = R[0];
				if (R[1] > min)min = R[1];
				if (R[2] > min)min = R[2];
				if (R[3] > min)min = R[3];
				if (R[4] > min)min = R[4];
				if (R[5] > min)min = R[5];
				if (R[6] > min)min = R[6];
				if (R[7] > min)min = R[7];
				if (R[8] > min)min = R[8];
				return min;
			}

			// --------------------------------------------------------
			// compute the minimum in the matrix

			template <typename T>
			__forceinline T GetAbsMax(const vml::math::mat3x3<T>& R)
			{
				T min = fabs(R[0]);
				if (fabs(R[1]) > min)min = fabs(R[1]);
				if (fabs(R[2]) > min)min = fabs(R[2]);
				if (fabs(R[3]) > min)min = fabs(R[3]);
				if (fabs(R[4]) > min)min = fabs(R[4]);
				if (fabs(R[5]) > min)min = fabs(R[5]);
				if (fabs(R[6]) > min)min = fabs(R[6]);
				if (fabs(R[7]) > min)min = fabs(R[7]);
				if (fabs(R[8]) > min)min = fabs(R[8]);
				return min;
			}

			// --------------------------------------------------------
			// compute the floor of a given matrix

			template <typename T>
			__forceinline mat3x3<T> Floor(const vml::math::mat3x3<T>& R)
			{
				return mat3x3<T>(floor(R[0]), floor(R[1]), floor(R[2]), 
					             floor(R[3]), floor(R[4]), floor(R[5]),
								 floor(R[6]), floor(R[7]), floor(R[8]));
			}

			// --------------------------------------------------------
			// compute the ceil of a given matrix

			template <typename T>
			__forceinline mat3x3<T> Ceil(const vml::math::mat3x3<T>& R)
			{
				return mat3x3<T>(ceil(R[0]), ceil(R[1]), ceil(R[2]),
								 ceil(R[3]), ceil(R[4]), ceil(R[5]),
								 ceil(R[6]), ceil(R[7]), ceil(R[8]));
			}
			
			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat3x3<T> Saturate(const vml::math::mat3x3<T> &R)
			{
				T P[4]={R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],R[8]};
				if (P[0] > T(1)) P[0] = T(1);
				if (P[1] > T(1)) P[1] = T(1);
				if (P[2] > T(1)) P[2] = T(1);
				if (P[3] > T(1)) P[3] = T(1);
				if (P[4] > T(1)) P[4] = T(1);
				if (P[5] > T(1)) P[5] = T(1);
				if (P[6] > T(1)) P[6] = T(1);
				if (P[7] > T(1)) P[7] = T(1);
				if (P[8] > T(1)) P[8] = T(1);
				if (P[0] < T(0)) P[0] = T(0);
				if (P[1] < T(0)) P[1] = T(0);
				if (P[2] < T(0)) P[2] = T(0);
				if (P[3] < T(0)) P[3] = T(0);
				if (P[4] < T(0)) P[4] = T(0);
				if (P[5] < T(0)) P[5] = T(0);
				if (P[6] < T(0)) P[6] = T(0);
				if (P[7] < T(0)) P[7] = T(0);
				if (P[8] < T(0)) P[8] = T(0);
				return mat3x3<T>(P[0],P[1],P[2],P[3],P[4],P[5],P[6],P[7],P[8]);
			}

			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat3x3<T> SaturateToZero(const vml::math::mat3x3<T>& R)
			{
				T P[4] = { R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],R[8] };
				if (P[0] < T(0)) P[0] = T(0);
				if (P[1] < T(0)) P[1] = T(0);
				if (P[2] < T(0)) P[2] = T(0);
				if (P[3] < T(0)) P[3] = T(0);
				if (P[4] < T(0)) P[4] = T(0);
				if (P[5] < T(0)) P[5] = T(0);
				if (P[6] < T(0)) P[6] = T(0);
				if (P[7] < T(0)) P[7] = T(0);
				if (P[8] < T(0)) P[8] = T(0);
				return mat3x3<T>(P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8]);
			}

			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat3x3<T> SaturateToOne(const vml::math::mat3x3<T>& R)
			{
				T P[4] = { R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],R[8] };
				if (P[0] > T(1)) P[0] = T(1);
				if (P[1] > T(1)) P[1] = T(1);
				if (P[2] > T(1)) P[2] = T(1);
				if (P[3] > T(1)) P[3] = T(1);
				if (P[4] > T(1)) P[4] = T(1);
				if (P[5] > T(1)) P[5] = T(1);
				if (P[6] > T(1)) P[6] = T(1);
				if (P[7] > T(1)) P[7] = T(1);
				if (P[8] > T(1)) P[8] = T(1);
				return mat3x3<T>(P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8]);
			}

			// --------------------------------------------------------
			// construct a rotation matrix 

			template <typename T>
			__forceinline mat3x3<T> Rot(float yaw, float pitch, float roll)
			{
				yaw   *= vml::math::DEGTORAD;
				pitch *= vml::math::DEGTORAD;
				roll  *= vml::math::DEGTORAD;
				float sinx = sin(yaw);
				float siny = sin(pitch);
				float sinz = sin(roll);
				float cosx = cos(yaw);
				float cosy = cos(pitch);
				float cosz = cos(roll);
				return mat3x3<T>( cosy * cosz                      , cosy * sinz					  , -siny,
								  sinx * siny * cosz - cosx * sinz , sinx * siny * sinz + cosx * cosz ,  sinx * cosy,
								  cosx * siny * cosz + sinx * sinz , cosx * siny * sinz - sinx * cosz ,  cosx * cosy );
			}

			// --------------------------------------------------------
			// checks if two matrices are equal 

			template <typename T>
			__forceinline bool IsEqual(const vml::math::mat3x3<T>& R, const vml::math::mat3x3<T>& P)
			{
				return fabs(R[0] - P[0]) < vml::math::EPSILON &&
					   fabs(R[1] - P[1]) < vml::math::EPSILON &&
					   fabs(R[2] - P[2]) < vml::math::EPSILON &&
					   fabs(R[3] - P[3]) < vml::math::EPSILON &&
					   fabs(R[4] - P[4]) < vml::math::EPSILON &&
					   fabs(R[5] - P[5]) < vml::math::EPSILON &&
					   fabs(R[6] - P[6]) < vml::math::EPSILON &&
					   fabs(R[7] - P[7]) < vml::math::EPSILON &&
					   fabs(R[8] - P[8]) < vml::math::EPSILON;
			}

			// ---------------------------------------------------------------
			// Determine if a nxn matrix is singular

			template <typename T>
			__forceinline bool IsSingular(const vml::math::mat3x3<T>& R)
			{
				T det =R[0] * R[4] * R[8] + 
					   R[1] * R[5] * R[6] + 
					   R[2] * R[3] * R[7] - 
					   R[2] * R[4] * R[6] - 
					   R[5] * R[7] * R[0] - 
					   R[8] * R[1] * R[3];
			   if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
				   return true;
			   return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is upper triangulat

			template <typename T>
			__forceinline bool IsUpperTriangular(const vml::math::mat3x3<T>& R)
			{
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[1] < -vml::math::EPSILON || R[1] > vml::math::EPSILON)
				if (R[2] < -vml::math::EPSILON || R[2] > vml::math::EPSILON)
				if (R[3] > -vml::math::EPSILON && R[3] < vml::math::EPSILON)
				if (R[4] < -vml::math::EPSILON || R[4] > vml::math::EPSILON)
				if (R[5] < -vml::math::EPSILON || R[5] > vml::math::EPSILON)
				if (R[6] > -vml::math::EPSILON && R[6] < vml::math::EPSILON)
				if (R[7] > -vml::math::EPSILON && R[7] < vml::math::EPSILON)
				if (R[8] < -vml::math::EPSILON || R[8] > vml::math::EPSILON)
					return true;
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[1] < -vml::math::EPSILON || R[1] > vml::math::EPSILON)
				if (R[2] < -vml::math::EPSILON || R[2] > vml::math::EPSILON)
				if (R[3] < -vml::math::EPSILON || R[3] > vml::math::EPSILON)
				if (R[4] < -vml::math::EPSILON || R[4] > vml::math::EPSILON)
				if (R[5] > -vml::math::EPSILON && R[5] < vml::math::EPSILON)
				if (R[6] < -vml::math::EPSILON || R[6] > vml::math::EPSILON)
				if (R[7] > -vml::math::EPSILON && R[7] < vml::math::EPSILON)
				if (R[8] > -vml::math::EPSILON && R[8] < vml::math::EPSILON)
					return true;
				return false;
			}

			// Determine if a 2x2 matrix is lower triangular
			template <typename T>
			__forceinline bool IsLowerTriangular(const vml::math::mat3x3<T>& R)
			{
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON)
				if (R[2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON)
				if (R[3] < -vml::math::EPSILON || R[3] > vml::math::EPSILON)
				if (R[4] < -vml::math::EPSILON || R[4] > vml::math::EPSILON)
				if (R[5] > -vml::math::EPSILON && R[5] < vml::math::EPSILON)
				if (R[6] < -vml::math::EPSILON || R[6] > vml::math::EPSILON)
				if (R[7] < -vml::math::EPSILON || R[7] > vml::math::EPSILON)
				if (R[8] < -vml::math::EPSILON || R[8] > vml::math::EPSILON)
					return true;
				if (R[0] > -vml::math::EPSILON && R[0] < vml::math::EPSILON)
				if (R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON)
				if (R[2] < -vml::math::EPSILON || R[2] > vml::math::EPSILON)
				if (R[3] > -vml::math::EPSILON && R[3] < vml::math::EPSILON)
				if (R[4] < -vml::math::EPSILON || R[4] > vml::math::EPSILON)
				if (R[5] < -vml::math::EPSILON || R[5] > vml::math::EPSILON)
				if (R[6] > -vml::math::EPSILON && R[6] < vml::math::EPSILON)
				if (R[7] > -vml::math::EPSILON && R[7] < vml::math::EPSILON)
				if (R[8] > -vml::math::EPSILON && R[8] < vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is diagonal

			template <typename T>
			__forceinline bool IsDiagonal(const vml::math::mat3x3<T>& R)
			{
				if (R[0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON)
				if (R[2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON)
				if (R[3] > -vml::math::EPSILON && R[3] < vml::math::EPSILON)
				if (R[4] < -vml::math::EPSILON || R[4] > vml::math::EPSILON)
				if (R[5] > -vml::math::EPSILON && R[5] < vml::math::EPSILON)
				if (R[6] > -vml::math::EPSILON && R[6] < vml::math::EPSILON)
				if (R[7] > -vml::math::EPSILON && R[7] < vml::math::EPSILON)
				if (R[8] < -vml::math::EPSILON || R[8] > vml::math::EPSILON)
					return true;
				return false;
			}

			// Determine if a 2x2 matrix is diagonal

			template <typename T>
			__forceinline bool IsIdentity(const vml::math::mat3x3<T>& R)
			{
				if (R[0] > 1 - vml::math::EPSILON && R[0] < 1 + vml::math::EPSILON)
				if (R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON)
				if (R[2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON)
				if (R[3] > -vml::math::EPSILON && R[3] < vml::math::EPSILON)
				if (R[4] > 1 - vml::math::EPSILON && R[4] < 1 + vml::math::EPSILON)
				if (R[5] > -vml::math::EPSILON && R[5] < vml::math::EPSILON)
				if (R[6] > -vml::math::EPSILON && R[6] < vml::math::EPSILON)
				if (R[7] > -vml::math::EPSILON && R[7] < vml::math::EPSILON)
				if (R[8] > 1 - vml::math::EPSILON && R[8] < 1 + vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is zero

			template <typename T>
			__forceinline bool IsZero(const vml::math::mat3x3<T>& R)
			{
				if ((R[0] > -vml::math::EPSILON && R[0] < vml::math::EPSILON) &&
					(R[1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON) &&
					(R[2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[3] > -vml::math::EPSILON && R[3] < vml::math::EPSILON) &&
					(R[4] > -vml::math::EPSILON && R[4] < vml::math::EPSILON) &&
					(R[5] > -vml::math::EPSILON && R[5] < vml::math::EPSILON) &&
					(R[6] > -vml::math::EPSILON && R[6] < vml::math::EPSILON) &&
					(R[7] > -vml::math::EPSILON && R[7] < vml::math::EPSILON) &&
					(R[8] > -vml::math::EPSILON && R[8] < vml::math::EPSILON))
						return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is simmetric

			template <typename T>
			__forceinline bool IsSimmetric(const vml::math::mat3x3<T>& R)
			{
				T det = R[1] - R[3];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
				{
					det = R[2] - R[6];
					if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					{
						det = R[5] - R[7];
						if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
						{
							return true;
						}
					}
				}
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is simmetric

			template <typename T>
			__forceinline bool IsSkewSimmetric(const vml::math::mat3x3<T>& R)
			{
				T det= R[1] + R[3];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
				{
					det = R[2] + R[6];
					if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					{
						det = R[5] + R[7];
						if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
						{
							return true;
						}
					}
				}
				return false;
			}
		
		}

	} // end of namespace math

} // end of namesapce vml

