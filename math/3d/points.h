#pragma once

////////////////////////////////////////////////////////////////////////////////////
// point class

namespace vml
{
	namespace geo3d
	{
		////////////////////////////////////////////////////////////////////////////////////
		// Predicate for points

		typedef glm::vec3 Point;

		////////////////////////////////////////////////////////////////////////////////////////
		// null vertex

		const glm::vec3 NullVec     = glm::vec3( 0,  0, 0);
		const glm::vec3 UnaryVec    = glm::vec3( 1,  1, 1);
		const glm::vec3 UpVec       = glm::vec3( 0,  1, 0);
		const glm::vec3 DownVec     = glm::vec3( 0, -1, 0);
		const glm::vec3 LeftVec     = glm::vec3( 1,  0, 0);
		const glm::vec3 RighttVec   = glm::vec3(-1,  0, 0);
		const glm::vec3 ForwardVec  = glm::vec3( 0, 0,  1);
		const glm::vec3 BackwardVec = glm::vec3( 0, 0, -1);

		//////////////////////////////////////////////////////////////////
		// vertex class holding vertex attributes

		class Vertex
		{
			public:

				glm::vec4 Pos;		// homogeneous point coordinates
				glm::vec3 Normal;	// normal coordinates
				glm::vec2 UV;		// texture coordinates
				glm::vec4 Color;	// color coordiantes ( rgba )

				// ------------------------------------------
				// ctor / dtor

				Vertex(const glm::vec3& pos, 
					   const glm::vec3& normal, 
					   const glm::vec2& uv, 
					   const glm::vec4& color) : Pos( glm::vec4(pos.x,pos.y,pos.z,1)), Normal(normal), UV(uv), Color(color)
				{
				}

				Vertex() :Pos(glm::vec4(0,0,0,0)), Normal(glm::vec3(0, 0, 0)), UV(glm::vec2(0, 0)), Color(glm::vec4(0, 0, 0, 1))
				{}

				~Vertex()
				{}

		};

	}
}

////////////////////////////////////////////////////////////////////////////////////
// point class

namespace vml
{
	namespace geo3d
	{
		namespace metrics
		{

			/////////////////////////////////////////////////////////////////////////////
			// Transform point by a rotation matrix

			static glm::vec3 TransformPoint(glm::vec3 &p, const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale)
			{
				float sx = sin(rot.x*vml::math::DEGTORAD);
				float sy = sin(rot.y*vml::math::DEGTORAD);
				float sz = sin(rot.z*vml::math::DEGTORAD);
				float cx = cos(rot.x*vml::math::DEGTORAD);
				float cy = cos(rot.y*vml::math::DEGTORAD);
				float cz = cos(rot.z*vml::math::DEGTORAD);
				float M[16]={0};

				M[ 0] = (cy*cz)*scale.x;
				M[ 1] = (cy*sz)*scale.x;
				M[ 2] = (-sy)*scale.x;
				M[ 3] = 0;
				M[ 4] = (sx*sy*cz - cx * sz)*scale.y;
				M[ 5] = (sx*sy*sz + cx * cz)*scale.y;
				M[ 6] = (sx*cy)*scale.y;
				M[ 7] = 0;
				M[ 8] = (cx*sy*cz + sx * sz)*scale.z;
				M[ 9] = (cx*sy*sz - sx * cz)*scale.z;
				M[10] = (cx*cy)*scale.z;
				M[11] = 0;
				M[12] = pos.x;
				M[13] = pos.y;
				M[14] = pos.z;
				M[15] = 1;

				return glm::vec3(p[0] * M[0] + p[1] * M[4] + p[2] * M [8] + M[12],
								 p[0] * M[1] + p[1] * M[5] + p[2] * M [9] + M[13],
								 p[0] * M[2] + p[1] * M[6] + p[2] * M[10] + M[14]);
			}

		}

	}	// end of geo3d namespace

}	// end of vml namespace

