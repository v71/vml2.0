#pragma once

////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace geo3d
	{
		namespace collisions
		{

			/////////////////////////////////////////////////////////////////////
			// collision between a spehre and a triangle

			static int TriangleSphereCollision2(const glm::vec3& p, float radius,
											    const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& n,
											    vml::geo3d::collisions::MTV& mtv,
											    const float eps = vml::math::EPSILON)
			{
				// clean mtv 
				
				mtv.IsValid       = false;
				mtv.ContactPoint  = glm::vec3(0, 0, 0);
				mtv.ContactNormal = glm::vec3(0, 0, 0);
				mtv.Distance      = 0.0f;
				mtv.P0            = glm::vec3(0, 0, 0);
				mtv.P1            = glm::vec3(0, 0, 0);
				mtv.P2            = glm::vec3(0, 0, 0);
				mtv.Normal        = glm::vec3(0, 0, 0);

				glm::vec3 nearestpoint = vml::geo3d::distances::ClosestPointToTriangle2(a, b, c, p);

				glm::vec3 dir = p - nearestpoint;

				float sqdist = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
						
				if (sqdist < radius * radius)
				{
					float dist = sqrtf(sqdist);

					float magnitude = (radius - dist) * 1.01f;

					if (dist > -eps && dist < eps)
						return 0;

					dist = 1.0f / dist;

					dir.x *= dist;
					dir.y *= dist;
					dir.z *= dist;

					mtv.IsValid       = true;
					mtv.ContactPoint  = nearestpoint;
					mtv.Distance      = magnitude;
					mtv.ContactNormal = dir;
					mtv.P0            = a;
					mtv.P1            = b;
					mtv.P2            = c;
					mtv.Normal        = n;

					return 1;
				}

				return 0;
			}

			///////////////////////////////////////////
			// collision between an ellpsoid and a triangle

			static int TriangleEllipsoidCollision2(const glm::vec3& pos,
												   const glm::vec3& right,
												   const glm::vec3& up,
												   const glm::vec3& fwd,
												   const glm::vec3& ellipsoidradius,
												   const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
												   const glm::vec3& surfacenormal,
												   vml::geo3d::collisions::MTV& mtv,
												   const float eps = vml::math::EPSILON)
			{

				// clean mtv

				mtv.IsValid		  = false;
				mtv.ContactPoint  = glm::vec3(0, 0, 0);
				mtv.ContactNormal = glm::vec3(0, 0, 0);
				mtv.Distance	  = 0.0f;
				mtv.P0			  = glm::vec3(0, 0, 0);
				mtv.P1			  = glm::vec3(0, 0, 0);
				mtv.P2			  = glm::vec3(0, 0, 0);
				mtv.Normal		  = glm::vec3(0, 0, 0);

				// extract rotation matrix
				// we need only the rotational part
				// thus the matrix is multiplied by the inverse
				// scaling factors, also the matrix is 
				// scaled by the ellipsoid half sizes

				float halfxradius = 0.5f * ellipsoidradius.x;
				float halfyradius = 0.5f * ellipsoidradius.y;
				float halfzradius = 0.5f * ellipsoidradius.z;

				// scale parent matrix

				float M00 = right.x * halfxradius;
				float M01 = right.y * halfxradius;
				float M02 = right.z * halfxradius;

				float M10 = up.x * halfyradius;
				float M11 = up.y * halfyradius;
				float M12 = up.z * halfyradius;

				float M20 = fwd.x * halfzradius;
				float M21 = fwd.y * halfzradius;
				float M22 = fwd.z * halfzradius;

				// ellipsodi extremal points

				// glm::vec3 qa = glm::vec3( M00 + mptr[12], M01 + mptr[13], M02 + mptr[14]);
				// glm::vec3 qb = glm::vec3( M10 + mptr[12], M11 + mptr[13], M12 + mptr[14]);
				// glm::vec3 qc = glm::vec3( M20 + mptr[12], M21 + mptr[13], M22 + mptr[14]);

				// invert rotational matrix 
				// so the ellipsoid is now a 1 radius sphere
				// centered at 0,0,0

				float det = M00 * M11 * M22 - M00 * M21 * M12 +
							M10 * M21 * M02 - M10 * M01 * M22 +
							M20 * M01 * M12 - M20 * M11 * M02;

				if (det > -eps && det < eps)
					det = vml::math::EPSILON;

				det = 1.0f / det;

				float Invmm00 = (M11 * M22 - M21 * M12) * det;
				float Invmm01 = (M20 * M12 - M10 * M22) * det;
				float Invmm02 = (M10 * M21 - M20 * M11) * det;

				float Invmm10 = (M21 * M02 - M01 * M22) * det;
				float Invmm11 = (M00 * M22 - M20 * M02) * det;
				float Invmm12 = (M20 * M01 - M00 * M21) * det;

				float Invmm20 = (M01 * M12 - M02 * M11) * det;
				float Invmm21 = (M10 * M02 - M00 * M12) * det;
				float Invmm22 = (M00 * M11 - M10 * M01) * det;

				// map triangle vertices to sphere space

				glm::vec3 ta((p0.x - pos.x) * Invmm00 + (p0.y - pos.y) * Invmm01 + (p0.z - pos.z) * Invmm02,
							 (p0.x - pos.x) * Invmm10 + (p0.y - pos.y) * Invmm11 + (p0.z - pos.z) * Invmm12,
							 (p0.x - pos.x) * Invmm20 + (p0.y - pos.y) * Invmm21 + (p0.z - pos.z) * Invmm22);

				glm::vec3 tb((p1.x - pos.x) * Invmm00 + (p1.y - pos.y) * Invmm01 + (p1.z - pos.z) * Invmm02,
							 (p1.x - pos.x) * Invmm10 + (p1.y - pos.y) * Invmm11 + (p1.z - pos.z) * Invmm12,
							 (p1.x - pos.x) * Invmm20 + (p1.y - pos.y) * Invmm21 + (p1.z - pos.z) * Invmm22);

				glm::vec3 tc((p2.x - pos.x) * Invmm00 + (p2.y - pos.y) * Invmm01 + (p2.z - pos.z) * Invmm02,
							 (p2.x - pos.x) * Invmm10 + (p2.y - pos.y) * Invmm11 + (p2.z - pos.z) * Invmm12,
							 (p2.x - pos.x) * Invmm20 + (p2.y - pos.y) * Invmm21 + (p2.z - pos.z) * Invmm22);

				// find closest point to triangle

				glm::vec3 nearestpoint = vml::geo3d::distances::ClosestPointToTriangle2(ta, tb, tc, glm::vec3(0, 0, 0));

				// transformed sphere has origin as position
				// compute direction vector dir = glm::vec3(0,0,0)-nearestpoint;

				glm::vec3 dir = -nearestpoint;

				float sqdist = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;

				// transformed sphere has radius of 1
				 
				if (sqdist < 1.0f)
				{

					mtv.IsValid       = true;
					mtv.Distance      = (1.0f - sqrtf(sqdist));
					mtv.P0            = p0;
					mtv.P1            = p1;
					mtv.P2            = p2;
					mtv.Normal        = surfacenormal;
					
					// compute unmapped point

					mtv.ContactPoint.x = nearestpoint.x * M00 + nearestpoint.y * M10 + nearestpoint.z * M20 + pos.x;
					mtv.ContactPoint.y = nearestpoint.x * M01 + nearestpoint.y * M11 + nearestpoint.z * M21 + pos.y;
					mtv.ContactPoint.z = nearestpoint.x * M02 + nearestpoint.y * M12 + nearestpoint.z * M22 + pos.z;

					// compute contact normal direction

					glm::vec3 n = mtv.ContactPoint - pos;

					float dist = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);

					if (dist > -eps && dist < eps)
						dist = vml::math::EPSILON;

					dist = 1.0f / dist;

					n.x *= dist;
					n.y *= dist;
					n.z *= dist;

					mtv.ContactNormal = -n;

					return 1;
				}

				return 0;
			}

		}	//end of namespace boxes

	}	// end of geo3d namespace

}	// end of vml namespace

