#pragma once

///////////////////////////////////////////////////////////////////////////////////////
// signed distances

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////
			// Signed distance from point to  line
			// assumes line normal is normalized

			static float SignedDistanceFromPointToNormalizedRay(const glm::vec2& p, const glm::vec2& p0, const glm::vec2& normal)
			{
				return (glm::dot(p - p0, normal));
			}

			/////////////////////////////////////////////////////////////////////////////
			// signed distance from circle to circlae

			static float SignedDistanceFromCircleToCircle(const glm::vec2 &a,const float ra, const glm::vec2 &b,const float rb)
			{
				glm::vec2 c = b - a;
				float r0 = ra + rb;
				return c.x * c.x + c.y * c.y - r0 * r0;
			}

			/////////////////////////////////////////////////////////////////////////////
			// signed distance from circle to point

			static float SignedDistanceFromCircleToPoint(const glm::vec2& p, const glm::vec2& c, const float r, const float eps = vml::math::EPSILON)
			{
				glm::vec2 d = c-p;
				return sqrtf(d.x * d.x + d.y * d.y) - r;
			}

		}  // end of distances namespace
	} // end of geo2d namespace
} // end of vml namepsace

///////////////////////////////////////////////////////////////////////////////////////
// squared distances

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////
			// get distance from point

			static float SquaredDistanceFromPoints(const glm::vec2 &p0,const glm::vec2 &p1)
			{
				glm::vec2 d=p1 - p0;
				return d.x * d.x + d.y * d.y;
			}

			/////////////////////////////////////////////////////////////////////////////
			// minimum distance between lines
			// return values 
			// COLLINEAR		   : lines are collinear there are infinte contact points 
			// DOES_INTERSECTS	   : lines are intersectiong 
			// DOES_NOT_INTERSECTS : lines are parallele and there is a minimum distance 

			static unsigned int SquaredDistanceBetweenLines(const glm::vec2 &a, const glm::vec2 &b, 
														    const glm::vec2 &c, const glm::vec2 &d, 
													        glm::vec2 &qstart, glm::vec2 &qend, float &mindist, 
															const float eps = vml::math::EPSILON)
			{
				float denom = (c.y - d.y)*(b.x - a.x) - (c.x - d.x)*(b.y - a.y);
				float numa  = (c.x - d.x)*(a.y - d.y) - (c.y - d.y)*(a.x - d.x);
				float numb  = (b.x - a.x)*(a.y - d.y) - (b.y - a.y)*(a.x - d.x);

				if ((denom > -eps && denom < eps))
				{
					// lines are collinear if num a is equal to num b
					// added an epsilon range check
					// there are infinite points which are a t the same distance

					if ((-eps < numa && numa < eps) && (-eps < numb && numb < eps))
					{
						mindist = 0.0f;
						return vml::geo2d::Results::COLLINEAR;
					}
				}

				// compute intersection point

				float invdenom = 1.0f / denom;

				float ua = numa * invdenom;
				float ub = numb * invdenom;

				mindist = FLT_MAX;

				if (ua > -eps && ua <= 1.0f + eps && ub > -eps && ub < 1.0f + eps)
				{
					// Get the intersection point.

					qstart = a + ua * (b - a);
					qend = qstart;
					mindist = 0.0f;

					return vml::geo2d::Results::DOES_INTERSECT;
				}

				float dist;

				// extrmal point distance from a-b line to c-d 

				glm::vec2 n = glm::vec2(b.y - a.y, a.x - b.x);

				denom = n.y * n.y + n.x * n.x;

				if (denom < -eps || denom > eps)
				{
					invdenom = 1.0f / denom;

					ua = (n.x*(c.y - a.y) + n.y*(a.x - c.x)) *invdenom;

					if (ua > -eps && ua < 1.0f + eps)
					{
						// Get the intersection point.

						glm::vec2 q = a + ua * (b - a);

						glm::vec2 delta = c - q;

						dist = delta.x*delta.x + delta.y*delta.y;

						if (dist < mindist) { mindist = dist; qstart = q; qend = c; }

					}

					if (ua < -eps)
					{

						glm::vec2 delta = c - a;

						dist = delta.x*delta.x + delta.y*delta.y;

						if (dist < mindist) { mindist = dist; qstart = a; qend = c; }

					}

					if (ua > 1.0f + eps)
					{
						glm::vec2 delta = c - b;

						dist = delta.x*delta.x + delta.y*delta.y;

						if (dist < mindist) { mindist = dist; qstart = b; qend = c; }

					}

					//

					ua = (n.x*(d.y - a.y) + n.y*(a.x - d.x)) * invdenom;

					if (ua > -eps && ua < 1.0f + eps)
					{
						// Get the intersection point.

						glm::vec2 q = a + ua * (b - a);

						glm::vec2 delta = d - q;

						dist = delta.x*delta.x + delta.y*delta.y;

						if (dist < mindist) { mindist = dist; qstart = q; qend = d; }

					}

					if (ua < -eps)
					{
						glm::vec2 delta = d - a;

						dist = delta.x*delta.x + delta.y*delta.y;

						if (dist < mindist) { mindist = dist; qstart = a; qend = d; }

					}

					if (ua > 1.0f + eps)
					{
						glm::vec2 delta = d - b;

						dist = delta.x*delta.x + delta.y*delta.y;

						if (dist < mindist) { mindist = dist; qstart = b; qend = d; }

					}

				}

				// incident points from line c-d to a-b

				glm::vec2 s = glm::vec2(d.y - c.y, c.x - d.x);

				denom = s.x*s.x + s.y*s.y;

				if (denom < -eps || denom > eps)
				{

					invdenom = 1.0f / denom;

					ub = (s.x*(d.y - a.y) + s.y*(a.x - d.x)) * invdenom;

					if (ub >= -eps && ub <= 1.0f + eps)
					{
						// Get the intersection point.

						ua = ((c.x - d.x)*(a.y - d.y) - (c.y - d.y)*(a.x - d.x)) * invdenom;

						glm::vec2 q = a - ua * s;

						glm::vec2 delta = a - q;

						dist = delta.x*delta.x + delta.y*delta.y;

						if (dist < mindist) { mindist = dist; qstart = a; qend = q; }

					}

					//

					ub = (s.x * (d.y - b.y) + s.y * (b.x - d.x)) * invdenom;

					if (ub >= -eps && ub <= 1.0f + eps)
					{
						// Get the intersection point.

						ua = ((c.x - d.x)*(b.y - d.y) - (c.y - d.y)*(b.x - d.x)) * invdenom;

						glm::vec2 q = b - ua * s;

						glm::vec2 delta = b - q;

						dist = delta.x*delta.x + delta.y*delta.y;

						if (dist < mindist) { mindist = dist; qstart = b; qend = q; }

					}

				}

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// distance from point p to line a-b
			
			static float SquaredDistanceFromPointToLine(const glm::vec2 &a, const glm::vec2 &b,  
				                                        const glm::vec2 &p,
														const float eps = vml::math::EPSILON)
			{
				glm::vec2 ab = b - a;
				
				// Project c onto ab, computing parameterized position d(t) = a + t*(b – a)
				
				float t = glm::dot(p - a, ab) / (ab.x * ab.x + ab.y * ab.y);
				
				// If outside segment, clamp t (and therefore d) to the closest endpoint
				
				if (t < -vml::math::EPSILON)
					t = 0.0f;
			
				if (t > 1 + vml::math::EPSILON)
					t = 1.0f;
				
				// Compute projected position from the clamped t
				
				glm::vec2 d = p - (a + t * ab);

				return d.x * d.x + d.y * d.y;
				
			}

			/////////////////////////////////////////////////////////////////////////////
			// squared distance from circle to point

			static float SquaredDistanceFromCircleToPoint(const glm::vec2& p, const glm::vec2& c, const float r, const float eps = vml::math::EPSILON)
			{
				glm::vec2 d = c - p;
				float dist=sqrtf(d.x * d.x + d.y * d.y) - r;
				return dist * dist;
			}


		}  // end of squared distances namespace
	} // end of geo2d namespace
} // end of vml namepsace

///////////////////////////////////////////////////////////////////////////////////////
// closest distances

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q on a line that is closest  to p

			static glm::vec2 ClosestPointToLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p,const float &eps=vml::math::EPSILON)
			{
				glm::vec2 ab = p2 - p1;
				glm::vec2 ap = p - p1;
				float denum = ab.x * ab.x + ab.y * ab.y;
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;
				float t = (ap.x * ab.x + ap.y * ab.y) / denum;
				if (t < -vml::math::EPSILON)
					return p1;
				if (t > 1 + vml::math::EPSILON)
					return p2;
				return p1 + t * ab;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given a line with p1 and p2 points, and a cricle p with radius r
			// computes the closest point from circle to line

			static void ClosestPointFromLineToCircle(const glm::vec2& p1, const glm::vec2& p2,
													 const glm::vec2& p, float r,
													 glm::vec2& c1, glm::vec2& c2,
													 const float& eps = vml::math::EPSILON)
			{
				glm::vec2 ab = p2 - p1;
				glm::vec2 ap = p - p1;
				float denum = ab.x * ab.x + ab.y * ab.y;
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;
				float t = (ap.x * ab.x + ap.y * ab.y) / denum;
				glm::vec2 closest(FLT_MAX, FLT_MAX);
				if (t < -vml::math::EPSILON)
					closest = p1;
				if (t > 1 + vml::math::EPSILON)
					closest = p2;
				if (t > -vml::math::EPSILON && t < 1 + vml::math::EPSILON)
					closest = p1 + t * ab;
				glm::vec2 d = closest - p;
				t = r / sqrtf(d.x * d.x + d.y * d.y);
				c1 = closest;
				c2 = p + t * d;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q on or in AABB b whichis closest to p
			// For each coordinate axis, if the point coordinate value is
			// outside box, clamp it to the box, else keep it as is

			static glm::vec2 ClosestPointToAABBox(const glm::vec2 &p,
												  const glm::vec2 &bmin, const glm::vec2& bmax)
			{
				glm::vec2 q = p;
				if (q.x < bmin.x) q.x = bmin.x;		// v = max(v, bmin.x)
				if (q.x > bmax.x) q.x = bmax.x;		// v = min(v, bmax.x)
				if (q.y < bmin.y) q.y = bmin.y;		// v = max(v, bmin.y)
				if (q.y > bmax.y) q.y = bmax.y;		// v = min(v, bmax.y)
				return q;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Given point p, return the point q in Circle b which is closest to p

			static glm::vec2 ClosestPointToCircle(const glm::vec2& p, const glm::vec2& c, const float r, const float eps = vml::math::EPSILON)
			{
				glm::vec2 d = c- p;
				float a = d.x * d.x + d.y * d.y;
				if (a > -eps && a < eps)
					return c;	
				float t = (a - r * sqrtf(a)) / a;
				return p + t * d;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Return the shortest distance between the two segments

			static float ClosestPointBetweenSegments(const glm::vec2& a, const glm::vec2& b,
													 const glm::vec2& c, const glm::vec2& d,
													 glm::vec2& p, glm::vec2& q,
													 const float eps = vml::math::EPSILON)
			{
				// See if the segments intersect.

				float denom = (c.y - d.y) * (b.x - a.x) - (c.x - d.x) * (b.y - a.y);

				if (denom > -eps && denom < eps)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				// compute intersection point

				denom = 1.0f / denom;

				float numa = (c.x - d.x) * (a.y - d.y) - (c.y - d.y) * (a.x - d.x);
				float numb = (b.x - a.x) * (a.y - d.y) - (b.y - a.y) * (a.x - d.x);

				float ua = numa * denom;
				float ub = numb * denom;

				if (ua >= -eps && ua <= 1.0f + eps && ub >= -eps && ub <= 1.0f + eps)
				{
					// Get the intersection point.

					p = a + ua * (b - a);
					q = p;

					return vml::geo2d::Results::DOES_INTERSECT;
				}

				// Find the other possible distances.

				glm::vec2 closest;
				glm::vec2 ab, ap;
				float dx, dy;
				float denum;
				float t;
				float best_dist;
				float test_dist;

				best_dist = FLT_MAX;

				closest = glm::vec2(FLT_MAX, FLT_MAX);

				// Try p1.

				ab = d - c;

				denum = ab.x * ab.x + ab.y * ab.y;

				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;

				ap = a - c;

				t = (ap.x * ab.x + ap.y * ab.y) / denum;

				if (t < -vml::math::EPSILON)
					closest = c;
				if (t > 1 + vml::math::EPSILON)
					closest = d;
				if (t > -vml::math::EPSILON && t < 1 + vml::math::EPSILON)
					closest = c + t * ab;

				dx = closest.x - a.x;
				dy = closest.y - a.y;

				test_dist = dx * dx + dy * dy;

				if (test_dist < best_dist)
				{
					best_dist = test_dist;
					p = a;
					q = closest;
				}

				// Try p2.

				//

				// already computed

				// ab = d - c;

				// denum = ab.x * ab.x + ab.y * ab.y;

				// if (denum > -eps && denum < eps)
				//     denum = vml::math::EPSILON;

				ap = b - c;

				t = (ap.x * ab.x + ap.y * ab.y) / denum;

				if (t < -vml::math::EPSILON)
					closest = c;
				if (t > 1 + vml::math::EPSILON)
					closest = d;
				if (t > -vml::math::EPSILON && t < 1 + vml::math::EPSILON)
					closest = c + t * ab;

				dx = closest.x - b.x;
				dy = closest.y - b.y;

				test_dist = dx * dx + dy * dy;

				if (test_dist < best_dist)
				{
					best_dist = test_dist;
					p = b;
					q = closest;
				}

				// Try p3.

				//

				ab = b - a;

				denum = ab.x * ab.x + ab.y * ab.y;

				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;

				ap = c - a;

				t = (ap.x * ab.x + ap.y * ab.y) / denum;

				if (t < -vml::math::EPSILON)
					closest = a;
				if (t > 1 + vml::math::EPSILON)
					closest = b;
				if (t > -vml::math::EPSILON && t < 1 + vml::math::EPSILON)
					closest = a + t * ab;

				dx = closest.x - c.x;
				dy = closest.y - c.y;

				test_dist = dx * dx + dy * dy;

				if (test_dist < best_dist)
				{
					best_dist = test_dist;
					p = closest;
					q = c;
				}

				// Try p4.

				// already computed

				//  ab = b - a;

				//  denum = ab.x * ab.x + ab.y * ab.y;

				//  if (denum > -eps && denum < eps)
				//      denum = vml::math::EPSILON;

				ap = d - a;

				t = (ap.x * ab.x + ap.y * ab.y) / denum;

				if (t < -vml::math::EPSILON)
					closest = a;
				if (t > 1 + vml::math::EPSILON)
					closest = b;
				if (t > -vml::math::EPSILON && t < 1 + vml::math::EPSILON)
					closest = a + t * ab;

				dx = closest.x - d.x;
				dy = closest.y - d.y;

				test_dist = dx * dx + dy * dy;

				if (test_dist < best_dist)
				{
					best_dist = test_dist;
					p = closest;
					q = d;
				}

				return best_dist;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Return the shortest distance between a line and an axis alinged bounding box

			static void ClosestPointFromAABBToLine(const glm::vec2& linep, const glm::vec2& lineq,
												  const glm::vec2& aabbMin, const glm::vec2& aabbMax,
												  glm::vec2& closestp, glm::vec2& closestq)

			{
				glm::vec2 b0 = glm::vec2(aabbMin.x, aabbMin.y);
				glm::vec2 b1 = glm::vec2(aabbMax.x, aabbMin.y);
				glm::vec2 b2 = glm::vec2(aabbMax.x, aabbMax.y);
				glm::vec2 b3 = glm::vec2(aabbMin.x, aabbMax.y);

				glm::vec2 p0, q0;
				glm::vec2 p1, q1;
				glm::vec2 p2, q2;
				glm::vec2 p3, q3;

				float dmax = FLT_MAX;

				closestp = glm::vec2(FLT_MAX, FLT_MAX);
				closestq = glm::vec2(FLT_MAX, FLT_MAX);

				float d0 = vml::geo2d::distances::ClosestPointBetweenSegments(linep, lineq, b0, b1, p0, q0);
				float d1 = vml::geo2d::distances::ClosestPointBetweenSegments(linep, lineq, b1, b2, p1, q1);
				float d2 = vml::geo2d::distances::ClosestPointBetweenSegments(linep, lineq, b2, b3, p2, q2);
				float d3 = vml::geo2d::distances::ClosestPointBetweenSegments(linep, lineq, b3, b0, p3, q3);

				if (d0 < dmax) { dmax = d0; closestp = p0; closestq = q0; }
				if (d1 < dmax) { dmax = d1; closestp = p1; closestq = q1; }
				if (d2 < dmax) { dmax = d2; closestp = p2; closestq = q2; }
				if (d3 < dmax) { dmax = d3; closestp = p3; closestq = q3; }

			}

			/////////////////////////////////////////////////////////////////////////////
			// Return the shortest distance between a line and an axis alinged bounding box

			static void ClosestPointFromAOBBToLine(const glm::vec2& linep, const glm::vec2& lineq,
												   const glm::vec2& b0, const glm::vec2& b1, const glm::vec2& b2, const glm::vec2& b3,
												   glm::vec2& closestp, glm::vec2& closestq)

			{
				glm::vec2 p0, q0;
				glm::vec2 p1, q1;
				glm::vec2 p2, q2;
				glm::vec2 p3, q3;

				float dmax = FLT_MAX;

				closestp = glm::vec2(FLT_MAX, FLT_MAX);
				closestq = glm::vec2(FLT_MAX, FLT_MAX);

				float d0 = vml::geo2d::distances::ClosestPointBetweenSegments(linep, lineq, b0, b1, p0, q0);
				float d1 = vml::geo2d::distances::ClosestPointBetweenSegments(linep, lineq, b1, b2, p1, q1);
				float d2 = vml::geo2d::distances::ClosestPointBetweenSegments(linep, lineq, b2, b3, p2, q2);
				float d3 = vml::geo2d::distances::ClosestPointBetweenSegments(linep, lineq, b3, b0, p3, q3);

				if (d0 < dmax) { dmax = d0; closestp = p0; closestq = q0; }
				if (d1 < dmax) { dmax = d1; closestp = p1; closestq = q1; }
				if (d2 < dmax) { dmax = d2; closestp = p2; closestq = q2; }
				if (d3 < dmax) { dmax = d3; closestp = p3; closestq = q3; }

			}

		}  // end of closest distances namespace
	} // end of geo2d namespace
} // end of vml namepsace


///////////////////////////////////////////////////////////////////////////////////////
// polygon distances

namespace vml
{
	namespace geo2d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////
			// tests if a point is inside a concave or convex polygon,
			// not that this function might give you false positive if the 
			// point lies excatly on the border since multiple intersections
			// might be found. 
			
			static unsigned int IsPointInPolygon(const vml::geo2d::Polygon &polygon, const glm::vec2 &point)
			{
				bool oddnodes = false;

				int j = int(polygon.GetVertexCount()) - 1;

				for (int i = 0; i < polygon.GetVertexCount(); ++i)
				{

					glm::vec2 p1 = polygon.GetVertex(i);

					glm::vec2 p2 = polygon.GetVertex(j);

					if ((p1.y < point.y && p2.y >= point.y || p2.y < point.y && p1.y >= point.y) && (p1.x <= point.x || p2.x <= point.x))
					{
						oddnodes ^= (p1.x + (point.y - p1.y) / (p2.y - p1.y)*(p2.x - p1.x) <= point.x);
					}

					j = i;

				}

				if (!oddnodes)
					return vml::geo2d::Results::OUTSIDE;

				return vml::geo2d::Results::INSIDE;
			}
		}  // end of rays namespace
	} // end of geo2d namespace
} // end of vml namepsace

