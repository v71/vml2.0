#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{
		
			//////////////////////////////////////////////////////////////
			// ray-axis oriented bounding box intersections flags

			static const unsigned int TRIANGLE_DO_NOT_INTERSECTS_AABBOX = 0;
			static const unsigned int TRIANGLE_INTERSECTS_AABBOX		= 1;

			static const unsigned int TRIANGLE_DO_NOT_INTERSECTS_RAY = 0;
			static const unsigned int TRIANGLE_INTERSECTS_RAY		 = 1;

			//////////////////////////////////////////////////////
			//    use separating axis theorem to test overlap 
			//    between triangle and box 
			//    need to test for overlap in these directions: 
			//    1) the {x,y,z}-directions (actually, since we use 
			//       the AABB of the triangle 
			//       we do not even need to test these) 
			//    2) normal of the triangle 
			//    3) crossproduct(edge from tri, {x,y,z}-directin) 
			//       this gives 3x3=9 more tests

			static int TriangleVsAABB(const glm::vec3 &boxcenter,
									  const glm::vec3 &boxhalfsize,
									  const glm::vec3 &trivert0,
									  const glm::vec3 &trivert1,
									  const glm::vec3 &trivert2,
									  const glm::vec3 &normal)
			{

				float min, max;
				float p0, p1, p2;
				float d;
				float fex, fey, fez;
				float e0[3] = { 0 };
				float e1[3] = { 0 };
				float e2[3] = { 0 };
				float v0[3] = { 0 };
				float v1[3] = { 0 };
				float v2[3] = { 0 };
				float vmin[3] = { 0 };
				float vmax[3] = { 0 };

				// center triangle at box center

				v0[0] = trivert0.x - boxcenter.x;
				v0[1] = trivert0.y - boxcenter.y;
				v0[2] = trivert0.z - boxcenter.z;

				v1[0] = trivert1.x - boxcenter.x;
				v1[1] = trivert1.y - boxcenter.y;
				v1[2] = trivert1.z - boxcenter.z;

				v2[0] = trivert2.x - boxcenter.x;
				v2[1] = trivert2.y - boxcenter.y;
				v2[2] = trivert2.z - boxcenter.z;

				// compute triangle edges 

				e0[0] = v1[0] - v0[0];
				e0[1] = v1[1] - v0[1];
				e0[2] = v1[2] - v0[2];

				e1[0] = v2[0] - v1[0];
				e1[1] = v2[1] - v1[1];
				e1[2] = v2[2] - v1[2];

				e2[0] = v0[0] - v2[0];
				e2[1] = v0[1] - v2[1];
				e2[2] = v0[2] - v2[2];

				//  test the 9 tests for axis overlapping 

				// axis test x01

				fex = fabs(e0[0]);
				fey = fabs(e0[1]);
				fez = fabs(e0[2]);

				p0 = e0[2] * v0[1] - e0[1] * v0[2];
				p2 = e0[2] * v2[1] - e0[1] * v2[2];

				min = p2;
				max = p0;

				if (p0 < p2)
				{
					min = p0;
					max = p2;
				}

				d = fez * boxhalfsize[1] + fey * boxhalfsize[2];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test y02

				p0 = -e0[2] * v0[0] + e0[0] * v0[2];
				p2 = -e0[2] * v2[0] + e0[0] * v2[2];

				min = p2;
				max = p0;

				if (p0 < p2) { min = p0; max = p2; }

				d = fez * boxhalfsize[0] + fex * boxhalfsize[2];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test z12

				p1 = e0[1] * v1[0] - e0[0] * v1[1];
				p2 = e0[1] * v2[0] - e0[0] * v2[1];

				min = p1;
				max = p2;

				if (p2 < p1)
				{
					min = p2;
					max = p1;
				}

				d = fey * boxhalfsize[0] + fex * boxhalfsize[1];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				fex = fabs(e1[0]);
				fey = fabs(e1[1]);
				fez = fabs(e1[2]);

				// axis test x01

				p0 = e1[2] * v0[1] - e1[1] * v0[2];
				p2 = e1[2] * v2[1] - e1[1] * v2[2];

				min = p2;
				max = p0;

				if (p0 < p2)
				{
					min = p0;
					max = p2;
				}

				d = fez * boxhalfsize[1] + fey * boxhalfsize[2];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test y02

				p0 = -e1[2] * v0[0] + e1[0] * v0[2];
				p2 = -e1[2] * v2[0] + e1[0] * v2[2];

				min = p2;
				max = p0;

				if (p0 < p2)
				{
					min = p0;
					max = p2;
				}

				d = fez * boxhalfsize[0] + fex * boxhalfsize[2];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test z0

				p0 = e1[1] * v0[0] - e1[0] * v0[1];
				p1 = e1[1] * v1[0] - e1[0] * v1[1];

				min = p1;
				max = p0;

				if (p0 < p1)
				{
					min = p0;
					max = p1;
				}

				d = fey * boxhalfsize[0] + fex * boxhalfsize[1];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				fex = fabs(e2[0]);
				fey = fabs(e2[1]);
				fez = fabs(e2[2]);

				// axis test x02

				p0 = e2[2] * v0[1] - e2[1] * v0[2];
				p1 = e2[2] * v1[1] - e2[1] * v1[2];

				min = p1;
				max = p0;

				if (p0 < p1)
				{
					min = p0;
					max = p1;
				}

				d = fez * boxhalfsize[1] + fey * boxhalfsize[2];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test y01

				p0 = -e2[2] * v0[0] + e2[0] * v0[2];
				p1 = -e2[2] * v1[0] + e2[0] * v1[2];

				min = p1;
				max = p0;

				if (p0 < p1)
				{
					min = p0;
					max = p1;
				}

				d = fez * boxhalfsize[0] + fex * boxhalfsize[2];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// axis test z12 

				p1 = e2[1] * v1[0] - e2[0] * v1[1];
				p2 = e2[1] * v2[0] - e2[0] * v2[1];

				min = p1;
				max = p2;

				if (p2 < p1)
				{
					min = p2;
					max = p1;
				}

				d = fey * boxhalfsize[0] + fex * boxhalfsize[1];

				if (min > d || max < -d) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				//  first test overlap in the {x,y,z}-directions 
				//  find min, max of the triangle each direction, 
				//  and test for overlap in 
				//  that direction -- this is equivalent to testing 
				//  a minimal AABB around 
				//  the triangle against the AABB 

				// test in X-direction 

				min = v0[0];
				max = v0[0];

				if (v1[0] < min) min = v1[0];
				if (v1[0] > max) max = v1[0];
				if (v2[0] < min) min = v2[0];
				if (v2[0] > max) max = v2[0];

				if (min > boxhalfsize[0] || max < -boxhalfsize[0]) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// test in Y-direction 

				min = v0[1];
				max = v0[1];

				if (v1[1] < min) min = v1[1];
				if (v1[1] > max) max = v1[1];
				if (v2[1] < min) min = v2[1];
				if (v2[1] > max) max = v2[1];

				if (min > boxhalfsize[1] || max < -boxhalfsize[1]) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				// test in Z-direction 

				min = v0[2];
				max = v0[2];

				if (v1[2] < min) min = v1[2];
				if (v1[2] > max) max = v1[2];
				if (v2[2] < min) min = v2[2];
				if (v2[2] > max) max = v2[2];

				if (min > boxhalfsize[2] || max < -boxhalfsize[2]) 
					return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				//  test if the box intersects the 
				//  plane of the triangle 

				// test x

				if (normal[0] > 0.0f)
				{
					vmin[0] = -boxhalfsize[0];
					vmax[0] = boxhalfsize[0];
				}
				else
				{
					vmin[0] = boxhalfsize[0];
					vmax[0] = -boxhalfsize[0];
				}

				// test y

				if (normal[1] > 0.0f)
				{
					vmin[1] = -boxhalfsize[1];
					vmax[1] = boxhalfsize[1];
				}
				else
				{
					vmin[1] = boxhalfsize[1];
					vmax[1] = -boxhalfsize[1];
				}

				// test z

				if (normal[2] > 0.0f)
				{
					vmin[2] = -boxhalfsize[2];
					vmax[2] = boxhalfsize[2];
				}
				else
				{
					vmin[2] = boxhalfsize[2];
					vmax[2] = -boxhalfsize[2];
				}

				// test max and min 

				d = -(normal[0] * v0[0] + normal[1] * v0[1] + normal[2] * v0[2]);

				if (normal[0] * vmin[0] +
					normal[1] * vmin[1] +
					normal[2] * vmin[2] + d > 0.0f) 
						return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;

				if (normal[0] * vmax[0] +
					normal[1] * vmax[1] +
					normal[2] * vmax[2] + d > 0.0f) 
						return TRIANGLE_INTERSECTS_AABBOX;

				// box and triangle don't overlap 

				return TRIANGLE_DO_NOT_INTERSECTS_AABBOX;
			}

			/////////////////////////////////////////////////
			// intersection beetween triangle and line
			// returns point of intersection
			// computes normal
			
			static bool TriangleVsRay(const glm::vec3 &a,  const glm::vec3 &b,  const glm::vec3 &c, 
								      const glm::vec3 &ra, const glm::vec3 &rb, glm::vec3 &p, 
									  float &t,
				                      const float eps = vml::math::EPSILON)
			{
				glm::vec3 v, n, d;
				float denum;

				// compute normal

				n = cross(b - a, c - a);

				n = glm::normalize(n);

				// compute intersection with 
				// infinite plane

				d = rb - ra;

				t = dot(a - ra, n);

				denum = dot(n, d);

				if (fabs(denum) < eps)
					denum = eps;

				t /= denum;

				// store intersection point

				p = ra + t * d;

				// check which side the point lies on
				// respect segment a-b
				// computes the side tanget using
				// cross product between segmetn a-b
				// and normal

				v = cross(b - a, n);

				if (dot(v, a - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment c-b
				// computes the side tanget using
				// cross product between segmetn c-b
				// and normal

				v = cross(c - b, n);

				if (dot(v, b - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment a-c
				// computes the side tanget using
				// cross product between segmetn a-c
				// and normal

				v = cross(a - c, n);

				if (dot(v, c - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				return TRIANGLE_INTERSECTS_RAY;
			}
			
			/////////////////////////////////////////////////
			// intersection beetween triangle and line segment
			// returns point of intersection
			// computes normal

			static bool TriangleVSLine(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c,
									   const glm::vec3 &ra, const glm::vec3 &rb,
									   glm::vec3 &p,
									   float &t,
									   const float eps = vml::math::EPSILON)
			{
				glm::vec3 V, N, D;
				float denum;

				// compute normal

				N = glm::cross(b - a, c - a);

				N = glm::normalize(N);

				// compute intersection with 
				// infinite plane

				D = rb - ra;

				t = dot(a - ra, N);

				denum = dot(N, D);

				if (fabs(denum) < eps)
					denum = eps;

				t /= denum;

				if (t < -eps || t > 1.0f + eps)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// store intersection point

				p = ra + t * D;

				// check which side the point lies on
				// respect segment a-b
				// computes the side tanget using
				// cross product between segmetn a-b
				// and normal

				V = cross(b - a, N);

				if (dot(V, a - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment c-b
				// computes the side tanget using
				// cross product between segmetn c-b
				// and normal

				V = cross(c - b, N);

				if (dot(V, b - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				// check whcih side the point lies on
				// respect segment a-c
				// computes the side tanget using
				// cross product between segmetn a-c
				// and normal

				V = cross(a - c, N);

				if (dot(V, c - p) < 0)
					return TRIANGLE_DO_NOT_INTERSECTS_RAY;

				return TRIANGLE_INTERSECTS_RAY;
			}

		}
	}
}

