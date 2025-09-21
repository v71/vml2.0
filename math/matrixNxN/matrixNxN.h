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

//////////////////////////////////////////////////////////////////
// to do :
// test


/////////////////////////////////////////////////////////////////
// NxN matrix class

namespace vml
{
	namespace math
	{

			template < typename T, const size_t S >
			class matnxn
			{
				
				private:

					T Data[S*S];
					size_t N=S;
					size_t Size=S*S;

					// --------------------------------------------------------------------
					// access operators

					__forceinline T& operator[](const int pos)
					{
						return Data[pos];
					}

					__forceinline const T& operator[](const int pos)  const
					{
						return Data[pos];
					}

					__forceinline const T& operator()(const size_t i, const size_t j)
					{
						return Data[i + j * N];
					}

			
				public:
				
					// gets matrix order
					
					const size_t GetOrder() const 
					{
						return N; 
					}

					// gets matrix size
					
					const size_t GetSize() const 
					{
						return Size; 
					}

					__forceinline const T* GetData() const
					{
						return Data;
					}

					__forceinline T* GetData()
					{
						return Data;
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

					// index at i,j coords

					int Index(const size_t i, const size_t j) const 
					{
						return  i + j*N; 
					}

					// --------------------------------------------------------------
					// unary operators

					const matnxn operator -()
					{
						matnxn t;
						for (size_t i = 0; i < Size; ++i)
							t.Data[i] = -Data[i];
						return t;
					}

					const matnxn operator +()
					{
						return *this;
					}

					// --------------------------------------------------------------

					const matnxn& operator +=(const matnxn& M)
					{
						for (size_t i = 0; i < Size; ++i)
							Data[i] += M.Data[i];
						return *this;
					}

					// --------------------------------------------------------------

					const matnxn& operator -=(const matnxn& M)
					{
						for (size_t i = 0; i < Size; ++i)
							Data[i] -= M.Data[i];
						return *this;
					}

					// --------------------------------------------------------------

					const matnxn& operator *=(const matnxn& R)
					{
						if (R.N != N)
							vml::os::Error(L"Matrices must have the same order");

						T* t = new T[Size];

						size_t i, j, k, offset;

						for (i = 0; i < N; ++i)
						{
							for (j = 0; j < N; ++j)
							{
								offset = j + i*N;

								t[offset] = (T)0.0f;

								for (k = 0; k < N; ++k)
								{
									t[offset] += Data[k + i*N] * R.Data[j + k*N];
								}
							}
						}

						memcpy(Data, t, sizeof(T)*Size);

						delete t;

						return *this;
					}

					// --------------------------------------------------------------

					const matnxn& operator /=(const matnxn& R)
					{
						matnxn<T, S> P = R;

						T det = P.Determinant();

						if (det == 0.0f)
							vml::os::Error(L"Determinant is null");

						P.Invert();

						T* t = new T[Size];

						size_t i, j, k, offset;

						for (i = 0; i < N; ++i)
						{
							for (j = 0; j < N; ++j)
							{
								offset = j + i*N;

								t[offset] = (T)0.0f;

								for (k = 0; k < N; ++k)
								{
									t[offset] += Data[k + i*N] * P.Data[j + k*N];
								}
							}
						}

						memcpy(Data, t, sizeof(T)*Size);

						delete t;

						return *this;
					}

					// --------------------------------------------------------------

					const matnxn& operator +=(T s)
					{
						for (size_t i = 0; i < Size; ++i)
							Data[i] += s;
						return *this;
					}

					// --------------------------------------------------------------

					const matnxn& operator -=(T s)
					{
						for (size_t i = 0; i < Size; ++i)
							Data[i] -= s;
						return *this;
					}

					// --------------------------------------------------------------

					const matnxn& operator *=(T s)
					{
						for (size_t i = 0; i < Size; ++i)
							Data[i] *= s;
						return *this;
					}

					// --------------------------------------------------------------

					const matnxn& operator /=(T s)
					{
						for (size_t i = 0; i < Size; ++i)
							Data[i] /= s;
						return *this;
					}

					// --------------------------------------------------------------
					// comparison operators

					bool operator== (const matnxn& M) const
					{
						return memcmp(Data, M.Data, Size*sizeof(T)) == 0;
					}

					bool operator!= (const matnxn& M) const
					{
						return memcmp(Data, M.Data, Size *sizeof(T)) != 0;
					}

					bool operator<= (const matnxn& M) const
					{
						return memcmp(Data, M.Data, Size *sizeof(T)) <= 0;
					}

					bool operator>= (const matnxn& M) const
					{
						return memcmp(Data, M.Data, Size *sizeof(T)) >= 0;
					}

					bool operator> (const matnxn& M) const
					{
						return memcmp(Data,M.Data, Size *sizeof(T)) > 0;
					}

					bool operator< (const matnxn& M) const
					{
						return memcmp(Data, M.Data, Size *sizeof(T)) < 0;
					}
					
					/*
					template <typename T>
					__forceinline T GetMin(const vml::math::matNxN<T>& R)
					{
						T min = Data[0];
						for (size_t i = 1; i < Size; ++i)
							if (Data[i] < min)min = Data[i];
						return min;
					}
					
					// --------------------------------------------------------
					// compute the maximum value in the matrix

					const T Max()
					{
						T max = Data[0];
						for (size_t i = 1; i < Size; ++i)
							if (Data[i] > max)max = Data[i];
						return max;
					}

					// --------------------------------------------------------
					// returns the minimum value in a row of the matrix
					
					const T RowMin(int row) 
					{
						int offset = row * N;
						T min = Data[offset];
						for (size_t i = 1; i < N; ++i)
						{
							int index = i + offset;
							if (Data[index] < min)min = Data[index];
						}
						return min;
					}

					// --------------------------------------------------------
					// returns the maxmim value in a row of the matrix

					const T RowMax(int row)
					{
						int offset = row * N;
						T max = Data[offset];
						for (size_t i = 1; i < N; ++i)
						{
							int index = i + offset;
							if (Data[index] > max)max = Data[index];
						}
						return max;
					}

					// --------------------------------------------------------
					// returns the minimum value in a column of the matrix

					const T ColumnMin(int row)
					{
						T min = Data[row];
						for (size_t j = 0; j < N; ++j)
						{
							int index = row + j * N;
							if (Data[index] < min)min = Data[index];
						}
						return min;
					}

					// --------------------------------------------------------
					// returns the minimum value in a column of the matrix

					const T ColumnMax(int row)
					{
						T max = Data[row];
						for (size_t j = 0; j < N; ++j)
						{
							int index = row + j * N;
							if (Data[index] > max)max = Data[index];
						}
						return max;
					}

					// --------------------------------------------------------
					// computes column sum

					const T ColumnSum(int column) 
					{
						T sum = (T)0.0f;
						for (size_t j = 0; j < N; ++j)
							sum += Data[column+j*N];
						return sum;
					}

					// --------------------------------------------------------
					// computes column sum

					const T RowSum(int column)
					{
						T sum = (T)0.0f;
						for (size_t i = 0; i < N; ++i)
							sum += Data[i+column*N];
						return sum;
					}

					// --------------------------------------------------------
					// gets a wrapped submatrix

					const matnxn<T, S - 1> GetSubMatrix(const size_t a, const size_t b,bool wrap=false)
					{
						if (wrap)
						{
							matnxn<T, S - 1> m;
							for (size_t j = b; j < b + N - 1; ++j)
							{
								int v = j - b;
								int t = j % N;
								for (size_t i = a; i < a + N - 1; ++i)
								{
									int u = i - a;
									int s = i % N;
									m[u + v * (N - 1)] = Data[s + t * N];
								}
							}
							return m;
						}
						else
						{
							matnxn<T, S - 1> m(T(0.0f));
							size_t Rn = m.GetOrder();
							size_t je = b + Rn;
							size_t ie = a + Rn;
							if (ie > S - 1) ie -= (a - 1);
							if (je > S - 1) je -= (b - 1);
							for (size_t j = b; j < je; ++j)
								for (size_t i = a; i < ie; ++i)
									m[(i - a) + (j - b)*Rn]=Data[i + j * N];
							return m;
						}
						
						// we never get here but visual studio complains

						return matnxn<T, S - 1>(T(0.0f));
					}

					// --------------------------------------------------------
					// sets a submatrix with values from a second matrix

					void SetSubMatrix(const size_t row, const size_t col, const matnxn<T, S - 1> &R)
					{
						size_t Rn = R.GetOrder();
						size_t je = col + Rn;
						size_t ie = row + Rn;
						if (ie > S - 1) ie -= (row - 1);
						if (je > S - 1) je -= (col - 1);
						for (size_t j = col; j < je; ++j)
							for (size_t i = row; i < ie; ++i)
								Data[i + j * N] = R[(i-row)+(j-col)*Rn];
					}

					// --------------------------------------------------------
					// get diagonal vector 

					const vecn<T, S> GetDiagonal()
					{
						vecn<T, S> V;
						for (size_t i = 0; i < N; ++i) V[i] = Data[i + i * N];
						return V;
					}

					// --------------------------------------------------------

					const vecn<T, S> GetRow(const size_t a)
					{
						size_t t = a % N;
						vecn<T, S> V;
						for (size_t i = 0; i < N; ++i) V[i] = Data[i + t * N];
						return V;
					}

					// --------------------------------------------------------

					const vecn<T, S> GetCol(const size_t a)
					{
						size_t t = a % N;
						vecn<T, S> V;
						for (size_t i = 0; i < N; ++i) V[i] = Data[t + i * N];
						return V;
					}

					

					// --------------------------------------------------------
					// return sub matrix, starting at a,b coordinates

					matnxn<T, S - 1> GetCoFactor(const size_t a, const size_t b)
					{
						return ComputeCofactors(a, b, false);
					}

					// ---------------------------------------------------------------
					// computing matrix adjoint

					matnxn<T, S> Adjoint()
					{
						if (S <= 1) vml::os::Error(L"order must be > 1");
						size_t i, j, u, v;
						matnxn<T, S> M;
						T *dest = M.GetData();

						v = 0;

						for (j = 0; j < N; j++)
						{
							u = 0;
							for (i = 0; i < N; i++)
							{
								matnxn<T, S - 1> minor = ComputeCofactors(i, j, true);
								dest[v + u*N] = minor.Determinant();
								u++;
							}
							v++;
						}
						return M;
					}

					// --------------------------------------------------------
					// create a shuffled matrix fiven an nxn matrix

					void NormalizedDistance(const size_t p)
					{
						double invmax = -DBL_MAX;
						for (size_t i = 0; i < Size; ++i)
							if (Data[i] > invmax) invmax = Data[i];
						invmax = 1.0f / invmax;
						T o = Data[p];
						for (size_t j = 0; j < N; ++j)
						{
							for (size_t i = 0; i < N; ++i)
							{
								int index = i + j * N;
								Data[index] = (Data[index] - o)*invmax;
							}
						}
					}

					// ---------------------------------------------------------------
					// Determine if a nxn matrix is singular
					// makes use of CMatrixDeterminant class

					const bool IsSingular()
					{
						return Determinant() == (T)0.0f;
					}

					// ---------------------------------------------------------------
					// Determine if a nxn matrix is diagonal

					const bool IsIdentity()
					{
						size_t i, j;

						// all element not on the diagonal have to be zero

						for (i = 0; i < N; ++i)
							for (j = 0; j < N; ++j)
								if (i != j && Data[i + j*N] != (T)0.0f)
									return false;

						// all elements on the diagonal have to be one

						for (i = 0; i < N; ++i)
							for (j = 0; j < N; ++j)
								if (i == j && Data[i + j*N] != (T)1.0f)
									return false;

						return true;
					}

					// ---------------------------------------------------------------
					// Determine if a nxn matrix is diagonal

					const bool IsDiagonal()
					{
						size_t i, j;

						// all element not on the diagonal have to be zero

						for (i = 0; i < N; ++i)
							for (j = 0; j < N; ++j)
								if (i != j && Data[i + j*N] != (T)0.0f)
									return false;

						// all elements on the diagonal have to different than zero

						for (i = 0; i < N; ++i)
							for (j = 0; j < N; ++j)
								if (i == j && Data[i + j*N] == (T)0.0f)
									return false;

						return true;
					}

					// ---------------------------------------------------------------
					// Determine if a nxn matrix is upper triangular

					const bool IsUpperTriangular()
					{
						size_t i, j;

						// upper part must be equal to zero

						for (i = 0; i < N; ++i)
							for (j = 0; j < N; ++j)
								if (i > j)
									if (Data[i + j*N] != (T)0.0f) return false;

						// lower part must be different than zero

						for (i = 0; i < N; ++i)
							for (j = 0; j < N; ++j)
								if (i <= j)
									if (Data[i + j*N] == (T)0.0f) return false;

						return true;
					}

					// ---------------------------------------------------------------
					// Determine if a nxn matrix is lower triangular

					const bool IsLowerTriangular()
					{
						size_t i, j;

						// upper part must be different to zero

						for (i = 0; i < N; ++i)
							for (j = 0; j < N; ++j)
								if (i >= j)
									if (Data[i + j*N] == (T)0.0f) return false;

						// lower part must be equal to zero

						for (i = 0; i < N; ++i)
							for (j = 0; j < N; ++j)
								if (i < j)
									if (Data[i + j*N] != (T)0.0f) return false;

						return true;
					}

					// ---------------------------------------------------------------
					// Determine if a nxn matrix is skew-symmetric
					// simmetric elements have to be opposite in sign

					const bool IsSkewSimmetric()
					{
						size_t i, j;
						for (j = 0; j < N; ++j)
							for (i = j; i < N; ++i)
								if (Data[i + j*N] != -Data[j + i*N])	return false;
						return true;
					}

					// ---------------------------------------------------------------
					// test if a NxN matrix is simmetric
					// opoosite elements have to be identical

					const bool IsSimmetric(const matnxn<T, S>& R)
					{
						size_t i, j;
						for (j = 0; j < N; ++j)
							for (i = j; i < N; ++i)
								if (Data[i + j*N] != Data[j + i*N])	return false;
						return true;
					}

					// ---------------------------------------------------------------
					// accepted types are double or float 

					void ValidateType()
					{
						const char *typestring = typeid(*Data).name();
						if ( strcmp(typestring,"double") && strcmp(typestring, "float"))
							vml::os::Error("Accepted template typenames are float or double");
					}
					*/
					
					// --------------------------------------------------------------
					// ctor / dtor

					matnxn()
					{
						//	ValidateType();
						if (S<=1) vml::os::Error("order must be > 1");
						memset(Data, 0, sizeof(T)*Size);
						for (size_t i = 0; i < N; ++i)
							Data[i + i*N] = T(1.0f);
					}

					matnxn(const T val)
					{
						//	ValidateType();
						if (S <= 1) vml::os::Error("order must be > 1");
						for (size_t i = 0; i < Size; ++i)
							Data[i] = val;
					}

					matnxn(const std::initializer_list<T> &il)
					{
						//	ValidateType();
						if (S <= 1) vml::os::Error("order must be > 1");
						if (il.size()>Size) vml::os::Error("Too many initialisers");
						if (il.size()<Size) vml::os::Error("Too few initialisers");
						memset(Data, 0, sizeof(T)*S);
						for (size_t i = 0; i < il.size(); ++i)
							Data[i] = (*(il.begin() + i));
					}
					
					// ---------------------------------------------------------------
					// assignment operator
					// if you assign different order matrices
					// the dest matrix will take the source's order
				
					matnxn& operator= (const matnxn &M)
					{
						if (&M == this)
							return *this;
						//	ValidateType();
						memcpy(Data, M.Data, sizeof(T)*Size);
						return *this;
					}

					// ---------------------------------------------------------------
					// copy constructor
					// if you assign different order matrices
					// the dest matrix will take the source's order
					
					matnxn(const matnxn& M)
					{
						//	ValidateType();
						N = M.N;
						Size = M.Size;
						memcpy(Data, M.Data, sizeof(T)*Size);
					}
					
					~matnxn()
					{
					}
				
			};
		
			/////////////////////////////////////////////////////////////////////////////////
			// friend functions

			template < typename T, const size_t S>
			const matnxn<T, S> operator *(const matnxn<T, S>& R, const T s)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t[i] = R.Data[i] * s;
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator *(const T s, const matnxn<T, S>& R)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t[i] = R.Data[i] * s;
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator *(const matnxn<T, S> &L, const matnxn<T, S> &R)
			{
				size_t n = R.N;
				matnxn<T, S> t;

				for ( size_t i = 0; i < n; ++i)
				{
					for (size_t j = 0; j < n; ++j)
					{
						size_t offset = j + i*n;
						t.Data[offset] = (T)0.0f;

						for (size_t k = 0; k < n; ++k)
							t.Data[offset] += L.Data[k + i*n] * R.Data[j + k*n];
					}
				}
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator /(const matnxn<T, S>& R, const T s)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t[i] = R.Data[i] / s;
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator /(const T s, const matnxn<T, S>& R)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t[i] = R.Data[i] / s;
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator /(const matnxn<T, S>& L, const matnxn<T, S>& R)
			{
				size_t i, j, k, offset, n;

				matnxn<T, S> W = R;
				W.Invert();
				n = R.N;

				matnxn<T, S> t;

				for (i = 0; i < n; ++i)
				{
					for (j = 0; j < n; ++j)
					{
						offset = j + i*n;
						t.Data[offset] = (T)0.0f;

						for (k = 0; k < n; ++k)
							t.Data[offset] += L.Data[k + i*n] * W.Data[j + k*n];
					}
				}
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator +(const matnxn<T, S> &R, const matnxn<T, S> &L)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t.Data[i] = R.Data[i] + L.Data[i];
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator +(const T a, const matnxn<T, S> &R)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t.Data[i] = R.Data[i] + a;
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator +(const matnxn<T, S> &R,const T a)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t.Data[i] = R.Data[i] + a;
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator -(const matnxn<T, S> &R, const matnxn<T, S> &L)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t.Data[i] = R.Data[i] - L.Data[i];
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator -(const matnxn<T, S> &R, const T a)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t.Data[i] = R.Data[i] - a;
				return t;
			}

			template < typename T, const size_t S>
			const matnxn<T, S> operator -(const T a,const matnxn<T, S> &R)
			{
				matnxn<T, S> t;
				for (size_t i = 0; i < R.Size; ++i)
					t.Data[i] = a - R.Data[i];
				return t;
			}


			template < typename T, const size_t S>
			const std::string ToString(const matnxn<T, S>& M)
			{
				std::string strbuffer[S * S];
				size_t strlength[S * S] = { 0 };

				int maxlenght = 0;
				
				int N = M.GetSize();

				for (size_t j = 0; j < N; ++j)
				{
					for (size_t i = 0; i < N; ++i)
					{
						int index = i + j * N;
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

				for (size_t j = 0; j < N; ++j)
				{
					for (size_t i = 0; i < N; ++i)
					{
						int index = i + j * N;

						os << strbuffer[index] << std::setw(maxlenght - strlength[index]);
					}

					os << "\n";
				}

				return os.str();

			}

			///////////////////////////////////////////////////////////////////////
			// vector operations

			template < typename T, const size_t S>
			const vecn<T,S> operator *(const vecn<T,S> &L, const matnxn<T, S> &R)
			{
				vecn<T, S> V;
				T *v = V.GetData();
				const T *r = R.GetConstData();
				const T *l = L.GetConstData();
				const T *m = R.GetConstData();
				for (size_t j = 0; j < S; ++j)
				{
					v[j] = (T)0.0f;
					for (size_t i = 0; i < S; ++i)
						v[j] += r[j + i*S] * l[i];
				}
				return V;
			}

			template < typename T, const size_t S>
			const vecn<T,S> operator *(const matnxn<T, S> &R, const vecn<T,S> &L)
			{
				vecn<T, S> V;
				T *v = V.GetData();
				const T *r = R.GetConstData();
				const T *l = L.GetConstData();
				for (size_t j = 0; j < S; ++j)
				{
					v[j] = (T)0.0f;
					for (size_t i = 0; i < S; ++i)
						v[j] += r[i + j*S] * l[i];
				}
				
				return V;
			}


	} // end of namespace math

} // end of namesapce vml


///////////////////////////////////////////////////////////////////////////////////
// predefined types

namespace vml
{
	namespace math
	{
		template < const size_t S> struct matnxnb    { typedef matnxn<byte    , S> matnxn; };		// 8-bit  unsigned.
		template < const size_t S> struct matnxnui8  { typedef matnxn<uint8   , S> matnxn; };		// 8-bit  unsigned.
		template < const size_t S> struct matnxnui16 { typedef matnxn<uint16  , S> matnxn; };		// 16-bit  unsigned.
		template < const size_t S> struct matnxnui32 { typedef matnxn<uint32  , S> matnxn; };		// 32-bit  unsigned.
		template < const size_t S> struct matnxnui64 { typedef matnxn<uint64  , S> matnxn; };		// 64-bit  unsigned.
		template < const size_t S> struct matnxni8   { typedef matnxn<int8    , S> matnxn; };	    // 8-bit  signed.
		template < const size_t S> struct matnxni16  { typedef matnxn<int16   , S> matnxn; };		// 16-bit  signed.
		template < const size_t S> struct matnxni32  { typedef matnxn<int32   , S> matnxn; };		// 32-bit  signed.
		template < const size_t S> struct matnxni64  { typedef matnxn<int64   , S> matnxn; };		// 64-bit  signed.
		template < const size_t S> struct matnxnuf   { typedef matnxn<float32 , S> matnxn; };		// 32-bit  signed.
		template < const size_t S> struct matnxnud   { typedef matnxn<float64 , S> matnxn; };		// 64-bit  signed.
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace math
	{
		namespace matnxnutils
		{

			
			// --------------------------------------------------------
			// matrix determinant
		
			template < typename T, const size_t S>
			__forceinline T Determinant(const matnxn<T, S>& R)
			{
				// compute determinant trhough the use of
				// lu decomposition

				size_t i, j, k;		// Matrix subscripts
				size_t counti;		// Counts number of I operations
				T factor;			// Greatest common factor
				T temp;				// Temporary variable
		
				counti = 0;			// Initialize the I count

				// Allocate a one-dimensional array of pointers, one pointer per row
						
				const size_t N = R.GetOrder();

				T **mirrormatrix = new T*[N];

				for (i = 0; i < N; ++i)
					mirrormatrix[i] = new T[N];

				// Fill matrix

				for (i = 0; i < N; i++)
					for (j = 0; j < N; j++)
						mirrormatrix[i][j] = R[i + j*N];

				// Transform matrix into upper triangular

				for (i = 0; i < N - 1; i++)
				{
					if (mirrormatrix[i][i] ==(T) 0.0f)
					{
						for (k = i; k < N; k++)
						{
							if (mirrormatrix[k][i] != 0.0f)
							{
								for (j = 0; j < N; j++)
								{
									temp = mirrormatrix[i][j];
									mirrormatrix[i][j] = mirrormatrix[k][j];
									mirrormatrix[k][j] = temp;
								}
								k = N;
							}
						}
						counti++;
					}

					if (mirrormatrix[i][i] != (T)0.0f)
					{
						for (k = i + 1; k < N; k++)
						{
							factor = -1.0f * mirrormatrix[k][i] / mirrormatrix[i][i];

							for (j = i; j < N; j++)
							{
								mirrormatrix[k][j] = mirrormatrix[k][j] + (factor * mirrormatrix[i][j]);
							}
						}
					}
				}

				temp = (T)1.0f;

				//Calculate determinant

				for (i = 0; i < N; i++)
					temp *= mirrormatrix[i][i];

				if (counti % 2 != 0)
					temp = -temp;

				// free memory

				for (i = 0; i < N; i++)
					delete[]mirrormatrix[i];
		
				delete[]mirrormatrix;
						
				return temp;

			}
				
			// --------------------------------------------------------
			// matrix norm
			
			template < typename T, const size_t S>
			__forceinline T Norm(const matnxn<T, S>& R)
			{
				T retVal = (T)0.0f;
				for (size_t i = 0; i < R.GetSize(); ++i)
					retVal += R[i] * R[i];
				return sqrtf(retVal);
			}

			// --------------------------------------------------------
			// computes the trace for anxn matrix
			// the sum of all elelemnts of the main diagonal

			template < typename T, const size_t S>
			__forceinline T Trace(const matnxn<T, S>& R)
			{
				const size_t N = R.GetOrder();
				T retVal = (T)0.0f;
				for (size_t i = 0; i < N; ++i)
					retVal += R[i + i * N];
				return retVal;
			}

			// --------------------------------------------------------
			// computes the rank for a nxn matrix

			template < typename T, const size_t S>
			__forceinline int Rank(const matnxn<T, S>& R)
			{
				// even if matrix entries are 
				// templated, these values are
				// doubles for maximum accuracy

				int rank;
				int iSign, jSign;
				double MaxValue;
				double tmp, d;
				const size_t Size = R.GetSize();
				const size_t N = R.GetOrder();

				T* m = new T[Size];

				memcpy(m, R.GetData(), sizeof(T) * Size);

				rank = 0;

				for (int k = 0; k < N; k++)
				{

					MaxValue = 0.0f;

					for (int i = k; i < N; ++i)
					{
						for (int j = k; j < N; ++j)
						{
							tmp = fabsf(m[i + j * N]);

							if (tmp > MaxValue)
							{
								MaxValue = tmp;
								iSign = i;
								jSign = j;
							}
						}
					}

					// if maxValue is near zero, the rank has been found

					if (MaxValue == (T)0.0f)
						break;					//return mRank;
					else
						rank++;					// increase rank and go aehad

					if (k == (N - 1))
						break;					//return mRank;

					if (iSign != k)
					{
						for (int j = k; j < N; ++j)
							std::swap(m[k + j * N], m[iSign + j * N]);	// swap columns
					}

					if (jSign != k)
					{
						for (int i = k; i < N; ++i)
							std::swap(m[i + jSign * N], m[i + k * N]);	// swap columns
					}

					for (int i = k + 1; i < N; ++i)
					{
						d = (double)m[i + k * N] / (double)m[k + k * N];
						for (int j = k + 1; j < N; ++j)
							m[i + j * N] -= d * m[k + j * N];
					}

				}

				delete[] m;

				return rank;
			}

			// --------------------------------------------------------
			// invert a nxn matrix

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> Invert(matnxn<T, S>& R)
			{
				const double MINVALUE = 0.0001f;
				const size_t N = R.GetOrder();
				matnxn<T, S> P;

				// 2D array declared to store augmented matrix 

				T temporary, r;
				size_t i, j, k, temp;

				// creating augmented matrix

				T** mirrormatrix = new T * [N];

				for (i = 0; i < N; ++i)
					mirrormatrix[i] = new T[2 * N];

				//   storing augmented matrix as a matrix of n
				//    (n)x(2*n) in 2D array  

				for (i = 0; i < N; ++i)
					for (j = 0; j < N; ++j)
						mirrormatrix[i][j] = R[i + j * N];

				// augmenting with identity matrix of similar dimensions 

				for (i = 0; i < N; ++i)
					for (j = N; j < 2 * N; ++j)
						if (i == j % N)
							mirrormatrix[i][j] = (T)1.0f;
						else
							mirrormatrix[i][j] = (T)0.0f;

				// using gauss-jordan elimination 

				for (j = 0; j < N; ++j)
				{
					temp = j;

					// finding maximum jth column element in last (n-j) rows 

					for (i = j + 1; i < N; ++i)
						if (mirrormatrix[i][j] > mirrormatrix[temp][j])	temp = i;

					if (fabsf(mirrormatrix[temp][j]) < MINVALUE)
					{
						// elements are too small to deal with
						// probably determinant is near 0						
						// free memory

						for (i = 0; i < N; ++i)
							delete[]mirrormatrix[i];
						delete[]mirrormatrix;

						return P;
					}

					// swapping row which has maximum jth column element 

					if (temp != j)
					{
						for (k = 0; k < 2 * N; ++k)
						{
							temporary = mirrormatrix[j][k];
							mirrormatrix[j][k] = mirrormatrix[temp][k];
							mirrormatrix[temp][k] = temporary;
						}
					}

					// performing row operations to form required identity matrix out of the input matrix 

					for (i = 0; i < N; ++i)
						if (i != j)
						{
							r = mirrormatrix[i][j];
							for (k = 0; k < 2 * N; k++)
								mirrormatrix[i][k] -= (mirrormatrix[j][k] / mirrormatrix[j][j]) * r;
						}
						else
						{
							r = mirrormatrix[i][j];
							for (k = 0; k < 2 * N; k++)
								mirrormatrix[i][k] /= r;
						}
				}

				// store augmented matrix in original matrix

				for (j = N; j < 2 * N; ++j)
					for (i = 0; i < N; ++i)
						P[i + (j - N) * N] = mirrormatrix[i][j];

				// free memory

				for (i = 0; i < N; ++i)
					delete[]mirrormatrix[i];
				delete[]mirrormatrix;

				return P;
			}

			// --------------------------------------------------------
			// transposes a nxn matrix

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> Transpose(const matnxn<T, S>& R)
			{
				const size_t N = R.GetOrder();
				matnxn<T, S> P = R;
				for (size_t i = 0; i < N; ++i)
				{
					for (size_t j = i + 1; j < N; ++j)
					{
						size_t a = i + j * N;
						size_t b = j + i * N;
						T temp = P[a];
						P[a] = P[b];
						P[b] = temp;
					}
				}
				return P;
			}

			// --------------------------------------------------------
			// zero matrix

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> Zero()
			{
				matnxn<T, S> P;
				for (size_t i = 0; i < P.GetSize(); ++i)
					P[i] = 0;
				return P;
			}

			// --------------------------------------------------------
			// matrix identity

			template < typename T, const size_t S>
			__forceinline  matnxn<T, S>  Identity()
			{
				return matnxn <T, S>();
			}

			// --------------------------------------------------------
			// initialise matrix with random values inn the min ,max range

			template < typename T, const size_t S>
			__forceinline  matnxn<T, S> Random(const matnxn<T, S> &R,const int min, const int max)
			{
				matnxn<T, S> P;
				for (size_t i = 0; i < P.GetSize(); ++i)
					P[i] = min + (int)(rand() / (double)(RAND_MAX + 1) * (max - min + 1));
				return P;
			}

			// --------------------------------------------------------
			// initialise matrix with value

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> Diagonal(const T value)
			{
				matnxn<T, S> P;
				const size_t N = P.GetOrder();
				for (size_t i = 0; i < P.GetSize(); ++i)
					P[i] = 0;
				for (size_t i = 0; i < N; ++i)
					P[i + i * N] = value;
				return P;
			}

			// --------------------------------------------------------
			// Create a nxn upper triangular matrix within a random range

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> UpperTriangular(matnxn<T, S> &R,int min, int max)
			{

				matnxn<T, S> P;
				
				const size_t N = P.GetOrder();

				// upper part must be different to zero

				if (min > max) std::swap(min, max);

				for (size_t i = 0; i < N; ++i)
					for (size_t j = 0; j < N; ++j)
						if (i >= j)
							P[i + j * N] = min + (int)(rand() / (double)(RAND_MAX + 1) * (max - min + 1));

				// lower part must be equal to zero

				for (size_t i = 0; i < N; ++i)
					for (size_t j = 0; j < N; ++j)
						if (i < j)
							P[i + j * N] = (T)0.0f;
			
				return P;
			}

			// --------------------------------------------------------
			// Create a nxn upper triangular matrix within a random range

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> UpperTriangular(matnxn<T, S>& R, const T value)
			{

				matnxn<T, S> P;

				const size_t N = P.GetOrder();

				// upper part must be different to zero

				for (size_t i = 0; i < N; ++i)
					for (size_t j = 0; j < N; ++j)
						if (i >= j)
							P[i + j * N] = value;

				// lower part must be equal to zero

				for (size_t i = 0; i < N; ++i)
					for (size_t j = 0; j < N; ++j)
						if (i < j)
							P[i + j * N] = (T)0.0f;

				return P;
			}

			// --------------------------------------------------------
			// initialise matrix with a lower triangular values
			// int the min,max range

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> LowerTriangular(matnxn<T, S>& R, int min, int max)
			{
				matnxn<T, S> P;

				const size_t N = P.GetOrder();

				// upper part must be equal to zero

				for (size_t i = 0; i < N; ++i)
					for (size_t j = 0; j < N; ++j)
						if (i > j)
							P[i + j * N] = 0.0f;

				// lower part must be different than zero

				if (min > max) std::swap(min, max);

				for (size_t i = 0; i < N; ++i)
					for (size_t j = 0; j < N; ++j)
						if (i <= j)
							P[i + j * N] = min + (int)(rand() / (double)(RAND_MAX + 1) * (max - min + 1));

				return P;

			}

			// --------------------------------------------------------
			// initialise matrix with a lower triangular values
			// int the min,max range

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> LowerTriangular(matnxn<T, S>& R, const T value)
			{
				matnxn<T, S> P;

				const size_t N = P.GetOrder();

				// upper part must be equal to zero

				for (size_t i = 0; i < N; ++i)
					for (size_t j = 0; j < N; ++j)
						if (i > j)
							P[i + j * N] = 0.0f;

				// lower part must be different than zero

				for (size_t i = 0; i < N; ++i)
					for (size_t j = 0; j < N; ++j)
						if (i <= j)
							P[i + j * N] = value;

				return P;

			}

			// --------------------------------------------------------
			// compute the absolute value of a matriz

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> Abs(matnxn<T, S>& R)
			{
				matnxn<T, S> P;
				for (size_t i = 0; i < P.GetSize(); ++i)
						P[i] = fabs(R[i]);
				return P;
			}

			// --------------------------------------------------------
			// flat matrix value in the the [0..1] range

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> Flat(matnxn<T, S>& R)
			{
				matnxn<T, S> P;
				T min = R[0];
				const size_t Size = P.GetSize();
				for (size_t i = 1; i < Size; ++i)
					if (R[i] < min)min = R[i];
				T max = R[0];
				for (size_t i = 1; i < Size; ++i)
					if (R[i] > max)max = R[i];
				T invdenum = max - min;
				if (invdenum > -math::EPSILON && invdenum < math::EPSILON) 
					vml::os::Error("null difference");
				invdenum = 1.0f / invdenum;
				for (size_t i = 0; i < Size; i++)
					P[i] = (R[i] - min) * invdenum;
			}

			
			//-------------------------------------------------------------------------------
			// helper functions to compute cofactors
			// which is a lower order matrix obtaind eliminating the a row
			// and b column

			template < typename T, const size_t S>
			__forceinline matnxn<T, S-1> GetCofactor(matnxn<T, S>& R,const size_t a, const size_t b, bool evalsign)
			{
				const size_t N = R.GetOrder();

				if (a < 0 || a >= N)
					vml::os::Error(L"a is out of range");
				if (b < 0 || b >= N)
					vml::os::Error(L"b is out of range");

				matnxn<T, S - 1> minor;
				
				T* p = minor.GetData();

				const size_t l = S - 1;

				size_t v = 0;

				for (size_t j = 0; j < N; ++j)
				{
					size_t u = 0;

					for (size_t i = 0; i < N; ++i)
					{
						if (i != a && j != b)
						{
							T sign = 1;

							if (evalsign)
							{
								if (((i + j) & 1) == 0) sign = 1;
								else sign = -1;
							}

							p[u + v*l] = (T)sign*(T)R[i + j*N];

							u++;
						}
					}

					if (j != b) v++;
				}

				return minor;
			}

			// ---------------------------------------------------------------
			// minors of a nxn matrix

			template < typename T, const size_t S>
			__forceinline std::vector<matnxn<T, S-1>> GetCofactor(const matnxn<T, S>& R, const size_t a, const size_t b, bool evalsign)
			{
				if (S-1 <= 1) vml::os::Error("order must be > 1");
				std::vector <vml::math::matnxn<T, S - 1>> cofactors;
				for (size_t j = 0; j < S; ++j)
					for (size_t i = 0; i < S; ++i)
						cofactors.emplace_back(GetCofactor(R,i, j, false));
				return cofactors;
			}

			// --------------------------------------------------------
			// calculate the condition number
			// which is the norm multiplied
			// byt the norm of the matrix 
			// inverse

			template < typename T, const size_t S>
			__forceinline T ConditionNumber(const matnxn<T, S>& R)
			{
				matnxn<T, S> temp(R);
				if (!temp.Invert()) return (T)0.0f;
				return Norm(R) * Norm(temp);
			}


			//-------------------------------------------------------------------------------
			// helper functions to compute cofactors
			// which is a lower order matrix obtaind eliminating the a row
			// and b column

			template < typename T, const size_t S>
			__forceinline matnxn<T, S - 1> GetAdjoint(const matnxn<T, S>& R, const size_t a, const size_t b)
			{
				return GetCofactor(R, a, b, true);
			}

			// ---------------------------------------------------------------
			// minors of a 4x4 matrix

			template < typename T, const size_t S>
			__forceinline std::vector<matnxn<T, S - 1>> GetAdjoits(const matnxn<T, S>& R)
			{
				if (S - 1 <= 1) vml::os::Error("order must be > 1");
				std::vector <vml::math::matnxn<T, S - 1>> cofactors;
				for (size_t j = 0; j < S; ++j)
					for (size_t i = 0; i < S; ++i)
						cofactors.emplace_back(GetAdjoint(R, i, j));
				return cofactors;
			}

			// --------------------------------------------------------
			// create a submatrix given an nxn matrix

			template < typename T, const size_t S>
			__forceinline matnxn<T, S - 1> GetSubMatrix(const matnxn<T, S>& R, const size_t a, const size_t b)
			{
				return GetCofactor(R, a, b, false);
			}

			// --------------------------------------------------------
			// create a distance matrix given an 2x2 matrix

			template < typename T, const size_t S>
			__forceinline T DistanceFromMatrix(const matnxn<T, S>& R, const matnxn<T, S>& M)
			{
				T dist = T(0);
				for (size_t i = 0; i < S; ++i)
					dist += R[i] - M[i];
				return T(sqrtf(dist));
			}

			// --------------------------------------------------------
			// create a normalized distance matrix given an 2x2 matrix

			template < typename T, const size_t S>
			__forceinline T NormalizedDistanceFromMatrix(const matnxn<T, S>& R, const matnxn<T, S>& M)
			{
				T dist = T(0);
				for (size_t i = 0; i < S; ++i)
					dist += R[i] - M[i];
				T dmax = R[0];
				for (size_t i = 0; i < S; ++i)
					if (R[i] > dmax)dmax = R[i];
				return T(sqrtf(dist)/dmax);
			}

			
			// ---------------------------------------------------------------
		    // make matrix simmetric using the realtion m=p+ transposed of p

			template < typename T, const size_t S>
			__forceinline matnxn<T, S> MakeSimmetric(const mat4x4<T>& R)
			{
				return R + Inverse(R);
			}

			/*// --------------------------------------------------------
			// normalize matrix

			template <typename T>
			__forceinline void Normalize(const vml::math::mat2x2<T>& R)
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
			__forceinline void IsLowerTriangular(const vml::math::mat4x4<T> &R)
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
			__forceinline void IsZero(const vml::math::mat2x2<T> &R)
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

			*/
		}
	}
}
