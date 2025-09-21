#pragma once

///////////////////////////////////////////////////////////////////////
// metrics

///////////////////////////////////////////////////////////////////////
// points

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------
			// Transform points 
			// px , py is input point
			// posx , posy is translation
			// angle is the rotation angle
			// scalex, scale y is the scaling
			// rx,ry , output point

			static void TransformPoint(const glm::vec2 &p,
									   const glm::vec2 &pos,
									   const float angle, 
									   const glm::vec2 scale,
									   glm::vec2 &r)
			{
				float rads = angle * vml::math::DEGTORAD;
				float sx = sin(rads);
				float cx = cos(rads);
				float M0 = cx * scale.x;
				float M1 = sx * scale.x;
				//	float M2 = 0;
				float M3 = -sx * scale.y;
				float M4 =  cx * scale.y;
				//	float M5 = 0;
				float M6 = pos.x;
				float M7 = pos.y;
				//	float M8 = 1;
				r.x = p.x * M0 + p.y * M3 + M6;
				r.y = p.x * M1 + p.y * M4 + M7;
			}

			// ------------------------------------------------------------------------
			// test if 3 2d points are collinear

			static bool ArePointsCollinear(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const float eps = vml::math::EPSILON)
			{
				return fabs((p0.y - p1.y) * (p0.x - p2.x) - (p0.y - p2.y) * (p0.x - p1.x)) < eps;
			}

			// ------------------------------------------------------------------------
			// test if 2 2d points are whitin a radius

			static bool ArePointsInRange(const glm::vec2 &p0, const glm::vec2 &p1, const float radius)
			{
				glm::vec2 d = p1 - p0;
				return d.x * d.x + d.y * d.y - radius * radius < 0;
			}

			// ------------------------------------------------------------------------
			// test if 2 2d points are equals whitin an epsilon

			static bool ArePointsEquals(const glm::vec2 &p0, const glm::vec2 &p1, const float eps = vml::math::EPSILON)
			{
				glm::vec2 d = p1 - p0;
				return d.x * d.x + d.y * d.y < eps;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////
// rays

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------
			// compute direction vector given two points

			static glm::vec2 LineDirection(const glm::vec2& p0, const glm::vec2& p1, const float eps = vml::math::EPSILON)
			{
				glm::vec2 dir = p1 - p0;
				float denum = sqrtf(dir.x * dir.x + dir.y * dir.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = 1.0f / denum;
				return glm::vec2(dir.x * denum, dir.y * denum);
			}
			
			// ------------------------------------------------------------------------
			// returns normal to a direction, ray input vector must be normalized

			static glm::vec2 LineNormal(const glm::vec2& p0, const glm::vec2& p1, const float eps = vml::math::EPSILON)
			{
				glm::vec2 ray = p1 - p0;
				float denum = sqrtf(ray.x * ray.x + ray.y * ray.y);
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;
				denum = 1.0f / denum;
				return glm::vec2(-ray.y * denum, ray.x * denum);
			}

			// ------------------------------------------------------------------------
			// returns ray slope

			static float LineSlope(const glm::vec2& p0, const glm::vec2& p1)
			{
				return vml::math::angles::Atan2(p1.x-p0.x, p1.y-p0.y, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f;
			}

			// ------------------------------------------------------------------------

			static glm::vec2 LineCenter(const glm::vec2& bmin, const glm::vec2& bmax)
			{
				return glm::vec2((bmin.x + bmax.x) * 0.5f, (bmin.y + bmax.y) * 0.5f);
			}

			// ------------------------------------------------------------------------
			// returns whcih side the point is respect a line of p1 and p2 extremes

			static int OrientationBetweenLineAndPoint(const glm::vec2& p1,
													  const glm::vec2& p2,
													  const glm::vec2& p,
													  const float eps = vml::math::EPSILON)
			{
				float dp = (p2.x - p1.x) * (p.y - p1.y) - (p.x - p1.x) * (p2.y - p1.y);
				if (dp > -eps) return vml::geo2d::Results::LEFT;						// Orientaion is to the left-hand side  
				else if (dp < eps) return vml::geo2d::Results::RIGHT;					// Orientaion is to the right-hand side 
				return vml::geo2d::Results::COLLINEAR;									// Orientaion is neutral aka collinear  
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////
// rays

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------
			// compute direction vector given two points

			static glm::vec2 RayDirection(const glm::vec2& p0, const glm::vec2& p1, const float eps = vml::math::EPSILON)
			{
				glm::vec2 dir = p1 - p0;
				float denum = sqrtf(dir.x * dir.x + dir.y * dir.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = 1.0f / denum;
				return glm::vec2(dir.x * denum, dir.y * denum);
			}

			// ------------------------------------------------------------------------
			// returns normal to a direction, ray input vector must be normalized

			static glm::vec2 RayNormal(const glm::vec2& p0, const glm::vec2& p1, const float eps = vml::math::EPSILON)
			{
				glm::vec2 ray = p1 - p0;
				float denum = sqrtf(ray.x * ray.x + ray.y * ray.y);
				if (denum > -eps && denum < eps)
					denum = vml::math::EPSILON;
				denum = 1.0f / denum;
				return glm::vec2(-ray.y * denum, ray.x * denum);
			}

			// ------------------------------------------------------------------------
			// returns ray slope

			static float RaySlope(const glm::vec2& p0, const glm::vec2& p1)
			{
				return vml::math::angles::Atan2(p1.x - p0.x, p1.y - p0.y, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f;
			}


			// ------------------------------------------------------------------------
			// returns whcih side the point is respect a line of p1 and p2 extremes

			static int OrientationBetweenRayAndPoint(const glm::vec2& p1,
													 const glm::vec2& p2,
													 const glm::vec2& p, 
				                                     const float eps = vml::math::EPSILON)
			{
				float dp = (p2.x - p1.x) * (p.y - p1.y) - (p.x - p1.x) * (p2.y - p1.y);
				if (dp > -eps) return vml::geo2d::Results::LEFT;						// Orientaion is to the left-hand side  
				else if (dp < eps) return vml::geo2d::Results::RIGHT;					// Orientaion is to the right-hand side 
				return vml::geo2d::Results::COLLINEAR;									// Orientaion is neutral aka collinear  
			}
		
			// ------------------------------------------------------------------------
			//  computes the angle in radians swept out between two rays in 2D
			//  a,b,c define the rays
			//  a-b and  c-b which in turn define the
			//  the origin point is a
			//  angle, counterclockwise from  b-a 
			//  the returned value is in radians int the range  0 <= angle < 2 PI
			//  if either ray has zero length, then result is 0

			static float AngleBetweenRays(const glm::vec2& a,
										  const glm::vec2& b,
										  const glm::vec2& c,
										  const float eps = vml::math::EPSILON)
			{
				glm::vec2 d1 = a - b;
				glm::vec2 d2 = c - b;
				float x = d1.x * d2.x + d1.y * d2.y;
				float y = d1.x * d2.y - d1.y * d2.x;
				if (x >= -eps && x <= eps) x = eps;
				return vml::math::angles::Atan2(y, x, vml::math::EPSILON);
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// axis aligned bounding box

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------

			static glm::vec2 AABBCenter(const glm::vec2 &bmin, const glm::vec2& bmax )
			{
				return glm::vec2( (bmin.x + bmax.x) * 0.5f, (bmin.y + bmax.y) * 0.5f);
			}

			// ------------------------------------------------------------------------

			static glm::vec2 AABBExtents(const glm::vec2& bmin, const glm::vec2& bmax)
			{
				return glm::vec2(fabs(bmax.x - bmin.x), fabs(bmax.y - bmin.y));
			}

			// ------------------------------------------------------------------------

			static glm::vec2 AABBHalfExtents(const glm::vec2& bmin, const glm::vec2& bmax)
			{
				return glm::vec2(fabs(bmax.x - bmin.x) * 0.5f, fabs(bmax.y - bmin.y) * 0.5f);
			}

			// ------------------------------------------------------------------------

			static vml::geo2d::AABB CentreAABB(const vml::geo2d::AABB& aabb)
			{
				glm::vec2 c((aabb.Min.x + aabb.Max.x) * 0.5f, (aabb.Min.y + aabb.Max.y) * 0.5f);
				return vml::geo2d::AABB(aabb.Min.x - c.x, aabb.Min.y - c.y, aabb.Max.x - c.x, aabb.Max.y - c.y);
			}

			// ------------------------------------------------------------------------

			static vml::geo2d::AABB TransformAABB(const vml::geo2d::AABB& aabb, const glm::vec2 &pos)
			{
				return vml::geo2d::AABB(aabb.Min.x + pos.x, aabb.Min.y + pos.y, aabb.Max.x + pos.x, aabb.Max.y + pos.y);
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// axis oriented bounding box

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------------

			static vml::geo2d::AOBB TransformAOBB(const vml::geo2d::AOBB& aobb, const glm::vec2 &pos, float deg)
			{
				float rad = deg * vml::math::DEGTORAD;
				float cx = cos(rad);
				float sx = sin(rad);
				return vml::geo2d::AOBB(aobb.P1.x * cx - aobb.P1.y * sx + pos.x,
										aobb.P1.x * sx + aobb.P1.y * cx + pos.y,
										aobb.P2.x * cx - aobb.P2.y * sx + pos.x,
										aobb.P2.x * sx + aobb.P2.y * cx + pos.y,
										aobb.P3.x * cx - aobb.P3.y * sx + pos.x,
										aobb.P3.x * sx + aobb.P3.y * cx + pos.y,
										aobb.P4.x * cx - aobb.P4.y * sx + pos.x,
										aobb.P4.x * sx + aobb.P4.y * cx + pos.y);
			}

			// ------------------------------------------------------------------------

			static glm::vec2 AOBBCenter(const vml::geo2d::AOBB& aobb)
			{
				return glm::vec2((aobb.P1.x + aobb.P2.x + aobb.P3.x + aobb.P4.x) * 0.25f, (aobb.P1.y + aobb.P2.y + aobb.P3.y + aobb.P4.y) * 0.25f);
			}

			// ------------------------------------------------------------------------

			static glm::vec2 AABBExtents(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
			{
				return glm::vec2((p1-p0).length(), (p3-p0).length());
			}

			// ------------------------------------------------------------------------

			static glm::vec2 AABBHalfExtents(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
			{
				return glm::vec2((p1 - p0).length(), (p3 - p0).length())*0.5f;
			}

			// ------------------------------------------------------------------------

			static vml::geo2d::AOBB CentreAOBB(const vml::geo2d::AOBB& aobb)
			{
				glm::vec2 c = (aobb.P1 + aobb.P2 + aobb.P3 + aobb.P4) * 0.25f;
				return vml::geo2d::AOBB(aobb.P1 -c, aobb.P2-c, aobb.P3-c, aobb.P4-c);
			}

			// ------------------------------------------------------------------------

			static glm::vec2 GetAABBoxExtremaPointAlongDirection(const glm::vec2& min, const glm::vec2& max, const glm::vec3& direction)
			{
				return glm::vec2((direction.x >= 0.f ? max.x : min.x), (direction.y >= 0.f ? max.y : min.y));
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo2d
	{
		namespace metrics
		{

			// -------------------------------------------------------------------------------------------------
			// area of a triangle

			static float TriangleArea(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
			{
				return fabs((c.y - a.y)*(b.x - a.x) - (b.y - a.y)*(c.x - a.x)) *0.5f;
			}

			// -------------------------------------------------------------------------------------------------
			// signed triangel area

			static float SignedTriangleArea(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
			{
				return ((c.y - a.y)*(b.x - a.x) - (b.y - a.y)*(c.x - a.x)) *0.5f;
			}

			// -------------------------------------------------------------------------------------------------
			// computes triangle centroid

			static glm::vec2 TriangleCentroid(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
			{
				return glm::vec2((a.x + b.x + c.x)*0.33333f, (a.y + b.y + c.y)*0.33333f);
			}

			// -------------------------------------------------------------------------------------------------
			// computes if triangle is ccw

			static bool IsTtriangleCCW(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
			{
				return (c.y - a.y)*(b.x - a.x) > (b.y - a.y)*(c.x - a.x);
			}

		} // end of namespace metrics

	} // end of geo2d namespace

} // end of vml namepsace
