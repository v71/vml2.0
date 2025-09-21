#pragma once

////////////////////////////////////////////////////////////////////////////////////
// point class

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{

			////////////////////////////////////////////////////////////////////////////////////
			// Containment

			static const unsigned int POINT_OUTSIDE = 0;
			static const unsigned int POINT_INSIDE = 1;

			////////////////////////////////////////////////////////////////////////////////////
			// Distances functions for points

			// ---------------------------------------------------------------------------------
			// get distance from point

			static float DistanceFromPoints(const glm::vec3& p0, const glm::vec3& p1)
			{
				glm::vec3 d = p1 - p0;
				return sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
			}

			// ---------------------------------------------------------------------------------
			// get squared distance from point

			static float DistanceFromPointsSquared(const glm::vec3& p0, const glm::vec3& p1)
			{
				glm::vec3 d = p1 - p0;
				return d.x * d.x + d.y * d.y + d.z * d.z;
			}

			// ---------------------------------------------------------------------------------
			// Checks to see if the three points given are
			// collinear by checking to see if each element of the cross
			// product is zero.

			static bool ArePointsCollinear(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,const float& eps = vml::math::EPSILON)
			{
				float u = (v3.z - v1.z) * (v2.y - v1.y) - (v2.z - v1.z) * (v3.y - v1.y);
				float v = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
				float w = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
				return((u > -eps && u < eps) &&
					   (v > -eps && v < eps) &&
					   (w > -eps && w < eps));
			}

			// ---------------------------------------------------------------------------------
			// inclusion of point in an axis aligned bounding box

			static int IsPointInAABBox(const glm::vec3& bmin, const glm::vec3& bmax, const glm::vec3& p)
			{
				if (p.x >= bmin.x && p.x <= bmax.x)
					if (p.y >= bmin.y && p.y <= bmax.y)
						if (p.z >= bmin.z && p.z <= bmax.z)
							return POINT_INSIDE;
				return POINT_OUTSIDE;
			}

			// ---------------------------------------------------------------------------------
			// Vertices must be given in this order
			// or function won't work correctly
			//  
			//          d0 ---------------- c0
			//          | \					| \
			//          |  \				|  \
			//          |	h0 ---------------- g0
			//          |	 |				|    |
			//			a0 ---------------- b0   |
			//			  \	 |				  \  |
			//			   \ |			       \ |
			//              e0 ---------------- f0

			static int IsPointInAOBB(const glm::vec3& p,
									 const glm::vec3& a0, const glm::vec3& b0, const glm::vec3& c0, const glm::vec3& d0,
									 const glm::vec3& e0, const glm::vec3& f0, const glm::vec3& g0, const glm::vec3& h0)
			{
				if (glm::dot(a0 - p, b0 - a0) < 0 &&
					glm::dot(p - b0, b0 - a0) < 0 &&
					glm::dot(a0 - p, d0 - a0) < 0 &&
					glm::dot(p - d0, d0 - a0) < 0 &&
					glm::dot(a0 - p, e0 - a0) < 0 &&
					glm::dot(p - e0, e0 - a0) < 0)
						return POINT_INSIDE;
				return POINT_OUTSIDE;
			}

			// ---------------------------------------------------------------------------------
			// Compute barycentric coordinates (u, v, w) for
			// point p with respect to triangle (a, b, c)

			static int IsPointInTriangle(const glm::vec3& p,
										 const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
										 const float& eps = vml::math::EPSILON)
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
				float v = (d11 * d20 - d01 * d21) * denom;
				float w = (d00 * d21 - d01 * d20) * denom;
				float u = 1.0f - v - w;
				if (u > -eps && u < 1 + eps && v > -eps && v < 1 + eps && w > -eps && w < 1 + eps)
					return POINT_INSIDE;
				return POINT_OUTSIDE;
			}

			// ---------------------------------------------------------------------------------
			// test inclusion between point and
			// sphere

			static bool IsPointInSphere(const glm::vec3& p, const glm::vec3& c, const float radius)
			{
				glm::vec3 r = p - c;
				return r.x * r.x + r.y * r.y + r.z * r.z <= radius * radius ? POINT_INSIDE : POINT_OUTSIDE;
			}

		}
	}
}

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{

			/////////////////////////////////////////////////////////////////////////////
			// Computes the distance between a point p and an AABB b

			static float SquaredClosestDistanceFromPointToAABB(const glm::vec3 &p, const glm::vec3 &bmin, const glm::vec3 &bmax)
			{
				float sqDist = 0.0f;
				float v;

				// For each axis count any excess distance outside box extents

				v = p.x;
				if (v < bmin.x) sqDist += (bmin.x - v) * (bmin.x - v);
				if (v > bmax.x) sqDist += (v - bmax.x) * (v - bmax.x);

				v = p.y;
				if (v < bmin.y) sqDist += (bmin.y - v) * (bmin.y - v);
				if (v > bmax.y) sqDist += (v - bmax.y) * (v - bmax.y);

				v = p.z;
				if (v < bmin.z) sqDist += (bmin.z - v) * (bmin.z - v);
				if (v > bmax.z) sqDist += (v - bmax.z) * (v - bmax.z);

				return sqDist;
			}

		}	// end of distances namespace

	}	// end of geo3d namespace

}	// end of vml namespace

