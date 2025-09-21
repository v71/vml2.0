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
		// vec2 class

		template <typename T>
		class vec2 
		{
			private:

			  T Data[2];

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

			  __forceinline vec2<T>& operator*=(const T s)
			  {
				  Data[0] *= s;
				  Data[1] *= s;
				  return *this;
			  }

			  __forceinline vec2<T>& operator/=(const T s)
			  {
				  if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
					  vml::os::Error("Vec2 : s is zero");
				  T invs= T(1) / T(s);
				  Data[0] *= invs;
				  Data[1] *= invs;
				  return *this;
			  }

			  __forceinline vec2<T>& operator+=(const vec2<T> &v)
			  {
				  Data[0] += v[0];
				  Data[1] += v[1];
				  return *this;
			  }

			  __forceinline vec2<T>& operator+=(const T s)
			  {
				  Data[0] += s;
				  Data[1] += s;
				  return *this;
			  }

			  __forceinline vec2<T>& operator-=(const T s)
			  {
				  Data[0] -= s;
				  Data[1] -= s;
				  return *this;
			  }

			  __forceinline vec2<T>& operator-=(const vec2<T> &v)
			  {
				  Data[0] -= v[0];
				  Data[1] -= v[1];
				  return *this;
			  }

			  __forceinline T operator|=(const vec2<T>& v) const
			  {
				  return Data[0] * v[0] + Data[1] * v[1];
			  }

			  __forceinline T operator^=(const vec2<T>& v) const
			  {
				  return Data[0] * v[1] - Data[1] * v[0];
			  }

			  __forceinline vec2<T> &operator+() 
			  {
				  return *this;
			  }

			  __forceinline vec2<T> operator-() 
			  {
				  return vec2<T>(-Data[0], -Data[1]);
			  }
			  
			  // --------------------------------------------------------------------
			  // comparison operators

			  __forceinline bool operator< ( const vec2<T> &v ) const
			  {
				return memcmp(Data,v.Data, 2 * sizeof(T)) < 0;
			  }

			  __forceinline bool operator== ( const vec2<T> &v ) const
			  {
				return memcmp(Data,v.Data, 2 * sizeof(T)) == 0;
			  }
 
			  __forceinline bool operator<= ( const vec2<T> &v ) const
			  {
				return memcmp(Data,v.Data, 2 * sizeof(T)) <= 0;
			  }

			  __forceinline bool operator> ( const vec2<T> &v ) const
			  {
				return memcmp(Data,v.Data, 2 * sizeof(T)) > 0;
			  }

			  __forceinline bool operator>= ( const vec2<T> &v ) const
			  {
				return memcmp(Data,v.Data, 2 * sizeof(T)) >= 0;
			  }

			  __forceinline bool operator!= ( const vec2<T> &v ) const
			  {
				return memcmp(Data,v.Data, 2 * sizeof(T)) != 0;
			  }
			
			  __forceinline const T* GetData() const
			  {
				  return Data;
			  }

			  // --------------------------------------------------------------------
			  // query functions

			  __forceinline T* GetData() 
			  {
				  return Data;
			  }

			// --------------------------------------------------------------------
			// ctor / dtor

			vec2()
			{ 
				Data[0] = T(0);
				Data[1] = T(0);
			}

			vec2(const T x, const T y)
			{
				Data[0] = x;
				Data[1] = y;
			}

			// ---------------------------------------------------------------
			// assignment operator

			vec2& operator= (const vec2<T> &v)
			{
				if (&v == this)
					return *this;
				Data[0] = v[0];
				Data[1] = v[1];
				return *this;
			}

			// ---------------------------------------------------------------
			// copy constructor

			vec2(const vec2<T> &v)
			{
				Data[0] = v[0];
				Data[1] = v[1];
			}

			~vec2()
			{}

		};

		/////////////////////////////////////////////////////////////////////////////////
		// friend functions
	
		template <typename T>
		__forceinline vec2<T> operator+(const vec2<T> &v, const vec2<T> &w)
		{
			return vec2<T>(v[0] + w[0], v[1] + w[1]);
		}

		template <typename T>
		__forceinline vec2<T> operator-(const vec2<T> &v, const vec2<T> &w)
		{
			return vec2<T>(v[0] - w[0], v[1] - w[1]);
		}

		template <typename T>
		__forceinline vec2<T> operator+(const vec2<T>& v, const T s)
		{
			return vec2<T>(v[0] + s, v[1] + s);
		}

		template <typename T>
		__forceinline vec2<T> operator-(const vec2<T>& v, const T s)
		{
			return vec2<T>(v[0] - s, v[1] - s);
		}

		template <typename T>
		__forceinline vec2<T> operator*(const vec2<T> &v, const T s)
		{
			return vec2<T>(v[0] * s, v[1] * s);
		}

		template <typename T>
		__forceinline vec2<T> operator*(const T s, const vec2<T> &v)
		{
			return vec2<T>(s*v[0], s*v[1]);
		}

		template <typename T>
		__forceinline vec2<T> operator/(const vec2<T> &v, const T s)
		{
			if (s > -vml::math::EPSILON && s < vml::math::EPSILON) 
				vml::os::Error("Vec2 : scalar is zero");
			T invs = T(1) / s;
			return vec2<T>(v[0] * invs, v[1] * invs);
		}

		template <typename T>
		__forceinline T operator| (const vec2<T>& u, const vec2<T>& v) 
		{
			return u[0] * v[0] + u[1] * v[1];
		}

		template <typename T>
		__forceinline T operator^ (const vec2<T>& u, const vec2<T>& v)
		{
			return u[0] * v[1] - u[1] * v[0];
		}

		template <typename T >
		std::ostream& operator<< (std::ostream& output, const vml::math::vec2<T> &v)
		{
			std::ostringstream os;
			os << " [ " << v[0] << " , " << v[1] << " ]";
			return os.str();
		}

		///////////////////////////////////////////////////////////////////////////////////
		// predefined types
		
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

	} // end of namespace math
}
// end of namesapce vml

/////////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace math
	{

			// --------------------------------------------------------------------
			// metrics functions

			template <typename T>
			__forceinline T SquaredLength(const vec2<T> &v)
			{
				return v[0] * v[0] + v[1] * v[1];
			}

			template <typename T>
			__forceinline T Length(const vec2<T>& v) 
			{
				return sqrtf(v[0] * v[0] + v[1] * v[1]);
			}

			// --------------------------------------------------------
			// return sthe dot product between two vectors

			template <typename T>
			__forceinline T Dot(const vec2<T>& v, const vec2<T>& w)
			{
				return (v[0] * w[0] + v[1] * w[1]);
			}

			template <typename T>
			__forceinline const vec2<T> Normalize(const vec2<T>& v)
			{
				T d = sqrtf(v[0] * v[0] + v[1] * v[1]);
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec2 : singular value");
				d = T(1) / d;
				return (v[0] * d, v[1] * d); 
			}

			// --------------------------------------------------------
			// cross product

			template <typename T>
			__forceinline vec2<T> Cross(const vec2<T>& v, const vec2<T>& w)
			{
				return vec2<T>(v[0] * w[1], -v[1] * w[0]);
			}

			// --------------------------------------------------------
			// return a vector orthogonal to a given vector

			template <typename T>
			__forceinline vec2<T> Ortho(const vec2<T>& v)
			{
				return vec2<T>(-v[1], v[0]);
			}

			// --------------------------------------------------------
			// reciprocal of a vector 

			template <typename T>
			__forceinline vec2<T> Reciprocal(const vec2<T>& v)
			{
				if (v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				if (v[1] > -vml::math::EPSILON && v[1] < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				return vec2<T>(T(1) / v[0], T(1) / v[1]);
			}

			// --------------------------------------------------------
			// absolute value

			template <typename T >
			__forceinline vec2<T> Abs(const vec2<T>& v)
			{
				return vec2<T>(fabsf(v[0]), fabsf(v[1]));
			}

			// --------------------------------------------------------
			// center value

			template <typename T>
			__forceinline vec2<T> Mid(const vec2<T>& v, const vec2<T>& w)
			{
				return vec2<T>((v[0] + w[0]) * T(0.5), (v[1] + w[1]) * T(0.5));
			}

			// --------------------------------------------------------
			// Linera interpolation

			template <typename T>
			__forceinline vec2<T> Lerp(const vec2<T>& v, const vec2<T>& w, const T s)
			{
				return vec2<T>(v[0] + (w[0] - v[0]) * s, v[1] + (w[1] - v[1]) * s);
			}

			// --------------------------------------------------------

			template <typename T>
			__forceinline vec2<T> Saturate(const vec2<T>& v)
			{
				T x = v[0];
				T y = v[1];
				if (x > T(1) - vml::math::EPSILON) x = T(1);
				if (y > T(1) - vml::math::EPSILON) y = T(1);
				if (x < T(0) + vml::math::EPSILON) x = T(0);
				if (y < T(0) + vml::math::EPSILON) y = T(0);
				return vec2<T>(x, y);
			}

			// --------------------------------------------------------
			// Saturate to zero

			template <typename T>
			__forceinline vec2<T> SaturateToZero(const vec2<T>& v)
			{
				T x = v[0];
				T y = v[1];
				if (x < T(0) + vml::math::EPSILON) x = T(0);
				if (y < T(0) + vml::math::EPSILON) y = T(0);
				return vec2<T>(x, y);
			}

			// --------------------------------------------------------
			// Saturate a vector to one

			template <typename T>
			__forceinline vec2<T> SaturateToOne(const vec2<T>& v)
			{
				T x = v[0];
				T y = v[1];
				if (x > T(1) - vml::math::EPSILON) x = T(1);
				if (y > T(1) - vml::math::EPSILON) y = T(1);
				return vec2<T>(x, y);
			}

			// --------------------------------------------------------
			// floors a vector

			template <typename T>
			__forceinline vec2<T> Floor(const vec2<T>& v)
			{
				return vec2<T>(floor(v[0]), floor(v[1]));
			}

			// --------------------------------------------------------
			// ceils a vector

			template <typename T>
			__forceinline vec2<T> Ceil(const vec2<T>& v)
			{
				return vec2<T>(ceil(v[0]), ceil(v[1]));
			}

			// --------------------------------------------------------
			// negate a vector

			template <typename T>
			__forceinline vec2<T> Negate(const vec2<T>& v)
			{
				return vec2<T>(-v[0], -v[1]);
			}

			template <typename T>
			__forceinline vec2<T> Zero()
			{
				return vec2<T>(T(0), T(0));
			}

			template <typename T>
			__forceinline vec2<T> Identity()
			{
				return vec2<T>(T(1), T(1));
			}

			// --------------------------------------------------------
			// returns a random vector in the min - max range

			template <typename T>
			__forceinline vec2<T> Random(const int min, const int max)
			{
				return vec2<T>(T(min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1))),
							   T(min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1))));
			}
			
			// --------------------------------------------------------
			// returns the distance from two vectors

			template <typename T>
			__forceinline T DistanceFrom(const vec2<T>& v, const vec2<T>& w)
			{
				T dx = v[0] - w[0];
				T dy = v[1] - w[1];
				return sqrtf(dx * dx + dy * dy);
			}

			// --------------------------------------------------------
			// return the trace of a given vector

			template <typename T>
			__forceinline T Trace(const vec2<T>& v)
			{
				return v[0] + v[1];
			}

			// --------------------------------------------------------
			// transpose a vector

			template <typename T>
			__forceinline vec2<T> Transpose(const vec2<T>& v)
			{
				return vec2<T>(v[1], v[0]);
			}

			// --------------------------------------------------------
			// return the angle between two vectors

			template <typename T>
			__forceinline T AngleBetween(const vec2<T>& v, const vec2<T>& w)
			{
				T num = v[0] * w[0] + v[1] * w[1];
				T r = num;
				T denum = sqrtf(v[0] * v[0] + v[1] * v[1]);
				if (denum > -vml::math::EPSILON && denum < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				r /= denum;
				denum = sqrtf(w[0] * w[0] + w[1] * w[1]);
				if (denum > -vml::math::EPSILON && denum < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				r /= denum;
				if (r < -T(1)) r = -T(1);
				if (r > T(1)) r = T(1);
				return acos(r);
			}

			// --------------------------------------------------------
			// return the atan ( y/x ) in radians

			template <typename T>
			__forceinline T Angle(const vec2<T>& v)
			{
				if (v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON)
					vml::os::Error("Vec2 : x component is null");
				return atan2(v[1], v[0]);
			}

			// --------------------------------------------------------
			// gets the maximum value component of a given vector

			template <typename T>
			__forceinline T GetMax(const vec2<T>& v)
			{
				return v[0] > v[1] ? v[0] : v[1];
			}

			// --------------------------------------------------------
			// gets the absolute maximum value component of a given vector

			template <typename T>
			__forceinline T GetAbsMax(const vec2<T>& v)
			{
				return fabs(v[0]) > fabs(v[1]) ? fabs(v[0]) : fabs(v[1]);
			}

			// --------------------------------------------------------
			// gets the minimum value component of a given vector

			template <typename T>
			__forceinline T GetMin(const vec2<T>& v)
			{
				return v[0] < v[1] ? v[0] : v[1];
			}

			// --------------------------------------------------------
			// gets the absolute maximum value component of a given vector

			template <typename T>
			__forceinline T GetAbsMin(const vec2<T>& v)
			{
				return fabs(v[0]) < fabs(v[1]) ? fabs(v[0]) : fabs(v[1]);
			}

			// --------------------------------------------------------------------
			// check if vector is nearly zero

			template <typename T>
			__forceinline bool IsZero(const vec2<T>& v)
			{
				if ((v[0] > -vml::math::EPSILON && v[0] < vml::math::EPSILON) &&
					(v[1] > -vml::math::EPSILON && v[1] < vml::math::EPSILON))
					return true;
				return false;
			}

			// --------------------------------------------------------------------
			// checks if vector is nearly identticaly to (1,1)
			template <typename T>
			__forceinline bool IsIdentity(const vec2<T>& v)
			{
				if ((v[0] > T(1) - vml::math::EPSILON && v[0] < T(1) + vml::math::EPSILON) &&
					(v[1] > T(1) - vml::math::EPSILON && v[1] < T(1) + vml::math::EPSILON))
					return true;
				return false;
			}

			// --------------------------------------------------------------------
			// checks if two vectors are nearly equal

			template <typename T>
			__forceinline bool IsEqualTo(const vec2<T>& v, const vec2<T>& w)
			{
				return fabs(v[0] - w[0]) < vml::math::EPSILON &&
				       fabs(v[1] - w[1]) < vml::math::EPSILON;
			}

			// --------------------------------------------------------------------
			// checks if two vectors are not nearly equal

			template <typename T>
			__forceinline bool IsNotEqualTo(const vec2<T>& v, const vec2<T>& w)
			{
				return fabs(v[0] - w[0]) > vml::math::EPSILON ||
					   fabs(v[1] - w[1]) > vml::math::EPSILON;
			}

			// --------------------------------------------------------
			// vector flatting

			template < class T>
			__forceinline vec2<T> Flat(const vec2<T>& v)
			{
				T min = v[0];
				if (v[1] < min)min = v[1];
				T max = v[0];
				if (v[1] > max)max = v[1];
				T invdenum = max - min;
				if (invdenum > -vml::math::EPSILON && invdenum < vml::math::EPSILON)
					vml::os::Error("Vec2 : determinat is null");
				invdenum = T(1) / invdenum;
				return vec2<T>((v[0] - min) * invdenum, (v[1] - min) * invdenum);
			}

			// --------------------------------------------------------
			// Calculates the projection of 'v' onto 'w'.

			template <class T>
			__forceinline vec2<T> Project(const vec2<T>& v, const vec2<T>& w)
			{
				T d = w[0] * w[0] + w[1] * w[1];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1]) / d;
				return num * w;
			}

			// --------------------------------------------------------
			// Calculates the component of 'v' perpendicular to 'w'.

			template <class T>
			__forceinline vec2<T> Perpendicular(const vec2<T>& v, const vec2<T>& w)
			{
				T d = w[0] * w[0] + w[1] * w[1];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1]) / d;
				return v - num * w;
			}

			// --------------------------------------------------------
			// Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
			// turn them into orthonormal basis vectors.

			template < class T>
			__forceinline void Orthogonalize(const vec2<T>& v, const vec2<T>& w, vec2<T>& pv1, vec2<T>& pv2)
			{
				// v is the basis vector normalized

				pv1 = v;
				T d = pv1[0] * pv1[0] + pv1[1] * pv1[1];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				d = T(1) / sqrtf(d);
				pv1[0] *= d;
				pv1[1] *= d;

				// compute w-Perpendicular(w,v)

				d = v[0] * v[0] + v[1] * v[1];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				T num = (v[0] * w[0] + v[1] * w[1]) / d;
				pv2 = w - num * v;

				// normalize resulting vector

				d = pv2[0] * pv2[0] + pv2[1] * pv2[1];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				d = T(1) / sqrtf(d);
				pv2[0] *= d;
				pv2[1] *= d;
			}

			// --------------------------------------------------------
			// Calculates reflection vector from entering ray direction 'v'
			// and surface normal 'w'.

			template < class T>
			__forceinline vec2<T> Reflect(const vec2<T>& v, const vec2<T>& n)
			{
				T d = n[0] * n[0] + n[1] * n[1];
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					vml::os::Error("Vec2 : zero lenght vector");
				d = T(1) / sqrtf(d);
				T nx = n[0] * d;
				T ny = n[1] * d;
				T num = T(2) * (v[0] * nx + v[1] * ny);
				return vec2<T>(v[0] - num * nx, v[1] - num * ny);
			}

	} // end of namespace math
}
// end of namesapce vml
