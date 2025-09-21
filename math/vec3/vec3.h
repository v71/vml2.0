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
			class vec3 
			{
				
				private:

					T Data[3];
	
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

					__forceinline const T& operator()(const int i)
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

					__forceinline vec3<T>& operator*=(const T s)
					{
						Data[0] *= s;
						Data[1] *= s;
						Data[2] *= s;
						return *this;
					}

					__forceinline vec3<T>& operator/=(const T s)
					{
						if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
							vml::os::Error("Vec3 : s is zero");
						T invs = T(1) / T(s);
						Data[0] *= invs;
						Data[1] *= invs;
						Data[2] *= invs;
						return *this;
					}

					__forceinline vec3<T>& operator+=(const vec3<T>& v)
					{
						Data[0] += v[0];
						Data[1] += v[1];
						Data[2] += v[2];
						return *this;
					}

					__forceinline vec3<T>& operator+=(const T s)
					{
						Data[0] += s;
						Data[1] += s;
						Data[2] += s;
						return *this;
					}

					__forceinline vec3<T>& operator-=(const T s)
					{
						Data[0] -= s;
						Data[1] -= s;
						Data[2] -= s;
						return *this;
					}

					__forceinline vec3<T>& operator-=(const vec3<T> &v)
					{
						Data[0] -= v[0];
						Data[1] -= v[1];
						Data[2] -= v[2];
						return *this;
					}

					__forceinline T operator|=(const vec3<T>& v) const
					{
						return Data[0] * v[0] + Data[1] * v[1] + Data[2] * v[2];
					}

					__forceinline vec3<T> operator^=(const vec3<T>& v) const
					{
						return vec3<T>(Data[1] * v[2] - Data[2] * v[1],
									   Data[2] * v[0] - Data[0] * v[2],
									   Data[0] * v[1] - Data[1] * v[0]);
					}

					__forceinline vec3<T> &operator+()
					{
						return *this;
					}

					__forceinline vec3<T> operator-()
					{
						return vec3<T>(-Data[0], -Data[1], -Data[2]);
					}

					// --------------------------------------------------------------------
					// comparison operators

					__forceinline bool operator< (const vec3<T> &v) const
					{
						return memcmp(Data, v.Data, 3 * sizeof(T)) < 0;
					}

					__forceinline bool operator== (const vec3<T> &v) const
					{
						return memcmp(Data, v.Data, 3 * sizeof(T)) == 0;
					}

					__forceinline bool operator<= (const vec3<T> &v) const
					{
						return memcmp(Data, v.Data, 3 * sizeof(T)) <= 0;
					}

					__forceinline bool operator> (const vec3<T> &v) const
					{
						return memcmp(Data, v.Data, 3 * sizeof(T)) > 0;
					}

					__forceinline bool operator>= (const vec3<T> &v) const
					{
						return memcmp(Data, v.Data, 3 * sizeof(T)) >= 0;
					}

					__forceinline bool operator!= (const vec3<T> &v) const
					{
						return memcmp(Data, v.Data, 3 * sizeof(T)) != 0;
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

					vec3() 
					{ 
						Data[0] = T(0);
						Data[1] = T(0);
						Data[2] = T(0);
					}

					vec3(const T x, const T y, const T z)
					{
						Data[0] = x;
						Data[1] = y;
						Data[2] = z;
					}

					// ---------------------------------------------------------------
					// assignment operator

					vec3& operator= (const vec3 &v)
					{
						if (&v == this)
							return *this;
						Data[0] = v[0];
						Data[1] = v[1];
						Data[2] = v[2];
						return *this;
					}

					// ---------------------------------------------------------------
					// copy constructor

					vec3(const vec3& v)
					{
						Data[0] = v[0];
						Data[1] = v[1];
						Data[2] = v[2];
					}

					~vec3()
					{}

			};
			
			/////////////////////////////////////////////////////////////////////////////////
			// friend functions

			template <typename T>
			__forceinline vec3<T> operator+(const vec3<T> &v, const vec3<T> &w)
			{
				return vec3<T>(v[0] + w[0], v[1] + w[1], v[2] + w[2]);
			}

			template <typename T>
			__forceinline vec3<T> operator-(const vec3<T> &v, const vec3<T> &w)
			{
				return vec3<T>(v[0] - w[0], v[1] - w[1], v[2] - w[2]);
			}

			template <typename T>
			__forceinline vec3<T> operator+(const vec3<T>& v, const T s)
			{
				return vec3<T>(v[0] + s, v[1] + s, v[2] + s);
			}

			template <typename T>
			__forceinline vec3<T> operator-(const vec3<T>& v, const T s)
			{
				return vec3<T>(v[0] - s, v[1] - s, v[2] - s);
			}

			template <typename T>
			__forceinline vec3<T> operator*(const vec3<T> &v, const T s)
			{
				return vec3<T>(v[0] * s, v[1] * s, v[2] * s);
			}

			template <class T>
			__forceinline vec3<T> operator*(const T s, const vec3<T> &v)
			{
				return vec3<T>(s * v[0], s * v[1], s * v[2]);
			}

			template <class T>
			__forceinline vec3<T> operator/(const vec3<T> &v, const T s)
			{
				if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
					vml::os::Error("Vec3 : scalar is null");
				T invs = T(1) / s;
				return vec3<T>(v[0] * invs, v[1] * invs, v[2] * invs);
			}

			template <typename T>
			__forceinline T operator| (const vec3<T>& u, const vec3<T>& v)
			{
				return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
			}

			template <typename T>
			__forceinline  vec3<T> operator^ (const vec3<T>& u, const vec3<T>& v)
			{
				return vec3<T>(u[1] * v[2] - u[2] * v[1],
							   u[2] * v[0] - u[0] * v[2],
							   u[0] * v[1] - u[1] * v[0]);
			}

			template <typename T >
			std::ostream& operator<< (std::ostream& output, const vml::math::vec3<T>& v)
			{
				std::ostringstream os;
				os << " [ " << v[0] << " , " << v[1] << " , " << v[2] << " ]";
				return os.str();
			}

			///////////////////////////////////////////////////////////////////////////////////
			// predefined types

			typedef vec3<byte>	  vec3b;	// 8-bit  unsigned.
			typedef vec3<uint8>	  vec3ui8;	// 8-bit  unsigned.
			typedef vec3<uint16>  vec3ui16;	// 16-bit unsigned.
			typedef vec3<uint32>  vec3ui32;	// 32-bit unsigned.
			typedef vec3<uint64>  vec3ui64;	// 64-bit unsigned.
			typedef vec3<int8>	  vec3i8;	// 8-bit  signed.
			typedef vec3<int16>	  vec3i16;	// 16-bit signed.
			typedef vec3<int32>	  vec3i32;	// 32-bit signed.
			typedef vec3<int64>	  vec3i64;	// 64-bit signed.
			typedef vec3<float32> vec3f;	// 32-bit float
			typedef vec3<float64> vec3d;	// 64-bit float


	}	// end of namespace math

}	// end of namesapce vml

/////////////////////////////////////////////////////////////////////////////////////////////////
// expand vector helpers with the newly defined vector n class

namespace vml
{
	namespace math
	{
		namespace vec3utils
		{
			
			// --------------------------------------------------------------------
			// metrics functions
			template <typename T>
			__forceinline T SquaredLength(const vec3<T>& v)
			{
				return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
			}

			template <typename T>
			__forceinline T Length(const vec3<T>& v)
			{
				return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			}
			// --------------------------------------------------------
			// return sthe dot product between two vectors

			template <typename T>
			__forceinline T Dot(const vec3<T>& v, const vec3<T>& w)
			{
				return (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]);
			}

			template <typename T>
			__forceinline vec3<T> Reciprocal(const vec3<T>& v)
			{
				if (v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON)
					if (v[1] > -vml::math::EPSILON && v[1] < vml::math::EPSILON)
						if (v[2] > -vml::math::EPSILON && v[2] < vml::math::EPSILON)
							vml::os::Error("Vec3 : scalar is null");
				return vec3<T>(T(1) / v[0], T(1) / v[1], T(1) / v[2]);
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec3<T> Cross(const vec3<T>& v, const vec3<T>& w)
			{
				return vec3<T>(v[1] * w[2] - v[2] * w[1],
							   v[2] * w[0] - v[0] * w[2],
							   v[0] * w[1] - v[1] * w[0]);
			}

			// --------------------------------------------------------
			// returns angles computed from spherical coordinates 

			template <typename T>
			__forceinline vec3<T> Ortho(const vec3<T>& v)
			{
				if (v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON)
					if (v[1] > -vml::math::EPSILON && v[1] < vml::math::EPSILON)
						if (v[2] > -vml::math::EPSILON && v[2] < vml::math::EPSILON)
							vml::os::Error("Vec3 : zero lenght vector");

				// If one dimension is zero, this can be solved by setting that to
				// non - zero and the others to zero.Example: (4, 2, 0) lies in the
				// x - y - Plane, so(0, 0, 1) is orthogonal to the plane.

				if (v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON)
					return vec3<T>(1, 0, 0);

				if (v[1] > -vml::math::EPSILON && v[1] < vml::math::EPSILON)
					return vec3<T>(0, 1, 0);

				if (v[2] > -vml::math::EPSILON && v[2] < vml::math::EPSILON)
					return vec3<T>(0, 0, 1);

				// arbitrarily set a = b = 1
				// then the equation simplifies to
				// c = -(x + y) / z

				return vec3<T>(1, 1, -(v[0] + v[1]) / v[2]);
			}

			// --------------------------------------------------------

			template <typename T >
			__forceinline vec3<T> Abs(const vec3<T>& v)
			{
				return vec3<T>(fabsf(v[0]), fabsf(v[1]), fabsf(v[2]));
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec3<T> Mid(const vec3<T>& v, const vec3<T>& w)
			{
				return vec3<T>((v[0] + w[0]) * T(0.5), (v[1] + w[1]) * T(0.5), (v[2] + w[2]) * T(0.5));
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec3<T> Lerp(const vec3<T>& v, const vec3<T>& w, const T s)
			{
				return vec3<T>(v[0] + (w[0] - v[0]) * s, v[1] + (w[1] - v[1]) * s, v[2] + (w[2] - v[2]) * s);
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec3<T> Saturate(const vec3<T>& v)
			{
				T x = v[0];
				T y = v[1];
				T z = v[2];
				if (x > T(1) - vml::math::EPSILON) x = T(1);
				if (y > T(1) - vml::math::EPSILON) y = T(1);
				if (z > T(1) - vml::math::EPSILON) z = T(1);
				if (x < T(0) + vml::math::EPSILON) x = T(0);
				if (y < T(0) + vml::math::EPSILON) y = T(0);
				if (z < T(0) + vml::math::EPSILON) z = T(0);
				return vec3<T>(x, y, z);
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec3<T> SaturateToZero(const vec3<T>& v)
			{
				T x = v[0];
				T y = v[1];
				T z = v[2];
				if (x < T(0) + vml::math::EPSILON) x = T(0);
				if (y < T(0) + vml::math::EPSILON) y = T(0);
				if (z < T(0) + vml::math::EPSILON) z = T(0);
				return vec3<T>(x, y, z);
			}

			// --------------------------------------------------------
			// saturate a vector to one

			template <typename T>
			__forceinline vec3<T> SaturateToOne(const vec3<T>& v)
			{
				T x = v[0];
				T y = v[1];
				T z = v[2];
				if (x > T(1) - vml::math::EPSILON) x = T(1);
				if (y > T(1) - vml::math::EPSILON) y = T(1);
				if (z > T(1) - vml::math::EPSILON) z = T(1);
				return vec3<T>(x, y, z);
			}

			// --------------------------------------------------------
			// floors a vector

			template <typename T>
			__forceinline vec3<T> Floor(const vec3<T>& v)
			{
				return vec3<T>(floor(v[0]), floor(v[1]), floor(v[2]));
			}

			// --------------------------------------------------------
			// ceils a vector

			template <typename T>
			__forceinline vec3<T> Ceil(const vec3<T>& v)
			{
				return vec3<T>(ceil(v[0]), ceil(v[1]), ceil(v[2]));
			}

			// --------------------------------------------------------
			// negate a vector

			template <typename T>
			__forceinline vec3<T> Negate(const vec3<T>& v)
			{
				return vec3<T>(-v[0], -v[1], -v[2]);
			}

			// --------------------------------------------------------
			// zeros a vector

			template <typename T >
			__forceinline vec3<T> Zero()
			{
				return vec3<T>(T(0), T(0), T(0));
			}

			// --------------------------------------------------------
			// identity vector

			template <typename T >
			__forceinline vec3<T> Identity()
			{
				return vec3<T>(T(1), T(1), T(1));
			}

			// --------------------------------------------------------
			// creates a random vector

			template <typename T>
			__forceinline vec3<T> Random(const int min, const int max)
			{
				return (vec3<T>(min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								min + (int)(rand() / (T)(RAND_MAX + 1) * (max - min + 1))));
			}

			// --------------------------------------------------------
			// normalize vector

			template <typename T>
			__forceinline vec3<T> Normalize(const vec3<T>& v)
			{
				T d = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				d = T(1) / d;
				return vec3<T>(v[0] * d, v[1] * d, v[2] * d);
			}

			// --------------------------------------------------------
			// returns the distance from two vectors

			template <typename T>
			__forceinline T DistanceFrom(const vec3<T>& v, const vec3<T>& w)
			{
				T dx = v[0] - w[0];
				T dy = v[1] - w[1];
				T dz = v[2] - w[2];
				return sqrtf(dx * dx + dy * dy + dz * dz);
			}

			// --------------------------------------------------------
			// return the trace of a given vector

			template <typename T>
			__forceinline T Trace(const vec3<T>& v)
			{
				return v[0] + v[1] + v[2];
			}

			// --------------------------------------------------------
			// transpose a vector

			template <typename T>
			__forceinline vec3<T> Transpose(const vec3<T>& v)
			{
				return vec3<T>(v[2], v[1], v[0]);
			}

			// --------------------------------------------------------
			// tests if a vector is zero whitin a given epsilon range

			template <typename T>
			__forceinline bool IsZero(const vec3<T>& v)
			{
				if ((v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON) &&
					(v[1] > -vml::math::EPSILON && v[1] < vml::math::EPSILON) &&
					(v[2] > -vml::math::EPSILON && v[2] < vml::math::EPSILON))
					return true;
				return false;
			}

			// --------------------------------------------------------
			// tests if a vector is zero whitin a given epsilon range

			template <typename T>
			__forceinline bool IsIdentity(const vec3<T>& v)
			{
				if ((v[0] > T(1) - vml::math::EPSILON && v[0] < T(1) + vml::math::EPSILON) &&
					(v[1] > T(1) - vml::math::EPSILON && v[1] < T(1) + vml::math::EPSILON) &&
					(v[2] > T(1) - vml::math::EPSILON && v[2] < T(1) + vml::math::EPSILON))
					return true;
				return false;
			}

			// --------------------------------------------------------
			// gets the maximum value component of a given vector

			template <typename T>
			__forceinline T GetMax(const vec3<T>& v)
			{
				T max = v[0];
				if (v[1] > max) max = v[1];
				if (v[2] > max) max = v[2];
				return max;
			}

			// --------------------------------------------------------
			// gets the absolute maximum value component of a given vector

			template <typename T>
			__forceinline T GetAbsMax(const vec3<T>& v)
			{
				T max = fabs(v[0]);
				if (fabs(v[1]) > max) max = fabs(v[1]);
				if (fabs(v[2]) > max) max = fabs(v[2]);
				return max;
			}

			// --------------------------------------------------------
			// gets the minimum value component of a given vector

			template <typename T>
			__forceinline T GetMin(const vec3<T>& v)
			{
				T min = v[0];
				if (v[1] < min) min = v[1];
				if (v[2] < min) min = v[2];
				return min;
			}

			// --------------------------------------------------------
			// gets the absolute maximum value component of a given vector

			template <typename T>
			__forceinline T GetAbsMin(const vec3<T>& v)
			{
				T max = fabs(v[0]);
				if (fabs(v[1]) < max) max = fabs(v[1]);
				if (fabs(v[2]) < max) max = fabs(v[2]);
				return max;
			}

			// --------------------------------------------------------
			// checks if two vectors are the same whithin an epsilon range

			template <typename T>
			__forceinline bool IsEqual(const vec3<T>& v, const vec3<T>& w)
			{
				return fabs(v[0] - w[0]) < vml::math::EPSILON &&
						fabs(v[1] - w[1]) < vml::math::EPSILON &&
						fabs(v[2] - w[2]) < vml::math::EPSILON;
			}

			// --------------------------------------------------------
			// checks if two vectors are not the same whithin an epsilon range

			template <typename T>
			__forceinline bool IsNotEqual(const vec3<T>& v, const vec3<T>& w)
			{
				return fabs(v[0] - w[0]) > vml::math::EPSILON ||
						fabs(v[1] - w[1]) > vml::math::EPSILON ||
						fabs(v[2] - w[2]) > vml::math::EPSILON;
			}

			// --------------------------------------------------------
			// vector flatting

			template < class T>
			__forceinline vec3<T> Flat(const vec3<T>& v)
			{
				T min = T(v[0]);
				if (v[1] < min)min = v[1];
				if (v[2] < min)min = v[2];
				T max = T(v[0]);
				if (v[1] > max)max = v[1];
				if (v[2] > max)max = v[2];
				T invdenum = max - min;
				if (invdenum > -vml::math::EPSILON && invdenum < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				invdenum = T(1) / invdenum;
				return vec3<T>((v[0] - min) * invdenum, (v[1] - min) * invdenum, (v[2] - min) * invdenum);
			}

			// --------------------------------------------------------
			// return the angle between two vectors

			template <typename T>
			__forceinline T AngleBetween(const vec3<T>& v, const vec3<T>& w)
			{
				T num = v[0] * w[0] + v[1] * w[1] + v[2] * w[2];
				T r = num;
				T denum = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
				if (denum > -vml::math::EPSILON && denum < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				r /= denum;
				denum = sqrtf(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
				if (denum > -vml::math::EPSILON && denum < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				r /= denum;
				if (r < -T(1)) r = -T(1);
				if (r > T(1)) r = T(1);
				return acos(r);
			}

			// --------------------------------------------------------
			// returns angles computed from spherical coordinates 

			template <typename T>
			__forceinline vml::math::vec2<T> Angle(const vec3<T>& v)
			{
				T radius = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
				if (radius > -vml::math::EPSILON && radius < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				return vec2<T>(atan2(v[1], v[0]), acos(v[2] / radius));
			}

			// --------------------------------------------------------
			// Calculates the projection of 'v' onto 'w'.

			template <class T>
			__forceinline vec3<T> Project(const vec3<T>& v, const vec3<T>& w)
			{
				T d = w[0] * w[0] + w[1] * w[1] + w[2] * w[2];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]) / d;
				return num * w;
			}

			// --------------------------------------------------------
			// Calculates the component of 'v' perpendicular to 'w'.

			template <class T>
			__forceinline vec3<T> Perpendicular(const vec3<T>& v, const vec3<T>& w)
			{
				T d = w[0] * w[0] + w[1] * w[1] + w[2] * w[2];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]) / d;
				return v - num * w;
			}

			// --------------------------------------------------------
			// Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
			// turn them into orthonormal basis vectors.

			template < class T>
			__forceinline void Orthogonalize(const vec3<T>& v, const vec3<T>& w, vec3<T>& pv1, vec3<T>& pv2)
			{
				// v is the basis vector normalized

				pv1 = v;
				T d = pv1[0] * pv1[0] + pv1[1] * pv1[1] + pv1[2] * pv1[2];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				d = T(1) / sqrtf(d);
				pv1[0] *= d;
				pv1[1] *= d;
				pv1[2] *= d;

				// compute w-Perpendicular(w,v)

				d = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]) / d;
				pv2 = w - num * v;

				// normalize resulting vector

				d = pv2[0] * pv2[0] + pv2[1] * pv2[1] + pv2[2] * pv2[2];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				d = T(1) / sqrtf(d);
				pv2[0] *= d;
				pv2[1] *= d;
				pv2[2] *= d;

			}

			// --------------------------------------------------------
			// Calculates reflection vector from entering ray direction 'v'
			// and surface normal 'w'.

			template < class T>
			__forceinline vec3<T> Reflect(const vec3<T>& v, const vec3<T>& n)
			{
				T d = n[0] * n[0] + n[1] * n[1] + n[2] * n[2];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec3 : zero lenght vector");
				d = T(1) / sqrtf(d);
				T nx = n[0] * d;
				T ny = n[1] * d;
				T nz = n[2] * d;
				T num = T(2) * (v[0] * nx + v[1] * ny + v[2] * nz);
				return vec3<T>(v[0] - num * nx, v[1] - num * ny, v[2] - num * nz);
			}
			
		}
		
	} // end of namespace math

} // end of namesapce vml


