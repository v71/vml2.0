#pragma once

namespace vml
{
	namespace geo2d
	{
		// axis aligned bounding box

		struct AABB
		{
			glm::vec2 Min;
			glm::vec2 Max;
			float Width;
			float Height;

			AABB() :Min(0,0), Max(0,0),Width(0),Height(0) {}
			
			AABB(const glm::vec2& min, const glm::vec2& max) :Min(min), Max(max), Width(max.x-min.x), Height(max.y - min.y)
			{
				if (Max.x < Min.x) { float t = Min.x; Min.x = Max.x; Max.x = t; Width = Max.x - Min.x; Height = Max.y - Min.y; }
				if (Max.y < Min.y) { float t = Min.y; Min.y = Max.y; Max.y = t; Width = Max.x - Min.x; Height = Max.y - Min.y; }
			}
			
			AABB(const float minx, const float miny, const float maxx, const float maxy) : Min(minx, miny), Max(maxx, maxy), Width(maxx - minx), Height(maxy - miny)
			{
				if (Max.x < Min.x) { float t = Min.x; Min.x = Max.x; Max.x = t; Width = Max.x - Min.x; Height = Max.y - Min.y; }
				if (Max.y < Min.y) { float t = Min.y; Min.y = Max.y; Max.y = t; Width = Max.x - Min.x; Height = Max.y - Min.y; }
			}

		};

	}	// end of geo3d namespace
}	// end of vml namespace

