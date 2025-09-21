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

///////////////////////////////////////////////////////////////////////////////////
// fixed size vector ( similar to stl array )

namespace vml
{
	namespace math
	{
			
			///////////////////////////////////////////////////////////////////////////////////
			// vecn class

			template < typename T, const size_t S >
			class vecn 
			{

				private:

					T Data[S] = { 0 };
					size_t N = S;

				public :
					
					// --------------------------------------------------------------------
					// access operators

					__forceinline const T& operator[](int i) const
					{
						return Data[i];
					}

					__forceinline T& operator[](int i)
					{
						return Data[i];
					}

					__forceinline const T &operator()(const int i)
					{
						return Data[i];
					}
					
					// --------------------------------------------------------------------
					// casting to pointer

					__forceinline operator T* () const 
					{
						return (T*)Data; 
					}
					
					__forceinline operator const T* () const 
					{
						return (const T*)Data; 
					}

					// --------------------------------------------------------------------
					// artimetic operators

					__forceinline vecn& operator *=(const T s)
					{
						for (size_t i = 0; i < N; ++i) Data[i] *= s;
						return *this;
					}

					__forceinline vecn& operator /=(const T s)
					{
						if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
							vml::os::Error("Vecn  : scalar is zero");
						T invs = T(1) / T(s);
						for (size_t i = 0; i < N; ++i) Data[i] *= invs;
						return *this;
					}

					__forceinline vecn& operator +=(const vecn& v)
					{
						for (size_t i = 0; i < N; ++i) Data[i] += v[i];
						return *this;
					}

					__forceinline vecn& operator -=(const vecn& v)
					{
						for (size_t i = 0; i < N; ++i) Data[i] -= v[i];
						return *this;
					}

					__forceinline vecn &operator+()
					{
						return *this;
					}
					
					__forceinline vecn<T,S> operator-()
					{
						vecn<T, S> t;
						for (int i = 0; i < S; ++i) t[i] = -Data[i];
						return t;
					}

					// computes dot product 

					__forceinline vecn& operator |=(const vecn& v) const
					{
						T acc = T(0);
						for (int i = 0; i < N; ++i)
							acc += Data[i] * v[i];
						return acc;
					}

					// --------------------------------------------------------------------
					// comparison operators

					__forceinline bool operator< (const vecn<T,S> &v) const
					{
						return memcmp(Data, v.Data, v.N * sizeof(T)) < 0;
					}

					__forceinline bool operator== (const vecn<T,S> &v) const
					{
						return memcmp(Data, v.Data, v.N * sizeof(T)) == 0;
					}

					__forceinline bool operator<= (const vecn<T,S> &v) const
					{
						return memcmp(Data, v.Data, v.N * sizeof(T)) <= 0;
					}

					__forceinline bool operator> (const vecn<T, S> &v) const
					{
						return memcmp(Data, v.Data, v.N * sizeof(T)) > 0;
					}

					__forceinline bool operator>= (const vecn<T, S> &v) const
					{
						return memcmp(Data, v.Data, v.N * sizeof(T)) >= 0;
					}

					__forceinline bool operator!= (const vecn<T, S> &v) const
					{
						return memcmp(Data, v.Data, v.N * sizeof(T)) != 0;
					}

					// --------------------------------------------------------------------
					// query functions

					__forceinline int GetSize() const 
					{
						return int(N); 
					}
					
					__forceinline const T* GetData() const
					{
						return Data;
					}

					__forceinline T* GetData()
					{
						return Data;
					}

					// ------------------------------------------------------------
					// ctor / dtor

					vecn()
					{
						for (int i = 0; i < N; ++i)
							Data[i] = 0;
					}

					vecn(const std::initializer_list<T>& il)
					{
						if ( il.size()>N)
							vml::os::Error("Vecn  : initializer list size mismatch");
						for (int i = 0; i < N; ++i)
							Data[i] = 0;
						for (int i = 0; i < il.size(); ++i)
							Data[i] = (*(il.begin() + i));
					}

					// ---------------------------------------------------------------
					// copy constructor

					const vecn(const vecn &M )
					{
						N=M.N;
						memcpy(Data, M.Data, sizeof(T)*N);
					}

					vecn& operator= (const vecn &M)
					{
						if (&M == this)
							return *this;
						N = M.N;
						memcpy(Data, M.Data, sizeof(T)*N);
						return *this;
					}

					~vecn()
					{
					}

			};

			/////////////////////////////////////////////////////////////////////////////////
			// friend functions
			
			template <typename T, const size_t S>
			__forceinline vecn<T, S> operator+(const vecn<T,S> &u, const vecn<T,S> &v)
			{
				if (u.GetSize() != v.GetSize())
					vml::os::Error("VecN : vector size mismatch");
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = u[i] + v[i];
				return temp;
			}

			template <typename T, const size_t S>
			__forceinline vecn<T, S> operator-(const vecn<T, S> &u, const vecn<T, S> &v)
			{
				if (u.GetSize() != v.GetSize())
					vml::os::Error("VecN : vector size mismatch");
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = u[i] - v[i];
				return temp;
			}

			template <typename T, const size_t S>
			__forceinline vecn<T, S> operator*(const vecn<T, S> &v,const T s)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = s * v[i];
				return temp;
			}

			template <typename T, const size_t S>
			__forceinline vecn<T,S> operator*(const T s, const vecn<T,S> &v)
			{
				vecn<T, S> temp;	
				for ( int i = 0; i < v.GetSize(); ++i)
					temp[i] = s * v[i];
				return temp;
			}

			template <typename T, const size_t S>
			__forceinline vecn<T, S> operator/(const vecn<T, S> &v,const T s)
			{
				if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
					vml::os::Error("VecN : scalar is zero");
				T invs = 1 / s;
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = v[i]*invs;
				return temp;
			}

			template <typename T, const size_t S>
			__forceinline T operator| (const vecn<T,S>& u, const vecn<T,S>& v)
			{
				if (u.N !=v.N)
					vml::os::Error("VecN : vector size mismatch");
				T acc = T(0);
				for (int i = 0; i < u.N; ++i)
					acc += u[i] * v[i];
				return acc;
			}

			template <typename T, const size_t S >
			std::ostream& operator<< (std::ostream& output, const vml::math::vecn<T,S> &v)
			{
				std::ostringstream os;
				int i;
				os << " [ ";
				for (i = 0; i < v.GetSize() - 1; i++)
					os << v[i] << " , ";
				os << v[i] << " ] ";
				return os.str();
			}

			///////////////////////////////////////////////////////////////////////////////////
			// predefined types
			/*
			typedef vec2<byte>	  vec2b;	// 8-bit  unsigned.
			typedef vec2<uint8>	  vec2ui8;	// 8-bit  unsigned.
			typedef vec2<uint16>  vec2ui16;	// 16-bit unsigned.
			typedef vec2<uint32>  vec2ui32;	// 32-bit unsigned.
			typedef vec2<uint64>  vec2ui64;	// 64-bit unsigned.
			typedef vec2<int8>	  vec2i8;	// 8-bit  signed.
			typedef vec2<int16>	  vec2i16;	// 16-bit signed.
			typedef vec2<int32>	  vec2i32;	// 32-bit signed.
			typedef vec2<int64>	  vec2i64;	// 64-bit signed.
			typedef vec2<float32> vec2f;	// 32-bit float
			typedef vec2<float64> vec2d;	// 64-bit float
			*/
	}	// end of math namespace

} // end of namespace vml

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // expand vector helpers with the newly defined vector n class

namespace vml
{
	namespace math
	{
		namespace vecnutils
		{

			// --------------------------------------------------------
			// squared length

			template <typename T, const size_t S>
			__forceinline T SquaredLength(const vecn<T, S>& v)
			{
				T acc = T(0);
				for (int i = 0; i < v.GetSize(); ++i)
					acc += v[i] * v[i];
				return acc;
			}

			// --------------------------------------------------------
			// lenght

			template <typename T, const size_t S>
			__forceinline T Length(const vecn<T, S>& v)
			{
				T acc = T(0);
				for (int i = 0; i < v.GetSize(); ++i)
					acc += v[i] * v[i];
				return sqrtf(acc);
			}
			
			// --------------------------------------------------------
			// return the dot product between two vectors

			template <typename T,const size_t S>
			__forceinline T Dot(const vecn<T,S> &v, const vecn<T,S> &w)
			{
				if (v.GetSize()!=w.GetSize())
					vml::os::Error("VecN : size must match");
				T acc = T(0);
				for (int i = 0; i < v.GetSize(); ++i)
					acc += v[i] * w[i];
				return acc;
			}
			
			// --------------------------------------------------------

			template <typename T,const size_t S>
			__forceinline vecn<T,S> Reciprocal(const vecn<T,S> &v)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
				{
					if ( v[i] > -vml::math::EPSILON && v[i] < vml::math::EPSILON)
						vml::os::Error("VecN : vector component is zero");
					temp[i] = T(1) / v[i];
				}
				return temp;
			}
			
			// --------------------------------------------------------

			template <typename T,const size_t S>
			__forceinline vecn<T,S> Abs(const vecn<T,S> &v)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = fabs(v[i]);
				return temp;
			}

			// --------------------------------------------------------

			template <typename T,const size_t S>
			__forceinline vecn<T,S> Mid(const vecn<T,S> &v, const vecn<T,S> &w)
			{
				if (v.GetSize() != w.GetSize())
					vml::os::Error("VecN : size must match");
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = (v[i] + w[i])*T(0.5);
				return temp;
			}

			// --------------------------------------------------------

			template <typename T,const size_t S>
			__forceinline vecn<T, S> Lerp(const vecn<T, S> &v, const vecn<T, S> &w, const T s)
			{
				if (v.GetSize() != w.GetSize())
					vml::os::Error("VecN : size must match");
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = (v[i] + w[i])* s;
				return temp;
			}

			// --------------------------------------------------------

			template <typename T,const size_t S>
			__forceinline vecn<T, S> Saturate(const vecn<T, S> &v)
			{
				vecn<T, S> temp;
				int vn=v.GetSize();
				for (int i = 0; i < vn; ++i)
					if (v[i] > T(1) - vml::math::EPSILON) temp[i] = T(1);
				for (int i = 0; i < vn; ++i)
					if (v[i] < T(0) + vml::math::EPSILON) temp[i] = T(0);
				return temp;
			}

			// --------------------------------------------------------

			template <typename T, const size_t S>
			__forceinline vecn<T, S> SaturateToZero(const vecn<T, S> &v)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					if (v[i] < T(0) + vml::math::EPSILON) temp[i] = T(0);
				return temp;
			}

			// --------------------------------------------------------

			template <typename T, const size_t S>
			__forceinline vecn<T, S> SaturateToOne(const vecn<T, S> &v)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					if (v[i] > T(1) - vml::math::EPSILON) temp[i] = T(1);
				return temp;
			}
	
			// --------------------------------------------------------
			// floors a vector

			template <typename T,const size_t S>
			__forceinline vecn<T, S> Floor(const vecn<T, S> &v)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i]=floor(v[i]);
				return temp;
			}

			// --------------------------------------------------------
			// ceils a vector

			template <typename T, const size_t S>
			__forceinline vecn<T, S> Ceil(const vecn<T, S> &v)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = ceil(v[i]);
				return temp;
			}
	
			// --------------------------------------------------------
			// negate a vector

			template <typename T, const size_t S>
			__forceinline vecn<T, S> Negate(const vecn<T, S>& v)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[i] = -v[i];
				return temp;
			}

			// --------------------------------------------------------
			// zeros a vector

			template <typename T,const size_t S>
			__forceinline vecn<T, S> Zero()
			{
				return vecn<T, S>();
			}

			// --------------------------------------------------------
			// fill vector with 1

			template <typename T, const size_t S>
			__forceinline vecn<T, S> Identity()
			{
				vecn<T, S> temp;
				for (int i = 0; i < temp.GetSize(); ++i)
					temp[i] = T(1);
				return temp;
			}

			// --------------------------------------------------------
			// creates a random vector

			template <typename T,const size_t S>
			__forceinline vecn<T, S> Random(const int min, const int max)
			{
				vecn<T, S> temp;
				for (int i = 0; i < temp.GetSize(); ++i)
					temp[i] = min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1));
				return temp;
			}

			// --------------------------------------------------------
			// normalize vector

			template <typename T,const size_t S>
			__forceinline vecn<T, S> Normalize(const vecn<T, S> &v)
			{
				vecn<T, S> temp;
				T acc = T(0);
				int vn = v.GetSize();
				for (int i = 0; i < vn; ++i)
					acc += v[i] * v[i];
				if (acc > -vml::math::EPSILON && acc < vml::math::EPSILON)
					vml::os::Error("VecN : scalar is zero");
				T invdenum = T(1) / sqrtf(acc);
				for (int i = 0; i < vn; ++i)
					temp[i] = v[i]*invdenum;
				return temp;
			}

			// --------------------------------------------------------
			// returns the distance from two vectors

			template <typename T,const size_t S>
			__forceinline T DistanceFrom(const vecn<T, S> &v, const vecn<T, S> &w)
			{
				int vn = v.GetSize();
				if (vn != w.GetSize())
					vml::os::Error("VecN : size must match");
				T acc = T(0);
				for (int i = 0; i < vn; ++i)
				{
					T d = v[i] - w[i];
					acc += d * d;
				}
				return sqrtf(acc);
			}

			// --------------------------------------------------------
			// return the trace of a given vector

			template <typename T,const size_t S>
			__forceinline T Trace(const vecn<T, S> &v)
			{
				T acc = T(0);
				for (int i = 0; i < v.GetSize(); ++i)
					acc += v[i];
				return acc;
			}

			// --------------------------------------------------------
			// transpose a vector

			template <typename T,const size_t S>
			__forceinline vecn<T, S> Transpose(const vecn<T, S> &v)
			{
				vecn<T, S> temp;
				for (int i = 0; i < v.GetSize(); ++i)
					temp[S-i-1] = v[i];
				return temp;
			}

			// --------------------------------------------------------
			// vector flatting

			template <typename T, const size_t S>
			__forceinline vecn<T, S> Flat(const vecn<T, S>& v)
			{
				vecn<T, S> temp;
				int vn = v.GetSize();
				T min = v[0];
				for (int i = 1; i < vn; ++i)
					if (v[i] < min) min = v[i];
				T max = v[0];
				for (int i = 1; i < vn; ++i)
					if (v[i] > max) max = v[i];
				T invdenum = max - min;
				if (invdenum > -vml::math::EPSILON && invdenum < vml::math::EPSILON)
					vml::os::Error("VecN : scalar is zero");
				invdenum = T(1) / invdenum;
				for (int i = 0; i < vn; i++)
					temp[i] = (v[i] - min) * invdenum;
				return temp;
			}

			// --------------------------------------------------------
			// tests if a vector is zero whitin a given epsilon range

			template <typename T,const size_t S>
			__forceinline bool IsZero(const vecn<T, S> &v)
			{
				for (int i = 0; i < v.GetSize(); ++i)
					if (v[i] < -vml::math::EPSILON || v[i] > vml::math::EPSILON)
						return false;
				return true;
			}

			// --------------------------------------------------------
			// tests if a vector is zero whitin a given epsilon range

			template <typename T,const size_t S>
			__forceinline bool IsIdentity(const vecn<T, S> &v)
			{
				for (int i = 0; i < v.GetSize(); ++i)
					if (v[i] < T(1) - vml::math::EPSILON || v[i] > T(1) + vml::math::EPSILON)
						return false;
				return true;
			}

			// --------------------------------------------------------
			// gets the maximum value component of a given vector

			template <typename T,const size_t S>
			__forceinline T GetMax(const vecn<T, S> &v)
			{
				T max = v[0];
				for (int i = 1; i < v.GetSize(); ++i)
					if (v[i] > max) max = v[i];
				return max;
			}

			// --------------------------------------------------------
			// gets the maximum value component of a given vector

			template <typename T, const size_t S>
			__forceinline T GetAbsMax(const vecn<T, S>& v)
			{
				T max = fabs(v[0]);
				for (int i = 1; i < v.GetSize(); ++i)
					if (fabs(v[i]) > max) max = fabs(v[i]);
				return max;
			}

			// --------------------------------------------------------
			// gets the minimum value component of a given vector

			template <typename T,const size_t S>
			__forceinline T GetMin(const vecn<T, S> &v)
			{
				T min = v[0];
				for (int i = 1; i < v.GetSize(); ++i)
					if (v[i] < min) min = v[i];
				return min;
			}

			// --------------------------------------------------------
			// gets the maximum value component of a given vector

			template <typename T, const size_t S>
			__forceinline T GetAbsMin(const vecn<T, S>& v)
			{
				T max = fabs(v[0]);
				for (int i = 1; i < v.GetSize(); ++i)
					if (fabs(v[i]) < max) max = fabs(v[i]);
				return max;
			}

			// --------------------------------------------------------
			// checks if two vectors are the same whithin an epsilon range

			template <typename T,const size_t S>
			__forceinline bool IsEqual(const vecn<T, S> &v, const vecn<T, S> &w)
			{
				int vn = v.GetSize();
				if (vn != w.GetSize())
					vml::os::Error("VecN : size must match");
				for (int i = 0; i < vn; ++i)
					if (fabs(v[i] - w[i]) > vml::math::EPSILON)
						return false;
				return true;
			}

			// --------------------------------------------------------
			// checks if two vectors are not the same whithin an epsilon range

			template <typename T,const size_t S>
			__forceinline bool IsNotEqual(const vecn<T, S> &v, const vecn<T, S> &w)
			{
				int vn = v.GetSize();
				if (vn != w.GetSize())
					vml::os::Error("VecN : size must match");
				for (int i = 0; i < vn; ++i)
					if (fabs(v[i] - w[i]) > vml::math::EPSILON)
						return true;
				return false;
			}

		}

	} // end of namespace math

} // end of namesapce vml

