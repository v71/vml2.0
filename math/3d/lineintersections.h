#pragma once

/////////////////////////////////////////////////////////////////////////////
// flags for spheres

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{

			/////////////////////////////////////////////////////////////////////////////
			// checks if lines intersect each other

			static const unsigned int LINE_DO_NOT_INTERSECTS_LINE = 0;
			static const unsigned int LINE_INTERSECTS_LINE        = 1;

			///////////////////////////////////////////////////////////////////////////////////////////
			// intersection between 3d lines

			static int LineLineIntersect(const glm::vec3 &p0, const glm::vec3 &p1,
										 const glm::vec3 &q0, const glm::vec3 &q1,
										 glm::vec3 &pa, glm::vec3 &pb,
										 float eps = vml::math::EPSILON)
			{

				glm::vec3 u = p1 - p0;
				glm::vec3 v = q1 - q0;
				glm::vec3 w = p0 - q0;

				// computing cross component of u X v

				float det1 = u.y*v.z - u.z*v.y;
				float det2 = u.x*v.z - u.z*v.x;
				float det3 = u.x*v.y - u.y*v.x;

				// if cross is zero , then the vectors are parallel

				if (det1 > -eps && det1 < eps)
				if (det2 > -eps && det2 < eps)
				if (det3 > -eps && det3 < eps)
					return LINE_DO_NOT_INTERSECTS_LINE;

				// compute cofatctrs

				float a = u.x*u.x + u.y*u.y + u.z*u.z;
				float b = u.x*v.x + u.y*v.y + u.z*v.z;
				float c = v.x*v.x + v.y*v.y + v.z*v.z;
				float d = u.x*w.x + u.y*w.y + u.z*w.z;
				float e = v.x*w.x + v.y*w.y + v.z*w.z;

				// check if intersection lies within 0 , 1 range

				float denum = 1.0f / (a * c - b * b);

				float mua = (b*e - c * d) * denum;
				if (mua< -eps || mua >1.0f + eps) 
					return LINE_DO_NOT_INTERSECTS_LINE;

				float mub = (a*e - b * d) * denum;
				if (mub< -eps || mub >1.0f + eps) 
					return LINE_DO_NOT_INTERSECTS_LINE;

				// compute intersection points

				pa.x = p0.x + mua * u.x;
				pa.y = p0.y + mua * u.y;
				pa.z = p0.z + mua * u.z;
				pb.x = q0.x + mub * v.x;
				pb.y = q0.y + mub * v.y;
				pb.z = q0.z + mub * v.z;

				return LINE_INTERSECTS_LINE;
			}

		}	// end of intersections namespace

	}	// end of geo3d namespace

}	// end of vml namespace
