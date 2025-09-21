#pragma once

///////////////////////////////////////
// 4x4 matrix class

namespace vml
{
	namespace math
	{

		template< class T>
		class mat4x4
		{
			private:

				T	   Data[16];

			public:

				// --------------------------------------------------------------------
				// access operators

				__forceinline T& operator[](const int i)
				{
					return Data[i];
				}

				__forceinline const T& operator[](const int i)  const
				{
					return Data[i];
				}

				__forceinline const T& operator()(const size_t i, const size_t j) const
				{
					return Data[i + j * 4];
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

				// ---------------------------------------------------------------
				// unary operators

				__forceinline const mat4x4 operator -()
				{
					return mat4x4(-Data[ 0], -Data[ 1], -Data[ 2], -Data[ 3],
									  -Data[ 4], -Data[ 5], -Data[ 6], -Data[ 7],
									  -Data[ 8], -Data[ 9], -Data[10], -Data[11],
									  -Data[12], -Data[13], -Data[14], -Data[15]);
				}

				__forceinline const mat4x4 operator +()
				{
					return *this;
				}
				
				// ---------------------------------------------------------------
				// aritmetic operators

				__forceinline const mat4x4& operator +=(const mat4x4& M)
				{
					Data[ 0] += M.Data[ 0]; Data[ 1] += M.Data[ 1]; Data[ 2] += M.Data[ 2]; Data[ 3] += M.Data[3];
					Data[ 4] += M.Data[ 4]; Data[ 5] += M.Data[ 5]; Data[ 6] += M.Data[ 6]; Data[ 7] += M.Data[7];
					Data[ 8] += M.Data[ 8]; Data[ 9] += M.Data[ 9]; Data[10] += M.Data[10]; Data[11] += M.Data[11];
					Data[12] += M.Data[12]; Data[13] += M.Data[13]; Data[14] += M.Data[14]; Data[15] += M.Data[15];
					return *this;
				}

				__forceinline const mat4x4& operator +=(const T s)
				{
					Data[ 0] += s; Data[ 1] += s; Data[ 2] += s; Data[ 3] += s;
					Data[ 4] += s; Data[ 5] += s; Data[ 6] += s; Data[ 7] += s;
					Data[ 8] += s; Data[ 9] += s; Data[10] += s; Data[11] += s;
					Data[12] += s; Data[13] += s; Data[14] += s; Data[15] += s;
					return *this;
				}

				__forceinline const mat4x4& operator -=(const mat4x4& M)
				{
					Data[ 0] -= M.Data[ 0]; Data[ 1] -= M.Data[ 1]; Data[ 2] -= M.Data[ 2]; Data[ 3] -= M.Data[3];
					Data[ 4] -= M.Data[ 4]; Data[ 5] -= M.Data[ 5]; Data[ 6] -= M.Data[ 6]; Data[ 7] -= M.Data[7];
					Data[ 8] -= M.Data[ 8]; Data[ 9] -= M.Data[ 9]; Data[10] -= M.Data[10]; Data[11] -= M.Data[11];
					Data[12] -= M.Data[12]; Data[13] -= M.Data[13]; Data[14] -= M.Data[14]; Data[15] -= M.Data[15];
					return *this;
				}

				__forceinline const mat4x4& operator -=(const T s)
				{
					Data[ 0] -= s; Data[ 1] -= s; Data[ 2] -= s; Data[ 3] -= s;
					Data[ 4] -= s; Data[ 5] -= s; Data[ 6] -= s; Data[ 7] -= s;
					Data[ 8] -= s; Data[ 9] -= s; Data[10] -= s; Data[11] -= s;
					Data[12] -= s; Data[13] -= s; Data[14] -= s; Data[15] -= s;
					return *this;
				}

				__forceinline const mat4x4& operator *=(const mat4x4& M)
				{
					T  t[16] = { Data[ 0] * M.Data[0] + Data[ 1] * M.Data[4] + Data[ 2] * M.Data[ 8] + Data[ 3] * M.Data[12],
								 Data[ 0] * M.Data[1] + Data[ 1] * M.Data[5] + Data[ 2] * M.Data[ 9] + Data[ 3] * M.Data[13],
								 Data[ 0] * M.Data[2] + Data[ 1] * M.Data[6] + Data[ 2] * M.Data[10] + Data[ 3] * M.Data[14],
								 Data[ 0] * M.Data[3] + Data[ 1] * M.Data[7] + Data[ 2] * M.Data[11] + Data[ 3] * M.Data[15],
								 Data[ 4] * M.Data[0] + Data[ 5] * M.Data[4] + Data[ 6] * M.Data[ 8] + Data[ 7] * M.Data[12],
								 Data[ 4] * M.Data[1] + Data[ 5] * M.Data[5] + Data[ 6] * M.Data[ 9] + Data[ 7] * M.Data[13],
								 Data[ 4] * M.Data[2] + Data[ 5] * M.Data[6] + Data[ 6] * M.Data[10] + Data[ 7] * M.Data[14],
								 Data[ 4] * M.Data[3] + Data[ 5] * M.Data[7] + Data[ 6] * M.Data[11] + Data[ 7] * M.Data[15],
								 Data[ 8] * M.Data[0] + Data[ 9] * M.Data[4] + Data[10] * M.Data[ 8] + Data[11] * M.Data[12],
								 Data[ 8] * M.Data[1] + Data[ 9] * M.Data[5] + Data[10] * M.Data[ 9] + Data[11] * M.Data[13],
								 Data[ 8] * M.Data[2] + Data[ 9] * M.Data[6] + Data[10] * M.Data[10] + Data[11] * M.Data[14],
								 Data[ 8] * M.Data[3] + Data[ 9] * M.Data[7] + Data[10] * M.Data[11] + Data[11] * M.Data[15],
								 Data[12] * M.Data[0] + Data[13] * M.Data[4] + Data[14] * M.Data[ 8] + Data[15] * M.Data[12],
								 Data[12] * M.Data[1] + Data[13] * M.Data[5] + Data[14] * M.Data[ 9] + Data[15] * M.Data[13],
								 Data[12] * M.Data[2] + Data[13] * M.Data[6] + Data[14] * M.Data[10] + Data[15] * M.Data[14],
								 Data[12] * M.Data[3] + Data[13] * M.Data[7] + Data[14] * M.Data[11] + Data[15] * M.Data[15] };
					
					Data[ 0] = t[ 0];
					Data[ 1] = t[ 1];
					Data[ 2] = t[ 2];
					Data[ 3] = t[ 3];
					Data[ 4] = t[ 4];
					Data[ 5] = t[ 5];
					Data[ 6] = t[ 6];
					Data[ 7] = t[ 7];
					Data[ 8] = t[ 8];
					Data[ 9] = t[ 9];
					Data[10] = t[10];
					Data[11] = t[11];
					Data[12] = t[12];
					Data[13] = t[13];
					Data[14] = t[14];
					Data[15] = t[15];

					return *this;
				}

				__forceinline const mat4x4& operator *=(const T s)
				{
					Data[ 0] *= s; Data[ 1] *= s; Data[ 2] *= s; Data[ 3] *= s;
					Data[ 4] *= s; Data[ 5] *= s; Data[ 6] *= s; Data[ 7] *= s;
					Data[ 8] *= s; Data[ 9] *= s; Data[10] *= s; Data[11] *= s;
					Data[12] *= s; Data[13] *= s; Data[14] *= s; Data[15] *= s;
					return *this;
				}

				const mat4x4& operator /=(const mat4x4& R)
				{
					T det = R[12] * R[ 9] * R[ 6] * R[ 3] -
							R[ 8] * R[13] * R[ 6] * R[ 3] -
							R[12] * R[ 5] * R[10] * R[ 3] +
							R[ 4] * R[13] * R[10] * R[ 3] +
							R[ 8] * R[ 5] * R[14] * R[ 3] -
							R[ 4] * R[ 9] * R[14] * R[ 3] -
							R[12] * R[ 9] * R[ 2] * R[ 7] +
							R[ 8] * R[13] * R[ 2] * R[ 7] +
							R[12] * R[ 1] * R[10] * R[ 7] -
							R[ 0] * R[13] * R[10] * R[ 7] -
							R[ 8] * R[ 1] * R[14] * R[ 7] +
							R[ 0] * R[ 9] * R[14] * R[ 7] +
							R[12] * R[ 5] * R[ 2] * R[11] -
							R[ 4] * R[13] * R[ 2] * R[11] -
							R[12] * R[ 1] * R[ 6] * R[11] +
							R[ 0] * R[13] * R[ 6] * R[11] +
							R[ 4] * R[ 1] * R[14] * R[11] -
							R[ 0] * R[ 5] * R[14] * R[11] -
							R[ 8] * R[ 5] * R[ 2] * R[15] +
							R[ 4] * R[ 9] * R[ 2] * R[15] +
							R[ 8] * R[ 1] * R[ 6] * R[15] -
							R[ 0] * R[ 9] * R[ 6] * R[15] -
							R[ 4] * R[ 1] * R[10] * R[15] +
							R[ 0] * R[ 5] * R[10] * R[15];

					// cannot invert singular matrix

					if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
						vml::os::Error("Matrix4x4 : Matrix is singular");

					T m00 = R[ 9] * R[14] * R[ 7] - R[13] * R[10] * R[ 7] + R[13] * R[ 6] * R[11] -
						    R[ 5] * R[14] * R[11] - R[ 9] * R[ 6] * R[15] + R[ 5] * R[10] * R[15];
					T m01 = R[12] * R[10] * R[ 7] - R[ 8] * R[14] * R[ 7] - R[12] * R[ 6] * R[11] +
						    R[ 4] * R[14] * R[11] + R[ 8] * R[ 6] * R[15] - R[ 4] * R[10] * R[15];
					T m02 = R[ 8] * R[13] * R[ 7] - R[12] * R[ 9] * R[ 7] + R[12] * R[ 5] * R[11] -
						    R[ 4] * R[13] * R[11] - R[ 8] * R[ 5] * R[15] + R[ 4] * R[ 9] * R[15];
					T m03 = R[12] * R[ 9] * R[ 6] - R[ 8] * R[13] * R[ 6] - R[12] * R[ 5] * R[10] +
						    R[ 4] * R[13] * R[10] + R[ 8] * R[ 5] * R[14] - R[ 4] * R[ 9] * R[14];
					T m10 = R[13] * R[10] * R[ 3] - R[ 9] * R[14] * R[ 3] - R[13] * R[ 2] * R[11] +
						    R[ 1] * R[14] * R[11] + R[ 9] * R[ 2] * R[15] - R[ 1] * R[10] * R[15];
					T m11 = R[ 8] * R[14] * R[ 3] - R[12] * R[10] * R[ 3] + R[12] * R[ 2] * R[11] -
						    R[ 0] * R[14] * R[11] - R[ 8] * R[ 2] * R[15] + R[ 0] * R[10] * R[15];
					T m12 = R[12] * R[ 9] * R[ 3] - R[ 8] * R[13] * R[ 3] - R[12] * R[ 1] * R[11] +
						    R[ 0] * R[13] * R[11] + R[ 8] * R[ 1] * R[15] - R[ 0] * R[ 9] * R[15];
					T m13 = R[ 8] * R[13] * R[ 2] - R[12] * R[ 9] * R[ 2] + R[12] * R[ 1] * R[10] -
						    R[ 0] * R[13] * R[10] - R[ 8] * R[ 1] * R[14] + R[ 0] * R[ 9] * R[14];
					T m20 = R[ 5] * R[14] * R[ 3] - R[13] * R[ 6] * R[ 3] + R[13] * R[ 2] * R[ 7] -
						    R[ 1] * R[14] * R[ 7] - R[ 5] * R[ 2] * R[15] + R[ 1] * R[ 6] * R[15];
					T m21 = R[12] * R[ 6] * R[ 3] - R[ 4] * R[14] * R[ 3] - R[12] * R[ 2] * R[ 7] +
						    R[ 0] * R[14] * R[ 7] + R[ 4] * R[ 2] * R[15] - R[ 0] * R[ 6] * R[15];
					T m22 = R[ 4] * R[13] * R[ 3] - R[12] * R[ 5] * R[ 3] + R[12] * R[ 1] * R[ 7] -
						    R[ 0] * R[13] * R[ 7] - R[ 4] * R[ 1] * R[15] + R[ 0] * R[ 5] * R[15];
					T m23 = R[12] * R[ 5] * R[ 2] - R[ 4] * R[13] * R[ 2] - R[12] * R[ 1] * R[ 6] +
						    R[ 0] * R[13] * R[ 6] + R[ 4] * R[ 1] * R[14] - R[ 0] * R[ 5] * R[14];
					T m30 = R[ 9] * R[ 6] * R[ 3] - R[ 5] * R[10] * R[ 3] - R[ 9] * R[ 2] * R[ 7] +
						    R[ 1] * R[10] * R[ 7] + R[ 5] * R[ 2] * R[11] - R[ 1] * R[ 6] * R[11];
					T m31 = R[ 4] * R[10] * R[ 3] - R[ 8] * R[ 6] * R[ 3] + R[ 8] * R[ 2] * R[ 7] -
						    R[ 0] * R[10] * R[ 7] - R[ 4] * R[ 2] * R[11] + R[ 0] * R[ 6] * R[11];
					T m32 = R[ 8] * R[ 5] * R[ 3] - R[ 4] * R[ 9] * R[ 3] - R[ 8] * R[ 1] * R[ 7] +
						    R[ 0] * R[ 9] * R[ 7] + R[ 4] * R[ 1] * R[11] - R[ 0] * R[ 5] * R[11];
					T m33 = R[ 4] * R[ 9] * R[ 2] - R[ 8] * R[ 5] * R[ 2] + R[ 8] * R[ 1] * R[ 6] -
						    R[ 0] * R[ 9] * R[ 6] - R[ 4] * R[ 1] * R[10] + R[ 0] * R[ 5] * R[10];

					det = 1.0f / det;

					m00 *= det;	m10 *= det;	m20 *= det;	m30 *= det;
					m01 *= det;	m11 *= det;	m21 *= det;	m31 *= det;
					m02 *= det;	m12 *= det;	m22 *= det;	m32 *= det;
					m03 *= det;	m13 *= det;	m23 *= det;	m33 *= det;

					T  t[16] = { Data[ 0] * m00 + Data[ 1] * m01 + Data[ 2] * m02 + Data[ 3] * m03,
								 Data[ 0] * m10 + Data[ 1] * m11 + Data[ 2] * m12 + Data[ 3] * m13,
								 Data[ 0] * m20 + Data[ 1] * m21 + Data[ 2] * m22 + Data[ 3] * m23,
								 Data[ 0] * m30 + Data[ 1] * m31 + Data[ 2] * m32 + Data[ 3] * m33,
								 Data[ 4] * m00 + Data[ 5] * m01 + Data[ 6] * m02 + Data[ 7] * m03,
								 Data[ 4] * m10 + Data[ 5] * m11 + Data[ 6] * m12 + Data[ 7] * m13,
								 Data[ 4] * m20 + Data[ 5] * m21 + Data[ 6] * m22 + Data[ 7] * m23,
								 Data[ 4] * m30 + Data[ 5] * m31 + Data[ 6] * m32 + Data[ 7] * m33,
								 Data[ 8] * m00 + Data[ 9] * m01 + Data[10] * m02 + Data[11] * m03,
								 Data[ 8] * m10 + Data[ 9] * m11 + Data[10] * m12 + Data[11] * m13,
								 Data[ 8] * m20 + Data[ 9] * m21 + Data[10] * m22 + Data[11] * m23,
								 Data[ 8] * m30 + Data[ 9] * m31 + Data[10] * m32 + Data[11] * m33,
								 Data[12] * m00 + Data[13] * m01 + Data[14] * m02 + Data[15] * m03,
								 Data[12] * m10 + Data[13] * m11 + Data[14] * m12 + Data[15] * m13,
								 Data[12] * m20 + Data[13] * m21 + Data[14] * m22 + Data[15] * m23,
								 Data[12] * m30 + Data[13] * m31 + Data[14] * m32 + Data[15] * m33 };

					Data[0] = t[0];
					Data[1] = t[1];
					Data[2] = t[2];
					Data[3] = t[3];
					Data[4] = t[4];
					Data[5] = t[5];
					Data[6] = t[6];
					Data[7] = t[7];
					Data[8] = t[8];
					Data[9] = t[9];
					Data[10] = t[10];
					Data[11] = t[11];
					Data[12] = t[12];
					Data[13] = t[13];
					Data[14] = t[14];
					Data[15] = t[15];

					return *this;
				}

				const mat4x4& operator /=(T s)
				{
					if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
						vml::os::Error("Matrix4x4 : scalar is null");
					double invs = 1.0f / (T)s;
					Data[ 0] *= invs; Data[ 1] *= invs; Data[ 2] *= invs; Data[ 3] *= invs;
					Data[ 4] *= invs; Data[ 5] *= invs; Data[ 6] *= invs; Data[ 7] *= invs;
					Data[ 8] *= invs; Data[ 9] *= invs; Data[10] *= invs; Data[11] *= invs;
					Data[12] *= invs; Data[13] *= invs; Data[14] *= invs; Data[15] *= invs;
					return *this;
				}

				// --------------------------------------------------------------
				// comparison operators

				bool operator== (const mat4x4& M) const
				{
					return memcmp(Data, M.Data, 16 * sizeof(T)) == 0;
				}

				bool operator!= (const mat4x4& M) const
				{
					return memcmp(Data, M.Data, 16 * sizeof(T)) != 0;
				}

				bool operator<= (const mat4x4& M) const
				{
					return memcmp(Data, M.Data, 16 * sizeof(T)) <= 0;
				}

				bool operator>= (const mat4x4& M) const
				{
					return memcmp(Data, M.Data, 16 * sizeof(T)) >= 0;
				}

				bool operator> (const mat4x4& M) const
				{
					return memcmp(Data, M.Data, 16 * sizeof(T)) > 0;
				}

				bool operator< (const mat4x4& M) const
				{
					return memcmp(Data, M.Data, 16 * sizeof(T)) < 0;
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
					return  i + j * 4;
				}
				
				// ---------------------------------------------------------------
				// copy constructor
				// if you assign different order matrices
				// the dest matrix will take the source's order

				mat4x4(const mat4x4& M)
				{
					Data[ 0] = M.Data[ 0]; Data[ 1] = M.Data[ 1]; Data[ 2] = M.Data[ 2]; Data[ 3] = M.Data[ 3];
					Data[ 4] = M.Data[ 4]; Data[ 5] = M.Data[ 5]; Data[ 6] = M.Data[ 6]; Data[ 7] = M.Data[ 7];
					Data[ 8] = M.Data[ 8]; Data[ 9] = M.Data[ 9]; Data[10] = M.Data[10]; Data[11] = M.Data[11];
					Data[12] = M.Data[12]; Data[13] = M.Data[13]; Data[14] = M.Data[14]; Data[15] = M.Data[15];
				}

				// ---------------------------------------------------------------
				// assignment operator
				// if you assign different order matrices
				// the dest matrix will take the source's order

				const mat4x4& operator =(const mat4x4& M)
				{
					if (this == &M)
						return *this;
					Data[ 0] = M.Data[ 0]; Data[ 1] = M.Data[ 1]; Data[ 2] = M.Data[ 2]; Data[ 3] = M.Data[ 3];
					Data[ 4] = M.Data[ 4]; Data[ 5] = M.Data[ 5]; Data[ 6] = M.Data[ 6]; Data[ 7] = M.Data[ 7];
					Data[ 8] = M.Data[ 8]; Data[ 9] = M.Data[ 9]; Data[10] = M.Data[10]; Data[11] = M.Data[11];
					Data[12] = M.Data[12]; Data[13] = M.Data[13]; Data[14] = M.Data[14]; Data[15] = M.Data[15];
					return *this;
				}

				// --------------------------------------------------------------
				// ctor / dtor

				mat4x4()
				{
					Data[ 0] = 1; Data[ 1] = 0; Data[ 2] = 0; Data[ 3] = 0;
					Data[ 4] = 0; Data[ 5] = 1; Data[ 6] = 0; Data[ 7] = 0;
					Data[ 8] = 0; Data[ 9] = 0; Data[10] = 1; Data[11] = 0;
					Data[12] = 0; Data[13] = 0; Data[14] = 0; Data[15] = 1;
				}

				mat4x4(T m11, T m12, T m13, T m14,
					   T m21, T m22, T m23, T m24,
					   T m31, T m32, T m33, T m34,
					   T m41, T m42, T m43, T m44)
				{
					Data[ 0] = m11; Data[ 1] = m12; Data[ 2] = m13; Data[ 3] = m14;
					Data[ 4] = m21; Data[ 5] = m22; Data[ 6] = m23; Data[ 7] = m24;
					Data[ 8] = m31; Data[ 9] = m32; Data[10] = m33; Data[11] = m34;
					Data[12] = m41; Data[13] = m42; Data[14] = m43; Data[15] = m44;
				}

				~mat4x4()
				{
				}

		};

		/////////////////////////////////////////////////////////////////////////////////
		// friend functions

		template<class T>
		const mat4x4<T> operator *(const mat4x4<T>& R, const T s)
		{
			return mat4x4<T>(R[ 0] * s, R[ 1] * s, R[ 2] * s, R[ 3] * s,
								 R[ 4] * s, R[ 5] * s, R[ 6] * s, R[ 7] * s,
								 R[ 8] * s, R[ 9] * s, R[10] * s, R[11] * s,
								 R[12] * s, R[13] * s, R[14] * s, R[15] * s);
		}
		
		template<class T>
		const mat4x4<T> operator *(const T s, const mat4x4<T>& R)
		{
			return mat4x4<T>(R[ 0] * s, R[ 1] * s, R[ 2] * s, R[ 3] * s,
								 R[ 4] * s, R[ 5] * s, R[ 6] * s, R[ 7] * s,
								 R[ 8] * s, R[ 9] * s, R[10] * s, R[11] * s,
								 R[12] * s, R[13] * s, R[14] * s, R[15] * s);
		}

		template<class T>
		const mat4x4<T> operator *(const mat4x4<T>& L, const mat4x4<T>& R)
		{
			return mat4x4<T>(L[ 0] * R[0] + L[ 1] * R[4] + L[ 2] * R[ 8] + L[ 3] * R[12],	//0
								 L[ 0] * R[1] + L[ 1] * R[5] + L[ 2] * R[ 9] + L[ 3] * R[13],	//1
								 L[ 0] * R[2] + L[ 1] * R[6] + L[ 2] * R[10] + L[ 3] * R[14],	//2
								 L[ 0] * R[3] + L[ 1] * R[7] + L[ 2] * R[11] + L[ 3] * R[15],	//3
								 L[ 4] * R[0] + L[ 5] * R[4] + L[ 6] * R[ 8] + L[ 7] * R[12],	//4
								 L[ 4] * R[1] + L[ 5] * R[5] + L[ 6] * R[ 9] + L[ 7] * R[13],	//5
								 L[ 4] * R[2] + L[ 5] * R[6] + L[ 6] * R[10] + L[ 7] * R[14],	//6
								 L[ 4] * R[3] + L[ 5] * R[7] + L[ 6] * R[11] + L[ 7] * R[15],	//7
								 L[ 8] * R[0] + L[ 9] * R[4] + L[10] * R[ 8] + L[11] * R[12],	//8
								 L[ 8] * R[1] + L[ 9] * R[5] + L[10] * R[ 9] + L[11] * R[13],	//9
								 L[ 8] * R[2] + L[ 9] * R[6] + L[10] * R[10] + L[11] * R[14],	//10
								 L[ 8] * R[3] + L[ 9] * R[7] + L[10] * R[11] + L[11] * R[15],	//11
								 L[12] * R[0] + L[13] * R[4] + L[14] * R[ 8] + L[15] * R[12],	//12
								 L[12] * R[1] + L[13] * R[5] + L[14] * R[ 9] + L[15] * R[13],	//13
								 L[12] * R[2] + L[13] * R[6] + L[14] * R[10] + L[15] * R[14],	//14
								 L[12] * R[3] + L[13] * R[7] + L[14] * R[11] + L[15] * R[15]);	//15
		}

		template<class T>
		vec4<T> operator*(const mat4x4<T>& R, vec4<T>& V)
		{
			return vec4<T>(R[0] * V[0] + R[4] * V[1] + R[ 8] * V[2] + R[12] * V[3],
						   R[1] * V[0] + R[5] * V[1] + R[ 9] * V[2] + R[13] * V[3],
						   R[2] * V[0] + R[6] * V[1] + R[10] * V[2] + R[14] * V[3],
						   R[3] * V[0] + R[7] * V[1] + R[11] * V[2] + R[15] * V[3]);
		}

		template<class T>
		const mat4x4<T> operator /(const mat4x4<T>& R, const T s)
		{
			if (s > -vml::math::EPSILON && s < vml::math::EPSILON)
				vml::os::Error("Mat4x4 : scalar is null");
			T invs = T(1) / T(s);
			return mat4x4<T>(R[ 0] * invs, R[ 1] * invs, R[ 2] * invs, R[ 3] * invs,
								 R[ 4] * invs, R[ 5] * invs, R[ 6] * invs, R[ 7] * invs,
								 R[ 8] * invs, R[ 9] * invs, R[10] * invs, R[11] * invs,
								 R[12] * invs, R[13] * invs, R[14] * invs, R[15] * invs);
		}

		template <typename T>
		const mat4x4<T> operator /(const T s, const mat4x4<T>& R)
		{
			T det = R[12] * R[ 9] * R[ 6] * R[ 3] -
					R[ 8] * R[13] * R[ 6] * R[ 3] -
					R[12] * R[ 5] * R[10] * R[ 3] +
					R[ 4] * R[13] * R[10] * R[ 3] +
					R[ 8] * R[ 5] * R[14] * R[ 3] -
					R[4 ] * R[ 9] * R[14] * R[ 3] -
					R[12] * R[ 9] * R[ 2] * R[ 7] +
					R[ 8] * R[13] * R[ 2] * R[ 7] +
					R[12] * R[ 1] * R[10] * R[ 7] -
					R[ 0] * R[13] * R[10] * R[ 7] -
					R[ 8] * R[ 1] * R[14] * R[ 7] +
					R[ 0] * R[ 9] * R[14] * R[ 7] +
					R[12] * R[ 5] * R[ 2] * R[11] -
					R[ 4] * R[13] * R[ 2] * R[11] -
					R[12] * R[ 1] * R[ 6] * R[11] +
					R[ 0] * R[13] * R[ 6] * R[11] +
					R[ 4] * R[ 1] * R[14] * R[11] -
					R[ 0] * R[ 5] * R[14] * R[11] -
					R[ 8] * R[ 5] * R[ 2] * R[15] +
					R[ 4] * R[ 9] * R[ 2] * R[15] +
					R[ 8] * R[ 1] * R[ 6] * R[15] -
					R[ 0] * R[ 9] * R[ 6] * R[15] -
					R[ 4] * R[ 1] * R[10] * R[15] +
					R[ 0] * R[ 5] * R[10] * R[15];

			// cannot invert singuRar matrix

			if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
				vml::os::Error("Matrix4x4 : Matrix is singuRar");

			T m00 = R[ 9] * R[14] * R[ 7] - R[13] * R[10] * R[ 7] + R[13] * R[ 6] * R[11] -
				    R[ 5] * R[14] * R[11] - R[ 9] * R[ 6] * R[15] + R[ 5] * R[10] * R[15];
			T m01 = R[12] * R[10] * R[ 7] - R[ 8] * R[14] * R[ 7] - R[12] * R[ 6] * R[11] +
				    R[ 4] * R[14] * R[11] + R[ 8] * R[ 6] * R[15] - R[ 4] * R[10] * R[15];
			T m02 = R[ 8] * R[13] * R[ 7] - R[12] * R[ 9] * R[ 7] + R[12] * R[ 5] * R[11] -
				    R[ 4] * R[13] * R[11] - R[ 8] * R[ 5] * R[15] + R[ 4] * R[ 9] * R[15];
			T m03 = R[12] * R[ 9] * R[ 6] - R[ 8] * R[13] * R[ 6] - R[12] * R[ 5] * R[10] +
				    R[ 4] * R[13] * R[10] + R[ 8] * R[ 5] * R[14] - R[ 4] * R[ 9] * R[14];
			T m10 = R[13] * R[10] * R[ 3] - R[ 9] * R[14] * R[ 3] - R[13] * R[ 2] * R[11] +
				    R[ 1] * R[14] * R[11] + R[ 9] * R[ 2] * R[15] - R[ 1] * R[10] * R[15];
			T m11 = R[ 8] * R[14] * R[ 3] - R[12] * R[10] * R[ 3] + R[12] * R[ 2] * R[11] -
				    R[ 0] * R[14] * R[11] - R[ 8] * R[ 2] * R[15] + R[ 0] * R[10] * R[15];
			T m12 = R[12] * R[ 9] * R[ 3] - R[ 8] * R[13] * R[ 3] - R[12] * R[ 1] * R[11] +
				    R[ 0] * R[13] * R[11] + R[ 8] * R[ 1] * R[15] - R[ 0] * R[ 9] * R[15];
			T m13 = R[ 8] * R[13] * R[ 2] - R[12] * R[ 9] * R[ 2] + R[12] * R[ 1] * R[10] -
				    R[ 0] * R[13] * R[10] - R[ 8] * R[ 1] * R[14] + R[ 0] * R[ 9] * R[14];
			T m20 = R[ 5] * R[14] * R[ 3] - R[13] * R[ 6] * R[ 3] + R[13] * R[ 2] * R[ 7] -
				    R[ 1] * R[14] * R[ 7] - R[ 5] * R[ 2] * R[15] + R[ 1] * R[ 6] * R[15];
			T m21 = R[12] * R[ 6] * R[ 3] - R[ 4] * R[14] * R[ 3] - R[12] * R[ 2] * R[ 7] +
				    R[ 0] * R[14] * R[ 7] + R[ 4] * R[ 2] * R[15] - R[ 0] * R[ 6] * R[15];
			T m22 = R[ 4] * R[13] * R[ 3] - R[12] * R[ 5] * R[ 3] + R[12] * R[ 1] * R[ 7] -
				    R[ 0] * R[13] * R[ 7] - R[ 4] * R[ 1] * R[15] + R[ 0] * R[ 5] * R[15];
			T m23 = R[12] * R[ 5] * R[ 2] - R[ 4] * R[13] * R[ 2] - R[12] * R[ 1] * R[ 6] +
				    R[ 0] * R[13] * R[ 6] + R[ 4] * R[ 1] * R[14] - R[ 0] * R[ 5] * R[14];
			T m30 = R[ 9] * R[ 6] * R[ 3] - R[ 5] * R[10] * R[ 3] - R[ 9] * R[ 2] * R[ 7] +
				    R[ 1] * R[10] * R[ 7] + R[ 5] * R[ 2] * R[11] - R[ 1] * R[ 6] * R[11];
			T m31 = R[ 4] * R[10] * R[ 3] - R[ 8] * R[ 6] * R[ 3] + R[ 8] * R[ 2] * R[ 7] -
				    R[ 0] * R[10] * R[ 7] - R[ 4] * R[ 2] * R[11] + R[ 0] * R[ 6] * R[11];
			T m32 = R[ 8] * R[ 5] * R[ 3] - R[ 4] * R[ 9] * R[ 3] - R[ 8] * R[ 1] * R[ 7] +
				    R[ 0] * R[ 9] * R[ 7] + R[ 4] * R[ 1] * R[11] - R[ 0] * R[ 5] * R[11];
			T m33 = R[ 4] * R[ 9] * R[ 2] - R[ 8] * R[ 5] * R[ 2] + R[ 8] * R[ 1] * R[ 6] -
				    R[ 0] * R[ 9] * R[ 6] - R[ 4] * R[ 1] * R[10] + R[ 0] * R[ 5] * R[10];

			det = s / det;

			return mat4x4<T>(m00 * det, m10 * det, m20 * det, m30 * det,
								 m01 * det, m11 * det, m21 * det, m31 * det,
								 m02 * det, m12 * det, m22 * det, m32 * det,
								 m03 * det, m13 * det, m23 * det, m33 * det);
		}

		template<class T>
		const mat4x4<T> operator +(const mat4x4<T>& L, const mat4x4<T>& R)
		{
			return mat4x4<T>(L[ 0] + R[ 0], L[ 1] + R[ 1], L[ 2] + R[ 2], L[ 3] + R[ 3],
							 L[ 4] + R[ 4], L[ 5] + R[ 5], L[ 6] + R[ 6], L[ 7] + R[ 7],
							 L[ 8] + R[ 8], L[ 9] + R[ 9], L[10] + R[10], L[11] + R[11],
							 L[12] + R[12], L[13] + R[13], L[14] + R[14], L[15] + R[15] );
		}

		template <typename T>
		const mat4x4<T> operator +(const T a, const mat4x4<T>& R)
		{
			return mat4x4<T>(a + R[ 0], a + R[ 1], a + R[ 2], a + R[ 3],
							 a + R[ 4], a + R[ 5], a + R[ 6], a + R[ 7],
							 a + R[ 8], a + R[ 9], a + R[10], a + R[11],
							 a + R[12], a + R[13], a + R[14], a + R[15] );
		}

		template <typename T>
		const mat4x4<T> operator +(const mat4x4<T>& R, const T a)
		{
			return mat4x4<T>(a + R[ 0], a + R[ 1], a + R[ 2], a + R[ 3],
							 a + R[ 4], a + R[ 5], a + R[ 6], a + R[ 7],
							 a + R[ 8], a + R[ 9], a + R[10], a + R[11],
							 a + R[12], a + R[13], a + R[14], a + R[15]);
		}

		template<class T>
		const mat4x4<T> operator -(const mat4x4<T>& L, const mat4x4<T>& R)
		{
			return mat4x4<T>(L[ 0] - R[ 0], L[ 1] - R[ 1], L[ 2] - R[ 2], L[ 3] - R[ 3],
							 L[ 4] - R[ 4], L[ 5] - R[ 5], L[ 6] - R[ 6], L[ 7] - R[ 7],
							 L[ 8] - R[ 8], L[ 9] - R[ 9], L[10] - R[10], L[11] - R[11],
							 L[12] - R[12], L[13] - R[13], L[14] - R[14], L[15] - R[15] );
		}

		template <typename T>
		const mat4x4<T> operator -(const T a, const mat4x4<T>& R)
		{
			return mat4x4<T>(a - R[ 0], a - R[ 1], a - R[ 2], a - R[ 3],
							 a - R[ 4], a - R[ 5], a - R[ 6], a - R[ 7],
							 a - R[ 8], a - R[ 9], a - R[10], a - R[11],
							 a - R[12], a - R[13], a - R[14], a - R[15]);
		}

		template <typename T>
		const mat4x4<T> operator -(const mat4x4<T>& R, const T a)
		{
			return mat4x4<T>(R[ 0] - a, R[ 1] - a, R[ 2] - a, R[ 3] - a,
							 R[ 4] - a, R[ 5] - a, R[ 6] - a, R[ 7] - a,
							 R[ 8] - a, R[ 9] - a, R[10] - a, R[11] - a,
							 R[12] - a, R[13] - a, R[14] - a, R[15] - a);
		}

		template <typename T>
		const mat4x4<T> operator /(const mat3x3<T> &R, const mat3x3<T> &L)
		{
			T det = L[12] * L[ 9] * L[ 6] * L[ 3] -
					L[ 8] * L[13] * L[ 6] * L[ 3] -
					L[12] * L[ 5] * L[10] * L[ 3] +
					L[ 4] * L[13] * L[10] * L[ 3] +
					L[ 8] * L[ 5] * L[14] * L[ 3] -
					L[ 4] * L[ 9] * L[14] * L[ 3] -
					L[12] * L[ 9] * L[ 2] * L[ 7] +
					L[ 8] * L[13] * L[ 2] * L[ 7] +
					L[12] * L[ 1] * L[10] * L[ 7] -
					L[ 0] * L[13] * L[10] * L[ 7] -
					L[ 8] * L[ 1] * L[14] * L[ 7] +
					L[ 0] * L[ 9] * L[14] * L[ 7] +
					L[12] * L[ 5] * L[ 2] * L[11] -
					L[ 4] * L[13] * L[ 2] * L[11] -
					L[12] * L[ 1] * L[ 6] * L[11] +
					L[ 0] * L[13] * L[ 6] * L[11] +
					L[ 4] * L[ 1] * L[14] * L[11] -
					L[ 0] * L[ 5] * L[14] * L[11] -
					L[ 8] * L[ 5] * L[ 2] * L[15] +
					L[ 4] * L[ 9] * L[ 2] * L[15] +
					L[ 8] * L[ 1] * L[ 6] * L[15] -
					L[ 0] * L[ 9] * L[ 6] * L[15] -
					L[ 4] * L[ 1] * L[10] * L[15] +
					L[ 0] * L[ 5] * L[10] * L[15];

			// cannot invert singular matrix

			if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
				vml::os::Error("Matrix4x4 : Matrix is singular");

			T m00 = L[0], m01 = L[4], m02 = L[ 8], m03 = L[12];
			T m10 = L[1], m11 = L[5], m12 = L[ 9], m13 = L[13];
			T m20 = L[2], m21 = L[6], m22 = L[10], m23 = L[14];
			T m30 = L[3], m31 = L[7], m32 = L[11], m33 = L[15];

			T data[16] = { m12 * m23 * m31 - m13 * m22 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 + m11 * m22 * m33,
						   m03 * m22 * m31 - m02 * m23 * m31 - m03 * m21 * m32 + m01 * m23 * m32 + m02 * m21 * m33 - m01 * m22 * m33,
						   m02 * m13 * m31 - m03 * m12 * m31 + m03 * m11 * m32 - m01 * m13 * m32 - m02 * m11 * m33 + m01 * m12 * m33,
						   m03 * m12 * m21 - m02 * m13 * m21 - m03 * m11 * m22 + m01 * m13 * m22 + m02 * m11 * m23 - m01 * m12 * m23,
						   m13 * m22 * m30 - m12 * m23 * m30 - m13 * m20 * m32 + m10 * m23 * m32 + m12 * m20 * m33 - m10 * m22 * m33,
						   m02 * m23 * m30 - m03 * m22 * m30 + m03 * m20 * m32 - m00 * m23 * m32 - m02 * m20 * m33 + m00 * m22 * m33,
						   m03 * m12 * m30 - m02 * m13 * m30 - m03 * m10 * m32 + m00 * m13 * m32 + m02 * m10 * m33 - m00 * m12 * m33,
						   m02 * m13 * m20 - m03 * m12 * m20 + m03 * m10 * m22 - m00 * m13 * m22 - m02 * m10 * m23 + m00 * m12 * m23,
						   m11 * m23 * m30 - m13 * m21 * m30 + m13 * m20 * m31 - m10 * m23 * m31 - m11 * m20 * m33 + m10 * m21 * m33,
						   m03 * m21 * m30 - m01 * m23 * m30 - m03 * m20 * m31 + m00 * m23 * m31 + m01 * m20 * m33 - m00 * m21 * m33,
						   m01 * m13 * m30 - m03 * m11 * m30 + m03 * m10 * m31 - m00 * m13 * m31 - m01 * m10 * m33 + m00 * m11 * m33,
						   m03 * m11 * m20 - m01 * m13 * m20 - m03 * m10 * m21 + m00 * m13 * m21 + m01 * m10 * m23 - m00 * m11 * m23,
						   m12 * m21 * m30 - m11 * m22 * m30 - m12 * m20 * m31 + m10 * m22 * m31 + m11 * m20 * m32 - m10 * m21 * m32,
						   m01 * m22 * m30 - m02 * m21 * m30 + m02 * m20 * m31 - m00 * m22 * m31 - m01 * m20 * m32 + m00 * m21 * m32,
						   m02 * m11 * m30 - m01 * m12 * m30 - m02 * m10 * m31 + m00 * m12 * m31 + m01 * m10 * m32 - m00 * m11 * m32,
						   m01 * m12 * m20 - m02 * m11 * m20 + m02 * m10 * m21 - m00 * m12 * m21 - m01 * m10 * m22 + m00 * m11 * m22 };

			det = 1.0f / det;

			data[ 0] *= det;	data[ 1] *= det;	data[ 2] *= det;	data[ 3] *= det;
			data[ 4] *= det;	data[ 5] *= det;	data[ 6] *= det;	data[ 7] *= det;
			data[ 8] *= det;	data[ 9] *= det;	data[10] *= det;	data[11] *= det;
			data[12] *= det;	data[13] *= det;	data[14] *= det;	data[15] *= det;

			return mat4x4<T>(R[ 0] * data[0] + R[ 1] * data[4] + R[ 2] * data[ 8] + R[ 3] * data[12],	//0
								 R[ 0] * data[1] + R[ 1] * data[5] + R[ 2] * data[ 9] + R[ 3] * data[13],	//1
								 R[ 0] * data[2] + R[ 1] * data[6] + R[ 2] * data[10] + R[ 3] * data[14],	//2
								 R[ 0] * data[3] + R[ 1] * data[7] + R[ 2] * data[11] + R[ 3] * data[15],	//3
								 R[ 4] * data[0] + R[ 5] * data[4] + R[ 6] * data[ 8] + R[ 7] * data[12],	//4
								 R[ 4] * data[1] + R[ 5] * data[5] + R[ 6] * data[ 9] + R[ 7] * data[13],	//5
								 R[ 4] * data[2] + R[ 5] * data[6] + R[ 6] * data[10] + R[ 7] * data[14],	//6
								 R[ 4] * data[3] + R[ 5] * data[7] + R[ 6] * data[11] + R[ 7] * data[15],	//7
								 R[ 8] * data[0] + R[ 9] * data[4] + R[10] * data[ 8] + R[11] * data[12],	//8
								 R[ 8] * data[1] + R[ 9] * data[5] + R[10] * data[ 9] + R[11] * data[13],	//9
								 R[ 8] * data[2] + R[ 9] * data[6] + R[10] * data[10] + R[11] * data[14],	//10
								 R[ 8] * data[3] + R[ 9] * data[7] + R[10] * data[11] + R[11] * data[15],	//11
								 R[12] * data[0] + R[13] * data[4] + R[14] * data[ 8] + R[15] * data[12],	//12
								 R[12] * data[1] + R[13] * data[5] + R[14] * data[ 9] + R[15] * data[13],	//13
								 R[12] * data[2] + R[13] * data[6] + R[14] * data[10] + R[15] * data[14],	//14
								 R[12] * data[3] + R[13] * data[7] + R[14] * data[11] + R[15] * data[15]);	//15
		}


		template <typename T >
		__forceinline std::ostream& operator<< (std::ostream& output, const vml::math::mat4x4<T>& M)
		{
				std::string strbuffer[16];
				size_t strlength[16] = { 0 };

				int maxlenght = 0;

				for (size_t j = 0; j < 4; ++j)
				{
					for (size_t i = 0; i < 4; ++i)
					{
						int index = i + j * 4;
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

				for (size_t j = 0; j < 4; ++j)
				{
					for (size_t i = 0; i < 4; ++i)
					{
						int index = i + j * 4;

						os << strbuffer[index] << std::setw(maxlenght - strlength[index]);
					}

					os << "\n";
				}

				return os;

		}

	}
}

///////////////////////////////////////////////////////////////////////////////////
// predefined types

namespace vml
{
	namespace math
	{
		typedef mat4x4<byte>	mat4x4b;	// 8-bit  unsigned.
		typedef mat4x4<uint8>	mat4x4ui8;	// 8-bit  unsigned.
		typedef mat4x4<uint16>	mat4x4ui16;	// 16-bit unsigned.
		typedef mat4x4<uint32>  mat4x4ui32;	// 32-bit unsigned.
		typedef mat4x4<uint64>  mat4x4ui64;	// 64-bit unsigned.
		typedef mat4x4<int8>	mat4x4i8;	// 8-bit  signed.
		typedef mat4x4<int16>	mat4x4i16;	// 16-bit signed.
		typedef mat4x4<int32>	mat4x4i32;	// 32-bit signed.
		typedef mat4x4<int64>	mat4x4i64;	// 64-bit signed.
		typedef mat4x4<float32> mat4x4f;	// 32-bit float
		typedef mat4x4<float64> mat4x4d;	// 64-bit float
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace math
	{
		namespace mat4x4utils
		{


			// --------------------------------------------------------
			// matrix determinant

			template <typename T>
			__forceinline T Determinant(const mat4x4<T>& R)
			{
				return (R[12] * R[ 9] * R[ 6] * R[ 3] -
						R[ 8] * R[13] * R[ 6] * R[ 3] -
						R[12] * R[ 5] * R[10] * R[ 3] +
						R[ 4] * R[13] * R[10] * R[ 3] +
						R[ 8] * R[ 5] * R[14] * R[ 3] -
						R[ 4] * R[ 9] * R[14] * R[ 3] -
						R[12] * R[ 9] * R[ 2] * R[ 7] +
						R[ 8] * R[13] * R[ 2] * R[ 7] +
						R[12] * R[ 1] * R[10] * R[ 7] -
						R[ 0] * R[13] * R[10] * R[ 7] -
						R[ 8] * R[ 1] * R[14] * R[ 7] +
						R[ 0] * R[ 9] * R[14] * R[ 7] +
						R[12] * R[ 5] * R[ 2] * R[11] -
						R[ 4] * R[13] * R[ 2] * R[11] -
						R[12] * R[ 1] * R[ 6] * R[11] +
						R[ 0] * R[13] * R[ 6] * R[11] +
						R[ 4] * R[ 1] * R[14] * R[11] -
						R[ 0] * R[ 5] * R[14] * R[11] -
						R[ 8] * R[ 5] * R[ 2] * R[15] +
						R[ 4] * R[ 9] * R[ 2] * R[15] +
						R[ 8] * R[ 1] * R[ 6] * R[15] -
						R[ 0] * R[ 9] * R[ 6] * R[15] -
						R[ 4] * R[ 1] * R[10] * R[15] +
						R[ 0] * R[ 5] * R[10] * R[15]);
			}

			// --------------------------------------------------------
			// matrix norm

			template <typename T>
			__forceinline T Norm(const mat4x4<T>& R)
			{
				return sqrtf(R[ 0] * R[ 0] + R[ 1] * R[ 1] + R[ 2] * R[ 2] + R[ 3] * R[ 3] +
							 R[ 4] * R[ 4] + R[ 5] * R[ 5] + R[ 6] * R[ 6] + R[ 7] * R[ 7] +
							 R[ 8] * R[ 8] + R[ 9] * R[ 9] + R[10] * R[10] + R[11] * R[11] +
							 R[12] * R[12] + R[13] * R[13] + R[14] * R[14] + R[15] * R[15]);
			}

			// --------------------------------------------------------
			// computes the trace for anxn matrix
			// the sum of all elelemnts of the main diagonal

			template <typename T>
			__forceinline T Trace(const mat4x4<T>& R)
			{
				return  R[0] + R[5] + R[10] + R[15];
			}

			// --------------------------------------------------------
			// computes the rank for a 3x3 matrix

			template <typename T>
			__forceinline int Rank(const mat4x4<T>& R)
			{
				if (R[12] * R[ 9] * R[ 6] * R[ 3] -
					R[ 8] * R[13] * R[ 6] * R[ 3] -
					R[12] * R[ 5] * R[10] * R[ 3] +
					R[ 4] * R[13] * R[10] * R[ 3] +
					R[ 8] * R[ 5] * R[14] * R[ 3] -
					R[ 4] * R[ 9] * R[14] * R[ 3] -
					R[12] * R[ 9] * R[ 2] * R[ 7] +
					R[ 8] * R[13] * R[ 2] * R[ 7] +
					R[12] * R[ 1] * R[10] * R[ 7] -
					R[ 0] * R[13] * R[10] * R[ 7] -
					R[ 8] * R[ 1] * R[14] * R[ 7] +
					R[ 0] * R[ 9] * R[14] * R[ 7] +
					R[12] * R[ 5] * R[ 2] * R[11] -
					R[ 4] * R[13] * R[ 2] * R[11] -
					R[12] * R[ 1] * R[ 6] * R[11] +
					R[ 0] * R[13] * R[ 6] * R[11] +
					R[ 4] * R[ 1] * R[14] * R[11] -
					R[ 0] * R[ 5] * R[14] * R[11] -
					R[ 8] * R[ 5] * R[ 2] * R[15] +
					R[ 4] * R[ 9] * R[ 2] * R[15] +
					R[ 8] * R[ 1] * R[ 6] * R[15] -
					R[ 0] * R[ 9] * R[ 6] * R[15] -
					R[ 4] * R[ 1] * R[10] * R[15] +
					R[ 0] * R[ 5] * R[10] * R[15] != 0) return 4;

				if (R[ 5] * R[10] * R[15] +
					R[ 6] * R[11] * R[13] +
					R[ 7] * R[ 9] * R[14] -
					R[ 7] * R[10] * R[13] -
					R[11] * R[14] * R[ 5] -
					R[ 6] * R[ 9] * R[15] != 0) return 3;

				if (R[ 4] * R[10] * R[15] +
					R[ 6] * R[11] * R[12] +
					R[ 7] * R[ 8] * R[14] -
					R[ 7] * R[10] * R[12] -
					R[11] * R[14] * R[ 4] -
					R[ 6] * R[ 8] * R[15] != 0) return 3;

				if (R[ 4] * R[ 9] * R[15] +
					R[ 5] * R[11] * R[12] +
					R[ 7] * R[ 8] * R[13] -
					R[ 7] * R[ 9] * R[12] -
					R[11] * R[13] * R[ 4] -
					R[ 5] * R[ 8] * R[15] != 0) return 3;

				if (R[ 4] * R[ 9] * R[14] +
					R[ 5] * R[10] * R[12] +
					R[ 6] * R[ 8] * R[13] -
					R[ 6] * R[ 9] * R[12] -
					R[10] * R[13] * R[ 4] -
					R[ 5] * R[ 8] * R[14] != 0) return 3;

				if (R[ 1] * R[10] * R[15] +
					R[ 2] * R[11] * R[13] +
					R[ 3] * R[ 9] * R[14] -
					R[ 3] * R[10] * R[13] -
					R[11] * R[14] * R[ 1] -
					R[15] * R[ 2] * R[ 9] != 0) return 3;

				if (R[ 0] * R[10] * R[15] +
					R[ 2] * R[11] * R[12] +
					R[ 3] * R[ 8] * R[14] -
					R[ 3] * R[10] * R[12] -
					R[11] * R[14] * R[ 0] -
					R[15] * R[ 2] * R[ 8] != 0) return 3;

				if (R[ 0] * R[ 9] * R[15] +
					R[ 1] * R[11] * R[12] +
					R[ 3] * R[ 8] * R[13] -
					R[ 3] * R[ 9] * R[12] -
					R[11] * R[13] * R[ 0] -
					R[15] * R[ 1] * R[ 8] != 0) return 3;

				if (R[ 0] * R[ 9] * R[14] +
					R[ 1] * R[10] * R[12] +
					R[ 2] * R[ 8] * R[13] -
					R[ 2] * R[ 9] * R[12] -
					R[10] * R[13] * R[ 0] -
					R[14] * R[ 1] * R[ 8] != 0) return 3;

				if (R[ 1] * R[ 6] * R[15] +
					R[ 2] * R[ 7] * R[13] +
					R[ 3] * R[ 5] * R[14] -
					R[ 3] * R[ 6] * R[13] -
					R[ 7] * R[14] * R[ 1] -
					R[ 2] * R[ 5] * R[13] != 0) return 3;

				if (R[ 0] * R[ 6] * R[15] +
					R[ 2] * R[ 7] * R[12] +
					R[ 3] * R[ 4] * R[14] -
					R[ 3] * R[ 6] * R[12] -
					R[ 7] * R[14] * R[ 0] -
					R[ 2] * R[ 4] * R[15] != 0) return 3;

				if (R[ 0] * R[ 5] * R[15] +
					R[ 1] * R[ 7] * R[12] +
					R[ 3] * R[ 4] * R[13] -
					R[ 3] * R[ 5] * R[12] -
					R[ 7] * R[13] * R[ 0] -
					R[ 1] * R[ 4] * R[15] != 0) return 3;

				if (R[ 0] * R[ 5] * R[14] +
					R[ 1] * R[ 6] * R[12] +
					R[ 2] * R[ 4] * R[13] -
					R[ 2] * R[ 5] * R[12] -
					R[ 6] * R[13] * R[ 0] -
					R[ 1] * R[ 4] * R[14] != 0) return 3;

				if (R[ 1] * R[ 6] * R[11] +
					R[ 2] * R[ 7] * R[ 9] +
					R[ 3] * R[ 5] * R[10] -
					R[ 3] * R[ 6] * R[ 9] -
					R[ 7] * R[10] * R[ 1] -
					R[11] * R[ 2] * R[ 5] != 0) return 3;

				if (R[ 0] * R[ 6] * R[11] +
					R[ 2] * R[ 7] * R[ 8] +
					R[ 3] * R[ 4] * R[10] -
					R[ 3] * R[ 6] * R[ 8] -
					R[ 7] * R[10] * R[ 0] -
					R[11] * R[ 2] * R[ 4] != 0) return 3;

				if (R[ 0] * R[ 5] * R[11] +
					R[ 1] * R[ 7] * R[ 8] +
					R[ 3] * R[ 4] * R[ 9] -
					R[ 3] * R[ 5] * R[ 8] -
					R[ 7] * R[ 9] * R[ 0] -
					R[11] * R[ 1] * R[ 4] != 0) return 3;

				if (R[ 0] * R[ 5] * R[10] +
					R[ 1] * R[ 6] * R[ 8] +
					R[ 2] * R[ 4] * R[ 9] -
					R[ 2] * R[ 5] * R[ 8] -
					R[ 6] * R[ 9] * R[ 0] -
					R[10] * R[ 1] * R[ 4] != 0) return 3;

				if (R[10] * R[15] - R[11] * R[14] != 0) return 2;
				if (R[ 9] * R[15] - R[11] * R[13] != 0) return 2;
				if (R[ 9] * R[14] - R[10] * R[13] != 0) return 2;
				if (R[ 6] * R[15] - R[ 7] * R[14] != 0) return 2;
				if (R[ 5] * R[15] - R[ 7] * R[13] != 0) return 2;
				if (R[ 5] * R[14] - R[ 6] * R[13] != 0) return 2;
				if (R[ 6] * R[11] - R[ 7] * R[10] != 0) return 2;
				if (R[ 5] * R[11] - R[ 7] * R[ 9] != 0) return 2;
				if (R[ 5] * R[10] - R[ 6] * R[ 9] != 0) return 2;

				if (R[10] * R[15] - R[11] * R[14] != 0) return 2;
				if (R[ 8] * R[15] - R[11] * R[12] != 0) return 2;
				if (R[ 8] * R[14] - R[10] * R[12] != 0) return 2;
				if (R[ 6] * R[15] - R[ 7] * R[14] != 0) return 2;
				if (R[ 4] * R[15] - R[ 7] * R[12] != 0) return 2;
				if (R[ 4] * R[14] - R[ 6] * R[12] != 0) return 2;
				if (R[ 6] * R[11] - R[ 7] * R[10] != 0) return 2;
				if (R[ 4] * R[11] - R[ 7] * R[ 8] != 0) return 2;
				if (R[ 4] * R[10] - R[ 6] * R[ 8] != 0) return 2;

				if (R[ 9] * R[15] - R[11] * R[13] != 0) return 2;
				if (R[ 8] * R[15] - R[11] * R[12] != 0) return 2;
				if (R[ 8] * R[13] - R[ 9] * R[12] != 0) return 2;
				if (R[ 5] * R[15] - R[ 7] * R[13] != 0) return 2;
				if (R[ 4] * R[15] - R[ 7] * R[12] != 0) return 2;
				if (R[ 4] * R[13] - R[ 5] * R[12] != 0) return 2;
				if (R[ 5] * R[11] - R[ 7] * R[ 9] != 0) return 2;
				if (R[ 4] * R[11] - R[ 7] * R[ 8] != 0) return 2;
				if (R[ 4] * R[ 9] - R[ 5] * R[ 8] != 0) return 2;

				if (R[ 9] * R[14] - R[10] * R[13] != 0) return 2;
				if (R[ 8] * R[14] - R[10] * R[12] != 0) return 2;
				if (R[ 8] * R[13] - R[ 9] * R[12] != 0) return 2;
				if (R[ 5] * R[14] - R[ 6] * R[13] != 0) return 2;
				if (R[ 4] * R[14] - R[ 6] * R[12] != 0) return 2;
				if (R[ 4] * R[13] - R[ 5] * R[12] != 0) return 2;
				if (R[ 5] * R[10] - R[ 6] * R[ 9] != 0) return 2;
				if (R[ 4] * R[10] - R[ 6] * R[ 8] != 0) return 2;
				if (R[ 4] * R[ 9] - R[ 5] * R[ 8] != 0) return 2;

				if (R[10] * R[15] - R[11] * R[14] != 0) return 2;
				if (R[ 9] * R[15] - R[11] * R[13] != 0) return 2;
				if (R[ 9] * R[14] - R[10] * R[13] != 0) return 2;
				if (R[ 2] * R[15] - R[ 3] * R[14] != 0) return 2;
				if (R[ 1] * R[15] - R[ 3] * R[13] != 0) return 2;
				if (R[ 1] * R[14] - R[ 2] * R[13] != 0) return 2;
				if (R[ 2] * R[11] - R[ 3] * R[10] != 0) return 2;
				if (R[ 1] * R[11] - R[ 3] * R[ 9] != 0) return 2;
				if (R[ 1] * R[10] - R[ 2] * R[ 9] != 0) return 2;

				if (R[10] * R[15] - R[11] * R[14] != 0) return 2;
				if (R[ 8] * R[15] - R[11] * R[12] != 0) return 2;
				if (R[ 8] * R[14] - R[10] * R[12] != 0) return 2;
				if (R[ 2] * R[15] - R[ 3] * R[14] != 0) return 2;
				if (R[ 0] * R[15] - R[ 3] * R[12] != 0) return 2;
				if (R[ 0] * R[14] - R[ 2] * R[12] != 0) return 2;
				if (R[ 2] * R[11] - R[ 3] * R[10] != 0) return 2;
				if (R[ 0] * R[11] - R[ 3] * R[ 8] != 0) return 2;
				if (R[ 0] * R[10] - R[ 2] * R[ 8] != 0) return 2;

				if (R[ 9] * R[15] - R[11] * R[13] != 0) return 2;
				if (R[ 8] * R[15] - R[11] * R[12] != 0) return 2;
				if (R[ 8] * R[13] - R[ 9] * R[12] != 0) return 2;
				if (R[ 1] * R[15] - R[ 3] * R[13] != 0) return 2;
				if (R[ 0] * R[15] - R[ 3] * R[12] != 0) return 2;
				if (R[ 0] * R[13] - R[ 1] * R[12] != 0) return 2;
				if (R[ 1] * R[11] - R[ 3] * R[ 9] != 0) return 2;
				if (R[ 0] * R[11] - R[ 3] * R[ 8] != 0) return 2;
				if (R[ 0] * R[ 9] - R[ 1] * R[ 8] != 0) return 2;

				if (R[ 9] * R[14] - R[10] * R[13] != 0) return 2;
				if (R[ 8] * R[14] - R[10] * R[12] != 0) return 2;
				if (R[ 8] * R[13] - R[ 9] * R[12] != 0) return 2;
				if (R[ 1] * R[14] - R[ 2] * R[13] != 0) return 2;
				if (R[ 0] * R[14] - R[ 2] * R[12] != 0) return 2;
				if (R[ 0] * R[13] - R[ 1] * R[12] != 0) return 2;
				if (R[ 1] * R[10] - R[ 2] * R[ 9] != 0) return 2;
				if (R[ 0] * R[10] - R[ 2] * R[ 8] != 0) return 2;
				if (R[ 0] * R[ 9] - R[ 1] * R[ 8] != 0) return 2;

				if (R[ 6] * R[15] - R[ 7] * R[14] != 0) return 2;
				if (R[ 5] * R[15] - R[ 7] * R[13] != 0) return 2;
				if (R[ 5] * R[14] - R[ 6] * R[13] != 0) return 2;
				if (R[ 2] * R[15] - R[ 3] * R[14] != 0) return 2;
				if (R[ 1] * R[15] - R[ 3] * R[13] != 0) return 2;
				if (R[ 1] * R[14] - R[ 2] * R[13] != 0) return 2;
				if (R[ 2] * R[ 7] - R[ 3] * R[ 6] != 0) return 2;
				if (R[ 1] * R[ 7] - R[ 3] * R[ 5] != 0) return 2;
				if (R[ 1] * R[ 6] - R[ 2] * R[ 5] != 0) return 2;

				if (R[ 6] * R[15] - R[ 7] * R[14] != 0) return 2;
				if (R[ 4] * R[15] - R[ 7] * R[12] != 0) return 2;
				if (R[ 4] * R[14] - R[ 6] * R[12] != 0) return 2;
				if (R[ 2] * R[15] - R[ 3] * R[14] != 0) return 2;
				if (R[ 0] * R[15] - R[ 3] * R[12] != 0) return 2;
				if (R[ 0] * R[14] - R[ 2] * R[12] != 0) return 2;
				if (R[ 2] * R[ 7] - R[ 3] * R[ 6] != 0) return 2;
				if (R[ 0] * R[ 7] - R[ 3] * R[ 4] != 0) return 2;
				if (R[ 0] * R[ 6] - R[ 2] * R[ 4] != 0) return 2;

				if (R[ 5] * R[15] - R[ 7] * R[13] != 0) return 2;
				if (R[ 4] * R[15] - R[ 7] * R[12] != 0) return 2;
				if (R[ 4] * R[13] - R[ 5] * R[12] != 0) return 2;
				if (R[ 1] * R[15] - R[ 3] * R[13] != 0) return 2;
				if (R[ 0] * R[15] - R[ 3] * R[12] != 0) return 2;
				if (R[ 0] * R[13] - R[ 1] * R[12] != 0) return 2;
				if (R[ 1] * R[ 7] - R[ 3] * R[ 5] != 0) return 2;
				if (R[ 0] * R[ 7] - R[ 3] * R[ 4] != 0) return 2;
				if (R[ 0] * R[ 5] - R[ 1] * R[ 4] != 0) return 2;

				if (R[ 5] * R[14] - R[ 6] * R[13] != 0) return 2;
				if (R[ 4] * R[14] - R[ 6] * R[12] != 0) return 2;
				if (R[ 4] * R[13] - R[ 5] * R[12] != 0) return 2;
				if (R[ 1] * R[14] - R[ 2] * R[13] != 0) return 2;
				if (R[ 0] * R[14] - R[ 2] * R[12] != 0) return 2;
				if (R[ 0] * R[13] - R[ 1] * R[12] != 0) return 2;
				if (R[ 1] * R[ 6] - R[ 2] * R[ 5] != 0) return 2;
				if (R[ 0] * R[ 6] - R[ 2] * R[ 4] != 0) return 2;
				if (R[ 0] * R[ 5] - R[ 1] * R[ 4] != 0) return 2;

				if (R[ 6] * R[11] - R[ 7] * R[10] != 0) return 2;
				if (R[ 5] * R[11] - R[ 7] * R[ 9] != 0) return 2;
				if (R[ 5] * R[10] - R[ 6] * R[ 9] != 0) return 2;
				if (R[ 2] * R[11] - R[ 3] * R[10] != 0) return 2;
				if (R[ 1] * R[11] - R[ 3] * R[ 9] != 0) return 2;
				if (R[ 1] * R[10] - R[ 2] * R[ 9] != 0) return 2;
				if (R[ 2] * R[ 7] - R[ 3] * R[ 6] != 0) return 2;
				if (R[ 1] * R[ 7] - R[ 3] * R[ 5] != 0) return 2;
				if (R[ 1] * R[ 6] - R[ 2] * R[ 5] != 0) return 2;

				if (R[ 6] * R[11] - R[ 7] * R[10] != 0) return 2;
				if (R[ 4] * R[11] - R[ 7] * R[ 8] != 0) return 2;
				if (R[ 4] * R[10] - R[ 6] * R[ 8] != 0) return 2;
				if (R[ 2] * R[11] - R[ 3] * R[10] != 0) return 2;
				if (R[ 0] * R[11] - R[ 3] * R[ 8] != 0) return 2;
				if (R[ 0] * R[10] - R[ 2] * R[ 8] != 0) return 2;
				if (R[ 2] * R[ 7] - R[ 3] * R[ 6] != 0) return 2;
				if (R[ 0] * R[ 7] - R[ 3] * R[ 4] != 0) return 2;
				if (R[ 0] * R[ 6] - R[ 2] * R[ 4] != 0) return 2;

				if (R[ 5] * R[11] - R[ 7] * R[ 9] != 0) return 2;
				if (R[ 4] * R[11] - R[ 7] * R[ 8] != 0) return 2;
				if (R[ 4] * R[ 9] - R[ 5] * R[ 8] != 0) return 2;
				if (R[ 1] * R[11] - R[ 3] * R[ 9] != 0) return 2;
				if (R[ 0] * R[11] - R[ 3] * R[ 8] != 0) return 2;
				if (R[ 0] * R[ 9] - R[ 1] * R[ 8] != 0) return 2;
				if (R[ 1] * R[ 7] - R[ 3] * R[ 5] != 0) return 2;
				if (R[ 0] * R[ 7] - R[ 3] * R[ 4] != 0) return 2;
				if (R[ 0] * R[ 5] - R[ 1] * R[ 4] != 0) return 2;

				if (R[ 5] * R[10] - R[ 6] * R[ 9] != 0) return 2;
				if (R[ 4] * R[10] - R[ 6] * R[ 8] != 0) return 2;
				if (R[ 4] * R[ 9] - R[ 5] * R[ 8] != 0) return 2;
				if (R[ 1] * R[10] - R[ 2] * R[ 9] != 0) return 2;
				if (R[ 0] * R[10] - R[ 2] * R[ 8] != 0) return 2;
				if (R[ 0] * R[ 9] - R[ 1] * R[ 8] != 0) return 2;
				if (R[ 1] * R[ 6] - R[ 2] * R[ 5] != 0) return 2;
				if (R[ 0] * R[ 6] - R[ 2] * R[ 4] != 0) return 2;
				if (R[ 0] * R[ 5] - R[ 1] * R[ 4] != 0) return 2;

				if (R[ 0] != 0) return 1;
				if (R[ 1] != 0) return 1;
				if (R[ 2] != 0) return 1;
				if (R[ 3] != 0) return 1;
				if (R[ 4] != 0) return 1;
				if (R[ 5] != 0) return 1;
				if (R[ 6] != 0) return 1;
				if (R[ 7] != 0) return 1;
				if (R[ 8] != 0) return 1;
				if (R[ 9] != 0) return 1;
				if (R[10] != 0) return 1;
				if (R[11] != 0) return 1;
				if (R[12] != 0) return 1;
				if (R[13] != 0) return 1;
				if (R[14] != 0) return 1;
				if (R[15] != 0) return 1;

				return 0;
			}
			
			// --------------------------------------------------------
			// matrix inversion

			template <typename T>
			__forceinline mat4x4<T> Invert(const mat4x4<T>& R)
			{
				T det = R[12] * R[ 9] * R[ 6] * R[ 3] -
						R[ 8] * R[13] * R[ 6] * R[ 3] -
						R[12] * R[ 5] * R[10] * R[ 3] +
						R[ 4] * R[13] * R[10] * R[ 3] +
						R[ 8] * R[ 5] * R[14] * R[ 3] -
						R[ 4] * R[ 9] * R[14] * R[ 3] -
						R[12] * R[ 9] * R[ 2] * R[ 7] +
						R[ 8] * R[13] * R[ 2] * R[ 7] +
						R[12] * R[ 1] * R[10] * R[ 7] -
						R[ 0] * R[13] * R[10] * R[ 7] -
						R[ 8] * R[ 1] * R[14] * R[ 7] +
						R[ 0] * R[ 9] * R[14] * R[ 7] +
						R[12] * R[ 5] * R[ 2] * R[11] -
						R[ 4] * R[13] * R[ 2] * R[11] -
						R[12] * R[ 1] * R[ 6] * R[11] +
						R[ 0] * R[13] * R[ 6] * R[11] +
						R[ 4] * R[ 1] * R[14] * R[11] -
						R[ 0] * R[ 5] * R[14] * R[11] -
						R[ 8] * R[ 5] * R[ 2] * R[15] +
						R[ 4] * R[ 9] * R[ 2] * R[15] +
						R[ 8] * R[ 1] * R[ 6] * R[15] -
						R[ 0] * R[ 9] * R[ 6] * R[15] -
						R[ 4] * R[ 1] * R[10] * R[15] +
						R[ 0] * R[ 5] * R[10] * R[15];

				// cannot invert singular matrix

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Error("Matrix4x4 : Matrix is singular");

				T m00 = R[0], m01 = R[4], m02 = R[ 8], m03 = R[12];
				T m10 = R[1], m11 = R[5], m12 = R[ 9], m13 = R[13];
				T m20 = R[2], m21 = R[6], m22 = R[10], m23 = R[14];
				T m30 = R[3], m31 = R[7], m32 = R[11], m33 = R[15];

				R[ 0] = m12 * m23 * m31 - m13 * m22 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 + m11 * m22 * m33;
				R[ 4] = m03 * m22 * m31 - m02 * m23 * m31 - m03 * m21 * m32 + m01 * m23 * m32 + m02 * m21 * m33 - m01 * m22 * m33;
				R[ 8] = m02 * m13 * m31 - m03 * m12 * m31 + m03 * m11 * m32 - m01 * m13 * m32 - m02 * m11 * m33 + m01 * m12 * m33;
				R[12] = m03 * m12 * m21 - m02 * m13 * m21 - m03 * m11 * m22 + m01 * m13 * m22 + m02 * m11 * m23 - m01 * m12 * m23;
				R[ 1] = m13 * m22 * m30 - m12 * m23 * m30 - m13 * m20 * m32 + m10 * m23 * m32 + m12 * m20 * m33 - m10 * m22 * m33;
				R[ 5] = m02 * m23 * m30 - m03 * m22 * m30 + m03 * m20 * m32 - m00 * m23 * m32 - m02 * m20 * m33 + m00 * m22 * m33;
				R[ 9] = m03 * m12 * m30 - m02 * m13 * m30 - m03 * m10 * m32 + m00 * m13 * m32 + m02 * m10 * m33 - m00 * m12 * m33;
				R[13] = m02 * m13 * m20 - m03 * m12 * m20 + m03 * m10 * m22 - m00 * m13 * m22 - m02 * m10 * m23 + m00 * m12 * m23;
				R[ 2] = m11 * m23 * m30 - m13 * m21 * m30 + m13 * m20 * m31 - m10 * m23 * m31 - m11 * m20 * m33 + m10 * m21 * m33;
				R[ 6] = m03 * m21 * m30 - m01 * m23 * m30 - m03 * m20 * m31 + m00 * m23 * m31 + m01 * m20 * m33 - m00 * m21 * m33;
				R[10] = m01 * m13 * m30 - m03 * m11 * m30 + m03 * m10 * m31 - m00 * m13 * m31 - m01 * m10 * m33 + m00 * m11 * m33;
				R[14] = m03 * m11 * m20 - m01 * m13 * m20 - m03 * m10 * m21 + m00 * m13 * m21 + m01 * m10 * m23 - m00 * m11 * m23;
				R[ 3] = m12 * m21 * m30 - m11 * m22 * m30 - m12 * m20 * m31 + m10 * m22 * m31 + m11 * m20 * m32 - m10 * m21 * m32;
				R[ 7] = m01 * m22 * m30 - m02 * m21 * m30 + m02 * m20 * m31 - m00 * m22 * m31 - m01 * m20 * m32 + m00 * m21 * m32;
				R[11] = m02 * m11 * m30 - m01 * m12 * m30 - m02 * m10 * m31 + m00 * m12 * m31 + m01 * m10 * m32 - m00 * m11 * m32;
				R[15] = m01 * m12 * m20 - m02 * m11 * m20 + m02 * m10 * m21 - m00 * m12 * m21 - m01 * m10 * m22 + m00 * m11 * m22;

				det = 1.0f / det;

				return mat4x4<T> ( R[ 0] * det, R[ 1] * det, R[ 2] * det, R[ 3] * det,
								   R[ 4] * det,	R[ 5] * det, R[ 6] * det, R[ 7] * det,
								   R[ 8] * det,	R[ 9] * det, R[10] * det, R[11] * det,
								   R[12] * det,	R[13] * det, R[14] * det, R[15] * det );
			}

			// --------------------------------------------------------
			// transposes a 4x4 matrix

			template <typename T>
			__forceinline mat4x4<T> Transpose(const mat4x4<T>& R)
			{
				return mat4x4<T>(R[ 0], R[ 4], R[ 8], R[12],
								 R[ 1], R[ 5], R[ 9], R[13],
								 R[ 2], R[ 6], R[10], R[14],
								 R[ 3], R[ 7], R[11], R[15]);
			}

			// --------------------------------------------------------
			// transposes a 4x4 matrix

			template <typename T>
			__forceinline mat4x4<T> Zero()
			{
				return mat4x4<T>(0, 0, 0, 0,
							     0, 0, 0, 0,
								 0, 0, 0, 0,
								 0, 0, 0, 0);
			}

			// --------------------------------------------------------
			// matrix identity

			template <typename T>
			__forceinline mat4x4<T> Identity()
			{
				return mat4x4<T>(1, 0, 0, 0,
								 0, 1, 0, 0,
								 0, 0, 1, 0,
								 0, 0, 0, 1);
			}

			// --------------------------------------------------------
			// initialise matrix with random values inn the min ,max range

			template <typename T>
			__forceinline mat4x4<T> Random(const mat4x4<T>& R,const int min, const int max)
			{
				return mat4x4<T> ( min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)),
								   min + (T)(rand() / (T)(RAND_MAX + 1) * (max - min + 1)));
			}

			// --------------------------------------------------------
			// initialise matrix with diagonal values

			template <typename T>
			__forceinline mat4x4<T> Diagonal(const T value)
			{
				return mat4x4<T>(T(value) , T(0)     , T(0)     , T(0),
					             T(0)     , T(value) , T(0)     , T(0),
							 	 T(0)     , T(0)     , T(value) , T(0),
							   	 T(0)     , T(0)     , T(0)     , T(value));
			}

			// --------------------------------------------------------
			// initialise matrix with an upper triangular values
			// int the min,max range

			template <typename T>
			__forceinline mat4x4<T> UpperTriangular(const T value)
			{
				return mat4x4<T>(T(value), T(value), T(value), T(value),
								 T(0)    , T(value), T(value), T(value),
								 T(0)    , T(0)    , T(value), T(value),
								 T(0)    , T(0)    , T(0)    , T(value));
			}

			// --------------------------------------------------------
			// initialise matrix with an lower triangular values
			// int the min,max range

			template <typename T>
			__forceinline mat4x4<T> LowerTriangular(const T value)
			{
				return mat4x4<T>(T(value), T(value), T(value), T(value),
								 T(0)    , T(0)    , T(0)    , T(value),
								 T(0)    , T(value), T(value), T(value),
								 T(0)    , T(value), T(value), T(value));
			}

			// --------------------------------------------------------
			// compute the absolute value of a matriz

			template <typename T>
			__forceinline mat4x4<T> Abs(const mat4x4<T>& R)
			{
				return mat4x4<T>(fabs(R[ 0]), fabs(R[ 1]), fabs(R[ 2]), fabs(R[ 3]),
								 fabs(R[ 4]), fabs(R[ 5]), fabs(R[ 6]), fabs(R[ 7]),
								 fabs(R[ 8]), fabs(R[ 9]), fabs(R[10]), fabs(R[11]),
								 fabs(R[12]), fabs(R[13]), fabs(R[14]), fabs(R[15]));
			}

			// --------------------------------------------------------
			// flat matrix value in the the [0..1] range

			template <typename T>
			__forceinline mat4x4<T> Flat(const mat4x4<T>& R)
			{
				T min = R[0];
				if (R[ 1] < min)min = R[1];
				if (R[ 2] < min)min = R[2];
				if (R[ 3] < min)min = R[3];
				if (R[ 4] < min)min = R[4];
				if (R[ 5] < min)min = R[5];
				if (R[ 6] < min)min = R[6];
				if (R[ 7] < min)min = R[7];
				if (R[ 8] < min)min = R[8];
				if (R[ 9] < min)min = R[9];
				if (R[10] < min)min = R[10];
				if (R[11] < min)min = R[11];
				if (R[12] < min)min = R[12];
				if (R[13] < min)min = R[13];
				if (R[14] < min)min = R[14];
				if (R[15] < min)min = R[15];
				T max = R[0];
				if (R[ 1] > max)max = R[1];
				if (R[ 2] > max)max = R[2];
				if (R[ 3] > max)max = R[3];
				if (R[ 4] > max)max = R[4];
				if (R[ 5] > max)max = R[5];
				if (R[ 6] > max)max = R[6];
				if (R[ 7] > max)max = R[7];
				if (R[ 8] > max)max = R[8];
				if (R[ 9] > max)max = R[9];
				if (R[10] > max)max = R[10];
				if (R[11] > max)max = R[11];
				if (R[12] > max)max = R[12];
				if (R[13] > max)max = R[13];
				if (R[14] > max)max = R[14];
				if (R[15] > max)max = R[15];
				T invdenum = max - min;
				if (invdenum > -vml::math::EPSILON && invdenum < vml::math::EPSILON)
					vml::os::Error("Matrix4x4 : Matrix is singular");
				invdenum = 1.0f / invdenum;
				return mat4x4 <T> ((R[ 0] - min)* invdenum, (R[ 1] - min)* invdenum, (R[ 2] - min)* invdenum, (R[ 3] - min)* invdenum,
								   (R[ 4] - min)* invdenum, (R[ 5] - min)* invdenum, (R[ 6] - min)* invdenum, (R[ 7] - min)* invdenum,
								   (R[ 8] - min)* invdenum, (R[ 9] - min)* invdenum, (R[10] - min)* invdenum, (R[11] - min)* invdenum,
								   (R[12] - min)* invdenum, (R[13] - min)* invdenum, (R[14] - min)* invdenum, (R[15] - min)* invdenum);
			}

			//-------------------------------------------------------------------------------
			// helper functions to compute cofactors
			// which is a lower order matrix obtaind eliminating the a row
			// and b column

			template <typename T>
			__forceinline const mat3x3<T> GetCoFactor(const mat4x4<T>& R,const size_t a, const size_t b, bool evalsign)
			{
				if (a > 4)
					vml::os::Error("a is out of range");
				if (b > 4)
					vml::os::Error("b is out of range");
				if (a == 0 && b == 0 && !evalsign) return  mat3x3<T>(R[5], R[6], R[7], R[9], R[10], R[11], R[13], R[14], R[15]);
				if (a == 0 && b == 0 &&  evalsign) return  mat3x3<T>(R[5], R[6], R[7], R[9], R[10], R[11], R[13], R[14], R[15]);
				if (a == 1 && b == 0 && !evalsign) return  mat3x3<T>(R[4], R[6], R[7], R[8], R[10], R[11], R[12], R[14], R[15]);
				if (a == 1 && b == 0 &&  evalsign) return -mat3x3<T>(R[4], R[6], R[7], R[8], R[10], R[11], R[12], R[14], R[15]);
				if (a == 2 && b == 0 && !evalsign) return  mat3x3<T>(R[4], R[5], R[7], R[8], R[ 9], R[11], R[12], R[13], R[15]);
				if (a == 2 && b == 0 &&  evalsign) return  mat3x3<T>(R[4], R[5], R[7], R[8], R[ 9], R[11], R[12], R[13], R[15]);
				if (a == 3 && b == 0 && !evalsign) return  mat3x3<T>(R[4], R[5], R[6], R[8], R[ 9], R[10], R[12], R[13], R[14]);
				if (a == 3 && b == 0 &&  evalsign) return -mat3x3<T>(R[4], R[5], R[6], R[8], R[ 9], R[10], R[12], R[13], R[14]);
				if (a == 0 && b == 1 && !evalsign) return  mat3x3<T>(R[1], R[2], R[3], R[9], R[10], R[11], R[13], R[14], R[15]);
				if (a == 0 && b == 1 &&  evalsign) return -mat3x3<T>(R[1], R[2], R[3], R[9], R[10], R[11], R[13], R[14], R[15]);
				if (a == 1 && b == 1 && !evalsign) return  mat3x3<T>(R[0], R[2], R[3], R[8], R[10], R[11], R[12], R[14], R[15]);
				if (a == 1 && b == 1 &&  evalsign) return  mat3x3<T>(R[0], R[2], R[3], R[8], R[10], R[11], R[12], R[14], R[15]);
				if (a == 2 && b == 1 && !evalsign) return  mat3x3<T>(R[0], R[1], R[3], R[8], R[ 9], R[11], R[12], R[13], R[15]);
				if (a == 2 && b == 1 &&  evalsign) return -mat3x3<T>(R[0], R[1], R[3], R[8], R[ 9], R[11], R[12], R[13], R[15]);
				if (a == 3 && b == 1 && !evalsign) return  mat3x3<T>(R[0], R[1], R[2], R[8], R[ 9], R[10], R[12], R[13], R[14]);
				if (a == 3 && b == 1 &&  evalsign) return  mat3x3<T>(R[0], R[1], R[2], R[8], R[ 9], R[10], R[12], R[13], R[14]);
				if (a == 0 && b == 2 && !evalsign) return  mat3x3<T>(R[1], R[2], R[3], R[5], R[ 6], R[ 7], R[13], R[14], R[15]);
				if (a == 0 && b == 2 &&  evalsign) return  mat3x3<T>(R[1], R[2], R[3], R[5], R[ 6], R[ 7], R[13], R[14], R[15]);
				if (a == 1 && b == 2 && !evalsign) return  mat3x3<T>(R[0], R[2], R[3], R[4], R[ 6], R[ 7], R[12], R[14], R[15]);
				if (a == 1 && b == 2 &&  evalsign) return -mat3x3<T>(R[0], R[2], R[3], R[4], R[ 6], R[ 7], R[12], R[14], R[15]);
				if (a == 2 && b == 2 && !evalsign) return  mat3x3<T>(R[0], R[1], R[3], R[4], R[ 5], R[ 7], R[12], R[13], R[15]);
				if (a == 2 && b == 2 &&  evalsign) return  mat3x3<T>(R[0], R[1], R[3], R[4], R[ 5], R[ 7], R[12], R[13], R[15]);
				if (a == 3 && b == 2 && !evalsign) return  mat3x3<T>(R[0], R[1], R[2], R[4], R[ 5], R[ 6], R[12], R[13], R[14]);
				if (a == 3 && b == 2 &&  evalsign) return -mat3x3<T>(R[0], R[1], R[2], R[4], R[ 5], R[ 6], R[12], R[13], R[14]);
				if (a == 0 && b == 3 && !evalsign) return  mat3x3<T>(R[1], R[2], R[3], R[5], R[ 6], R[ 7], R[ 9], R[10], R[11]);
				if (a == 0 && b == 3 &&  evalsign) return -mat3x3<T>(R[1], R[2], R[3], R[5], R[ 6], R[ 7], R[ 9], R[10], R[11]);
				if (a == 1 && b == 3 && !evalsign) return  mat3x3<T>(R[0], R[2], R[3], R[4], R[ 6], R[ 7], R[ 8], R[10], R[11]);
				if (a == 1 && b == 3 &&  evalsign) return  mat3x3<T>(R[0], R[2], R[3], R[4], R[ 6], R[ 7], R[ 8], R[10], R[11]);
				if (a == 2 && b == 3 && !evalsign) return  mat3x3<T>(R[0], R[1], R[3], R[4], R[ 5], R[ 7], R[ 8], R[ 9], R[11]);
				if (a == 2 && b == 3 &&  evalsign) return -mat3x3<T>(R[0], R[1], R[3], R[4], R[ 5], R[ 7], R[ 8], R[ 9], R[11]);
				if (a == 3 && b == 3 && !evalsign) return  mat3x3<T>(R[0], R[1], R[2], R[4], R[ 5], R[ 6], R[ 8], R[ 9], R[10]);
				if (a == 3 && b == 3 &&  evalsign) return  mat3x3<T>(R[0], R[1], R[2], R[4], R[ 5], R[ 6], R[ 8], R[ 9], R[10]);
				return mat3x3<T>(0, 0, 0, 0, 0, 0, 0, 0, 0);
			}

			// ---------------------------------------------------------------
			// minors of a 4x4 matrix

			template <typename T>
			__forceinline const std::vector<mat3x3<T>> GetCoFactors(const mat4x4<T>& R,bool evalsign=true)
			{
				std::vector <mat3x3<T>> cofactors;
				cofactors.emplace_back(GetCoFactor(R,0, 0, evalsign));
				cofactors.emplace_back(GetCoFactor(R,1, 0, evalsign));
				cofactors.emplace_back(GetCoFactor(R,2, 0, evalsign));
				cofactors.emplace_back(GetCoFactor(R,3, 0, evalsign));
				cofactors.emplace_back(GetCoFactor(R,0, 1, evalsign));
				cofactors.emplace_back(GetCoFactor(R,1, 1, evalsign));
				cofactors.emplace_back(GetCoFactor(R,2, 1, evalsign));
				cofactors.emplace_back(GetCoFactor(R,3, 1, evalsign));
				cofactors.emplace_back(GetCoFactor(R,0, 2, evalsign));
				cofactors.emplace_back(GetCoFactor(R,1, 2, evalsign));
				cofactors.emplace_back(GetCoFactor(R,2, 2, evalsign));
				cofactors.emplace_back(GetCoFactor(R,3, 2, evalsign));
				cofactors.emplace_back(GetCoFactor(R,0, 3, evalsign));
				cofactors.emplace_back(GetCoFactor(R,1, 3, evalsign));
				cofactors.emplace_back(GetCoFactor(R,2, 3, evalsign));
				cofactors.emplace_back(GetCoFactor(R,3, 3, evalsign));
				return cofactors;
			}

			// --------------------------------------------------------
			// calculate the condition number
			// which is the norm multiplied
			// byt the norm of the matrix
			// inverse

			template <typename T>
			__forceinline T ConditionNumber(const mat4x4<T>& R)
			{
				T det = R[12] * R[ 9] * R[ 6] * R[ 3] -
						R[ 8] * R[13] * R[ 6] * R[ 3] -
						R[12] * R[ 5] * R[10] * R[ 3] +
						R[ 4] * R[13] * R[10] * R[ 3] +
						R[ 8] * R[ 5] * R[14] * R[ 3] -
						R[ 4] * R[ 9] * R[14] * R[ 3] -
						R[12] * R[ 9] * R[ 2] * R[ 7] +
						R[ 8] * R[13] * R[ 2] * R[ 7] +
						R[12] * R[ 1] * R[10] * R[ 7] -
						R[ 0] * R[13] * R[10] * R[ 7] -
						R[ 8] * R[ 1] * R[14] * R[ 7] +
						R[ 0] * R[ 9] * R[14] * R[ 7] +
						R[12] * R[ 5] * R[ 2] * R[11] -
						R[ 4] * R[13] * R[ 2] * R[11] -
						R[12] * R[ 1] * R[ 6] * R[11] +
						R[ 0] * R[13] * R[ 6] * R[11] +
						R[ 4] * R[ 1] * R[14] * R[11] -
						R[ 0] * R[ 5] * R[14] * R[11] -
						R[ 8] * R[ 5] * R[ 2] * R[15] +
						R[ 4] * R[ 9] * R[ 2] * R[15] +
						R[ 8] * R[ 1] * R[ 6] * R[15] -
						R[ 0] * R[ 9] * R[ 6] * R[15] -
						R[ 4] * R[ 1] * R[10] * R[15] +
						R[ 0] * R[ 5] * R[10] * R[15];

				// cannot invert singular matrix

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Error("Matrix4x4 : Matrix is singular");

				T m00 = R[0], m01 = R[4], m02 = R[ 8], m03 = R[12];
				T m10 = R[1], m11 = R[5], m12 = R[ 9], m13 = R[13];
				T m20 = R[2], m21 = R[6], m22 = R[10], m23 = R[14];
				T m30 = R[3], m31 = R[7], m32 = R[11], m33 = R[15];

				T t[16] = { m12 * m23 * m31 - m13 * m22 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 + m11 * m22 * m33,
						    m03 * m22 * m31 - m02 * m23 * m31 - m03 * m21 * m32 + m01 * m23 * m32 + m02 * m21 * m33 - m01 * m22 * m33,
						    m02 * m13 * m31 - m03 * m12 * m31 + m03 * m11 * m32 - m01 * m13 * m32 - m02 * m11 * m33 + m01 * m12 * m33,
						    m03 * m12 * m21 - m02 * m13 * m21 - m03 * m11 * m22 + m01 * m13 * m22 + m02 * m11 * m23 - m01 * m12 * m23,
						    m13 * m22 * m30 - m12 * m23 * m30 - m13 * m20 * m32 + m10 * m23 * m32 + m12 * m20 * m33 - m10 * m22 * m33,
						    m02 * m23 * m30 - m03 * m22 * m30 + m03 * m20 * m32 - m00 * m23 * m32 - m02 * m20 * m33 + m00 * m22 * m33,
						    m03 * m12 * m30 - m02 * m13 * m30 - m03 * m10 * m32 + m00 * m13 * m32 + m02 * m10 * m33 - m00 * m12 * m33,
						    m02 * m13 * m20 - m03 * m12 * m20 + m03 * m10 * m22 - m00 * m13 * m22 - m02 * m10 * m23 + m00 * m12 * m23,
						    m11 * m23 * m30 - m13 * m21 * m30 + m13 * m20 * m31 - m10 * m23 * m31 - m11 * m20 * m33 + m10 * m21 * m33,
						    m03 * m21 * m30 - m01 * m23 * m30 - m03 * m20 * m31 + m00 * m23 * m31 + m01 * m20 * m33 - m00 * m21 * m33,
						    m01 * m13 * m30 - m03 * m11 * m30 + m03 * m10 * m31 - m00 * m13 * m31 - m01 * m10 * m33 + m00 * m11 * m33,
						    m03 * m11 * m20 - m01 * m13 * m20 - m03 * m10 * m21 + m00 * m13 * m21 + m01 * m10 * m23 - m00 * m11 * m23,
						    m12 * m21 * m30 - m11 * m22 * m30 - m12 * m20 * m31 + m10 * m22 * m31 + m11 * m20 * m32 - m10 * m21 * m32,
						    m01 * m22 * m30 - m02 * m21 * m30 + m02 * m20 * m31 - m00 * m22 * m31 - m01 * m20 * m32 + m00 * m21 * m32,
						    m02 * m11 * m30 - m01 * m12 * m30 - m02 * m10 * m31 + m00 * m12 * m31 + m01 * m10 * m32 - m00 * m11 * m32,
						    m01 * m12 * m20 - m02 * m11 * m20 + m02 * m10 * m21 - m00 * m12 * m21 - m01 * m10 * m22 + m00 * m11 * m22 };

				det = 1.0f / det;

				t[ 0] *= det;	t[ 1] *= det;	t[ 2] *= det;	t[ 3] *= det;
				t[ 4] *= det;	t[ 5] *= det;	t[ 6] *= det;	t[ 7] *= det;
				t[ 8] *= det;	t[ 9] *= det;	t[10] *= det;	t[11] *= det;
				t[12] *= det;	t[13] *= det;	t[14] *= det;	t[15] *= det;

				double n0 = sqrtf(t[ 0] * t[ 0] + t[ 1] * t[ 1] + t[ 2] * t[ 2] + t[ 3] * t[ 3] +
								  t[ 4] * t[ 4] + t[ 5] * t[ 5] + t[ 6] * t[ 6] + t[ 7] * t[ 7] +
								  t[ 8] * t[ 8] + t[ 9] * t[ 9] + t[10] * t[10] + t[11] * t[11] +
								  t[12] * t[12] + t[13] * t[13] + t[14] * t[14] + t[15] * t[15]);

				double n1 = sqrtf(R[ 0] * R[ 0] + R[ 1] * R[ 1] + R[ 2] * R[ 2] + R[ 3] * R[ 3] +
								  R[ 4] * R[ 4] + R[ 5] * R[ 5] + R[ 6] * R[ 6] + R[ 7] * R[ 7] +
								  R[ 8] * R[ 8] + R[ 9] * R[ 9] + R[10] * R[10] + R[11] * R[11] +
								  R[12] * R[12] + R[13] * R[13] + R[14] * R[14] + R[15] * R[15]);

				return n0 * n1;
			}

			//-------------------------------------------------------------------------------
			// helper functions to compute cofactors
			// which is a lower order matrix obtaind eliminating the a row
			// and b column

			template <typename T>
			__forceinline const mat3x3<T> GetAdjoint(const mat4x4<T>& R,const size_t a, const size_t b)
			{
				return GetCoFactors(R,a, b, true);
			}

			// ---------------------------------------------------------------
			// minors of a 4x4 matrix

			template <typename T>
			__forceinline const std::vector<mat3x3<T>> GetAdjoints(const mat4x4<T>& R)
			{
				std::vector <mat3x3<T>> cofactors;
				cofactors.emplace_back(GetAdjoint(R,0, 0));
				cofactors.emplace_back(GetAdjoint(R,1, 0));
				cofactors.emplace_back(GetAdjoint(R,2, 0));
				cofactors.emplace_back(GetAdjoint(R,3, 0));
				cofactors.emplace_back(GetAdjoint(R,0, 1));
				cofactors.emplace_back(GetAdjoint(R,1, 1));
				cofactors.emplace_back(GetAdjoint(R,2, 1));
				cofactors.emplace_back(GetAdjoint(R,3, 1));
				cofactors.emplace_back(GetAdjoint(R,0, 2));
				cofactors.emplace_back(GetAdjoint(R,1, 2));
				cofactors.emplace_back(GetAdjoint(R,2, 2));
				cofactors.emplace_back(GetAdjoint(R,3, 2));
				cofactors.emplace_back(GetAdjoint(R,0, 3));
				cofactors.emplace_back(GetAdjoint(R,1, 3));
				cofactors.emplace_back(GetAdjoint(R,2, 3));
				cofactors.emplace_back(GetAdjoint(R,3, 3));
				return cofactors;
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
				T t[16] = { R[ 0] - M[ 0], R[ 1] - M[ 1], R[ 2] - M[ 2], R[ 3] - M[ 3], 
					        R[ 4] - M[ 4], R[ 5] - M[ 5], R[ 6] - M[ 6], R[ 7] - M[ 7] ,
					        R[ 8] - M[ 8], R[ 9] - M[ 9], R[10] - M[10], R[11] - M[11],
							R[12] - M[12] ,R[13] - M[13], R[14] - M[14], R[15] - M[15] };

				return sqrtf(t[ 0] * t[ 0] + t[ 1] * t[ 1] + t[ 2] * t[ 2] + t[ 3] * t[ 3] + 
					         t[ 4] * t[ 4] + t[ 5] * t[ 5] + t[ 6] * t[ 6] + t[ 7] * t[ 7] + 
					         t[ 8] * t[ 8] + t[ 9] * t[ 9] + t[10] * t[10] + t[11] * t[11] +
							 t[12] * t[12] + t[13] * t[13] + t[14] * t[14] + t[15] * t[15]);
			}

			// --------------------------------------------------------
			// create a normalized distance matrix given an 2x2 matrix

			template <typename T>
			__forceinline T NormalizedDistanceFromMatrix(const mat4x4<T>& R, const mat4x4<T>& M)
			{
				T t[16] = { R[0] - M[0], R[1] - M[1], R[2] - M[2], R[3] - M[3],
							R[4] - M[4], R[5] - M[5], R[6] - M[6], R[7] - M[7] ,
							R[8] - M[8], R[9] - M[9], R[10] - M[10], R[11] - M[11],
							R[12] - M[12] ,R[13] - M[13], R[14] - M[14], R[15] - M[15] };
				T max = t[0];
				if (t[ 1] > max)max = t[ 1];
				if (t[ 2] > max)max = t[ 2];
				if (t[ 3] > max)max = t[ 3];
				if (t[ 4] > max)max = t[ 4];
				if (t[ 5] > max)max = t[ 5];
				if (t[ 6] > max)max = t[ 6];
				if (t[ 7] > max)max = t[ 7];
				if (t[ 8] > max)max = t[ 8];
				if (t[ 9] > max)max = t[ 9];
				if (t[10] > max)max = t[10];
				if (t[11] > max)max = t[11];
				if (t[12] > max)max = t[12];
				if (t[13] > max)max = t[13];
				if (t[14] > max)max = t[14];
				if (t[15] > max)max = t[15];

				return sqrtf(t[ 0] * t[ 0] + t[ 1] * t[ 1] + t[ 2] * t[ 2] + t[ 3] * t[ 3] +
							 t[ 4] * t[ 4] + t[ 5] * t[ 5] + t[ 6] * t[ 6] + t[ 7] * t[ 7] +
							 t[ 8] * t[ 8] + t[ 9] * t[ 9] + t[10] * t[10] + t[11] * t[11] +
							 t[12] * t[12] + t[13] * t[13] + t[14] * t[14] + t[15] * t[15]) / max;
			}

			// ---------------------------------------------------------------
		    // make matrix simmetric using the realtion m=p+ transposed of p

			template <typename T>
			__forceinline mat4x4<T> MakeSimmetric(mat4x4<T>& R)
			{
				T a = R[ 0], b = R[ 1], c = R[ 2], d = R[ 3],
				  e = R[ 4], f = R[ 5], g = R[ 6], h = R[ 7],
				  i = R[ 8], j = R[ 9], k = R[10], l = R[11],
				  m = R[12], n = R[13], o = R[14], p = R[15];
				return mat4x4<T>(a + a, b + e, c + i, d + m,
								 e + b, f + f, g + j, h + n,
								 i + c, j + g, k + k, l + o,
								 m + d, n + h, o + l, p + p);
			}

			// --------------------------------------------------------
			// normalize matrix

			template <typename T>
			__forceinline mat4x4<T> Normalize(const vml::math::mat2x2<T>& R)
			{
				T det = R[12] * R[ 9] * R[ 6] * R[ 3] -
						R[ 8] * R[13] * R[ 6] * R[ 3] -
						R[12] * R[ 5] * R[10] * R[ 3] +
						R[ 4] * R[13] * R[10] * R[ 3] +
						R[ 8] * R[ 5] * R[14] * R[ 3] -
						R[ 4] * R[ 9] * R[14] * R[ 3] -
						R[12] * R[ 9] * R[ 2] * R[ 7] +
						R[ 8] * R[13] * R[ 2] * R[ 7] +
						R[12] * R[ 1] * R[10] * R[ 7] -
						R[ 0] * R[13] * R[10] * R[ 7] -
						R[ 8] * R[ 1] * R[14] * R[ 7] +
						R[ 0] * R[ 9] * R[14] * R[ 7] +
						R[12] * R[ 5] * R[ 2] * R[11] -
						R[ 4] * R[13] * R[ 2] * R[11] -
						R[12] * R[ 1] * R[ 6] * R[11] +
						R[ 0] * R[13] * R[ 6] * R[11] +
						R[ 4] * R[ 1] * R[14] * R[11] -
						R[ 0] * R[ 5] * R[14] * R[11] -
						R[ 8] * R[ 5] * R[ 2] * R[15] +
						R[ 4] * R[ 9] * R[ 2] * R[15] +
						R[ 8] * R[ 1] * R[ 6] * R[15] -
						R[ 0] * R[ 9] * R[ 6] * R[15] -
						R[ 4] * R[ 1] * R[10] * R[15] +
						R[ 0] * R[ 5] * R[10] * R[15];

				// cannot invert singular matrix

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					vml::os::Error("Matrix4x4 : Matrix is singular");

				det = T(1) / det;

				return mat4x4<T>(R[ 0] * det, R[ 1] * det, R[ 2] * det, R[ 3] * det,
								 R[ 4] * det, R[ 5] * det, R[ 6] * det, R[ 7] * det,
								 R[ 8] * det, R[ 9] * det, R[10] * det, R[11] * det,
								 R[12] * det, R[13] * det, R[14] * det, R[15] * det);
			}

			// --------------------------------------------------------
			// extracting colums as vector

			template <typename T>
			__forceinline vec4<T> GetColumn(const vml::math::mat4x4<T>& R, const size_t i)
			{
				if (i > 3)
					vml::os::Error("Mat4x4 : index i out of range");
				if (i == 0) return vec4<T>(R[0], R[4], R[ 8], R[12]);
				if (i == 1) return vec4<T>(R[1], R[5], R[ 9], R[13]);
				if (i == 2) return vec4<T>(R[2], R[6], R[10], R[14]);
				if (i == 3) return vec4<T>(R[3], R[7], R[11], R[15]);
				return vec4<T>(T(0), T(0), T(0), T(0));
			}

			// --------------------------------------------------------
			// extracting row as vector

			template <typename T>
			__forceinline vec4<T> GetRow(const vml::math::mat4x4<T>& R, const size_t i)
			{
				if (i > 3)
					vml::os::Error("Mat4x4 : index i out of range");
				if (i == 0) return vec4<T>(R[ 0], R[ 1], R[ 2], R[ 3]);
				if (i == 1) return vec4<T>(R[ 4], R[ 5], R[ 6], R[ 7]);
				if (i == 2) return vec4<T>(R[ 8], R[ 9], R[10], R[11]);
				if (i == 3) return vec4<T>(R[12], R[13], R[14], R[15]);
				return vec4<T>(T(0), T(0), T(0), T(0));
			}

			// --------------------------------------------------------
			// compute the minimum in the matrix

			template <typename T>
			__forceinline T GetMin(const vml::math::mat4x4<T> &R)
			{
				T min = R[0];
				if (R[ 1] < min) min = R[ 1];
				if (R[ 2] < min) min = R[ 2];
				if (R[ 3] < min) min = R[ 3];
				if (R[ 4] < min) min = R[ 4];
				if (R[ 5] < min) min = R[ 5];
				if (R[ 6] < min) min = R[ 6];
				if (R[ 7] < min) min = R[ 7];
				if (R[ 8] < min) min = R[ 8];
				if (R[ 9] < min) min = R[ 9];
				if (R[10] < min) min = R[10];
				if (R[11] < min) min = R[11];
				if (R[12] < min) min = R[12];
				if (R[13] < min) min = R[13];
				if (R[14] < min) min = R[14];
				if (R[15] < min) min = R[15];
				return min;
			}

			// --------------------------------------------------------
			// compute the absoulte minimum in the matrix

			template <typename T>
			__forceinline T GetAbsMin(const vml::math::mat4x4<T>& R)
			{
				T min = fabs(R[0]);
				if (fabs(R[ 1]) < min) min = fabs(R[ 1]);
				if (fabs(R[ 2]) < min) min = fabs(R[ 2]);
				if (fabs(R[ 3]) < min) min = fabs(R[ 3]);
				if (fabs(R[ 4]) < min) min = fabs(R[ 4]);
				if (fabs(R[ 5]) < min) min = fabs(R[ 5]);
				if (fabs(R[ 6]) < min) min = fabs(R[ 6]);
				if (fabs(R[ 7]) < min) min = fabs(R[ 7]);
				if (fabs(R[ 8]) < min) min = fabs(R[ 8]);
				if (fabs(R[ 9]) < min) min = fabs(R[ 9]);
				if (fabs(R[10]) < min) min = fabs(R[10]);
				if (fabs(R[11]) < min) min = fabs(R[11]);
				if (fabs(R[12]) < min) min = fabs(R[12]);
				if (fabs(R[13]) < min) min = fabs(R[13]);
				if (fabs(R[14]) < min) min = fabs(R[14]);
				if (fabs(R[15]) < min) min = fabs(R[15]);
				return min;
			}

			// --------------------------------------------------------
			// compute the minimum in the matrix

			template <typename T>
			__forceinline T GetMax(const vml::math::mat4x4<T>& R)
			{
				T min = R[0];
				if (R[ 1] > min) min = R[ 1];
				if (R[ 2] > min) min = R[ 2];
				if (R[ 3] > min) min = R[ 3];
				if (R[ 4] > min) min = R[ 4];
				if (R[ 5] > min) min = R[ 5];
				if (R[ 6] > min) min = R[ 6];
				if (R[ 7] > min) min = R[ 7];
				if (R[ 8] > min) min = R[ 8];
				if (R[ 9] > min) min = R[ 9];
				if (R[10] > min) min = R[10];
				if (R[11] > min) min = R[11];
				if (R[12] > min) min = R[12];
				if (R[13] > min) min = R[13];
				if (R[14] > min) min = R[14];
				if (R[15] > min) min = R[15];
				return min;
			}

			// --------------------------------------------------------
			// compute the absoulte minimum in the matrix

			template <typename T>
			__forceinline T GetAbsMax(const vml::math::mat4x4<T>& R)
			{
				T min = fabs(R[0]);
				if (fabs(R[ 1]) > min) min = fabs(R[ 1]);
				if (fabs(R[ 2]) > min) min = fabs(R[ 2]);
				if (fabs(R[ 3]) > min) min = fabs(R[ 3]);
				if (fabs(R[ 4]) > min) min = fabs(R[ 4]);
				if (fabs(R[ 5]) > min) min = fabs(R[ 5]);
				if (fabs(R[ 6]) > min) min = fabs(R[ 6]);
				if (fabs(R[ 7]) > min) min = fabs(R[ 7]);
				if (fabs(R[ 8]) > min) min = fabs(R[ 8]);
				if (fabs(R[ 9]) > min) min = fabs(R[ 9]);
				if (fabs(R[10]) > min) min = fabs(R[10]);
				if (fabs(R[11]) > min) min = fabs(R[11]);
				if (fabs(R[12]) > min) min = fabs(R[12]);
				if (fabs(R[13]) > min) min = fabs(R[13]);
				if (fabs(R[14]) > min) min = fabs(R[14]);
				if (fabs(R[15]) > min) min = fabs(R[15]);
				return min;
			}

			// --------------------------------------------------------
			// compute the floor of a given matrix

			template <typename T>
			__forceinline mat4x4<T> Floor(const vml::math::mat4x4<T>& R)
			{
				return mat4x4<T>(floor(R[ 0]), floor(R[ 1]), floor(R[ 2]), floor(R[ 3]),
								 floor(R[ 4]), floor(R[ 5]), floor(R[ 6]), floor(R[ 7]),
								 floor(R[ 8]), floor(R[ 9]), floor(R[10]), floor(R[11]),
								 floor(R[12]), floor(R[13]), floor(R[14]), floor(R[15]));
			}

			// --------------------------------------------------------
			// compute the ceil of a given matrix

			template <typename T>
			__forceinline mat4x4<T> Ceil(const vml::math::mat4x4<T>& R)
			{
				return mat4x4<T>(ceil(R[ 0]), ceil(R[ 1]), ceil(R[ 2]), ceil(R[ 3]),
								 ceil(R[ 4]), ceil(R[ 5]), ceil(R[ 6]), ceil(R[ 7]),
								 ceil(R[ 8]), ceil(R[ 9]), ceil(R[10]), ceil(R[11]),
								 ceil(R[12]), ceil(R[13]), ceil(R[14]), ceil(R[15]));
			}

			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat4x4<T> Saturate(const vml::math::mat4x4<T> &R)
			{
				T t[16] = { R[ 0], R[ 1], R[ 2], R[ 3],	
					        R[ 4], R[ 5], R[ 6], R[ 7],
							R[ 8], R[ 9], R[10], R[11],
							R[12] ,R[13], R[14], R[15] };

				if (t[0] > T(1)) t[0] = T(1);
				if (t[1] > T(1)) t[1] = T(1);
				if (t[2] > T(1)) t[2] = T(1);
				if (t[3] > T(1)) t[3] = T(1);
				if (t[0] < T(0)) t[0] = T(0);
				if (t[1] < T(0)) t[1] = T(0);
				if (t[2] < T(0)) t[2] = T(0);
				if (t[3] < T(0)) t[3] = T(0);

				return mat4x4<T>(t[ 0] , t[ 1] , t[ 2] , t[ 3] ,
								 t[ 4] , t[ 5] , t[ 6] , t[ 7] ,
								 t[ 8] , t[ 9] , t[10] , t[11] ,
								 t[12] , t[13] , t[14] , t[15] );

			}

			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat4x4<T> SaturateToZero(const vml::math::mat4x4<T>& R)
			{
				T t[16] = { R[ 0], R[ 1], R[ 2], R[ 3],
							R[ 4], R[ 5], R[ 6], R[ 7],
							R[ 8], R[ 9], R[10], R[11],
							R[12] ,R[13], R[14], R[15] };

				if (t[0] < T(0)) t[0] = T(0);
				if (t[1] < T(0)) t[1] = T(0);
				if (t[2] < T(0)) t[2] = T(0);
				if (t[3] < T(0)) t[3] = T(0);

				return mat4x4<T>(t[ 0], t[ 1], t[ 2], t[ 3],
								 t[ 4], t[ 5], t[ 6], t[ 7],
								 t[ 8], t[ 9], t[10], t[11],
								 t[12], t[13], t[14], t[15]);

			}

			// --------------------------------------------------------
			// compute the saturated of a given matrix

			template <typename T>
			__forceinline mat4x4<T> SaturateToOne(const vml::math::mat4x4<T>& R)
			{
				T t[16] = { R[ 0], R[ 1], R[ 2], R[ 3],
							R[ 4], R[ 5], R[ 6], R[ 7],
							R[ 8], R[ 9], R[10], R[11],
							R[12] ,R[13], R[14], R[15] };

				if (t[0] > T(1)) t[0] = T(1);
				if (t[1] > T(1)) t[1] = T(1);
				if (t[2] > T(1)) t[2] = T(1);
				if (t[3] > T(1)) t[3] = T(1);

				return mat4x4<T>(t[ 0], t[ 1], t[ 2], t[ 3],
								 t[ 4], t[ 5], t[ 6], t[ 7],
								 t[ 8], t[ 9], t[10], t[11],
								 t[12], t[13], t[14], t[15]);

			}


			// --------------------------------------------------------
			// matrix rotation

			template <typename T>
			__forceinline void  Rot(const vml::math::mat4x4<T>& R,T yaw, T pitch, T roll, T tx, T ty, T tz, T sx, T sy, T sz)
			{

				yaw *= vml::math::DEGTORAD;
				pitch *= vml::math::DEGTORAD;
				roll *= vml::math::DEGTORAD;

				T sinx = sin(yaw);
				T siny = sin(pitch);
				T sinz = sin(roll);
				T cosx = cos(yaw);
				T cosy = cos(pitch);
				T cosz = cos(roll);

				return mat4x4<T> ( cosy * cosz * sx                      , cosy * sinz * sx                      , -siny * sx       , 0,
								   sinx * siny * cosz - cosx * sinz * sy , sinx * siny * sinz + cosx * cosz * sy , sinx * cosy * sy , 0,
								   cosx * siny * cosz + sinx * sinz * sz , cosx * siny * sinz - sinx * cosz * sz , cosx * cosy * sz , 0,
								   tx									 , ty									 , tz               , 1 );

			}

			// --------------------------------------------------------
			// return bool if two matrices are equal

			template <typename T>
			__forceinline void IsEqual(const vml::math::mat4x4<T> &R, const vml::math::mat4x4<T> &P)
			{
				return fabs(R[0 ] - P[ 0]) < vml::math::EPSILON &&
					   fabs(R[ 1] - P[ 1]) < vml::math::EPSILON &&
					   fabs(R[ 2] - P[ 2]) < vml::math::EPSILON &&
					   fabs(R[ 3] - P[ 3]) < vml::math::EPSILON &&
					   fabs(R[ 4] - P[ 4]) < vml::math::EPSILON &&
					   fabs(R[ 5] - P[ 5]) < vml::math::EPSILON &&
					   fabs(R[ 6] - P[ 6]) < vml::math::EPSILON &&
					   fabs(R[ 7] - P[ 7]) < vml::math::EPSILON &&
					   fabs(R[ 8] - P[ 8]) < vml::math::EPSILON &&
					   fabs(R[ 9] - P[ 9]) < vml::math::EPSILON &&
					   fabs(R[10] - P[10]) < vml::math::EPSILON &&
					   fabs(R[11] - P[11]) < vml::math::EPSILON &&
					   fabs(R[12] - P[12]) < vml::math::EPSILON &&
					   fabs(R[13] - P[13]) < vml::math::EPSILON &&
					   fabs(R[14] - P[14]) < vml::math::EPSILON &&
					   fabs(R[15] - P[15]) < vml::math::EPSILON;
			}

			// ---------------------------------------------------------------
			// Determine if a 4x4 matrix is singular

			template <typename T>
			__forceinline bool IsSingular(const vml::math::mat4x4<T>& R)
			{
				T det= (R[12] * R[ 9] * R[ 6] * R[ 3] -
						R[ 8] * R[13] * R[ 6] * R[ 3] -
						R[12] * R[ 5] * R[10] * R[ 3] +
						R[ 4] * R[13] * R[10] * R[ 3] +
						R[ 8] * R[ 5] * R[14] * R[ 3] -
						R[ 4] * R[ 9] * R[14] * R[ 3] -
						R[12] * R[ 9] * R[ 2] * R[ 7] +
						R[ 8] * R[13] * R[ 2] * R[ 7] +
						R[12] * R[ 1] * R[10] * R[ 7] -
						R[ 0] * R[13] * R[10] * R[ 7] -
						R[ 8] * R[ 1] * R[14] * R[ 7] +
						R[ 0] * R[ 9] * R[14] * R[ 7] +
						R[12] * R[ 5] * R[ 2] * R[11] -
						R[ 4] * R[13] * R[ 2] * R[11] -
						R[12] * R[ 1] * R[ 6] * R[11] +
						R[ 0] * R[13] * R[ 6] * R[11] +
						R[ 4] * R[ 1] * R[14] * R[11] -
						R[ 0] * R[ 5] * R[14] * R[11] -
						R[ 8] * R[ 5] * R[ 2] * R[15] +
						R[ 4] * R[ 9] * R[ 2] * R[15] +
						R[ 8] * R[ 1] * R[ 6] * R[15] -
						R[ 0] * R[ 9] * R[ 6] * R[15] -
						R[ 4] * R[ 1] * R[10] * R[15] +
						R[ 0] * R[ 5] * R[10] * R[15]);

				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					return true;
				return false;
			}

			// ---------------------------------------------------------------
			// Determine if a 4x4 matrix is upper triangular

			template <typename T>
			__forceinline bool IsUpperTriangular(const vml::math::mat4x4<T>& R)
			{
				if (R[ 0] < -vml::math::EPSILON || R[ 0] > vml::math::EPSILON)
				if (R[ 1] < -vml::math::EPSILON || R[ 1] > vml::math::EPSILON)
				if (R[ 2] > -vml::math::EPSILON || R[ 2] < vml::math::EPSILON)
				if (R[ 3] < -vml::math::EPSILON || R[ 3] > vml::math::EPSILON)
				if (R[ 4] > -vml::math::EPSILON && R[ 4] < vml::math::EPSILON)
				if (R[ 5] < -vml::math::EPSILON || R[ 5] > vml::math::EPSILON)
				if (R[ 6] < -vml::math::EPSILON || R[ 6] > vml::math::EPSILON)
				if (R[ 7] > -vml::math::EPSILON || R[ 7] < vml::math::EPSILON)
				if (R[ 8] > -vml::math::EPSILON && R[ 8] < vml::math::EPSILON)
				if (R[ 9] > -vml::math::EPSILON && R[ 9] < vml::math::EPSILON)
				if (R[10] < -vml::math::EPSILON || R[10] > vml::math::EPSILON)
				if (R[11] < -vml::math::EPSILON || R[11] > vml::math::EPSILON)
				if (R[12] > -vml::math::EPSILON && R[12] < vml::math::EPSILON)
				if (R[13] > -vml::math::EPSILON && R[13] < vml::math::EPSILON)
				if (R[14] > -vml::math::EPSILON && R[14] < vml::math::EPSILON)
				if (R[15] < -vml::math::EPSILON || R[15] > vml::math::EPSILON)
					return true;
				if (R[ 0] < -vml::math::EPSILON || R[ 0] > vml::math::EPSILON)
				if (R[ 1] < -vml::math::EPSILON || R[ 1] > vml::math::EPSILON)
				if (R[ 2] > -vml::math::EPSILON || R[ 2] < vml::math::EPSILON)
				if (R[ 3] < -vml::math::EPSILON || R[ 3] > vml::math::EPSILON)
				if (R[ 4] < -vml::math::EPSILON || R[ 4] > vml::math::EPSILON)
				if (R[ 5] < -vml::math::EPSILON || R[ 5] > vml::math::EPSILON)
				if (R[ 6] > -vml::math::EPSILON || R[ 6] < vml::math::EPSILON)
				if (R[ 7] > -vml::math::EPSILON && R[ 7] < vml::math::EPSILON)
				if (R[ 8] < -vml::math::EPSILON || R[ 8] > vml::math::EPSILON)
				if (R[ 9] < -vml::math::EPSILON || R[ 9] > vml::math::EPSILON)
				if (R[10] > -vml::math::EPSILON && R[10] < vml::math::EPSILON)
				if (R[11] > -vml::math::EPSILON && R[11] < vml::math::EPSILON)
				if (R[12] < -vml::math::EPSILON || R[12] > vml::math::EPSILON)
				if (R[13] > -vml::math::EPSILON && R[13] < vml::math::EPSILON)
				if (R[14] > -vml::math::EPSILON && R[14] < vml::math::EPSILON)
				if (R[15] > -vml::math::EPSILON && R[15] < vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is lower triangulat

			template <typename T>
			__forceinline bool IsLowerTriangular(const vml::math::mat4x4<T> &R)
			{
				if (R[ 0] < -vml::math::EPSILON || R[ 0] > vml::math::EPSILON)
				if (R[ 1] > -vml::math::EPSILON && R[ 1] < vml::math::EPSILON)
				if (R[ 2] > -vml::math::EPSILON && R[ 2] < vml::math::EPSILON)
				if (R[ 3] > -vml::math::EPSILON && R[ 3] < vml::math::EPSILON)
				if (R[ 4] < -vml::math::EPSILON || R[ 4] > vml::math::EPSILON)
				if (R[ 5] < -vml::math::EPSILON || R[ 5] > vml::math::EPSILON)
				if (R[ 6] > -vml::math::EPSILON && R[ 6] < vml::math::EPSILON)
				if (R[ 7] > -vml::math::EPSILON && R[ 7] < vml::math::EPSILON)
				if (R[ 8] < -vml::math::EPSILON || R[ 8] > vml::math::EPSILON)
				if (R[ 9] < -vml::math::EPSILON || R[ 9] > vml::math::EPSILON)
				if (R[10] < -vml::math::EPSILON || R[10] > vml::math::EPSILON)
				if (R[11] > -vml::math::EPSILON && R[11] < vml::math::EPSILON)
				if (R[12] > -vml::math::EPSILON && R[12] < vml::math::EPSILON)
				if (R[13] > -vml::math::EPSILON && R[13] < vml::math::EPSILON)
				if (R[14] > -vml::math::EPSILON && R[14] < vml::math::EPSILON)
				if (R[15] > -vml::math::EPSILON && R[15] < vml::math::EPSILON)
					return true;
				if (R[ 0] > -vml::math::EPSILON && R[ 0] < vml::math::EPSILON)
				if (R[ 1] > -vml::math::EPSILON && R[ 1] < vml::math::EPSILON)
				if (R[ 2] > -vml::math::EPSILON && R[ 2] < vml::math::EPSILON)
				if (R[ 3] < -vml::math::EPSILON || R[ 3] > vml::math::EPSILON)
				if (R[ 4] > -vml::math::EPSILON && R[ 4] < vml::math::EPSILON)
				if (R[ 5] > -vml::math::EPSILON && R[ 5] < vml::math::EPSILON)
				if (R[ 6] < -vml::math::EPSILON || R[ 6] > vml::math::EPSILON)
				if (R[ 7] < -vml::math::EPSILON || R[ 7] > vml::math::EPSILON)
				if (R[ 8] > -vml::math::EPSILON && R[ 8] < vml::math::EPSILON)
				if (R[ 9] < -vml::math::EPSILON || R[ 9] > vml::math::EPSILON)
				if (R[10] < -vml::math::EPSILON || R[10] > vml::math::EPSILON)
				if (R[11] < -vml::math::EPSILON || R[11] > vml::math::EPSILON)
				if (R[12] < -vml::math::EPSILON || R[12] > vml::math::EPSILON)
				if (R[13] < -vml::math::EPSILON || R[13] > vml::math::EPSILON)
				if (R[14] < -vml::math::EPSILON || R[14] > vml::math::EPSILON)
				if (R[15] < -vml::math::EPSILON || R[15] > vml::math::EPSILON)
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is diagonal

			template <typename T>
			__forceinline bool IsDiagonal(const vml::math::mat3x3<T>& R)
			{
				if (R[ 0] < -vml::math::EPSILON || R[0] > vml::math::EPSILON)
				if (R[ 1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON)
				if (R[ 2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON)
				if (R[ 3] > -vml::math::EPSILON && R[3] < vml::math::EPSILON)
				if (R[ 4] > -vml::math::EPSILON && R[4] < vml::math::EPSILON)
				if (R[ 5] > -vml::math::EPSILON && R[5] < vml::math::EPSILON)
				if (R[ 6] < -vml::math::EPSILON || R[6] > vml::math::EPSILON)
				if (R[ 7] > -vml::math::EPSILON && R[7] < vml::math::EPSILON)
				if (R[ 8] > -vml::math::EPSILON && R[8] < vml::math::EPSILON)
				if (R[ 9] > -vml::math::EPSILON && R[ 9] < vml::math::EPSILON)
				if (R[10] > -vml::math::EPSILON && R[10] < vml::math::EPSILON)
				if (R[11] < -vml::math::EPSILON || R[11] > vml::math::EPSILON)
				if (R[12] > -vml::math::EPSILON && R[12] < vml::math::EPSILON)
				if (R[13] > -vml::math::EPSILON && R[13] < vml::math::EPSILON)
				if (R[14] > -vml::math::EPSILON && R[14] < vml::math::EPSILON)
				if (R[15] > -vml::math::EPSILON && R[15] < vml::math::EPSILON)
				if (R[16] < -vml::math::EPSILON || R[16] > vml::math::EPSILON)
					return true;
				return false;
			}

			// Determine if a 2x2 matrix is diagonal

			template <typename T>
			__forceinline bool IsIdentity(const vml::math::mat3x3<T>& R)
			{
				if (R[ 0] > 1 - vml::math::EPSILON && R[ 0] < 1 + vml::math::EPSILON)
				if (R[ 1] >   - vml::math::EPSILON && R[ 1] <     vml::math::EPSILON)
				if (R[ 2] >   - vml::math::EPSILON && R[ 2] <     vml::math::EPSILON)
				if (R[ 3] >   - vml::math::EPSILON && R[ 3] <     vml::math::EPSILON)
				if (R[ 4] >    -vml::math::EPSILON && R[ 4] <     vml::math::EPSILON)
				if (R[ 5] > 1  -vml::math::EPSILON && R[ 5] < 1 + vml::math::EPSILON)
				if (R[ 6] >    -vml::math::EPSILON && R[ 6] <     vml::math::EPSILON)
				if (R[ 7] >    -vml::math::EPSILON && R[ 7] <     vml::math::EPSILON)
				if (R[ 8] >   - vml::math::EPSILON && R[ 8] <     vml::math::EPSILON)
				if (R[ 9] >   - vml::math::EPSILON && R[ 9] <     vml::math::EPSILON)
				if (R[10] > 1 - vml::math::EPSILON && R[10] < 1 + vml::math::EPSILON)
				if (R[11] >   - vml::math::EPSILON && R[11] <     vml::math::EPSILON)
				if (R[12] >   - vml::math::EPSILON && R[12] <     vml::math::EPSILON)
				if (R[13] >   - vml::math::EPSILON && R[13] <     vml::math::EPSILON)
				if (R[14] >   - vml::math::EPSILON && R[14] <     vml::math::EPSILON)
				if (R[15] > 1 - vml::math::EPSILON && R[15] < 1 + vml::math::EPSILON)
						return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is zero

			template <typename T>
			__forceinline bool IsZero(const vml::math::mat2x2<T> &R)
			{
				if ((R[ 0] > -vml::math::EPSILON && R[0] < vml::math::EPSILON) &&
					(R[ 1] > -vml::math::EPSILON && R[1] < vml::math::EPSILON) &&
					(R[ 2] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[ 3] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[ 4] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[ 5] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[ 6] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[ 7] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[ 8] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[ 9] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[10] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[11] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[12] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[13] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[14] > -vml::math::EPSILON && R[2] < vml::math::EPSILON) &&
					(R[15] > -vml::math::EPSILON && R[3] < vml::math::EPSILON))
					return true;
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is simmetric

			template <typename T>
			__forceinline bool IsSimmetric(const vml::math::mat4x4<T>& R)
			{
				T det = R[1] - R[4];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
				{
					T det = R[2] - R[8];
					if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					{
						T det = R[3] - R[12];
						if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
						{
							T det = R[6] - R[9];
							if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
							{
								T det = R[7] - R[13];
								if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
								{
									T det = R[11] - R[14];
									if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
									{
										return true;
									}
								}
							}
						}
					}
				}
				return false;
			}

			// --------------------------------------------------------
			// return bool if matrix is simmetric

			template <typename T>
			__forceinline bool IsSkewSimmetric(const vml::math::mat4x4<T>& R)
			{
				T det = R[1] + R[4];
				if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
				{
					T det = R[2] + R[8];
					if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
					{
						T det = R[3] + R[12];
						if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
						{
							T det = R[6] + R[9];
							if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
							{
								T det = R[7] + R[13];
								if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
								{
									T det = R[11] + R[14];
									if (det > -vml::math::EPSILON && det < vml::math::EPSILON)
									{
										return true;
									}
								}
							}
						}
					}
				}
				return false;
			}

		} // end of namespace mat4x4utils

	} // end of namespace math
}
// end of namesapce vml

