#pragma once

namespace vml
{
	namespace geo3d
	{
		namespace collisions
		{
			
			/////////////////////////////////////////////////////
			// sphere vs sphere collision

			static int SphereVsSphere(const glm::vec3 &pos1, float radius1, const glm::vec3 &pos2, float radius2, vml::geo3d::collisions::MTV &mtv)
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

				glm::vec3 d = pos2 - pos1;

				float a = d.x*d.x + d.y*d.y + d.z*d.z;

				if (a > radius1 * radius1 + radius2 * radius2 + 2 * radius1 * radius2)
					return 0;

				a = sqrtf(a);

				if (a > -vml::math::EPSILON && a < vml::math::EPSILON)
					return 0;

				// this is the contact point , but alternatively

				glm::vec3 contactpoint = pos2 - d * radius2 / a;

				// also this can be considered the contact point
				// start2 = pos1 + d * radius1 / sqrtf(a);

				mtv.IsValid      =  true;
				mtv.ContactPoint =  contactpoint;
				mtv.Normal       =  glm::normalize(d);
				mtv.Distance	 = -0.5f*(radius1 + radius2 - a) * 1.01f;

				return 1;

			}
		}	//end of namespace collisions

	}	// end of geo3d namespace

}	// end of vml namespace

