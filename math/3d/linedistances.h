#pragma once

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////////////////
			// Returns the squared distance between point c and segment ab

			static float SquaredDistanceFromPointToLine(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
			{
				glm::vec3 ab = b - a;
				glm::vec3 ac = c - a;
				glm::vec3 bc = c - b;
				float e = glm::dot(ac, ab);
				// Handle cases where c projects outside ab
				if (e <= 0.0f) return glm::dot(ac, ac);
				float f = glm::dot(ab, ab);
				if (e >= f) return glm::dot(bc, bc);
				// Handle cases where c projects onto ab
				return glm::dot(ac, ac) - e * e / f;
			}

			/////////////////////////////////////////////////////////////////////////////////////////
			// Closest point to segment

			static void ClosestPointToLine(const glm::vec3 &c, const glm::vec3 &a, const glm::vec3 &b, float &t, glm::vec3 &d)
			{
				glm::vec3 ab = b - a;

				// Project c onto ab, but deferring divide by Dot(ab, ab)

				t = glm::dot(c - a, ab);

				if (t <= 0.0f)
				{
					// c projects outside the [a,b] interval, on the a side; clamp to a

					t = 0.0f;
					d = a;
				}
				else
				{

					float denom = glm::dot(ab, ab); // Always nonnegative since denom = ||ab||∧2

					if (t >= denom)
					{
						// c projects outside the [a,b] interval, on the b side; clamp to b

						t = 1.0f;
						d = b;
					}
					else
					{
						// c projects inside the [a,b] interval; must do deferred divide now

						t = t / denom;

						d = a + t * ab;

					}
				}
			}

			///////////////////////////////////////////////////////////////////////////////////////////
			// shortest distance between 3d lines

			static void LineLineShortestDistance(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &q0, const glm::vec3 &q1,
												 glm::vec3 &pa, glm::vec3 &pb, float &dist)
			{
				glm::vec3   u = p1 - p0;
				glm::vec3   v = q1 - q0;
				glm::vec3   w = p0 - q0;
				float    a = dot(u, u);         // always >= 0
				float    b = dot(u, v);
				float    c = dot(v, v);         // always >= 0
				float    d = glm::dot(u, w);
				float    e = glm::dot(v, w);
				float    D = a * c - b * b;     // always >= 0
				float    sc, sN, sD = D;		// sc = sN / sD, default sD = D >= 0
				float    tc, tN, tD = D;		// tc = tN / tD, default tD = D >= 0

				// compute the line parameters of the two closest points

				if (D < vml::math::EPSILON)
				{
					// the lines are almost parallel

					sN = 0.0f;         // force using point P0 on segment S1
					sD = 1.0f;         // to prevent possible division by 0.0 later
					tN = e;
					tD = c;

				}
				else
				{
					// get the closest points on the infinite lines

					sN = (b*e - c * d);
					tN = (a*e - b * d);

					if (sN < 0.0f)
					{
						// sc < 0 => the s=0 edge is visible

						sN = 0.0f;
						tN = e;
						tD = c;

					}
					else if (sN > sD)
					{
						// sc > 1  => the s=1 edge is visible

						sN = sD;
						tN = e + b;
						tD = c;

					}

				}

				if (tN < 0.0f)
				{
					// tc < 0 => the t=0 edge is visible

					tN = 0.0f;

					// recompute sc for this edge

					if (-d < 0.0f)
					{
						sN = 0.0f;
					}
					else if (-d > a)
					{
						sN = sD;
					}
					else
					{
						sN = -d;
						sD = a;
					}
				}
				else if (tN > tD)
				{
					// tc > 1  => the t=1 edge is visible

					tN = tD;

					// recompute sc for this edge

					if ((-d + b) < 0.0f)
					{
						sN = 0.0f;
					}
					else if ((-d + b) > a)
					{
						sN = sD;
					}
					else
					{
						sN = (-d + b);
						sD = a;
					}
				}

				// finally do the division to get sc and tc

				sc = (abs(sN) < vml::math::EPSILON ? 0.0f : sN / sD);
				tc = (abs(tN) < vml::math::EPSILON ? 0.0f : tN / tD);

				// get the difference of the two closest points

				dist = glm::length(w + (sc * u) - (tc * v));

				pa = p0 + (sc * u);
				pb = q0 + (tc * v);

			}

		}	// end of distances namespace

	}	// end of geo3d namespace

}	// end of vml namespace
