#pragma once


namespace vml
{
	namespace geo2d
	{

		namespace hulls
		{
			///////////////////////////////////////////////////////////////////////////
			// 2d convex hull computation using the quick hull 

			class QuickHull
			{

				private:

					// ----------------------------------------------------------------------------
					// compute angle beetween vectors

					float Angle(const float p0x, const float p0y,
								const float p1x, const float p1y,
								const float p2x, const float p2y)
					{
						// "Angle" between P0->P1 and P0->P2.
						// actually returns: ||(P1-P0) ^ (P2-P0)||
						return (p2y - p1y) * (p1x - p0x) - (p2x - p1x) * (p1y - p0y);
					}

					// ----------------------------------------------------------------------------
					// Quick Hull algorithm

					int QHullInternal(int N, const vml::geo2d::VertexArray& points, int* ind, int iA, int iB, float eps = vml::math::EPSILON)
					{
						int n, nn, n2, m, Split1, Split2, iMid;
						int Mid_Pt;
						float dMax;

						if (N <= 2) return N;

						// As middle point, search the farthest away from line [A-->B]

						Mid_Pt = -1;
						dMax = 0.0f;
						float d;

						for (n = 1; n < N; ++n)
						{
							d = fabs(Angle(points[ind[n]].x, points[ind[n]].y,
								points[iA].x, points[iA].y,
								points[iB].x, points[iB].y));

							if (d >= dMax) { dMax = d; Mid_Pt = n; }
						}

						// Partition against midpoint

						iMid = ind[Mid_Pt];
						ind[Mid_Pt] = ind[N - 1];

						//Ind = [A|...........|xxx] (B)
						//  n = [0|1.......N-2|N-1|

						nn = N - 2;
						n = 1;

						while (n <= nn)
						{
							d = Angle(points[ind[n]].x, points[ind[n]].y,
								points[iA].x, points[iA].y,
								points[iMid].x, points[iMid].y);

							//if (d >= 0.0f)

							if (d > eps)
							{
								int t;
								t = ind[n];
								ind[n] = ind[nn];
								ind[nn] = t;
								nn--;
							}
							else n++;
						}

						ind[N - 1] = ind[n];
						ind[n] = iMid;
						Split1 = n++;

						//Ind = (A) [..(+)..| M |...........] (B)
						//  n =     [1......|Sp1|........N-1]

						nn = N - 1;

						while (n <= nn)
						{
							d = Angle(points[ind[n]].x, points[ind[n]].y,
								points[iMid].x, points[iMid].y,
								points[iB].x, points[iB].y);

							//if (d >= 0.0f)

							if (d > eps)
							{
								int t;
								t = ind[n];
								ind[n] = ind[nn];
								ind[nn] = t;
								nn--;
							}
							else n++;
						}

						Split2 = n;

						// Status:
						//Ind = (A) [....(+)...| M |....(-)....|(trash)......] (B)
						//  N =     [1.........|Sp1|...........|Sp2.......N-1]

						// Recurse each sub-partition

						n = QHullInternal(Split1, points, ind, iA, iMid);
						n2 = QHullInternal(Split2 - Split1, points, ind + Split1, iMid, iB);
						m = Split1;

						while (n2-- > 0)
						{
							int t;
							t = ind[n];
							ind[n] = ind[m];
							ind[m] = t;

							m++; n++;
						}

						return n;
					}

					// ----------------------------------------------------------------------------
					// set partition

					int Partition(const vml::geo2d::VertexArray& points, int* Ind, int start, int end)
					{

						int pivot = Ind[end];
						int bottom = start - 1;
						int top = end;

						bool notdone = true;

						while (notdone)
						{
							while (notdone)
							{
								bottom += 1;

								if (bottom == top)
								{
									notdone = false;
									break;
								}

								if (points[Ind[bottom]].x < points[pivot].x)
								{
									Ind[top] = Ind[bottom];
									break;
								}
							}

							while (notdone)
							{
								top = top - 1;

								if (top == bottom)
								{
									notdone = false;
									break;
								}

								if (points[Ind[top]].x > points[pivot].x)
								{
									Ind[bottom] = Ind[top];
									break;
								}

							}

						}

						Ind[top] = pivot;

						return top;
					}

					// ----------------------------------------------------------------------------
					// sort partition halves

					void QuickSort(const vml::geo2d::VertexArray& points, int* Ind, int start, int end)
					{

						if (start < end)
						{

							int  split = Partition(points, Ind, start, end);

							QuickSort(points, Ind, start, split - 1);

							QuickSort(points, Ind, split + 1, end);

						}

					}

					// ----------------------------------------------------------------------------
					// the quick hull algorithm

					int QuickHull2D(const vml::geo2d::VertexArray& points, int* Ind, int Nb, const float eps = vml::math::EPSILON)
					{
						int n, nn, m, iA, iB;
						float d;

						if (Nb <= 2) return Nb;

						// qsort half space

						QuickSort(points, Ind, 0, Nb - 1);

						// first partitioning: classify points with respect to
						// the line joining the extreme points #0 and #Nb-1

						iA = Ind[0];
						iB = Ind[Nb - 1];

						m = Nb - 2;
						n = 1;

						while (n <= m)
						{
							d = Angle(points[Ind[n]].x, points[Ind[n]].y,
								points[iA].x, points[iA].y,
								points[iB].x, points[iB].y);

							//						if (d >= 0.0f)
							if (d > eps)
							{
								int t;
								t = Ind[n];
								Ind[n] = Ind[m];
								Ind[m] = t;

								m--;
							}
							else n++;
						}

						Ind[Nb - 1] = Ind[n];
						Ind[n] = iB;

						// Partition is now:
						//  Ind = [ A|...(+)....[B]...(-)...|A ]
						//   n  = [0 |1.........[n].........|Nb]
						// We now process the two halves [A..(+)..B] and [B..(-)..A]

						m = QHullInternal(n, points, Ind, iA, iB); // 1st half [A..(+)..B]
						nn = QHullInternal(Nb - n, points, Ind + n, iB, iA); // 2nd half [B..(-)..A]

						while (nn-- > 0)
						{
							int t;
							t = Ind[n];
							Ind[n] = Ind[m];
							Ind[m] = t;

							m++; n++;
						}
						return m;
					}

				public:

					// ----------------------------------------------------------------------------
					// computes convex hull of points using the quick hull algorithm

					[[nodiscard]] vml::geo2d::Polygon Go(const vml::geo2d::VertexArray& points)
					{

						if (points.Size() <= 2)
							vml::os::Error(L"A minimum of 3 points are needed for computing the convex hull");

						// stores indices of convex hull points, they are clockwise ordered

						std::vector<int> Indices;

						// fill indices array

						for (size_t i = 0; i < points.Size(); ++i)
							Indices.push_back((int)i);

						// compute convex hull

						size_t HullPoints = QuickHull2D(points, &Indices[0], (int)points.Size());

						// create the polygon containing the convex hull

						vml::geo2d::Polygon polygon;

						polygon.Begin();

						for (size_t i = 0; i < (size_t)HullPoints; ++i)
							polygon.AddVertex(points[Indices[i]]);

						polygon.End();

						return polygon;

					}

					// ----------------------------------------------------------------------------
					// ctor / dtor

					QuickHull()
					{
					}

					~QuickHull()
					{
					}

			};

		} // end of hulls namespace

	} // end of namespace geo2d

} // end of namespace vml
