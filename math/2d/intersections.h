#pragma once

/////////////////////////////////////////////////////////////////////
// intersections

namespace vml
{
	namespace geo2d
	{
		namespace intersections
		{

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding box and infinit ray
			// origin of ray is not tested , this is an infinite line test

			static unsigned int AABBOXVsRay(const glm::vec2 &bmin, const glm::vec2 &bmax,
											const glm::vec2 &r0, const glm::vec2 &m,
										    glm::vec2& q0, glm::vec2& q1,
											const float eps = vml::math::EPSILON)
			{
			
				float rn_invx;
				float rn_invy;

				if (m.x > -eps && m.x < eps) 
					 rn_invx = 1.0f/eps; 
				else
					rn_invx = 1.0f/m.x;

				if (m.y > -eps && m.y < eps)  
					rn_invy = 1.0f/eps; 
				else
					rn_invy = 1.0f/m.y;

				float tx1 = (bmin.x - r0.x)*rn_invx;
				float tx2 = (bmax.x - r0.x)*rn_invx;
				float ty1 = (bmin.y - r0.y)*rn_invy;
				float ty2 = (bmax.y - r0.y)*rn_invy;

				float tmax = vml::math::ranges::Min(vml::math::ranges::Max(tx1, tx2), vml::math::ranges::Max(ty1, ty2));
				float tmin = vml::math::ranges::Max(vml::math::ranges::Min(tx1, tx2), vml::math::ranges::Min(ty1, ty2));
				
				if (tmin -tmax  > eps)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				q0 = r0 + tmin * m;
				q1 = r0 + tmax * m;

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			//Check collision between a line segment and an AABB
			//Param:    Start point of line segement, End point of line segment,
			//          One corner of AABB, opposite corner of AABB,
			//          Location where line hits the AABB (OUT)
			//Return:   True if a collision occurs, False otherwise
			//Note:     If no collision occurs, OUT param is not reassigned and is not considered useable
			//			Also only the firs tpoint of intersection is returned

			static unsigned int AABBOXVsLine(const glm::vec2& bmin, const glm::vec2& bmax,
											 const glm::vec2& r0, const glm::vec2& r1, 
											 glm::vec2& q0, glm::vec2& q1,
											 const float &eps = vml::math::EPSILON)
			{
				// check if line is entirely inside bounding box

				if (r0.x - bmin.x > eps)
				if (r0.x - bmax.x < eps)
				if (r0.y - bmin.y > eps)
				if (r0.y - bmax.y < eps)
				if (r1.x - bmin.x > eps)
				if (r1.x - bmax.x < eps)
				if (r1.y - bmin.y > eps)
				if (r1.y - bmax.y < eps)
					return vml::geo2d::Results::INSIDE;
				
				// check for intersection

				glm::vec2 n = (r1 - r0);
				
				if (n.x > -eps && n.x < eps)n.x = eps;
				if (n.y > -eps && n.y < eps)n.y = eps;
				
				float rn_invx = 1.0f / n.x;
				float rn_invy = 1.0f / n.y;
				float tx1 = (bmin.x - r0.x)*rn_invx;
				float tx2 = (bmax.x - r0.x)*rn_invx;
				float ty1 = (bmin.y - r0.y)*rn_invy;
				float ty2 = (bmax.y - r0.y)*rn_invy;

				float tmax = vml::math::ranges::Min(vml::math::ranges::Max(tx1, tx2), vml::math::ranges::Max(ty1, ty2));
				float tmin = vml::math::ranges::Max(vml::math::ranges::Min(tx1, tx2), vml::math::ranges::Min(ty1, ty2));

				if (tmin - tmax < eps)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				q0 = r0 + tmin * n;
				q1 = r0 + tmax * n;

				if (tmin > -eps && tmin < 1 - eps)
					return vml::geo2d::Results::DOES_INTERSECT;

				if (tmax > -eps && tmax < 1 - eps)
					return vml::geo2d::Results::DOES_INTERSECT;

			}

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding boxes tests

			static unsigned int AABBoxVsAABBox(const glm::vec2 &bmin1,
											   const glm::vec2 &bmax1,
											   const glm::vec2 &bmin2,
											   const glm::vec2 &bmax2)
			{
				// check if the first bounding box is entirely inside the second bounding box

				if ((bmin1.x >= bmin2.x && bmax1.x <= bmax2.x) &&
					(bmin1.y >= bmin2.y && bmax1.y <= bmax2.y))
						return vml::geo2d::Results::FIRST_INSIDE_SECOND;

				// check if the second bounding box is entirely inside the first bounding box

				if ((bmin2.x >= bmin1.x && bmax2.x <= bmax1.x) &&
					(bmin2.y >= bmin1.y && bmax2.y <= bmax1.y))
						return vml::geo2d::Results::SECOND_INSIDE_FIRST;

				// check if bounding box is out of second bounding box

				if ((bmax2.x <= bmin1.x || bmin2.x >= bmax1.x) ||
					(bmax2.y <= bmin1.y || bmin2.y >= bmax1.y))
						return vml::geo2d::Results::DOES_NOT_INTERSECT;

				// if we get here bounding boxes are intersecting each ohter

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding boxes tests using MinkowskySum
			// returns penetration depth

			static unsigned int MinkowskiAABBoxVsAABBox(const vml::geo2d::AABB& b1, const vml::geo2d::AABB& b2, glm::vec2 &penetration,const float eps = vml::math::EPSILON)
			{
				float minx = b1.Min.x - b2.Max.x;
				float maxx = b1.Max.x - b2.Min.x;
				float miny = b1.Min.y - b2.Max.y;
				float maxy = b1.Max.y - b2.Min.y;

				if (minx < -eps && miny < -eps && maxx > eps && maxy > eps)
				{
					float min = FLT_MAX;

					if (fabs(minx) < min)
					{
						min = fabs(minx);
						penetration.x = minx;
						penetration.y = 0.f;
					}

					if (fabs(maxx) < min)
					{
						min = fabs(maxx);
						penetration.x = maxx;
						penetration.y = 0.f;
					}

					if (fabs(miny) < min)
					{
						min = fabs(miny);
						penetration.x = 0.f;
						penetration.y = miny;
					}

					if (fabs(maxy) < min)
					{
						min = fabs(maxy);
						penetration.x = 0.f;
						penetration.y = maxy;
					}

					return vml::geo2d::Results::DOES_INTERSECT;
				}

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// checks if two bounding boxes are colliding using the swept collision detection

			static bool AABBoxVsAABBoxSweptCollision(const vml::geo2d::AABB& box1,
													 const glm::vec2& raydir,
													 vml::geo2d::AABB& targetbox,
													 glm::vec2& contactpoint,
													 glm::vec2& contactnormal)
			{

				if ((raydir.x > -vml::math::EPSILON && raydir.x < vml::math::EPSILON) ||
					(raydir.y > -vml::math::EPSILON && raydir.y < vml::math::EPSILON))
					return false;

				// center of first box

				glm::vec2 origin((box1.Min.x + box1.Max.x) * 0.5f, (box1.Min.y + box1.Max.y) * 0.5f);

				float tnear;

				// compute expanded target box with source box dimensions

				float expandedboxMinx = targetbox.Min.x - box1.Width * 0.5f;
				float expandedboxMiny = targetbox.Min.y - box1.Height * 0.5f;
				float expandedboxMaxx = targetbox.Max.x + box1.Width * 0.5f;
				float expandedboxMaxy = targetbox.Max.y + box1.Height * 0.5f;
				float expandedboxWidth = expandedboxMaxx - expandedboxMinx;
				float expandedboxHeight = expandedboxMaxy - expandedboxMiny;

				// compute near and far planes

				float nearx = (expandedboxMinx - origin.x) / raydir.x;
				float neary = (expandedboxMiny - origin.y) / raydir.y;
				float farx = (expandedboxMinx + expandedboxWidth - origin.x) / raydir.x;
				float fary = (expandedboxMiny + expandedboxHeight - origin.y) / raydir.y;

				// compute intersections and check if they are in the 0 - 1 range

				if (nearx > farx)
				{
					float tmp = nearx;
					nearx = farx;
					farx = tmp;
				}

				if (neary > fary)
				{
					float tmp = neary;
					neary = fary;
					fary = tmp;
				}

				if (nearx > fary || neary > farx)
					return false;

				tnear = fmaxf(nearx, neary);

				if (tnear > 1.0f)
					return false;

				float hitfar = fminf(farx, fary);

				if (hitfar < 0.0f)
					return false;

				// compute contact point

				contactpoint = origin + raydir * tnear;

				// compute contact normal

				if (nearx > neary)
				{
					if (raydir.x < 0.0f)
					{
						contactnormal.x = 1;
						contactnormal.y = 0;
					}
					else
					{
						contactnormal.x = -1;
						contactnormal.y = 0;
					}
				}
				else if (nearx < neary)
				{
					if (raydir.y < 0.0f)
					{
						contactnormal.x = 0;
						contactnormal.y = 1;
					}
					else
					{
						contactnormal.x = 0;
						contactnormal.y = -1;
					}
				}

				return true;
			}

			/////////////////////////////////////////////////////////////////////////////
			// checks if a point is inside an axis aligned / axis oriented bounding box

			static unsigned int IsPointInAABBox(const glm::vec2& bmin, const glm::vec2& bmax,
												const glm::vec2& p,
												const float eps = vml::math::EPSILON)
			{
				if (p.x - bmin.x > eps)
				if (p.x - bmax.x < eps)
				if (p.y - bmin.y > eps)
				if (p.y - bmax.y < eps)
					return vml::geo2d::Results::INSIDE;
				return vml::geo2d::Results::OUTSIDE;
			}

			/////////////////////////////////////////////////////////////////////////////
			// test if a point is inside a triangle

			static unsigned int IsPointInTriangle(const glm::vec2& p,
												  const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3,
												  const float eps = vml::math::EPSILON)
			{
				if ((p.x - v2.x) * (v1.y - v2.y) - (v1.x - v2.x) * (p.y - v2.y) > eps)
				if ((p.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (p.y - v3.y) > eps)
				if ((p.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (p.y - v1.y) > eps)
					return vml::geo2d::Results::INSIDE;
				return vml::geo2d::Results::OUTSIDE;
			}

			/////////////////////////////////////////////////////////////////////////////
			// signed distance from circle to circlae
			
			static unsigned int IsPointInCircle(const glm::vec2& a, const float ra,
												const glm::vec2& b, const float rb,
												const float eps = vml::math::EPSILON)
			{
				glm::vec2 c = b - a;
				float r0 = ra + rb;
				if (c.x * c.x + c.y * c.y - r0 * r0 < eps)
					return vml::geo2d::Results::INSIDE;
				return vml::geo2d::Results::OUTSIDE;
			}

		} // end of intersections namesapce
	}	// end of geo3d namespace
}	// end of vml namespace

namespace vml
{
	namespace geo2d
	{
		namespace intersections
		{

			///////////////////////////////////////////////////////////////////////////////////
			// test if two ray intersects 

			static unsigned int RayVsRay(const glm::vec2& a, const glm::vec2& b,
										 const glm::vec2& c, const glm::vec2& d,
										 glm::vec2 &q,
										 const float eps = vml::math::EPSILON)
			{
				float denom = (c.y - d.y)*(b.x - a.x) - (c.x - d.x)*(b.y - a.y);
				float numa  = (c.x - d.x)*(a.y - d.y) - (c.y - d.y)*(a.x - d.x);
				float numb  = (b.x - a.x)*(a.y - d.y) - (b.y - a.y)*(a.x - d.x);

				if (denom > -eps && denom < eps)
				{
					// lines are collinear if num a is equal to num b
					// added an epsilon range check

					if ((-eps < numa && numa < eps) && (-eps < numb && numb < eps))
						return vml::geo2d::Results::COLLINEAR;

					// else they are parallel

					return vml::geo2d::Results::PARALLEL;
				}

				// compute intersection point

				denom = 1.0f / denom;

				float ua = numa * denom;
				float ub = numb * denom;

				// Get the intersection point.

				q = a + ua * (b - a);

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////
			// check if point s on line

			static unsigned int PointVsRay(const glm::vec2&p0, const glm::vec2 &p1,
										   const glm::vec2 &q,
										   const float eps = vml::math::EPSILON)
			{
				glm::vec2 u = p1 - p0;
				glm::vec2 v = p1 - q;
				float dp = -v.x*u.y + v.y*u.x;
				if (dp > -eps && dp < eps)
					return vml::geo2d::Results::DOES_INTERSECT;
				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////
			// check if ray intersects circle

			static int RayVsCircle(const glm::vec2& c0, const float r,
								   const glm::vec2& p0, const glm::vec2& p1,
								   glm::vec2 &q0, glm::vec2& q1,
								   const float eps = vml::math::EPSILON)
			{
				// cache line and circle parms

				float dx = p1.x - p0.x;
				float dy = p1.y - p0.y;
				float sx = p0.x - c0.x;
				float sy = p0.y - c0.y;
				float a = dx * dx + dy * dy;
				float b = dx * sx + dy * sy;
				float c = sx * sx + sy * sy - r * r;
				float delta = b * b - a * c;

				// line does not inersects circle

				if (delta > -eps && delta < eps)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				delta = sqrtf(delta);

				a = 1.0f / a;

				float dxa = dx * a;
				float dya = dy * a;
				float b0 = -b * dx * a;
				float b0x = p0.x + b0;
				float b0y = p0.y + b0;

				q0.x = b0x + delta * dxa;
				q0.y = b0y + delta * dya;

				q1.x = b0x - delta * dxa;
				q1.y = b0y - delta * dya;

				return vml::geo2d::Results::DOES_INTERSECT;
			}

		}  // end of rays namespace

	} // end of geo2d namespace

} // end of vml namepsace

///////////////////////////////////////////////////////////////////////////////////////
// line intersections

namespace vml
{
	namespace geo2d
	{
		namespace intersections
		{

			///////////////////////////////////////////////////////////////////////////////////
			// test if two lines overlap 

			static bool LineVsLineOverlapTest(const glm::vec2 &p0, const glm::vec2 &p1,
											  const glm::vec2 &q0, const glm::vec2 &q1,
											  const float eps = vml::math::EPSILON)
			{

				glm::vec2 u, v, w;
				float	  d0, d1;
				bool	  c0, c1;

				u = p1 - p0;
				v = q0 - p0;
				w = q1 - p0;

				d0 = u.x * v.y - v.x * u.y;
				d1 = u.x * w.y - w.x * u.y;

				c0 = d0 > -eps && d0 < eps;
				c1 = d1 > -eps && d1 < eps;

				// if lines are coplanar 

				if (c0 && c1)
				{

					d0 = u.x*u.x + u.y*u.y;

					if (d0 > -eps && d0 < eps)
						d0 = eps;

					d0 = 1.0f / d0;

					glm::vec2 d = q1 - q0;

					// check if vectors are pointing towrds same direction

					float dir = u.x*d.x + u.y*d.y;

					// case in which vector points away form each other

					if (dir > 0)
					{

						glm::vec2 a = p0 - q0;
						glm::vec2 b = p0 - q1;

						float a2 = (u.x*a.x + u.y*a.y) * d0;
						float b2 = (u.x*b.x + u.y*b.y) * d0;

						if (a2 > -1 && b2 < 0)
							return true;

						return false;

					}

					// case in which vecotr points against each other

					if (dir < 0)
					{

						glm::vec2 a = p1 - q0;
						glm::vec2 b = p1 - q1;

						float a2 = (u.x*a.x + u.y*a.y) * d0;
						float b2 = (u.x*b.x + u.y*b.y) * d0;

						if (a2 < 1 && b2 > 0)
							return true;

						return false;

					}

				}

				return false;
			}

			///////////////////////////////////////////////////////////////////////////////////
			// test if two lines intersects , point of intersection is computed

			static unsigned int LineVsLine(const glm::vec2& a, const glm::vec2& b,
										   const glm::vec2& c, const glm::vec2& d,
										   glm::vec2 &q,
										   const float eps = vml::math::EPSILON)
			{

				float denom = (c.y - d.y)*(b.x - a.x) - (c.x - d.x)*(b.y - a.y);
				float numa  = (c.x - d.x)*(a.y - d.y) - (c.y - d.y)*(a.x - d.x);
				float numb  = (b.x - a.x)*(a.y - d.y) - (b.y - a.y)*(a.x - d.x);

				if (denom > -eps && denom < eps)
				{
					// lines are collinear if num a is equal to num b
					// added an epsilon range check

					if ((-eps < numa && numa < eps) && (-eps < numb && numb < eps))
					{
						// lines may be collinear but still intersecting

						if ( LineVsLineOverlapTest(a, b,c,d,eps))
							return vml::geo2d::Results::DOES_INTERSECT;

						return vml::geo2d::Results::COLLINEAR;
					}

					// else they are parallel

					return vml::geo2d::Results::PARALLEL;
				}

				// compute intersection point

				denom = 1.0f / denom;

				float ua = numa * denom;
				float ub = numb * denom;

				if (ua >= -eps && ua <= 1.0f + eps && ub >= -eps && ub <= 1.0f + eps)
				{
					// Get the intersection point.

					q = a + ua * (b - a);

					return vml::geo2d::Results::DOES_INTERSECT;
				}

				// else ther eis no intersection

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

			///////////////////////////////////////////////////////////////////////////////////
			// test if line and cricle intersects , point of intersection is computed

			static unsigned int LineVsCircle(const glm::vec2& c0, const float r,
								  		     const glm::vec2& p0, const glm::vec2& p1,
											 glm::vec2& q0, glm::vec2& q1,int &intersections,
											 const float eps = vml::math::EPSILON)
			{
				float dx = p1.x - p0.x;
				float dy = p1.y - p0.y;
				float sx = p0.x - c0.x;
				float sy = p0.y - c0.y;
				float a = dx * dx + dy * dy;
				float b = dx * sx + dy * sy;
				float c = sx * sx + sy * sy - r * r;
				float delta = b * b - a * c;

				if (delta > -eps && delta < eps)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				if (a > -eps && a < eps)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				b /= a;

				delta = sqrtf(delta) / a;

				float t0 = -b - delta;
				float t1 = -b + delta;

				intersections = 0;

				if (t0 < -eps || t0 >1.0f + eps)
				if (t1 < -eps || t1 >1.0f + eps)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
			
				if (t0 > -eps && t0 < 1.0f + eps)
				{
					q0 = glm::vec2(p0.x + t0 * dx, p0.y + t0 * dy);
					intersections++;
				}

				if (t1 > -eps && t1 < 1.0f + eps)
				{
					q1 = glm::vec2(p0.x + t1 * dx, p0.y + t1 * dy);
					intersections++;
				}

				return vml::geo2d::Results::DOES_INTERSECT;
			}
						
			///////////////////////////////////////////////////////////////////////////////////
			// test if two circles intersects , point(s) of intersection is computed

			static int CircleVsCircle(const glm::vec2& c0, float r0,
									  const glm::vec2& c1, float r1,
									  glm::vec2& q1, glm::vec2& q2,
									  const float eps = vml::math::EPSILON)
			{
				float a, dx, dy, d, h, rx, ry;
				float x2, y2;

				// dx and dy are the vertical and horizontal distances between
				// the circle centers.

				dx = c1.x - c0.x;
				dy = c1.y - c0.y;

				// Determine the straight-line distance between the centers. 

				d = sqrt((dy * dy) + (dx * dx));

				//d = hypot(dx, dy); // Suggested by Keith Briggs

				// Check for solvability. 

				if (d > (r0 + r1))
				{
					// no solution. circles do not intersect. 
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}
				if (d < fabs(r0 - r1))
				{
					// no solution. one circle is contained in the other 
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				// 'point 2' is the point where the line through the circle
				//  intersection points crosses the line between the circle
				//  centers.

				// Determine the distance from point 0 to point 2. 

				a = ((r0 * r0) - (r1 * r1) + (d * d)) / (2.0 * d);

				// Determine the coordinates of point 2. 

				x2 = c0.x + (dx * a / d);
				y2 = c0.y + (dy * a / d);

				// Determine the distance from point 2 to either of the
				// intersection points.

				h = sqrt((r0 * r0) - (a * a));

				// Now determine the offsets of the intersection points from point 2.

				rx = -dy * (h / d);
				ry = dx * (h / d);

				// Determine the absolute intersection points. 

				q1.x = x2 + rx;
				q2.x = x2 - rx;

				q1.y = y2 + ry;
				q2.y = y2 - ry;

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			////////////////////////////////////////////////////////////////////
			// intersection between line and polygon

			static unsigned int LineVsPolygon(const vml::geo2d::Polygon &polygon, 
				                              const vml::geo2d::Line &line, 
				                              std::vector<glm::vec2> &rs,
											  const float eps = vml::math::EPSILON)
			{

				rs.clear();

				int j = (int)polygon.GetVertexCount() - 1;

				for (int i = 0; i < polygon.GetVertexCount(); ++i)
				{

					glm::vec2 r;

					if (vml::geo2d::intersections::LineVsLine(polygon.GetVertex(i), polygon.GetVertex(j), line.P, line.Q, r) == vml::geo2d::Results::DOES_INTERSECT)
					{
						rs.emplace_back(r);
					}

					j = i;

				}

				if (rs.size() == 0)
					return vml::geo2d::Results::DOES_NOT_INTERSECT;

				return vml::geo2d::Results::DOES_INTERSECT;

			}
			
		} // end of intersections namesapce
	}	// end of geo3d namespace
}	// end of vml namespace


///////////////////////////////////////////////////////////////////////////////////////
// triangle intersections class

namespace vml
{
	namespace geo2d
	{
		namespace intersections
		{

			//////////////////////////////////////////////////////////////////////////////
			// Moller triangle vs triangle intersection test

			class TriangleTriangleIntersection
			{

				private:

					// ---------------------------------------------------------------
					// compute ccw or anticcw direction of a triangle formaed by a,b,c points

					static float Orient(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
					{
						return (a[0] - c[0]) * (b[1] - c[1]) - (a[1] - c[1]) * (b[0] - c[0]);
					}

					// ---------------------------------------------------------------
					// Checks if vertex is inside triangle

					static unsigned int IntersectionTestVertex(const glm::vec2 &P1, const glm::vec2 &Q1, const glm::vec2 &R1, 
											                   const glm::vec2 &P2, const glm::vec2 &Q2, const glm::vec2 &R2)
					{
						if (Orient(R2, P2, Q1) >= 0.0f)
						{
							if (Orient(R2, Q2, Q1) <= 0.0f)
							{
								if (Orient(P1, P2, Q1) > 0.0f)
								{
									if (Orient(P1, Q2, Q1) <= 0.0f)
									{
										return vml::geo2d::Results::DOES_INTERSECT;
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
								else
								{
									if (Orient(P1, P2, R1) >= 0.0f)
									{
										if (Orient(Q1, R1, P2) >= 0.0f)
										{
											return vml::geo2d::Results::DOES_INTERSECT;
										}
										else
										{
											return vml::geo2d::Results::DOES_NOT_INTERSECT;
										}
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
							}
							else
							{
								if (Orient(P1, Q2, Q1) <= 0.0f)
								{
									if (Orient(R2, Q2, R1) <= 0.0f)
									{
										if (Orient(Q1, R1, Q2) >= 0.0f)
										{
											return vml::geo2d::Results::DOES_INTERSECT;
										}
										else
										{
											return vml::geo2d::Results::DOES_NOT_INTERSECT;
										}
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
								else
								{
									return vml::geo2d::Results::DOES_NOT_INTERSECT;
								}
							}
						}
						else
						{
							if (Orient(R2, P2, R1) >= 0.0f)
							{
								if (Orient(Q1, R1, R2) >= 0.0f)
								{
									if (Orient(P1, P2, R1) >= 0.0f)
									{
										return vml::geo2d::Results::DOES_INTERSECT;
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
								else
								{
									if (Orient(Q1, R1, Q2) >= 0.0f)
									{
										if (Orient(R2, R1, Q2) >= 0.0f)
										{
											return vml::geo2d::Results::DOES_INTERSECT;
										}
										else
										{
											return vml::geo2d::Results::DOES_NOT_INTERSECT;
										}
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
							}
							else
							{
								return vml::geo2d::Results::DOES_NOT_INTERSECT;
							}
						}
					}

					// ---------------------------------------------------------------
					// Check for edges intersection

					static unsigned int IntersectionTestEdge(const glm::vec2 &P1, const glm::vec2 &Q1, const glm::vec2 &R1, 
											                 const glm::vec2 &P2, const glm::vec2 &Q2, const glm::vec2 &R2)
					{
						if (Orient(R2, P2, Q1) >= 0.0f)
						{
							if (Orient(P1, P2, Q1) >= 0.0f)
							{
								if (Orient(P1, Q1, R2) >= 0.0f)
								{
									return vml::geo2d::Results::DOES_INTERSECT;
								}
								else
								{
									return vml::geo2d::Results::DOES_NOT_INTERSECT;
								}
							}
							else
							{
								if (Orient(Q1, R1, P2) >= 0.0f)
								{
									if (Orient(R1, P1, P2) >= 0.0f)
									{
										return vml::geo2d::Results::DOES_INTERSECT;
									}
									else
									{
										return vml::geo2d::Results::DOES_NOT_INTERSECT;
									}
								}
								else
								{
									return 0;
								}
							}
						}
						else
						{
							if (Orient(R2, P2, R1) >= 0.0f)
							{
								if (Orient(P1, P2, R1) >= 0.0f)
								{
									if (Orient(P1, R1, R2) >= 0.0f)
									{
										return vml::geo2d::Results::DOES_INTERSECT;
									}
									else
									{
										if (Orient(Q1, R1, R2) >= 0.0f)
										{
											return vml::geo2d::Results::DOES_INTERSECT;
										}
										else
										{
											return vml::geo2d::Results::DOES_NOT_INTERSECT;
										}
									}
								}
								else
								{
									return vml::geo2d::Results::DOES_NOT_INTERSECT;
								}
							}
							else
							{
								return vml::geo2d::Results::DOES_NOT_INTERSECT;
							}
						}
					}

					// ---------------------------------------------------------------
					// 

					static unsigned int CCWTriTriIntersection(const glm::vec2 &p1, const glm::vec2 &q1, const glm::vec2 &r1,
													 const glm::vec2 &p2, const glm::vec2 &q2, const glm::vec2 &r2)
					{
						if (Orient(p2, q2, p1) >= 0.0f)
						{
							if (Orient(q2, r2, p1) >= 0.0f)
							{
								if (Orient(r2, p2, p1) >= 0.0f)
								{
									return vml::geo2d::Results::DOES_INTERSECT;
								}
								else
								{
									return IntersectionTestEdge(p1, q1, r1, p2, q2, r2);
								}
							}
							else
							{
								if (Orient(r2, p2, p1) >= 0.0f)
								{
									return IntersectionTestEdge(p1, q1, r1, r2, p2, q2);
								}
								else
								{
									return IntersectionTestVertex(p1, q1, r1, p2, q2, r2);
								}
							}
						}
						else
						{
							if (Orient(q2, r2, p1) >= 0.0f)
							{
								if (Orient(r2, p2, p1) >= 0.0f)
								{
									return IntersectionTestEdge(p1, q1, r1, q2, r2, p2);
								}
								else
								{
									return IntersectionTestVertex(p1, q1, r1, q2, r2, p2);
								}
							}
							else
							{
								return IntersectionTestVertex(p1, q1, r1, r2, p2, q2);
							}
						}
					};

				public:

					// -------------------------------------------------------------------------
					// 

					static unsigned int Test(const glm::vec2 &p1, const glm::vec2 &q1, const glm::vec2 &r1,
											 const glm::vec2 &p2, const glm::vec2 &q2, const glm::vec2 &r2)
					{
						if (Orient(p1, q1, r1) < 0.0f)
						{
							if (Orient(p2, q2, r2) < 0.0f)
							{
								return CCWTriTriIntersection(p1, r1, q1, p2, r2, q2);
							}
							else
							{
								return CCWTriTriIntersection(p1, r1, q1, p2, q2, r2);
							}
						}
						else
						{
							if (Orient(p2, q2, r2) < 0.0f)
							{
								return CCWTriTriIntersection(p1, q1, r1, p2, r2, q2);
							}
							else
							{
								return CCWTriTriIntersection(p1, q1, r1, p2, q2, r2);
							}
						}
					}

					// ----------------------------------------------------------
					// ctro / dtor

					TriangleTriangleIntersection()
					{}

					~TriangleTriangleIntersection()
					{}

			};

		} // end of intersections namesapce
	}	// end of geo3d namespace
}	// end of vml namespace

namespace vml
{
	namespace geo2d
	{
		namespace clipping
		{


			//////////////////////////////////////////////////////////////////
			// clip ab aann against another aabb
			// amin is target aabb
			// dmin is clipping aabb

			static unsigned int ClipAABBToAABB(const glm::vec2& amin, const glm::vec2& amax,
											   const glm::vec2& dmin, const glm::vec2& dmax,
											   float& aabb3Minx, float& aabb3Miny, float& aabb3Maxx, float& aabb3Maxy,
											   const float eps = vml::math::EPSILON)
			{
				// check if quad is out of aabb clipping box

				if ((dmax.x <= amin.x || dmin.x >= amax.x) ||
					(dmax.y <= amin.y || dmin.y >= amax.y))
				{
					aabb3Minx = amin.x;
					aabb3Miny = amin.y;
					aabb3Maxx = amax.x;
					aabb3Maxy = amax.y;
					return vml::geo2d::Results::DOES_NOT_INTERSECT;
				}

				// check if quad is inside of aabb clipping box

				if ((amin.x >= dmin.x && amax.x <= dmax.x) &&
					(amin.y >= dmin.y && amax.y <= dmax.y))
				{
					aabb3Minx = amin.x;
					aabb3Miny = amin.y;
					aabb3Maxx = amax.x;
					aabb3Maxy = amax.y;
					return vml::geo2d::Results::FIRST_INSIDE_SECOND;

				}

				// if we get here , we need to clip the target aabb
				// against the clipping aabb

				float aabb0Minx = 0;
				float aabb0Miny = 0;
				float aabb0Maxx = 0;
				float aabb0Maxy = 0;

				float aabb1Minx = 0;
				float aabb1Miny = 0;
				float aabb1Maxx = 0;
				float aabb1Maxy = 0;

				float aabb2Minx = 0;
				float aabb2Miny = 0;
				float aabb2Maxx = 0;
				float aabb2Maxy = 0;

				// clip dest square against top window

				if (amin.y - dmin.y < eps && amax.y - dmin.y > eps)
				{
					aabb0Minx = amin.x;
					aabb0Miny = dmin.y;
					aabb0Maxx = amax.x;
					aabb0Maxy = amax.y;
				}

				if (amin.y - dmin.y > eps)
				{
					aabb0Minx = amin.x;
					aabb0Miny = amin.y;
					aabb0Maxx = amax.x;
					aabb0Maxy = amax.y;
				}

				// clip dest square against bottom window

				if (aabb0Miny - dmax.y < eps && aabb0Maxy - dmax.y > eps)
				{
					aabb1Minx = aabb0Minx;
					aabb1Miny = aabb0Miny;
					aabb1Maxx = aabb0Maxx;
					aabb1Maxy = dmax.y;
				}

				if (aabb0Maxy - dmax.y < eps)
				{
					aabb1Minx = aabb0Minx;
					aabb1Miny = aabb0Miny;
					aabb1Maxx = aabb0Maxx;
					aabb1Maxy = aabb0Maxy;
				}

				// clip dest square against left window

				if (aabb1Minx - dmin.x < eps && aabb1Maxx - dmin.x > eps)
				{
					aabb2Minx = dmin.x;
					aabb2Miny = aabb1Miny;
					aabb2Maxx = aabb1Maxx;
					aabb2Maxy = aabb1Maxy;
				}

				if (aabb1Minx - dmin.x > eps)
				{
					aabb2Minx = aabb1Minx;
					aabb2Miny = aabb1Miny;
					aabb2Maxx = aabb1Maxx;
					aabb2Maxy = aabb1Maxy;
				}

				// clip dest square against right window

				if (aabb2Minx - dmax.x < eps && aabb2Maxx - dmax.x > eps)
				{
					aabb3Minx = aabb2Minx;
					aabb3Miny = aabb2Miny;
					aabb3Maxx = dmax.x;
					aabb3Maxy = aabb2Maxy;
				}

				if (aabb2Maxx - dmax.x < eps)
				{
					aabb3Minx = aabb2Minx;
					aabb3Miny = aabb2Miny;
					aabb3Maxx = aabb2Maxx;
					aabb3Maxy = aabb2Maxy;
				}

				// if we get here bounding boxes are intersecting each ohter

				return vml::geo2d::Results::DOES_INTERSECT;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Implementing Cohen - Sutherland algorithm
			// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)

			static unsigned int CohenSutherlandClip(int x1, int y1, int x2, int y2,
													int XMIN, int YMIN, int XMAX, int YMAX,
													int& xp, int& yp, int& xq, int& yq)
			{
				// Defining region codes

				constexpr unsigned int CS_INSIDE = 0;	// 0000 
				constexpr unsigned int CS_LEFT	 = 1;	// 0001
				constexpr unsigned int CS_RIGHT  = 2;	// 0010
				constexpr unsigned int CS_BOTTOM = 4;	// 0100
				constexpr unsigned int CS_TOP	 = 8;	// 1000

				// init with c1 as 'inside' code

				int c1 = CS_INSIDE;

				if (x1 < XMIN)				// to the left of rectangle
					c1 |= CS_LEFT;
				else if (x1 > XMAX)			// to the right of rectangle
					c1 |= CS_RIGHT;

				if (y1 < YMIN)				// below the rectangle
					c1 |= CS_BOTTOM;
				else if (y1 > YMAX)			// above the rectangle
					c1 |= CS_TOP;

				int c2 = CS_INSIDE;

				if (x2 < XMIN)				// to the left of rectangle
					c2 |= CS_LEFT;
				else if (x2 > XMAX)			// to the right of rectangle
					c2 |= CS_RIGHT;

				if (y2 < YMIN)				// below the rectangle
					c2 |= CS_BOTTOM;
				else if (y2 > YMAX)			// above the rectangle
					c2 |= CS_TOP;

				xp = -INT_MAX;
				yp = -INT_MAX;
				xq = INT_MAX;
				yq = INT_MAX;

				// Initialize line as outside the rectangular window

				while (true)
				{

					if ((c1 == 0) && (c2 == 0))
					{
						// If both endpoints lie within rectangle

						xp = x1;
						yp = y1;
						xq = x2;
						yq = y2;

						return vml::geo2d::Results::DOES_INTERSECT;

					}
					else if (c1 & c2)
					{
						// If both endpoints are outside rectangle,
						// in same region

						return vml::geo2d::Results::DOES_NOT_INTERSECT;
					}
					else
					{
						// Some segment of line lies within the
						// rectangle

						int c;
						int x, y;

						// At least one endpoint is outside the
						// rectangle, pick it.

						if (c1 != 0)
							c = c1;
						else
							c = c2;

						// Find intersection point;
						// using formulas y = y1 + slope * (x - x1),
						// x = x1 + (1 / slope) * (y - y1)

						if (c & CS_TOP)
						{
							// point is above the clip rectangle

							x = x1 + (x2 - x1) * (YMAX - y1) / (y2 - y1);
							y = YMAX;
						}
						else if (c & CS_BOTTOM)
						{
							// point is below the rectangle

							x = x1 + (x2 - x1) * (YMIN - y1) / (y2 - y1);
							y = YMIN;
						}
						else if (c & CS_RIGHT)
						{
							// point is to the right of rectangle

							y = y1 + (y2 - y1) * (XMAX - x1) / (x2 - x1);
							x = XMAX;
						}
						else if (c & CS_LEFT)
						{
							// point is to the left of rectangle

							y = y1 + (y2 - y1) * (XMIN - x1) / (x2 - x1);
							x = XMIN;
						}

						// Now intersection point x, y is found
						// We replace point outside rectangle
						// by intersection point

						if (c == c1)
						{
							x1 = x;
							y1 = y;

							c1 = CS_INSIDE;

							if (x1 < XMIN)				// to the left of rectangle
								c1 |= CS_LEFT;
							else if (x1 > XMAX)			// to the right of rectangle
								c1 |= CS_RIGHT;

							if (y1 < YMIN)				// below the rectangle
								c1 |= CS_BOTTOM;
							else if (y1 > YMAX)			// above the rectangle
								c1 |= CS_TOP;

						}
						else
						{
							x2 = x;
							y2 = y;

							c2 = CS_INSIDE;

							if (x2 < XMIN)				// to the left of rectangle
								c2 |= CS_LEFT;
							else if (x2 > XMAX)			// to the right of rectangle
								c2 |= CS_RIGHT;

							if (y2 < YMIN)				// below the rectangle
								c2 |= CS_BOTTOM;
							else if (y2 > YMAX)			// above the rectangle
								c2 |= CS_TOP;

						}

					}

				}

				return vml::geo2d::Results::DOES_NOT_INTERSECT;
			}

		}	// end of intersections namespace

	}	// end of geo3d namespace

}	// end of vml namespace
