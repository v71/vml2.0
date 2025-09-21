#pragma once

namespace vml
{
	namespace geo2d
	{
			class BayazitDecomposer
			{

				private:

					//----------------------------------------------------------

					static const unsigned int MAX_VERTICES = 64;

					//----------------------------------------------------------

					bool LeftOn(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
					{
						return vml::geo2d::metrics::SignedTriangleArea(a, b, c) >= 0;
					}

					//----------------------------------------------------------
					
					bool Right(int i, const std::vector<glm::vec2>& vertices)
					{
						return Right(At(i - 1, vertices), At(i, vertices), At(i + 1, vertices));
					}
					
					//----------------------------------------------------------

					bool Right(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
					{
						return vml::geo2d::metrics::SignedTriangleArea(a, b, c) < 0;
					}

					//----------------------------------------------------------

					bool RightOn(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
					{
						return vml::geo2d::metrics::SignedTriangleArea(a, b, c) <= 0;
					}

					//----------------------------------------------------------

					bool Left(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
					{
						return vml::geo2d::metrics::SignedTriangleArea(a, b, c) > 0;
					}

					//----------------------------------------------------------

					bool Reflex(int i, const std::vector<glm::vec2>& vertices)
					{
						return Right(i, vertices);
					}

					//----------------------------------------------------------

					glm::vec2 At(int i, const std::vector<glm::vec2>& vertices)
					{
						int s = (int)vertices.size();
						int idx = i < 0 ? s - 1 - ((-i - 1) % s) : i % s;
						return vertices[ idx ];
					}

					//----------------------------------------------------------

					float SquareDist(const glm::vec2& a, const glm::vec2& b)
					{
						float dx = b.x - a.x;
						float dy = b.y - a.y;
						return dx * dx + dy * dy;
					}

					//----------------------------------------------------------

					bool FloatEquals(float value1, float value2)
					{
						return fabs(value1 - value2) <= vml::math::EPSILON;
					}

					//----------------------------------------------------------

					bool CanSee(int i, int j, const std::vector<glm::vec2>& vertices)
					{
						if (Reflex(i, vertices))
						{
							if (LeftOn(At(i, vertices), At(i - 1, vertices), At(j, vertices)) && RightOn(At(i, vertices), At(i + 1, vertices), At(j, vertices)))
								return false;
						}
						else
						{
							if (RightOn(At(i, vertices), At(i + 1, vertices), At(j, vertices)) || LeftOn(At(i, vertices), At(i - 1, vertices), At(j, vertices)))
								return false;
						}
						if (Reflex(j, vertices))
						{
							if (LeftOn(At(j, vertices), At(j - 1, vertices), At(i, vertices)) && RightOn(At(j, vertices), At(j + 1, vertices), At(i, vertices)))
								return false;
						}
						else
						{
							if (RightOn(At(j, vertices), At(j + 1, vertices), At(i, vertices)) || LeftOn(At(j, vertices), At(j - 1, vertices), At(i, vertices)))
								return false;
						}
						for (int k = 0; k < (int)vertices.size(); ++k)
						{
							if ((k + 1) % (int)vertices.size() == i || k == i || (k + 1) % (int)vertices.size() == j || k == j)
								continue; // ignore incident edges

							glm::vec2 intersectionpoint;

							if (LineIntersect(At(i, vertices), At(j, vertices), At(k, vertices), At(k + 1, vertices), true,true,intersectionpoint))
								return false;
						}
						return true;
					}

					//----------------------------------------------------------

					void Copy(int i, int j, const  std::vector<glm::vec2>& vertices, std::vector<glm::vec2>& to)
					{
						to.clear();
						while (j < i) j += (int)vertices.size();
						for (; i <= j; ++i) to.push_back(At(i, vertices));
					}

					//----------------------------------------------------------

					glm::vec2 LineIntersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2)
					{
						glm::vec2 i(0,0);
						float a1 = p2.y - p1.y;
						float b1 = p1.x - p2.x;
						float c1 = a1 * p1.x + b1 * p1.y;
						float a2 = q2.y - q1.y;
						float b2 = q1.x - q2.x;
						float c2 = a2 * q1.x + b2 * q1.y;
						float det = a1 * b2 - a2 * b1;
						if (!FloatEquals(det, 0)) 
						{
							// lines are not parallel
							i.x = (b2 * c1 - b1 * c2) / det;
							i.y = (a1 * c2 - a2 * c1) / det;
						}
						return i;
					}
					
					//----------------------------------------------------------

					bool LineIntersect(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec2& point4, bool firstIsSegment, bool secondIsSegment, glm::vec2& point)
					{
						point.x = 0;
						point.y = 0;

						// these are reused later.
						// each lettered sub-calculation is used twice, except
						// for b and d, which are used 3 times
						float a = point4.y - point3.y;
						float b = point2.x - point1.x;
						float c = point4.x - point3.x;
						float d = point2.y - point1.y;

						// denominator to solution of linear system
						float denom = (a * b) - (c * d);

						// if denominator is 0, then lines are parallel
						if (!(denom >= -vml::math::EPSILON && denom <= vml::math::EPSILON))
						{
							float e = point1.y - point3.y;
							float f = point1.x - point3.x;
							float oneOverDenom = 1.0f / denom;

							// numerator of first equation
							float ua = (c * e) - (a * f);
							ua *= oneOverDenom;

							// check if intersection point of the two lines is on line segment 1
							if (!firstIsSegment || ua >= 0.0f && ua <= 1.0f)
							{
								// numerator of second equation
								float ub = (b * e) - (d * f);
								ub *= oneOverDenom;

								// check if intersection point of the two lines is on line segment 2
								// means the line segments intersect, since we know it is on
								// segment 1 as well.
								if (!secondIsSegment || ub >= 0.0f && ub <= 1.0f)
								{
									// check if they are coincident (no collision in this case)
									if (ua != 0.0f || ub != 0.0f)
									{
										//There is an intersection
										point.x = point1.x + ua * b;
										point.y = point1.y + ua * d;
										return true;
									}
								}
							}
						}

						return false;
					}

				public:

					//----------------------------------------------------------

					std::vector<std::vector<glm::vec2>> Decompose(const std::vector<glm::vec2>& vertices)
					{
						std::vector<std::vector<glm::vec2>> list;
						glm::vec2 lowerInt, upperInt;
						int lowerIndex = 0, upperIndex = 0;
						std::vector<glm::vec2> lowerPoly, upperPoly;

						for (int i = 0; i < (int)vertices.size(); ++i)
						{
							if (Reflex(i, vertices))
							{
								float upperDist;
								float lowerDist = upperDist = std::numeric_limits<float>().max();
								for (int j = 0; j < (int)vertices.size(); ++j)
								{
									// if line intersects with an edge
									float d;
									glm::vec2 p;
									if (Left(At(i - 1, vertices), At(i, vertices), At(j, vertices)) && RightOn(At(i - 1, vertices), At(i, vertices), At(j - 1, vertices)))
									{
										// find the point of intersection
										p = LineIntersect(At(i - 1, vertices), At(i, vertices), At(j, vertices), At(j - 1, vertices));

										if (Right(At(i + 1, vertices), At(i, vertices), p))
										{
											// make sure it's inside the poly
											d = SquareDist(At(i, vertices), p);
											if (d < lowerDist)
											{
												// keep only the closest intersection
												lowerDist = d;
												lowerInt = p;
												lowerIndex = j;
											}
										}
									}

									if (Left(At(i + 1, vertices), At(i, vertices), At(j + 1, vertices)) && RightOn(At(i + 1, vertices), At(i, vertices), At(j, vertices)))
									{
										p = LineIntersect(At(i + 1, vertices), At(i, vertices), At(j, vertices), At(j + 1, vertices));

										if (Left(At(i - 1, vertices), At(i, vertices), p))
										{
											d = SquareDist(At(i, vertices), p);
											if (d < upperDist)
											{
												upperDist = d;
												upperIndex = j;
												upperInt = p;
											}
										}
									}
								}

								// if there are no vertices to connect to, choose a point in the middle
								if (lowerIndex == (upperIndex + 1) % (int)vertices.size())
								{
									glm::vec2 p = ((lowerInt + upperInt) / 2.0f);

									Copy(i, upperIndex, vertices, lowerPoly);
									lowerPoly.push_back(p);
									Copy(lowerIndex, i, vertices, upperPoly);
									upperPoly.push_back(p);
								}
								else
								{
									double highestScore = 0, bestIndex = lowerIndex;
									while (upperIndex < lowerIndex)
										upperIndex += (int)vertices.size();

									for (int j = lowerIndex; j <= upperIndex; ++j)
									{
										if (CanSee(i, j, vertices))
										{
											double score = 1.0f / (SquareDist(At(i, vertices), At(j, vertices)) + 1);
											if (Reflex(j, vertices))
											{
												if (RightOn(At(j - 1, vertices), At(j, vertices), At(i, vertices)) && LeftOn(At(j + 1, vertices), At(j, vertices), At(i, vertices)))
													score += 3;
												else
													score += 2;
											}
											else
											{
												score += 1;
											}
											if (score > highestScore)
											{
												bestIndex = j;
												highestScore = score;
											}
										}
									}
									Copy(i, (int)bestIndex, vertices, lowerPoly);
									Copy((int)bestIndex, i, vertices, upperPoly);
								}

								std::vector<std::vector<glm::vec2>> lower = Decompose(lowerPoly);

								for (std::vector<glm::vec2> p : lower)
									list.push_back(p);

								std::vector<std::vector<glm::vec2>> upper = Decompose(upperPoly);

								for (std::vector<glm::vec2> p : upper)
									list.push_back(p);

								return list;
							}
						}

						// polygon is already convex

						if ((int)vertices.size() > MAX_VERTICES)
						{

							Copy(0, (int)vertices.size() / 2, vertices, lowerPoly);
							Copy((int)vertices.size() / 2, 0, vertices, upperPoly);

							std::vector<std::vector<glm::vec2>> lower = Decompose(lowerPoly);

							for (std::vector<glm::vec2> p : lower)
								list.push_back(p);

							std::vector<std::vector<glm::vec2>> upper = Decompose(upperPoly);

							for (std::vector<glm::vec2> p : upper)
								list.push_back(p);
						}
						else
						{
							list.push_back(vertices);
						}

						return list;
					}

					//----------------------------------------------------------

					std::vector< vml::geo2d::Polygon > Go(const std::vector<glm::vec2>& vertices)
					{
						std::vector< std::vector<glm::vec2> > polys;
						std::vector< vml::geo2d::Polygon > polygons;
						polys = Decompose(vertices);
						for (size_t i = 0; i < polys.size(); ++i)
							polygons.emplace_back(vml::geo2d::Polygon(polys[i]));
						return polygons;
					}

					//----------------------------------------------------------
					// ctor / dtor

					BayazitDecomposer()
					{}

					~BayazitDecomposer()
					{}

			};
	} // end of namespace geo2d

} // end of namespace vml
