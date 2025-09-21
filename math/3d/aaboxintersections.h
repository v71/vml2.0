#pragma once

// TO DO :

// axis aligned bounding box against oriented bounding box

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding boxes

			static const unsigned int AABBOX_DISJOINT			 = 0;
			static const unsigned int AABBOX_FIRST_INSIDE_SECOND = 1;
			static const unsigned int AABBOX_SECOND_INSIDE_FIRST = 2;
			static const unsigned int AABBOX_INTERSECTED		 = 3;

			/////////////////////////////////////////////////////////////////////////////
			// intersection between bounding boxe - sphere

			static const unsigned int SPHERE_OUTSIDE_AABBOX      = 0;
			static const unsigned int SPHERE_INSIDE_AABBOX       = 1;
			static const unsigned int SPHERE_INTERSECTS_AABBOX   = 2;
			static const unsigned int AABBOX_INSIDE_SPHERE       = 3;

			/////////////////////////////////////////////////////////////////////////////
			// checks if ray intersect an axis aligned bounding box

			static const unsigned int AABBOX_RAY_OUTSIDE         = 0;
			static const unsigned int AABBOX_RAY_INTERSECTS      = 1;
			static const unsigned int AABBOX_RAY_INSIDEAABBOX    = 2;

			/////////////////////////////////////////////////////////////////////////////
			// checks if ray intersect an axis aligned bounding box

			static const unsigned int AOBBOX_RAY_OUTSIDE	     = 0;
			static const unsigned int AOBBOX_RAY_INTERSECTS      = 1;

			/////////////////////////////////////////////////////////////////////////////
			// bounding boxe intersection tests 

			static unsigned int AABBoxVsAABBox(const glm::vec3 &bmin1, const glm::vec3 &bmax1, const glm::vec3 &bmin2, const glm::vec3 &bmax2)
			{
				// check if the first bounding box is entirely inside the second bounding box

				if ((bmin1.x >= bmin2.x && bmax1.x <= bmax2.x) &&
					(bmin1.y >= bmin2.y && bmax1.y <= bmax2.y) &&
					(bmin1.z >= bmin2.z && bmax1.z <= bmax2.z))
						return AABBOX_FIRST_INSIDE_SECOND;

				// check if the second bounding box is entirely inside the first bounding box

				if ((bmin2.x >= bmin1.x && bmax2.x <= bmax1.x) &&
					(bmin2.y >= bmin1.y && bmax2.y <= bmax1.y) &&
					(bmin2.z >= bmin1.z && bmax2.z <= bmax1.z))
						return AABBOX_SECOND_INSIDE_FIRST;

				// check if bounding box is out of second bounding box

				if ((bmax2.x < bmin1.x || bmin2.x > bmax1.x) ||
					(bmax2.y < bmin1.y || bmin2.y > bmax1.y) ||
					(bmax2.z < bmin1.z || bmin2.z > bmax1.z))
						return AABBOX_DISJOINT;

				// if we get here bounding boxes are intersecting each ohter

				return AABBOX_INTERSECTED;
			}

			/////////////////////////////////////////////////////////////////////////////
			// intersection between sphere and bounding box

			static int AABBoxVsSphere(const glm::vec3 &bmin, const glm::vec3 &bmax, const glm::vec3 &p0, const float radius)
			{
				// cache values

				float uax = p0.x - bmin.x;
				float uay = p0.y - bmin.y;
				float uaz = p0.z - bmin.z;
				float ubx = p0.x - bmax.x;
				float uby = p0.y - bmax.y;
				float ubz = p0.z - bmax.z;

				// checks if sphere is totally inside the bounding box

				if (uax + radius >= 0 && ubx + radius <= 0)
				if (uax - radius >= 0 && ubx - radius <= 0)
				if (uay + radius >= 0 && uby + radius <= 0)
				if (uay - radius >= 0 && uby - radius <= 0)
				if (uaz + radius >= 0 && ubz + radius <= 0)
				if (uaz + radius >= 0 && ubz - radius <= 0)
						return SPHERE_INSIDE_AABBOX;

				// check if bounding box is totally outside the sphere

				glm::vec3 v0(0,0,0);

				float dax = uax * uax;
				float day = uay * uay;
				float daz = uaz * uaz;
				float dbx = ubx * ubx;
				float dby = uby * uby;
				float dbz = ubz * ubz;

				v0.x = bmin.x;
				v0.y = bmin.y;
				v0.z = bmin.z;

				if (dbx >= dax) { v0.x = bmax.x; dax = dbx; }
				if (dby >= day) { v0.y = bmax.y; day = dby; }
				if (dbz >= daz) { v0.z = bmax.z; daz = dbz; }

				if (dax + day + daz < radius * radius)
					return SPHERE_INTERSECTS_AABBOX;

				// check if sphere and bounding box are intersecting
				// note that uax, uay,uaz,ubx,uby,ubz squared values
				// must be recomputed since we come from the other
				// step and values may get overwritten by the 
				// previous checks

				float dmin = 0.0f;

				if (uax < 0) dmin += uax * uax;
				else if (ubx > 0) dmin += ubx * ubx;

				if (uay < 0) dmin += uay * uay;
				else if (uby > 0) dmin += uby * uby;

				if (uaz < 0) dmin += uaz * uaz;
				else if (ubz > 0) dmin += ubz * ubz;

				if (dmin < radius * radius)
					return AABBOX_INSIDE_SPHERE;

				return SPHERE_OUTSIDE_AABBOX;
			}

			/////////////////////////////////////////////////////////////////////////////
			// intersection between infinit ray and bouding box
			// min is bounding box min
			// max is bounding box max
			// a is firt line point
			// b is sencond line point
			// p and q are intersection points

			static unsigned int AABBoxVsRay(const glm::vec3 &min, const glm::vec3 &max,
											const glm::vec3 &a, const glm::vec3 &b,
											glm::vec3 &p, glm::vec3 &q,
											float &tmin,float &tmax,
											const float eps = vml::math::EPSILON)
			{
				glm::vec3 d = b - a;

				if (d.x > -eps && d.x < eps)
					d.x = eps;

				float txmin = (min.x - a.x) / d.x;
				float txmax = (max.x - a.x) / d.x;

				if (txmin > txmax)
				{
					float t = txmin;
					txmin = txmax;
					txmax = t;
				}

				if (d.y > -eps && d.y < eps)
					d.y = eps;

				float tymin = (min.y - a.y) / d.y;
				float tymax = (max.y - a.y) / d.y;

				if (tymin > tymax)
				{
					float t = tymin;
					tymin = tymax;
					tymax = t;
				}

				if ((txmin > tymax) || (tymin > txmax))
					return AABBOX_RAY_OUTSIDE;

				if (tymin > txmin)
					txmin = tymin;

				if (tymax < txmax)
					txmax = tymax;

				if (d.z > -eps && d.z < eps)
					d.z = eps;

				float tzmin = (min.z - a.z) / d.z;
				float tzmax = (max.z - a.z) / d.z;

				if (tzmin > tzmax)
				{
					float t = tzmin;
					tzmin = tzmax;
					tzmax = t;
				}

				if ((txmin > tzmax) || (tzmin > txmax))
					return AABBOX_RAY_OUTSIDE;

				if (tzmin > txmin)
					txmin = tzmin;

				if (tzmax < txmax)
					txmax = tzmax;

				tmin = txmin;
				tmax = txmax;

				p = a + tmin * d;
				q = a + tmax * d;

				return AABBOX_RAY_INTERSECTS;
			}

			/////////////////////////////////////////////////////////////////////////////
			// intersection between infinit ray and bouding box
			// min is bounding box min
			// max is bounding box max
			// a is firt line point
			// b is sencond line point
			// p and q are intersection points

			static unsigned int AABBoxVsLine(const glm::vec3 &min, const glm::vec3 &max,
											 const glm::vec3 &a, const glm::vec3 &b,
											 glm::vec3 &p, glm::vec3 &q,
											 float &txmin, float &txmax,
											 const float eps = vml::math::EPSILON)
			{
				// line bounding box
				
				glm::vec3 rmin(0, 0, 0);
				glm::vec3 rmax(0, 0, 0);

				if (a.x < b.x) { rmin.x = a.x; rmax.x = b.x; } else { rmin.x = b.x; rmax.x = a.x; }
				if (a.y < b.y) { rmin.y = a.y; rmax.y = b.y; } else { rmin.y = b.y; rmax.y = a.y; }
				if (a.z < b.z) { rmin.z = a.z; rmax.z = b.z; } else { rmin.z = b.z; rmax.z = a.z; }

				// chck if line is contained inside bbounding box
				
				if ((rmin.x >= min.x && rmax.x <= max.x) &&
					(rmin.y >= min.y && rmax.y <= max.y) &&
					(rmin.z >= min.z && rmax.z <= max.z))
				{
					p = a;
					q = b;
					return AABBOX_RAY_INSIDEAABBOX;
				}
				
				// check if bounding box is out of second bounding box

				if ((max.x < rmin.x || min.x > rmax.x) ||
					(max.y < rmin.y || min.y > rmax.y) ||
					(max.z < rmin.z || min.z > rmax.z))
						return AABBOX_RAY_OUTSIDE;
					
				// check for intersection

				glm::vec3 d = b - a;

				if (d.x > -eps && d.x < eps)
					d.x = eps;

				txmin = (min.x - a.x) / d.x;
				txmax = (max.x - a.x) / d.x;

				if (txmin > txmax)
				{
					float t = txmin;
					txmin = txmax;
					txmax = t;
				}

				if (d.y > -eps && d.y < eps)
					d.y = eps;

				float tymin = (min.y - a.y) / d.y;
				float tymax = (max.y - a.y) / d.y;

				if (tymin > tymax)
				{
					float t = tymin;
					tymin = tymax;
					tymax = t;
				}

				if ((txmin > tymax) || (tymin > txmax))
					return AABBOX_RAY_OUTSIDE;

				if (tymin > txmin)
					txmin = tymin;

				if (tymax < txmax)
					txmax = tymax;

				if (d.z > -eps && d.z < eps)
					d.z = eps;

				float tzmin = (min.z - a.z) / d.z;
				float tzmax = (max.z - a.z) / d.z;

				if (tzmin > tzmax)
				{
					float t = tzmin;
					tzmin = tzmax;
					tzmax = t;
				}

				if ((txmin > tzmax) || (tzmin > txmax))
					return AABBOX_RAY_OUTSIDE;

				if (tzmin > txmin)
					txmin = tzmin;

				if (tzmax < txmax)
					txmax = tzmax;
				
				if (txmin > txmax) 
				{
					float t = txmin;
					txmin = txmax;
					txmax = t;
				}
				
				if (txmin < -eps)     txmin = 0.0f;
				if (txmax > 1.0f+eps) txmax = 1.0f;
				
				p = a + txmin * d;
				q = a + txmax * d;
				
				return AABBOX_RAY_INTERSECTS;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Ray origin, in world space
			// Ray direction (NOT target position!), in world space. Must be normalize()'d.
			// Minimum X,Y,Z coords of the mesh when not transformed at all.
			// Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
			// Transformation applied to the mesh (which will thus be also applied to its bounding box)
			// Output : distance between ray_origin and the intersection with the OBB

			static unsigned int AOBBoxVsRay( const glm::vec3 &start,
										     const glm::vec3 &end,    
										     const glm::vec3 &aabb_min,          
										     const glm::vec3 &aabb_max,          
										     const glm::mat4 &ModelMatrix,       
											 glm::vec3& p, glm::vec3& q,
											 float& tmin,float& tmax,
										     const float eps = vml::math::EPSILON)
			{
				tmin = -FLT_MAX;
				tmax =  FLT_MAX;

				glm::vec3 dir = end - start;

				glm::vec3 obbpos(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);
				
				glm::vec3 delta = obbpos- start;

				// Test intersection with the 2 planes perpendicular to the OBB's X axis
				
				{
					glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
				
					float e = glm::dot(xaxis, delta);
					float f = glm::dot(dir, xaxis);

					if ( fabs(f) > eps )
					{ 
						// Standard case

						float t1 = (e+aabb_min.x)/f; // Intersection with the "left" plane
						float t2 = (e+aabb_max.x)/f; // Intersection with the "right" plane
				
						// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

						// We want t1 to represent the nearest intersection, 
						// so if it's not the case, invert t1 and t2
						
						if (t1>t2)
						{
							// swap t1 and t2

							float w=t1;
							t1=t2;
							t2=w; 
						}

						// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
						
						if ( t2 < tmax )
							tmax = t2;
						// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
						
						if ( t1 > tmin )
							tmin = t1;

						// And here's the trick :
						// If "far" is closer than "near", then there is NO intersection.
						// See the images in the tutorials for the visual explanation.
						
						if (tmax < tmin )
							return AOBBOX_RAY_OUTSIDE;

					}
					else
					{ 
						// Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
					
						if(-e+aabb_min.x > -eps || -e+aabb_max.x < eps)
							return AOBBOX_RAY_OUTSIDE;
					}
				}

				// Test intersection with the 2 planes perpendicular to the OBB's Y axis
				// Exactly the same thing than above.
				
				{
					glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
					
					float e = glm::dot(yaxis, delta);
					float f = glm::dot(dir, yaxis);

					if ( fabs(f) > eps)
					{

						float t1 = (e+aabb_min.y)/f;
						float t2 = (e+aabb_max.y)/f;

						if (t1>t2)
						{
							// swap t1 and t2

							float w=t1;
							t1=t2;
							t2=w;
						}

						if ( t2 < tmax )
							tmax = t2;
					
						if ( t1 > tmin )
							tmin = t1;
						
						if (tmin > tmax)
							return AOBBOX_RAY_OUTSIDE;

					}
					else
					{
						if(-e+aabb_min.y > -eps || -e+aabb_max.y < eps)
							return AOBBOX_RAY_OUTSIDE;
					}
				}

				// Test intersection with the 2 planes perpendicular to the OBB's Z axis
				// Exactly the same thing than above.
				
				{

					glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
					
					float e = glm::dot(zaxis, delta);
					float f = glm::dot(dir, zaxis);

					if ( fabs(f) > eps)
					{

						float t1 = (e+aabb_min.z)/f;
						float t2 = (e+aabb_max.z)/f;

						if (t1>t2)
						{
							// swap t1 and t2

							float w=t1;
							t1=t2;
							t2=w;
						}

						if ( t2 < tmax )
							tmax = t2;
						
						if ( t1 > tmin )
							tmin = t1;
						
						if (tmin > tmax)
							return AOBBOX_RAY_OUTSIDE;

					}
					else
					{
					
						if(-e+aabb_min.z > -eps || -e+aabb_max.z < eps)
							return AOBBOX_RAY_OUTSIDE;
					}
				}

				p = start + tmin * dir;
				q = start + tmax * dir;

				return AOBBOX_RAY_INTERSECTS;
			}

		} // end of intersections namespace

	}	// end of geo3d namespace

}	// end of vml namespace

