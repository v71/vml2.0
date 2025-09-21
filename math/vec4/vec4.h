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

namespace vml
{
	namespace math
	{
		
		///////////////////////////////////////////////////////////////////////////////////
		// vec3 class

		template <class T>
		class vec4
		{
			
			private:

				T Data[4];

			public:
				
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

				__forceinline vec4<T>& operator*=(const T s)
				{
					Data[0] *= s;
					Data[1] *= s;
					Data[2] *= s;
					Data[3] *= s;
					return *this;
				}
				
				__forceinline vec4<T>& operator/=(const T s)
				{
					if ( s > -vml::math::EPSILON && s < vml::math::EPSILON)
						vml::os::Error("Vec4  : scalar is zero");
					T invs = T(1) / T(s);
					Data[0] *= invs;
					Data[1] *= invs;
					Data[2] *= invs;
					Data[3] *= invs;
					return *this;
				}

				__forceinline vec4<T>& operator+=(const vec4<T> &v)
				{
					Data[0] += v[0];
					Data[1] += v[1];
					Data[2] += v[2];
					Data[3] += v[3];
					return *this;
				}

				__forceinline vec4<T>& operator-=(const vec4<T> &v)
				{
					Data[0] -= v[0];
					Data[1] -= v[1];
					Data[2] -= v[2];
					Data[3] -= v[3];
					return *this;
				}

				__forceinline vec2<T>& operator+=(const T s)
				{
					Data[0] += s;
					Data[1] += s;
					Data[2] += s;
					Data[3] += s;
					return *this;
				}

				__forceinline vec2<T>& operator-=(const T s)
				{
					Data[0] -= s;
					Data[1] -= s;
					Data[2] -= s;
					Data[3] -= s;
					return *this;
				}

				__forceinline vec4<T> &operator+()
				{
					return *this;
				}

				__forceinline vec4<T> operator-()
				{
					return vec4<T>(-Data[0], -Data[1], -Data[2], -Data[3]);
				}

				__forceinline T operator|=(const vec4<T>& v) const
				{
					return Data[0] * v[0] + Data[1] * v[1] + Data[2] * v[2] + Data[3] * v[3];
				}

				// --------------------------------------------------------------------
				// comparison operators

				__forceinline bool operator< (const vec4<T> &v) const
				{
					return memcmp(Data, v.Data, 4 * sizeof(T)) < 0;
				}

				__forceinline bool operator== (const vec4<T> &v) const
				{
					return memcmp(Data, v.Data, 4 * sizeof(T)) == 0;
				}

				__forceinline bool operator<= (const vec4<T> &v) const
				{
					return memcmp(Data, v.Data, 4 * sizeof(T)) <= 0;
				}

				__forceinline bool operator> (const vec4<T> &v) const
				{
					return memcmp(Data, v.Data, 4 * sizeof(T)) > 0;
				}

				__forceinline bool operator>= (const vec4<T> &v) const
				{
					return memcmp(Data, v.Data, 4 * sizeof(T)) >= 0;
				}

				__forceinline bool operator!= (const vec4<T> &v) const
				{
					return memcmp(Data, v.Data, 4 * sizeof(T)) != 0;
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

				// --------------------------------------------------------------------
				// ctor / dtor

				vec4()
				{
					Data[0] = T(0);
					Data[1] = T(0);
					Data[2] = T(0);
					Data[3] = T(0);
				}

				vec4(const T x, const T y, const T z,const T w)
				{
					Data[0] = x;
					Data[1] = y;
					Data[2] = z;
					Data[3] = w;
				}

				// ---------------------------------------------------------------
				// assignment operator

				vec4& operator= (const vec4 &v)
				{
					if (&v == this)
						return *this;
					Data[0] = v[0];
					Data[1] = v[1];
					Data[2] = v[2];
					Data[3] = v[3];
					return *this;
				}
			
				// ---------------------------------------------------------------
				// copy constructor

				vec4(const vec4& v)
				{
					Data[0] = v[0];
					Data[1] = v[1];
					Data[2] = v[2];
					Data[3] = v[3];
				}

				~vec4()
				{}
			
		};
		
		/////////////////////////////////////////////////////////////////////////////////
		// friend functions
		
		template <typename T>
		__forceinline vec4<T> operator+(const vec4<T> &v, const vec4<T> &w)
		{
			return vec4<T>(v[0] + w[0], v[1] + w[1], v[2] + w[2]);
		}

		template <typename T>
		__forceinline vec4<T> operator-(const vec4<T> &v, const vec4<T> &w)
		{
			return vec4<T>(v[0] - w[0], v[1] - w[1], v[2] - w[2],v[3] - w[3]);
		}

		template <typename T>
		__forceinline vec4<T> operator*(const vec4<T> &v, const T s)
		{
			return vec4<T>(v[0] * s, v[1] * s, v[2] * s, v[3] * s);
		}

		template <typename T>
		__forceinline vec4<T> operator*(const T s, const vec4<T> &v)
		{
			return vec4<T>(s * v[0], s * v[1], s * v[2], s * v[3]);
		}

		template <typename T>
		__forceinline vec4<T> operator/(const vec4<T> &v, const T s)
		{
			if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
					vml::os::Error("Vec4 : scalar is zero");
			T invs = T(1) / s;
			return vec4<T>(v[0] * invs, v[1] * invs, v[2] * invs, v[3] * invs);
		}

		template <typename T>
		__forceinline T operator| (const vec4<T>& u, const vec4<T>& v)
		{
			return u[0] * v[0] + u[1] * v[1] + u[2] * v[2] + u[3] * v[3];
		}

		template <typename T >
		std::ostream& operator<< (std::ostream& output, const vml::math::vec4<T> &v)
		{
			std::ostringstream os;
			os << " [ " << v[0] << " , " << v[1] << " , " << v[2] << " , " << v[3] << " ]";
			return os.str();
		}

		///////////////////////////////////////////////////////////////////////////////////
		// predefined types

		typedef vec4<byte>	  vec4b;	// 8-bit  unsigned.
		typedef vec4<uint8>	  vec4ui8;	// 8-bit  unsigned.
		typedef vec4<uint16>  vec4ui16;	// 16-bit unsigned.
		typedef vec4<uint32>  vec4ui32;	// 32-bit unsigned.
		typedef vec4<uint64>  vec4ui64;	// 64-bit unsigned.
		typedef vec4<int8>	  vec4i8;	// 8-bit  signed.
		typedef vec4<int16>	  vec4i16;	// 16-bit signed.
		typedef vec4<int32>	  vec4i32;	// 32-bit signed.
		typedef vec4<int64>	  vec4i64;	// 64-bit signed.
		typedef vec4<float32> vec4f;	// 32-bit float
		typedef vec4<float64> vec4d;	// 64-bit float

	} // end of namespace math
}
// end of namesapce vml

/////////////////////////////////////////////////////////////////////////////////////////////////
// expand vector helpers with the newly defined vector n class

namespace vml
{
	namespace math
	{

		namespace vec4utils
		{

			// --------------------------------------------------------------------
			// metrics functions

			template <typename T>
			__forceinline T SquaredLength(const vec4<T> &v)
			{
				return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
			}

			template <typename T>
			__forceinline T Length(const vec4<T> &v)
			{
				return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
			}

			// --------------------------------------------------------
			// return the dot product between two vectors

			template <typename T>
			__forceinline T Dot(const vec4<T>& v, const vec4<T>& w)
			{
				return v[0] * w[0] + v[1] * w[1] + v[2] * w[2] + T(1);
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec4<T> Reciprocal(const vec4<T>& v)
			{
				if (v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON) vml::os::Error("Vec4 : vector component x is zero");
				if (v[1] > -vml::math::EPSILON && v[1] < vml::math::EPSILON) vml::os::Error("Vec4 : vector component y is zero");
				if (v[2] > -vml::math::EPSILON && v[2] < vml::math::EPSILON) vml::os::Error("Vec4 : vector component z is zero");
				if (v[3] > -vml::math::EPSILON && v[3] < vml::math::EPSILON) vml::os::Error("Vec4 : vector component w is zero");
				return vec4<T>(T(1) / v[0], T(1) / v[1], T(1) / v[2], T(1) / v[3]);
			}

			// --------------------------------------------------------

			template <typename T >
			__forceinline vec4<T> Abs(const vec4<T>& v)
			{
				return vec4<T>(fabsf(v[0]), fabsf(v[1]), fabsf(v[2]), fabsf(v[3]));
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec4<T> Mid(const vec4<T>& v, const vec4<T>& w)
			{
				return vec4<T>((v[0] + w[0]) * T(0.5), (v[1] + w[1]) * T(0.5), (v[2] + w[2]) * T(0.5), (v[3] + w[3]) * T(0.5));
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec4<T> Lerp(const vec4<T>& v, const vec4<T>& w, const T s)
			{
				return vec4<T>(v[0] + (w[0] - v[0]) * s, v[1] + (w[1] - v[1]) * s, v[2] + (w[2] - v[2]) * s, v[2] + (w[3] - v[3]) * s);
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec4<T> Saturate(const vec4<T>& v)
			{
				T x = v[0];
				T y = v[1];
				T z = v[2];
				T w = v[3];
				if (x > T(1) - vml::math::EPSILON) x = T(1);
				if (y > T(1) - vml::math::EPSILON) y = T(1);
				if (z > T(1) - vml::math::EPSILON) z = T(1);
				if (w > T(1) - vml::math::EPSILON) w = T(1);
				if (x < T(0) + vml::math::EPSILON) x = T(0);
				if (y < T(0) + vml::math::EPSILON) y = T(0);
				if (z < T(0) + vml::math::EPSILON) z = T(0);
				if (w < T(0) + vml::math::EPSILON) w = T(0);
				return vec4<T>(x, y, z, w);
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec4<T> SaturateToZero(const vec4<T>& v)
			{
				T x = v[0];
				T y = v[1];
				T z = v[2];
				T w = v[3];
				if (x < T(0) + vml::math::EPSILON) x = T(0);
				if (y < T(0) + vml::math::EPSILON) y = T(0);
				if (z < T(0) + vml::math::EPSILON) z = T(0);
				if (w < T(0) + vml::math::EPSILON) w = T(0);
				return vec4<T>(x, y, z, w);
			}

			// --------------------------------------------------------
			// saturate a vector to one

			template <typename T>
			__forceinline vec4<T> SaturateToOne(const vec4<T>& v)
			{
				T x = v[0];
				T y = v[1];
				T z = v[2];
				T w = v[3];
				if (x > T(1) - vml::math::EPSILON) x = T(1);
				if (y > T(1) - vml::math::EPSILON) y = T(1);
				if (z > T(1) - vml::math::EPSILON) z = T(1);
				if (w > T(1) - vml::math::EPSILON) w = T(1);
				return vec4<T>(x, y, z, w);
			}

			// --------------------------------------------------------
			// floors a vector

			template <typename T>
			__forceinline vec4<T> Floor(const vec4<T>& v)
			{
				return vec4<T>(floor(v[0]), floor(v[1]), floor(v[2]), floor(v[3]));
			}

			// --------------------------------------------------------
			// negate a vector

			template <typename T>
			__forceinline vec4<T> Negate(const vec4<T>& v)
			{
				return vec4<T>(-v[0], -v[1], -v[2], -v[3]);
			}

			// --------------------------------------------------------
			// ceils a vector

			template <typename T>
			__forceinline vec4<T> Ceil(const vec4<T>& v)
			{
				return vec4<T>(ceil(v[0]), ceil(v[1]), ceil(v[2]), ceil(v[3]));
			}

			// --------------------------------------------------------
			// zeros a vector

			template <typename T >
			__forceinline vec4<T> Zero()
			{
				return vec4<T>(T(0), T(0), T(0), T(0));
			}

			// --------------------------------------------------------
			// identity vector

			template <typename T >
			__forceinline vec4<T> Identity()
			{
				return vec4<T>(T(1), T(1), T(1), T(1));
			}

			// --------------------------------------------------------
			// creates a random vector

			template <typename T>
			__forceinline vec4<T> Random(const int min, const int max)
			{
				return vec4<T>(min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
					min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
					min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
					min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)));
			}

			// --------------------------------------------------------
			// normalize vector

			template <typename T>
			__forceinline vec4<T> Normalize(const vec4<T>& v)
			{
				T d = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + +v[3] * v[3]);
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec4 : zero lenght vector");
				d = T(1) / d;
				return vec4<T>(v[0] * d, v[1] * d, v[2] * d, v[3] * d);
			}

			// --------------------------------------------------------
			// returns the distance from two vectors

			template <typename T>
			__forceinline T DistanceFrom(const vec4<T>& v, const vec4<T>& w)
			{
				T dx = v[0] - w[0];
				T dy = v[1] - w[1];
				T dz = v[2] - w[2];
				T dw = v[3] - w[3];
				return sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
			}

			// --------------------------------------------------------
			// return the trace of a given vector

			template <typename T>
			__forceinline T Trace(const vec4<T>& v)
			{
				return v[0] + v[1] + v[2] + v[3];
			}

			// --------------------------------------------------------
			// transpose a vector

			template <typename T>
			__forceinline vec4<T> Transpose(const vec4<T>& v)
			{
				return vec4<T>(v[3], v[2], v[1], v[0]);
			}

			// --------------------------------------------------------
			// tests if a vector is zero whitin a given epsilon range

			template <typename T>
			__forceinline bool IsZero(const vec4<T>& v)
			{
				if ((v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON) &&
					(v[1] > -vml::math::EPSILON && v[1] < vml::math::EPSILON) &&
					(v[2] > -vml::math::EPSILON && v[2] < vml::math::EPSILON) &&
					(v[3] > -vml::math::EPSILON && v[3] < vml::math::EPSILON))
					return true;
				return false;
			}

			// --------------------------------------------------------
			// tests if a vector is zero whitin a given epsilon range

			template <typename T>
			__forceinline bool IsIdentity(const vec4<T>& v)
			{
				if ((v[0] > T(1) - vml::math::EPSILON && v[0] < T(1) + vml::math::EPSILON) &&
					(v[1] > T(1) - vml::math::EPSILON && v[1] < T(1) + vml::math::EPSILON) &&
					(v[2] > T(1) - vml::math::EPSILON && v[2] < T(1) + vml::math::EPSILON) &&
					(v[3] > T(1) - vml::math::EPSILON && v[3] < T(1) + vml::math::EPSILON))
					return true;
				return false;
			}

			// --------------------------------------------------------
			// gets the maximum value component of a given vector

			template <typename T>
			__forceinline T GetMax(const vec4<T>& v)
			{
				T max = v[0];
				if (v[1] > max) max = v[1];
				if (v[2] > max) max = v[2];
				if (v[3] > max) max = v[3];
				return max;
			}

			// --------------------------------------------------------
			// gets the absolute maximum value component of a given vector

			template <typename T>
			__forceinline T GetAbsMax(const vec4<T>& v)
			{
				T max = fabs(v[0]);
				if (fabs(v[1]) > max) max = fabs(v[1]);
				if (fabs(v[2]) > max) max = fabs(v[2]);
				if (fabs(v[3]) > max) max = fabs(v[3]);
				return max;
			}

			// --------------------------------------------------------
			// gets the minimum value component of a given vector

			template <typename T>
			__forceinline T GetMin(const vec4<T>& v)
			{
				T min = v[0];
				if (v[1] < min) min = v[1];
				if (v[2] < min) min = v[2];
				if (v[3] < min) min = v[3];
				return min;
			}

			// --------------------------------------------------------
			// gets the absolute minimum value component of a given vector

			template <typename T>
			__forceinline T GetAbsMin(const vec4<T>& v)
			{
				T max = fabs(v[0]);
				if (fabs(v[1]) < max) max = fabs(v[1]);
				if (fabs(v[2]) < max) max = fabs(v[2]);
				if (fabs(v[3]) < max) max = fabs(v[3]);
				return max;
			}

			// --------------------------------------------------------
			// checks if two vectors are the same whithin an epsilon range

			template <typename T>
			__forceinline bool IsEqual(const vec4<T>& v, const vec4<T>& w)
			{
				return fabs(v[0] - w[0]) < vml::math::EPSILON &&
					fabs(v[1] - w[1]) < vml::math::EPSILON &&
					fabs(v[2] - w[2]) < vml::math::EPSILON &&
					fabs(v[3] - w[3]) < vml::math::EPSILON;
			}

			// --------------------------------------------------------
			// checks if two vectors are not the same whithin an epsilon range

			template <typename T>
			__forceinline bool IsNotEqual(const vec4<T>& v, const vec4<T>& w)
			{
				return fabs(v[0] - w[0]) > vml::math::EPSILON ||
					fabs(v[1] - w[1]) > vml::math::EPSILON ||
					fabs(v[2] - w[2]) > vml::math::EPSILON ||
					fabs(v[3] - w[3]) > vml::math::EPSILON;
			}

			// --------------------------------------------------------
			// vector flatting

			template <typename T>
			__forceinline vec4<T> Flat(const vec4<T>& v)
			{
				T min = T(v[0]);
				if (v[1] < min)min = v[1];
				if (v[2] < min)min = v[2];
				if (v[3] < min)min = v[3];
				T max = T(v[0]);
				if (v[1] > max)max = v[1];
				if (v[2] > max)max = v[2];
				if (v[3] > max)max = v[3];
				T invdenum = max - min;
				if (invdenum > -vml::math::EPSILON && invdenum < vml::math::EPSILON)
					vml::os::Error("Vec4 : determinant is null");
				invdenum = T(1) / invdenum;
				return vec4<T>((v[0] - min) * invdenum, (v[1] - min) * invdenum, (v[2] - min) * invdenum, (v[3] - min) * invdenum);
			}

			// --------------------------------------------------------
			// Calculates the projection of 'v' onto 'w'.

			template <typename T>
			__forceinline vec4<T> Project(const vec4<T>& v, const vec4<T>& w)
			{
				T d = w[0] * w[0] + w[1] * w[1] + w[2] * w[2] + w[3] * w[3];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec4 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1] + v[2] * w[2] + v[3] * w[3]) / d;
				return num * w;
			}

			// --------------------------------------------------------
			// Calculates the component of 'v' perpendicular to 'w'.

			template <typename T>
			__forceinline vec4<T> Perpendicular(const vec4<T>& v, const vec4<T>& w)
			{
				T d = w[0] * w[0] + w[1] * w[1] + w[2] * w[2] + w[3] * w[3];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec4 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1] + v[2] * w[2] + w[3] * w[3]) / d;
				return v - num * w;
			}

			// --------------------------------------------------------
			// Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
			// turn them into orthonormal basis vectors.

			template <typename T>
			__forceinline void Orthogonalize(const vec4<T>& v, const vec4<T>& w, vec4<T>& pv1, vec4<T>& pv2)
			{
				// v is the basis vector normalized

				pv1 = v;
				T d = pv1[0] * pv1[0] + pv1[1] * pv1[1] + pv1[2] * pv1[2] + pv1[3] * pv1[3];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec4 : zero lenght vector");
				d = T(1) / sqrtf(d);
				pv1[0] *= d;
				pv1[1] *= d;
				pv1[2] *= d;
				pv1[3] *= d;
				// compute w-Perpendicular(w,v)

				d = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec4 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1] + v[2] * w[2] + v[3] * w[3]) / d;
				pv2 = w - num * v;

				// normalize resulting vector

				d = pv2[0] * pv2[0] + pv2[1] * pv2[1] + pv2[2] * pv2[2] + pv2[3] * pv2[3];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec4 : zero lenght vector");
				d = T(1) / sqrtf(d);
				pv2[0] *= d;
				pv2[1] *= d;
				pv2[2] *= d;
				pv2[3] *= d;
			}

			// --------------------------------------------------------
			// Calculates reflection vector from entering ray direction 'v'
			// and surface normal 'w'.

			template <typename T>
			__forceinline vec4<T> Reflect(const vec4<T>& v, const vec4<T>& n)
			{
				T d = n[0] * n[0] + n[1] * n[1] + n[2] * n[2] + T(1);
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec4 : zero lenght vector");
				d = T(1) / sqrtf(d);
				T nx = n[0] * d;
				T ny = n[1] * d;
				T nz = n[2] * d;
				T nw = n[3] * d;
				T num = T(2) * (v[0] * nx + v[1] * ny + v[2] * nz + v[3] * nw);
				return vec4<T>(v[0] - num * nx, v[1] - num * ny, v[2] - num * nz, v[3] - num * nw);
			}

		}

	} // end of namespace math

} // end of namesapce vml

