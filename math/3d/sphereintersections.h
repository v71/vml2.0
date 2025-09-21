#pragma once

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{

			/////////////////////////////////////////////////////////////////////////////
			// intersection return codes

			static const unsigned int SPHERE_DISJOINT			 = 0;
			static const unsigned int SPHERE_FIRST_INSIDE_SECOND = 1;
			static const unsigned int SPHERE_SECOND_INSIDE_FIRST = 2;
			static const unsigned int SPHERE_INTERSECTED		 = 3;

			static const unsigned int DO_NOT_INTERSECTS			 = 0;
			static const unsigned int DO_INTERSECTS				 = 1;

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding spheres

			static int SphereVsSphere(const glm::vec3 &pos1, float radius1, const glm::vec3 &pos2, float radius2)
			{
				glm::vec3 d = pos2 - pos1;
				float r = d.x*d.x + d.y*d.y + d.z*d.z;
				float r1sr2s = radius1 * radius1 + radius2 * radius2;
				float r1r2 = 2.0f*radius1*radius2;

				// check if the first sphere is entirely inside the second sphere or viceversa

				if (radius1 > radius2)
				{
					if (r < r1sr2s - r1r2) return SPHERE_FIRST_INSIDE_SECOND;
				}
				else
				{
					if (r < r1sr2s - r1r2) return SPHERE_SECOND_INSIDE_FIRST;
				}

				// check if spheres are disjoint from each other

				if (r > r1sr2s + r1r2) return SPHERE_DISJOINT;

				// if we get here spheres are intersecting each other

				return SPHERE_INTERSECTED;

			}

			/////////////////////////////////////////////////
			// tests if a ray intersects a sphere
			// uses a start and end point as ray description
			// and returns points of intersection
			
			static int SphereVsRay(const glm::vec3 &center, float radius,
								   const glm::vec3 &start, const glm::vec3 &end,
								   glm::vec3 &p,
								   glm::vec3 &q,
								   float& tmin,
								   float& tmax)
			{
		
				glm::vec3 d = end - start;
				glm::vec3 r = start - center;
				float a = d.x*d.x + d.y*d.y + d.z*d.z;
				float b = glm::dot(r, d);
				float c = glm::dot(r, r) - radius * radius;
				float delta = b * b - a * c;
				if (delta < 0.0f)
					return DO_NOT_INTERSECTS;
				delta = sqrtf(delta);
				float inva = 1.0f / a;
				tmin = (-b + delta)*inva;
				tmax = (-b - delta)*inva;
				if (tmax < tmin)
				{
					float temp = tmin;
					tmin = tmax;
					tmax = temp;
				}
				p = start + tmin * d;
				q = start + tmax * d;
				return DO_INTERSECTS;
			}
			
			//////////////////////////////////////////////////////////////////////////
			// ellipsoid ray intersection

			static int EllipsoidVsRay(const glm::vec3 &pos,
									  const glm::vec3 &right,
									  const glm::vec3 &up,
									  const glm::vec3 &fwd,
									  const glm::vec3 &ellipsoidradius,
									  const glm::vec3 &a,
									  const glm::vec3 &b,
									  float &t1,
									  float &t2,
									  glm::vec3 &u1,
									  glm::vec3 &u2,
									  const float eps = vml::math::EPSILON)
			{

				// extract rotation matrix
				// we need only the rotational part
				// thus the matrix is multiplied by the inverse
				// scaling factors, also the matrix is 
				// scaled by the ellipsoid half sizes

				float halfxradius = 0.5f * ellipsoidradius.x;
				float halfyradius = 0.5f * ellipsoidradius.y;
				float halfzradius = 0.5f * ellipsoidradius.z;

				// scale parent matrix

				float M00 = right.x  * halfxradius;
				float M01 = right.y  * halfxradius;
				float M02 = right.z  * halfxradius;

				float M10 = up.x     * halfyradius;
				float M11 = up.y     * halfyradius;
				float M12 = up.z     * halfyradius;

				float M20 = fwd.x    * halfzradius;
				float M21 = fwd.y    * halfzradius;
				float M22 = fwd.z    * halfzradius;

				// ellipsodi extremal points

				// glm::vec3 qa = glm::vec3( M00 + mptr[12], M01 + mptr[13], M02 + mptr[14]);
				// glm::vec3 qb = glm::vec3( M10 + mptr[12], M11 + mptr[13], M12 + mptr[14]);
				// glm::vec3 qc = glm::vec3( M20 + mptr[12], M21 + mptr[13], M22 + mptr[14]);

				// invert rotational matrix 
				// so the ellipsoid is now a 1 radius sphere

				float det = M00 * M11 * M22 - M00 * M21 * M12 +
							M10 * M21 * M02 - M10 * M01 * M22 +
							M20 * M01 * M12 - M20 * M11 * M02;

				if (det > -eps && det < eps)
					det = eps;

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

				// map ray vertices to ellipsoid space

				glm::vec3 ta((a.x - pos.x)*Invmm00 + (a.y - pos.y)*Invmm01 + (a.z - pos.z)*Invmm02,
							 (a.x - pos.x)*Invmm10 + (a.y - pos.y)*Invmm11 + (a.z - pos.z)*Invmm12,
							 (a.x - pos.x)*Invmm20 + (a.y - pos.y)*Invmm21 + (a.z - pos.z)*Invmm22);

				glm::vec3 tb((b.x - pos.x)*Invmm00 + (b.y - pos.y)*Invmm01 + (b.z - pos.z)*Invmm02,
							 (b.x - pos.x)*Invmm10 + (b.y - pos.y)*Invmm11 + (b.z - pos.z)*Invmm12,
							 (b.x - pos.x)*Invmm20 + (b.y - pos.y)*Invmm21 + (b.z - pos.z)*Invmm22);

				// collision detection between ray and sphere

				glm::vec3 d = tb - ta;

				float k1 = d.x *  d.x + d.y *  d.y + d.z *  d.z;
				float k2 = d.x * ta.x + d.y * ta.y + d.z * ta.z;
				float k3 = ta.x * ta.x + ta.y * ta.y + ta.z * ta.z - 1;
				
				float delta =  k2 * k2 - k1 * k3;
				
				if (delta > eps)
				{

					delta = sqrtf(delta);

					if (k1 > -eps && k1 < eps)
						k1 = eps;

					float invk1 = 1.0f / k1;

					t1 = (-k2 + delta) * invk1;
					t2 = (-k2 - delta) * invk1;

					if (t2 < t1)
					{
						float temp = t1;
						t1 = t2;
						t2 = temp;
					}

					glm::vec3 p1 = ta + t1 * d;
					glm::vec3 p2 = ta + t2 * d;

					// unmap point to world space

					u1.x = p1.x*M00 + p1.y*M10 + p1.z*M20 + pos.x;
					u1.y = p1.x*M01 + p1.y*M11 + p1.z*M21 + pos.y;
					u1.z = p1.x*M02 + p1.y*M12 + p1.z*M22 + pos.z;

					u2.x = p2.x*M00 + p2.y*M10 + p2.z*M20 + pos.x;
					u2.y = p2.x*M01 + p2.y*M11 + p2.z*M21 + pos.y;
					u2.z = p2.x*M02 + p2.y*M12 + p2.z*M22 + pos.z;

					return DO_INTERSECTS;

				}

				return DO_NOT_INTERSECTS;

			}

		} // end of intersections namespace

	}	// end of geo3d namespace

}	// end of vml namespace
