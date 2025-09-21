#pragma once

namespace vml
{
    namespace geo2d
    {

        // axis aligned bounding box

        struct AOBB
        {
            glm::vec2 P1;
            glm::vec2 P2;
            glm::vec2 P3;
            glm::vec2 P4;
            float Width;
            float Height;

            AOBB() : P1(0, 0), P2(0, 0), P3(0, 0), P4(0, 0),Width(0),Height(0) {}

            // this constructor accept 2 values, but you will ahve to make sure they are ordered
            // width is compute as the lenght of p2-p1 , height as the lenght of p4-p1

            AOBB(const glm::vec2& min, const glm::vec2& max)
            {
                glm::vec2 p = min;
                glm::vec2 q = max;
                if (q.x < p.x) { float t = p.x; p.x = q.x; q.x = t; }
                if (q.y < p.y) { float t = p.y; p.y = q.y; q.y = t; }
                P1 = glm::vec2(p.x, p.y);
                P2 = glm::vec2(q.x, p.y);
                P3 = glm::vec2(q.x, q.y);
                P4 = glm::vec2(p.x, q.y);
                Width = glm::length(P2 - P1);
                Height= glm::length(P4 - P1);
            }

            // this constructor accept 4 values, but you will ahve to make sure they are ordered
            // width is compute as the lenght of p2-p1 , height as the lenght of p4-p1

            AOBB(const float minx, const float miny, const float maxx, const float maxy)
            {
                glm::vec2 p(minx, miny);
                glm::vec2 q(maxx, maxy);
                if (q.x < p.x) { float t = p.x; p.x = q.x; q.x = t; }
                if (q.y < p.y) { float t = p.y; p.y = q.y; q.y = t; }
                P1 = glm::vec2(p.x, p.y);
                P2 = glm::vec2(q.x, p.y);
                P3 = glm::vec2(q.x, q.y);
                P4 = glm::vec2(p.x, q.y);
                Width = glm::length(P2 - P1);
                Height = glm::length(P4 - P1);
            }
        
            // this constructor accept 4 values, but you will ahve to make sure they are ordered
            // width is compute as the lenght of p2-p1 , height as the lenght of p4-p1

            AOBB(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4) : P1(p1), P2(p2), P3(p3), P4(p4)
            {
                Width = glm::length(P2 - P1);
                Height = glm::length(P4 - P1);
            }

            // this constructor accept 4 values, but you will ahve to make sure they are ordered
            // width is compute as the lenght of p2-p1 , height as the lenght of p4-p1

            AOBB(const float p1x, const float p1y, const float p2x, const float p2y, const float p3x, const float p3y, const float p4x, const float p4y) :P1(p1x, p1y), P2(p2x, p2y), P3(p3x, p3y), P4(p4x, p4y)
            {
                Width = glm::length(P2 - P1);
                Height = glm::length(P4 - P1);
            }
            
        };

    }	// end of geo3d namespace
}	// end of vml namespace

