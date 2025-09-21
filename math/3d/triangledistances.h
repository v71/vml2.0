#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////////////////
			// Closest point to triangle

			static float ClosestPointToTriangle2(const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc, const glm::vec3& x, glm::vec3& pt, glm::vec2& t, const float eps = vml::math::EPSILON)
			{
				// source: real time collision detection
				// check if x in vertex region outside pa

				glm::vec3 ab = pb - pa;
				glm::vec3 ac = pc - pa;
				glm::vec3 ax = x - pa;

				float d1 = glm::dot(ab, ax);
				float d2 = glm::dot(ac, ax);

				if (d1 <-eps && d2 <-eps)
				{
					// barycentric coordinates (1, 0, 0)

					t[0] = 1.0f;
					t[1] = 0.0f;
					pt = pa;

					return glm::length(x - pt);
				}

				// check if x in vertex region outside pb

				glm::vec3 bx = x - pb;

				float d3 = glm::dot(ab, bx);
				float d4 = glm::dot(ac, bx);

				if (d3 > eps && d4 <= d3)
				{

					// barycentric coordinates (0, 1, 0)

					t[0] = 0.0f;
					t[1] = 1.0f;
					pt = pb;

					return glm::length(x - pt);
				}

				// check if x in vertex region outside pc

				glm::vec3 cx = x - pc;

				float d5 = glm::dot(ab, cx);
				float d6 = glm::dot(ac, cx);

				if (d6 > eps && d5 <= d6)
				{

					// barycentric coordinates (0, 0, 1)

					t[0] = 0.0f;
					t[1] = 0.0f;
					pt = pc;

					return glm::length(x - pt);
				}

				// check if x in edge region of ab, if so return projection of x onto ab

				float vc = d1 * d4 - d3 * d2;

				if (vc < -eps && d1 > eps && d3 < -eps)
				{
					// barycentric coordinates (1 - v, v, 0)

					float v = d1 / (d1 - d3);

					t[0] = 1.0f - v;
					t[1] = v;
					pt = pa + ab * v;

					return glm::length(x - pt);
				}

				// check if x in edge region of ac, if so return projection of x onto ac

				float vb = d5 * d2 - d1 * d6;

				if (vb < -eps && d2 > eps && d6 < -eps)
				{
					// barycentric coordinates (1 - w, 0, w)

					float w = d2 / (d2 - d6);

					t[0] = 1.0f - w;
					t[1] = 0.0f;
					pt = pa + ac * w;

					return glm::length(x - pt);
				}


				// check if x in edge region of bc, if so return projection of x onto bc

				float va = d3 * d6 - d5 * d4;

				if (va < -eps && (d4 - d3) > eps && (d5 - d6) > eps)
				{
					// barycentric coordinates (0, 1 - w, w)

					float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));

					t[0] = 0.0f;
					t[1] = 1.0f - w;
					pt = pb + (pc - pb) * w;

					return glm::length(x - pt);
				}

				// x inside face region. Compute pt through its barycentric coordinates (u, v, w)

				float denom = 1.0f / (va + vb + vc);
				float v = vb * denom;
				float w = vc * denom;
				t[0] = 1.0f - v - w;
				t[1] = v;

				pt = pa + ab * v + ac * w; //= u*a + v*b + w*c, u = va*denom = 1.0f - v - w

				return glm::length(x - pt);
			}

			/////////////////////////////////////////////////////////////////////////////////////////
			// Closest point to triangle
			// same as above but without the uv baricentric out parameters

			static glm::vec3 ClosestPointToTriangle2(const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc, const glm::vec3& x, const float eps = vml::math::EPSILON)
			{
				// source: real time collision detection
				// check if x in vertex region outside pa

				glm::vec3 ab = pb - pa;
				glm::vec3 ac = pc - pa;
				glm::vec3 ax = x - pa;

				float d1 = glm::dot(ab, ax);
				float d2 = glm::dot(ac, ax);

				if (d1 < -eps && d2 < -eps)
				{
					// barycentric coordinates (1, 0, 0)

					return pa;
				}

				// check if x in vertex region outside pb

				glm::vec3 bx = x - pb;

				float d3 = glm::dot(ab, bx);
				float d4 = glm::dot(ac, bx);

				if (d3 > eps && d4 <= d3)
				{

					// barycentric coordinates (0, 1, 0)

					return pb;
				}

				// check if x in vertex region outside pc

				glm::vec3 cx = x - pc;

				float d5 = glm::dot(ab, cx);
				float d6 = glm::dot(ac, cx);

				if (d6 > eps && d5 <= d6)
				{

					// barycentric coordinates (0, 0, 1)

					return pc;
				}

				// check if x in edge region of ab, if so return projection of x onto ab

				float vc = d1 * d4 - d3 * d2;

				if (vc < -eps && d1 > eps && d3 <-eps)
				{
					// barycentric coordinates (1 - v, v, 0)

					return pa + ab * (d1 / (d1 - d3));

				}

				// check if x in edge region of ac, if so return projection of x onto ac

				float vb = d5 * d2 - d1 * d6;

				if (vb <-eps && d2 > eps && d6 <-eps)
				{
					// barycentric coordinates (1 - w, 0, w)

					return pa + ac * (d2 / (d2 - d6));

				}

				// check if x in edge region of bc, if so return projection of x onto bc

				float va = d3 * d6 - d5 * d4;

				if (va < -eps && (d4 - d3) > eps && (d5 - d6) > eps)
				{
					// barycentric coordinates (0, 1 - w, w)

					return pb + (pc - pb) * ((d4 - d3) / ((d4 - d3) + (d5 - d6)));

				}

				// x inside face region. Compute pt through its barycentric coordinates (u, v, w)

				float denom = 1.0f / (va + vb + vc);
				float v = vb * denom;
				float w = vc * denom;

				//= u*a + v*b + w*c, u = va*denom = 1.0f - v - w

				return pa + ab * v + ac * w; 

			}

			////////////////////////////////////////////////////////////////////////////////
			// Nearest point on triangle v0,v1,v2, from point 

			static glm::vec3 ClosestPointToTriangle(const glm::vec3& point, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,const float eps=vml::math::EPSILON)
			{
				glm::vec3 diff  =  point - v0;
				glm::vec3 edge0 =  v1 - v0;
				glm::vec3 edge1 =  v2 - v0;
				float a00       =  glm::dot(edge0, edge0);
				float a01       =  glm::dot(edge0, edge1);
				float a11       =  glm::dot(edge1, edge1);
				float b0        = -glm::dot(diff, edge0);
				float b1        = -glm::dot(diff, edge1);
				float zero      =      -eps;
				float one       =  1.0f+eps;
				float det       =  a00 * a11 - a01 * a01;
				float t0        =  a01 * b1  - a11 * b0;
				float t1        =  a01 * b0  - a00 * b1;

				if (t0 + t1 <= det)
				{
					if (t0 < zero)
					{
						if (t1 < zero)  // region 4
						{
							if (b0 < zero)
							{
								t1 = zero;
								if (-b0 >= a00)  // V1
								{
									t0 = one;
								}
								else  // E01
								{
									t0 = -b0 / a00;
								}
							}
							else
							{
								t0 = zero;
								if (b1 >= zero)  // V0
								{
									t1 = zero;
								}
								else if (-b1 >= a11)  // V2
								{
									t1 = one;
								}
								else  // E20
								{
									t1 = -b1 / a11;
								}
							}
						}
						else  // region 3
						{
							t0 = zero;
							if (b1 >= zero)  // V0
							{
								t1 = zero;
							}
							else if (-b1 >= a11)  // V2
							{
								t1 = one;
							}
							else  // E20
							{
								t1 = -b1 / a11;
							}
						}
					}
					else if (t1 < zero)  // region 5
					{
						t1 = zero;
						if (b0 >= zero)  // V0
						{
							t0 = zero;
						}
						else if (-b0 >= a00)  // V1
						{
							t0 = one;
						}
						else  // E01
						{
							t0 = -b0 / a00;
						}
					}
					else  // region 0, interior
					{
						float invDet = one / det;
						t0 *= invDet;
						t1 *= invDet;
					}
				}
				else
				{
					float tmp0, tmp1, numer, denom;

					if (t0 < zero)  // region 2
					{
						tmp0 = a01 + b0;
						tmp1 = a11 + b1;
						if (tmp1 > tmp0)
						{
							numer = tmp1 - tmp0;
							denom = a00 - (float)2 * a01 + a11;
							if (numer >= denom)  // V1
							{
								t0 = one;
								t1 = zero;
							}
							else  // E12
							{
								t0 = numer / denom;
								t1 = one - t0;
							}
						}
						else
						{
							t0 = zero;
							if (tmp1 <= zero)  // V2
							{
								t1 = one;
							}
							else if (b1 >= zero)  // V0
							{
								t1 = zero;
							}
							else  // E20
							{
								t1 = -b1 / a11;
							}
						}
					}
					else if (t1 < zero)  // region 6
					{
						tmp0 = a01 + b1;
						tmp1 = a00 + b0;
						if (tmp1 > tmp0)
						{
							numer = tmp1 - tmp0;
							denom = a00 - (float)2 * a01 + a11;
							if (numer >= denom)  // V2
							{
								t1 = one;
								t0 = zero;
							}
							else  // E12
							{
								t1 = numer / denom;
								t0 = one - t1;
							}
						}
						else
						{
							t1 = zero;
							if (tmp1 <= zero)  // V1
							{
								t0 = one;
							}
							else if (b0 >= zero)  // V0
							{
								t0 = zero;
							}
							else  // E01
							{
								t0 = -b0 / a00;
							}
						}
					}
					else  // region 1
					{
						numer = a11 + b1 - a01 - b0;
						if (numer <= zero)  // V2
						{
							t0 = zero;
							t1 = one;
						}
						else
						{
							denom = a00 - (float)2 * a01 + a11;
							if (numer >= denom)  // V1
							{
								t0 = one;
								t1 = zero;
							}
							else  // 12
							{
								t0 = numer / denom;
								t1 = one - t0;
							}
						}
					}
				}

				// paraqmetric coordiantes

				// u = one - t0 - t1;
				// v = t0;
				// w = t1;
				// distance = lenght of	point - result.closest

				return v0 + t0 * edge0 + t1 * edge1;
			}

			////////////////////////////////////////////////////////////////////////////////
			// Compute barycentric coordinates (u, v, w) for
			// point p with respect to triangle (a, b, c)

			static void Barycentric(const glm::vec3 &p,
									const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c,
									float &u, float &v, float &w)
			{
				glm::vec3 v0 = b - a;
				glm::vec3 v1 = c - a;
				glm::vec3 v2 = p - a;
				float d00 = glm::dot(v0, v0);
				float d01 = glm::dot(v0, v1);
				float d11 = glm::dot(v1, v1);
				float d20 = glm::dot(v2, v0);
				float d21 = glm::dot(v2, v1);
				float denom = 1.0f / (d00 * d11 - d01 * d01);
				v = (d11 * d20 - d01 * d21) * denom;
				w = (d00 * d21 - d01 * d20) * denom;
				u = 1.0f - v - w;
			}

		}

	}

}
