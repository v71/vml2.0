#pragma once

////////////////////////////////////////////////////////////////////////////////////
// plane class

namespace vml
{
	namespace geo3d
	{
		namespace planes
		{

			////////////////////////////////////////////////////////////////////////////////////
			// plane class

			class Plane
			{

				private:

					// ------------------------------------------------------------------
					// private data 

					glm::vec3 P0;
					glm::vec3 P1;
					glm::vec3 P2;
					glm::vec3 P3;
					glm::vec3 Normal;
					float D;

				public:

					// ------------------------------------------------------------------
					// public function

					// ------------------------------------------------------------------
					// sets parameters for plane

					void Set(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
					{
						P0 = p0;
						P1 = p1;
						P2 = p2;
						P3 = p3;
						glm::vec3 u = P0 - P1;
						glm::vec3 v = P0 - P2;
						Normal = glm::normalize(glm::cross(u, v));
						D = -glm::dot(P0, Normal);
					}

					// ------------------------------------------------------------------
					// getters

					const glm::vec3 &GetP0() const { return P0; }
					const glm::vec3 &GetP1() const { return P1; }
					const glm::vec3 &GetP2() const { return P2; }
					const glm::vec3 &GetP3() const { return P3; }
					const glm::vec3 &GetNormal() const { return Normal; }

					// ------------------------------------------------------------------
					// ctor / dtor

					Plane()
					{
						P0     = glm::vec3(0, 0, 0);
						P1     = glm::vec3(0, 0, 0);
						P2     = glm::vec3(0, 0, 0);
						P3     = glm::vec3(0, 0, 0);
						Normal = glm::vec3(0, 0, 0);
						D      = 0.0f;
					}

					Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3& p3)
					{
						Set(p0, p1, p2, p3);
					}

					~Plane()
					{}

			};

		} // end of namespace planes

	} // end of namespace goe3d

}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// distance functions for plane class

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{
			////////////////////////////////////////////////////////////////////////////////////
			// distance from point to plane assumes plane is normalized

			static float PlaneDistanceFromPoint(const glm::vec3 &p0,const glm::vec3 &normal, 
											    const glm::vec3 &p)
			{
				return fabs(glm::dot(p - p0, normal));
			}

			////////////////////////////////////////////////////////////////////////////////////
			// Signed distance from point to plane assumes plane is normalized

			static float SignedPlaneDistanceFromPoint(const glm::vec3 &p0, const glm::vec3 &normal, 
										              const glm::vec3 &p)
			{
				return (glm::dot(p - p0, normal));
			}
	
		} // end of namespace planes

	} // end of namespace goe3d

}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// metrics functions for plane class

namespace vml
{
	namespace geo3d
	{
		namespace metrics
		{

			/////////////////////////////////////////////////////////////////////////////
			// flags 

			static const unsigned int OUTSIDE   = 0;
			static const unsigned int INSIDE    = 1;
			static const unsigned int BOUNDARY  = 2;
			static const unsigned int INTERSECT = 3;
			static const unsigned int BELOW     = 0;
			static const unsigned int ABOVE     = 1;

			////////////////////////////////////////////////////////////////////////////////////
			// classify point against plane

			static const int ClassifyPointVsPlane(const glm::vec3 &p0, const glm::vec3 &normal,
												  const glm::vec3 &p,
												  float eps = vml::math::EPSILON)
			{
				float d = glm::dot(p - p0, normal);
				if (d > eps)
					return INSIDE;
				else if (d < -eps)
					return OUTSIDE;
				return BOUNDARY;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// classify triangle against a plane
			// returns which halfspace the triangle is
			// inside for positive half space
			// outside for negative halfspace
			// intersect if triangles intersects the plane

			static int ClassifyTriangleVsPlane(const glm::vec3 &p0, const glm::vec3 &normal,
											   const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, 
				                               float eps = vml::math::EPSILON)
			{
				float num;
				int front, back, on;

				front = 0;
				back = 0;
				on = 0;

				num = glm::dot(normal, a - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
					on++;
				}

				num = glm::dot(normal, b - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
					on++;
				}

				num = glm::dot(normal, c - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
					on++;
				}

				if (on == 3)
					return BOUNDARY;
				else if (front == 3)
					return INSIDE;
				else if (back == 3)
					return OUTSIDE;

				return INTERSECT;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// classify axis aligned bounding box against a plane
			// returns which halfspace the axis aligned bounding box lies on
			// inside for positive half space
			// outside for negative halfspace
			// intersect if axis aligned box intersects the plane

			static int ClassifyAABBoxVsPlane(const glm::vec3 &p0, const glm::vec3 &normal,
											 const glm::vec3 &bmin, const glm::vec3 &bmax,
											 float eps = vml::math::EPSILON)
			{
				float num;
				int front, back;

				front = 0;
				back = 0;

				num = glm::dot(normal, bmin - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, bmax - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				if (front == 2)	
					return INSIDE;
				else if (back == 2)	
					return OUTSIDE;

				return INTERSECT;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// classify axis oriented bounding box against a plane
			// returns which halfspace the axis oriented bounding box lies on
			// inside for positive half space
			// outside for negative halfspace
			// intersect if axis oriented box intersects the plane

			static int ClassifyAOBBoxVsPlane(const glm::vec3 &p0, const glm::vec3 &normal,
											 const glm::vec3 &a0,
											 const glm::vec3 &b0,
											 const glm::vec3 &c0,
											 const glm::vec3 &d0,
											 const glm::vec3 &e0,
											 const glm::vec3 &f0,
											 const glm::vec3 &g0,
											 const glm::vec3 &h0,
										     float eps = vml::math::EPSILON)
			{
				float num;
				int front, back;

				front = 0;
				back = 0;

				num = glm::dot(normal, a0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, b0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, c0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, d0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, e0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, f0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, g0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, h0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				if (front == 8)
					return INSIDE;
				else if (back == 8)
					return OUTSIDE;

				return INTERSECT;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// classify sphere against a plane
			// returns which halfspace the sphere box lies on
			// inside for positive half space
			// outside for negative halfspace
			// intersect if axis oriented box intersects the plane

			static int ClassifySphereVsPlane(const glm::vec3 &pos,
											 const float radius,
											 const glm::vec3 &p0,
											 const glm::vec3 &normal,
											 float eps = vml::math::EPSILON)
			{
				float dpmax = glm::dot(normal, pos - p0) - radius;

				if (dpmax > 0)
					return INSIDE;

				if (dpmax < -2.0f*radius)
					return OUTSIDE;

				return INTERSECT;

			}

			////////////////////////////////////////////////////////////////////////////////
			// Clips a segment against plane

			static int ClassifyLineVsPlane(const glm::vec3 &a, const glm::vec3& b,
										   const glm::vec3 &p0, const glm::vec3 &n,
										   glm::vec3 &q,
										   float eps = vml::math::EPSILON)
			{

				float b1 = glm::dot(p0 - a, n);
				float b2 = glm::dot(p0 - b, n);

				// segment is all below plane

				if (b1 < -eps && b2 < -eps)
					return BELOW;

				// segment is all above plane

				if (b1 > -eps && b2 > -eps)
					return ABOVE;

				// segment is intersected

				float denum = b1 - b2;

				if (denum > -eps && denum < eps)
					denum = eps;

				// computing intersection point

				q = a + b1 * (b - a) / denum;

				return INTERSECT;
			}

		} // end of namespace planes

	} // end of namespace goe3d

}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// Intersection functions for plane class

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{

			static const unsigned int LINE_DO_NOT_INTERSECTS_PLANE = 0;
			static const unsigned int LINE_DO_INTERSECTS_PLANE		   = 1;

			////////////////////////////////////////////////////////////////////////////////////
			// intersection beetween plane and ray returns point of intersection

			static glm::vec3 RayVsPlane(const glm::vec3 &p0, const glm::vec3 &normal, 
										const glm::vec3 &a, const glm::vec3 &b,
										float eps=vml::math::EPSILON)
			{
				glm::vec3 d = b - a;
				float denum = glm::dot(normal, d);
				if (denum > -eps && denum < eps)
					denum = eps;
				float intr = glm::dot(normal, p0 - a);
				intr /= denum;
				return a + intr * d;
			}

			////////////////////////////////////////////////////////////////////////////////
			// intersection beetween plane and line returns point of intersection

			static int LineVsPlane(const glm::vec3 &a, const glm::vec3 &b,
								   const glm::vec3 &p0, const glm::vec3 &n,
								   glm::vec3 &q,
								   float eps = vml::math::EPSILON)
			{
				glm::vec3 d = b - a;
				float denum = glm::dot(n, d);
				if (denum > -eps && denum < eps)
					denum = eps;
				float Intr = glm::dot(n, p0 - a);
				Intr /= denum;
				if (Intr < -eps || Intr > 1.0f + eps) 
					return LINE_DO_NOT_INTERSECTS_PLANE;
				q = a + Intr * d;
				return  LINE_DO_INTERSECTS_PLANE;
			}

		} // end of namespace planes

	} // end of namespace goe3d

}	// end of vml namespace


