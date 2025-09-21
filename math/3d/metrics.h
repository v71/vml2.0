#pragma once

///////////////////////////////////////////////////////////////////////
// metrics

namespace vml
{
	namespace geo3d
	{
		namespace metrics
		{

			// ------------------------------------------------------------------
			// return bounding box corner vertex

			static const glm::vec3 GetAABBoxExtremalPoint(const glm::vec3& min, const glm::vec3& max, const glm::vec3& direction) 
			{
				return glm::vec3((direction.x >= 0.f ? max.x : min.x),
								 (direction.y >= 0.f ? max.y : min.y),
								 (direction.z >= 0.f ? max.z : min.z));
			}

			// ------------------------------------------------------------------
			// return bounding box corner vertex

			static const glm::vec3 GetAABBoxCornerPoint(const glm::vec3& min, const glm::vec3& max, int cornerIndex)
			{
				switch (cornerIndex)
				{
					default:
						case 0: return glm::vec3(min.x, min.y, min.z); break;
						case 1: return glm::vec3(min.x, min.y, max.z); break;
						case 2: return glm::vec3(min.x, max.y, min.z); break;
						case 3: return glm::vec3(min.x, max.y, max.z); break;
						case 4: return glm::vec3(max.x, min.y, min.z); break;
						case 5: return glm::vec3(max.x, min.y, max.z); break;
						case 6: return glm::vec3(max.x, max.y, min.z); break;
						case 7: return glm::vec3(max.x, max.y, max.z); break;
					break;
				}
			}

			// ------------------------------------------------------------------
			// return boundng box edge in the form of a starting and ending vector

			static vml::geo3d::Line GetAABBoxEdge(const glm::vec3& min, const glm::vec3& max, int edgeIndex)
			{
				switch (edgeIndex)
				{
					default:
						case  0: return vml::geo3d::Line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z)); break;
						case  1: return vml::geo3d::Line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z)); break;
						case  2: return vml::geo3d::Line(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z)); break;
						case  3: return vml::geo3d::Line(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z)); break;
						case  4: return vml::geo3d::Line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z)); break;
						case  5: return vml::geo3d::Line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z)); break;
						case  6: return vml::geo3d::Line(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z)); break;
						case  7: return vml::geo3d::Line(glm::vec3(min.x, max.y, max.z), glm::vec3(max.x, max.y, max.z)); break;
						case  8: return vml::geo3d::Line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z)); break;
						case  9: return vml::geo3d::Line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z)); break;
						case 10: return vml::geo3d::Line(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z)); break;
						case 11: return vml::geo3d::Line(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z)); break;
					break;
				}
			}
		} // end of metrics namespace
	} // end of geo2d namespace
} // end of vml namespace


