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
		namespace matrixsolvers
		{

			///////////////////////////////////////////////////////////////////////////////////
			// linear system solvers

			////////////////////////////////////////////////////////////
			// This function uses Crout's method to decompose a row interchanged
			// version of the n x n matrix A into a lower triangular matrix L and a
			// unit upper triangular matrix U such that A = LU.
			// the function uses partial pivoting

			template < typename T, const size_t S >
			static bool CroutLUDecompositionWithPivoting(T *A, int* pivot, const size_t n)
			{

				int  i, j, k;
				T *p_k, *p_row, *p_col;
				T max;

				//  For each row and column, k = 0, ..., n-1,

				for (k = 0, p_k = A; k < n; p_k += n, k++)
				{

					//   find the pivot row

					pivot[k] = k;
					max = fabs(*(p_k + k));

					p_col = p_k;

					for (j = k + 1, p_row = p_k + n; j < n; j++, p_row += n)
					{
						if (max < fabs(*(p_row + k)))
						{
							max = fabs(*(p_row + k));
							pivot[k] = j;
							p_col = p_row;
						}
					}

					//     and if the pivot row differs from the current row, then
					//     interchange the two rows.

					if (pivot[k] != k)
					{
						for (j = 0; j < n; j++)
						{
							max = *(p_k + j);
							*(p_k + j) = *(p_col + j);
							*(p_col + j) = max;
						}
					}

					//     and if the matrix is singular, return error

					if (*(p_k + k) == 0.0) return false;

					//      otherwise find the upper triangular matrix elements for row k.

					for (j = k + 1; j < n; j++)
					{
						*(p_k + j) /= *(p_k + k);
					}

					//            update remaining matrix

					for (i = k + 1, p_row = p_k + n; i < n; p_row += n, i++)
						for (j = k + 1; j < n; j++)
							*(p_row + j) -= *(p_row + k) * *(p_k + j);

				}

				return true;
			}

			/////////////////////////////////////////////////////////////////////////////////

			template < typename T, const size_t S >
			static bool CroutLUWithPivotingSolve(T *LU, T* B, int* pivot, T* x, const size_t n)
			{
				int i, k;
				T *p_k;
				T dum;

				//   Solve the linear equation Lx = B for x, where L is a lower
				//   triangular matrix.

				for (k = 0, p_k = LU; k < n; p_k += n, k++)
				{
					if (pivot[k] != k) { dum = B[k]; B[k] = B[pivot[k]]; B[pivot[k]] = dum; }
					x[k] = B[k];
					for (i = 0; i < k; i++) x[k] -= x[i] * *(p_k + i);
					x[k] /= *(p_k + k);
				}

				//         Solve the linear equation Ux = y, where y is the solution
				//         obtained above of Lx = B and U is an upper triangular matrix.
				//         The diagonal part of the upper triangular part of the matrix is
				//         assumed to be 1.0.

				for (k = n - 1, p_k = LU + n*(n - 1); k >= 0; k--, p_k -= n)
				{
					if (pivot[k] != k) { dum = B[k]; B[k] = B[pivot[k]]; B[pivot[k]] = dum; }
					for (i = k + 1; i < n; i++) x[k] -= x[i] * *(p_k + i);
					if (*(p_k + k) == 0.0f) return false;
				}

				return true;
			}

			/////////////////////////////////////////////////////////////////////////////////

			template < typename T, const size_t S>
			static bool CroutMethod(const matn<T, S> &A, const vml::math::vecn<T, S> &B, vml::math::vecn<T, S> &x)
			{
				int *pivot = new int[S];
				matn<T, S> mirror(A);
				T *m = mirror.GetData();

				if (CroutLUDecompositionWithPivoting<T, S>(m, pivot, S))
				{
					if (CroutLUWithPivotingSolve<T, S>(m, B, pivot, x, S))
					{
						delete pivot;
						return true;
					}
				}

				delete pivot;
				return true;
			}

			/////////////////////////////////////////////////////////////////////////////////
			// This function uses Doolittle's method to decompose the n x n matrix A
			// into a unit lower triangular matrix L and an upper triangular matrix U
			// such that A = LU.
			// the function uses partial pivoting

			template < typename T, const size_t S>
			static bool DoolittleLUDecompositionWithPivoting(T *A, int* pivot, const size_t n)
			{
				int i, j, k;
				T *p_k, *p_row, *p_col;
				T max;

				//  For each row and column, k = 0, ..., n-1,

				for (k = 0, p_k = A; k < n; p_k += n, k++)
				{

					// find the pivot row

					pivot[k] = k;
					max = fabs(*(p_k + k));

					p_col = p_k;

					for (j = k + 1, p_row = p_k + n; j < n; j++, p_row += n)
					{
						if (max < fabs(*(p_row + k)))
						{
							max = fabs(*(p_row + k));
							pivot[k] = j;
							p_col = p_row;
						}
					}

					//   and if the pivot row differs from the current row, then
					//   interchange the two rows.

					if (pivot[k] != k)
					{
						for (j = 0; j < n; j++)
						{
							max = *(p_k + j);
							*(p_k + j) = *(p_col + j);
							*(p_col + j) = max;
						}
					}

					//  and if the matrix is singular, return error

					if (*(p_k + k) == (T) 0.0f) return false;

					// otherwise find the lower triangular matrix elements for column k.

					for (i = k + 1, p_row = p_k + n; i < n; p_row += n, i++)
					{
						*(p_row + k) /= *(p_k + k);
					}

					//  update remaining matrix

					for (i = k + 1, p_row = p_k + n; i < n; p_row += n, i++)
						for (j = k + 1; j < n; j++)
							*(p_row + j) -= *(p_row + k) * *(p_k + j);

				}

				return true;
			}

			/////////////////////////////////////////////////////////////////////////////////

			template < typename T, const size_t S >
			static bool DoolittleLUWithPivotingSolve(T *A, T* B, int* pivot, T *x, const size_t n)
			{
				int i, k;
				T *p_k;
				T dum;

				//         Solve the linear equation Lx = B for x, where L is a lower
				//         triangular matrix with an implied 1 along the diagonal.

				for (k = 0, p_k = A; k < n; p_k += n, k++)
				{
					if (pivot[k] != k) {
						dum = B[k]; B[k] = B[pivot[k]]; B[pivot[k]] = dum;
					}
					x[k] = B[k];
					for (i = 0; i < k; i++) x[k] -= x[i] * *(p_k + i);
				}

				//         Solve the linear equation Ux = y, where y is the solution
				//         obtained above of Lx = B and U is an upper triangular matrix.

				for (k = n - 1, p_k = A + n*(n - 1); k >= 0; k--, p_k -= n)
				{
					if (pivot[k] != k) { dum = B[k]; B[k] = B[pivot[k]]; B[pivot[k]] = dum; }
					for (i = k + 1; i < n; i++) x[k] -= x[i] * *(p_k + i);
					if (*(p_k + k) == 0.0) return false;
					x[k] /= *(p_k + k);
				}
				return true;
			}

			/////////////////////////////////////////////////////////////////////////////////

			template < typename T, const size_t S>
			static bool DoLittleMethod(const matn<T, S> &A, const vml::math::vecn<T, S> &B, vml::math::vecn<T, S> &x)
			{
				int *pivot = new int[S];
				matn<T, S> mirror(A);
				T *m = mirror.GetData();
				if (DoolittleLUDecompositionWithPivoting<T, S>(m, pivot, S))
				{
					if (DoolittleLUWithPivotingSolve<T, S>(m, B, pivot, x, S))
					{
						delete pivot;
						return true;
					}
				}
				delete pivot;
				return false;
			}

			/////////////////////////////////////////////////////////////////////////////////
			// Solve the linear system of equations AX=B where A is an n x n matrix
			// B is an n-dimensional column vector (n x 1 matrix) for the
			// n-dimensional column vector (n x 1 matrix) X.
			// This routine performs partial pivoting and the elements of A are
			// modified during computation.  The result X is returned in B.
			// If the matrix A is singular, the return value of the function call is
			// false,if the solution was found, the function return value is true

			template < typename T, const size_t S>
			static bool GaussSeidelMethod(const matn<T, S> &R, const vml::math::vecn<T, S> &B, vml::math::vecn<T, S> &x)
			{
				int row, i, j, pivot_row;
				T max, dum, *pa, *pA, *A_pivot_row;

				// make a copy of source matrix

				matn<T, S> A(R);

				// copy value for b in x since b values will be overwritten

				for (size_t i = 0; i < S; ++i) x[i] = B.GetConstData()[i];

				// for each variable find pivot row and perform forward substitution

				pa = A.GetData();

				size_t n = A.GetOrder();

				for (row = 0; row < (n - 1); row++, pa += n)
				{

					//  find the pivot row

					A_pivot_row = pa;
					max = fabs(*(pa + row));
					pA = pa + n;
					pivot_row = row;

					for (i = row + 1; i < n; pA += n, i++)
					{
						if ((dum = fabs(*(pA + row))) > max)
						{
							max = dum;
							A_pivot_row = pA;
							pivot_row = i;
						}
					}

					if (max == (T) 0.0f) return false;                // the matrix A is singular

				    // and if it differs from the current row, interchange the two rows.

					if (pivot_row != row)
					{
						for (i = row; i < n; i++)
						{
							dum = *(pa + i);
							*(pa + i) = *(A_pivot_row + i);
							*(A_pivot_row + i) = dum;
						}

						dum = x[row];
						x[row] = x[pivot_row];
						x[pivot_row] = dum;
					}

					// Perform forward substitution

					for (i = row + 1; i < n; i++)
					{
						pA = A.GetData() + i * n;
						dum = -*(pA + row) / *(pa + row);
						*(pA + row) = 0.0;
						for (j = row + 1; j < n; j++) *(pA + j) += dum * *(pa + j);
						x[i] += dum * x[row];
					}
				}

				// Perform backward substitution

				pa = A.GetData() + (n - 1) * n;

				for (row = n - 1; row >= 0; pa -= n, row--)
				{
					if (*(pa + row) == (T)0.0f) return false;           // matrix is singular
					dum = 1.0 / *(pa + row);
					for (i = row + 1; i < n; i++) *(pa + i) *= dum;
					x[row] *= dum;
					for (i = 0, pA = A; i < row; pA += n, i++)
					{
						dum = *(pA + row);
						for (j = row + 1; j < n; j++) *(pA + j) -= dum * *(pa + j);
						x[i] -= dum * x[row];
					}
				}

				return true;
			}

			////////////////////////////////////////////////////////////
			// this function uses the inverse of a n x n matrix non
			// singular matrix A such as Ax=B
			// simplest method, but the most computational demanding

			template < typename T, const  size_t S >
			static bool InverseMethod(const matn<T, S> &A, const vml::math::vecn<T, S> &B, vml::math::vecn<T, S> &x)
			{
				// is matrix singular ?

				matn<T, S> mirror(A);

				T det = mirror.Determinant();

				if ( det>-vml::math::EPSILON && det <vml::math::EPSILON)
					vml::os::Error("Matrix is singular");

				const T *m = mirror.GetConstData();

				size_t n = A.GetOrder();

				for (size_t j = 0; j < n; j++)
				{
					x[j] = (T)0.0f;

					for (size_t i = 0; i < n; i++)
					{
						x[j] += m[i + j*n] * B[i];
					}
				}

				return true;
			}
			
			/////////////////////////////////////////////////////////////
			// this function uses Choleski's method to decompose the n x n positive
			// definite symmetric matrix A into the product of a lower triangular
			// matrix L and an upper triangular matrix U equal to the transpose of L.
			// the functions return true if decomposition went well or false
			// if the matrix isn't symmetric or positively defined

			template < typename T, const size_t S >
			static bool CholeskiLUDecomposition(T *A, const size_t n)
			{
				int i, k, p;
				T *p_Lk0;                   // pointer to L[k][0]
				T *p_Lkp;                   // pointer to L[k][p]
				T *p_Lkk;                   // pointer to diagonal element on row k.
				T *p_Li0;                   // pointer to L[i][0]
				T reciprocal;

				for (k = 0, p_Lk0 = A; k < n; p_Lk0 += n, k++)
				{

					//            Update pointer to row k diagonal element.

					p_Lkk = p_Lk0 + k;

					//            Calculate the difference of the diagonal element in row k
					//            from the sum of squares of elements row k from column 0 to
					//            column k-1.

					for (p = 0, p_Lkp = p_Lk0; p < k; p_Lkp += 1, p++)
						*p_Lkk -= *p_Lkp * *p_Lkp;

					//            If diagonal element is not positive, return the error code,
					//            the matrix is not positive definite symmetric.

					if (*p_Lkk <= (T) 0.0f)
					{
						vml::os::Error("Matrix is not positive defined");
						return false;
					}
					//            Otherwise take the square root of the diagonal element.

					*p_Lkk = sqrt(*p_Lkk);
					reciprocal = 1.0 / *p_Lkk;

					//            For rows i = k+1 to n-1, column k, calculate the difference
					//            between the i,k th element and the inner product of the first
					//            k-1 columns of row i and row k, then divide the difference by
					//            the diagonal element in row k.
					//            Store the transposed element in the upper triangular matrix.

					p_Li0 = p_Lk0 + n;
	
					for (i = k + 1; i < n; p_Li0 += n, i++)
					{
						for (p = 0; p < k; p++)
							*(p_Li0 + k) -= *(p_Li0 + p) * *(p_Lk0 + p);
					
						*(p_Li0 + k) *= reciprocal;
						*(p_Lk0 + i) = *(p_Li0 + k);
					}
				}

				return true;
			}

			// solves a lower triangular matrix

			template < typename T, const size_t S >
			static bool LowerTriangularSolve(T *L, T* B, T *x, int n)
			{
				int i, k;

				//  Solve the linear equation Lx = B for x, where L is a lower
				//  triangular matrix.

				for (k = 0; k < n; L += n, k++)
				{
					if (*(L + k) == (T) 0.0f)
					{
						vml::os::Error("Matrix is singular");
						return false;           // The matrix L is singular
					}

					x[k] = B[k];
					for (i = 0; i < k; i++) x[k] -= x[i] * *(L + i);
					x[k] /= *(L + k);
				}

				return true;
			}

			// solves and upper triangular matrix

			template < typename T, const size_t S >
			static bool UpperTriangularSolve(T* U, T* B, T* x, const size_t n)
			{
				int i, k;

				//         Solve the linear equation Ux = B for x, where U is an upper
				//         triangular matrix.

				for (k = n - 1, U += n * (n - 1); k >= 0; U -= n, k--)
				{
					if (*(U + k) == (T) 0.0f)
					{
						vml::os::Error("Matrix is singular");
						return false;           // The matrix U is singular
					}
					x[k] = B[k];
					for (i = k + 1; i < n; i++) x[k] -= x[i] * *(U + i);
						x[k] /= *(U + k);
				}

				return true;
			}
			
			//////////////////////////////////////////////////////////////////////////////////////

			template < typename T,const size_t S >
			static bool CholeskiLUSolve(T *LU, T* B, T* x, int n)
			{

				//         Solve the linear equation Ly = B for y, where L is a lower
				//         triangular matrix.

				if (!LowerTriangularSolve<T,S>(LU, B, x, n)) return false;

				//         Solve the linear equation Ux = y, where y is the solution
				//         obtained above of Ly = B and U is an upper triangular matrix.

				return UpperTriangularSolve<T,S>(LU, x, x, n);

			}

			//////////////////////////////////////////////////////////////////////////////////////

			template < typename T,const size_t S >
			static bool CholeskiMethod(const matn<T, S> &R, const vml::math::vecn<T, S> &B, vml::math::vecn<T, S> &x)
			{
				matn<T, S> A(R);

				const T *m = A.GetData();
				size_t n = R.GetOrder();
				size_t i, j;
			
				// checks if matrix is simmetric

				for ( j = 0; j < n; ++j)
				 for (i = j; i < n; ++i)
				  if (m[i + j*n] != m[j + i*n])
					{
					  vml::os::Error("Matrix is not simmetric");
						return false;
					}
				
				// decompose matrix 

				if (!CholeskiLUDecomposition<T,S>(m, n))
					return false;

				// solve matrix

				if (!CholeskiLUSolve<T,S>(m, B, x, n))
					return false;

				return true;
			}

		} // end of namespace matrixsolvers
	}	// end of namespace math
}	// end of namespace vml
