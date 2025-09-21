#pragma once

namespace vml
{
    namespace geo2d
    {
        namespace hulls
        {

            //////////////////////////////////////////////////////////
            // graham scan algorithm to compute convexhl in 2d

            class GrahamScan
            {
                private:

                    // ----------------------------------------------------------------
                    // squared distance between points

                    int distsqr(const glm::vec2& a,
                                const glm::vec2& b)
                    {
                        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
                    }

                    float cross(const glm::vec2& a,
                                const glm::vec2& b,
                                const glm::vec2& c)
                    {
                        return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
                    }

                    bool angleCmp(const glm::vec2& pivot,
                                  const glm::vec2& self,
                                  const glm::vec2& that)
                    {
                        float t = cross(pivot, that, self);
                        if (t < 0.0f) return true;
                        if (t == 0.0f) return (distsqr(pivot, self) < distsqr(pivot, that));
                        return false;
                    }

                    // -----------------------------------------------------
                    // Quick sort

                    // ----------------------------------------------------
                    // Find and return the index of pivot element

                    int QuickPivot(const glm::vec2& pivot,
                                   std::vector<glm::vec2>& a,
                                   int first, int last)
                    {

                        int  p = first;

                        glm::vec2 pivotElement = a[first];

                        for (int i = first + 1; i <= last; i++)
                        {

                            if (angleCmp(pivot, a[i], pivotElement))
                            {
                                p++;

                                glm::vec2 temp;

                                temp = a[i];
                                a[i] = a[p];
                                a[p] = temp;

                            }

                        }

                        glm::vec2 temp;

                        temp = a[first];
                        a[first] = a[p];
                        a[p] = temp;

                        return p;
                    }

                    // ----------------------------------------------------
                    // Quicksort main function

                    void QuickSort(const glm::vec2& pivot,
                                   std::vector<glm::vec2>& a,
                                   int first, int last)
                    {
                        if (first < last)
                        {
                            int pivotElement = QuickPivot(pivot, a, first, last);
                            QuickSort(pivot, a, first, pivotElement - 1);
                            QuickSort(pivot, a, pivotElement + 1, last);
                        }
                    }

                public:

                    // --------------------------------------------------------------------------
                    // computes the convexl hull

                    [[nodiscard]] vml::geo2d::Polygon Go(vml::geo2d::VertexArray& points)
                    {
                        if (points.Size() <= 2)
                            vml::os::Error(L"A minimum of 3 points are needed for computing the convex hull");

                        std::vector<glm::vec2>& p = points.GetPointsArray();

                        for (int i = 1; i < p.size(); ++i)
                        {
                            if (p[i].y < p[0].y || (p[i].y == p[0].y && p[i].x < p[0].x))
                            {
                                glm::vec2 temp = p[0];
                                p[0] = p[i];
                                p[i] = temp;
                            }
                        }

                        glm::vec2 pivot;

                        pivot = p[0];

                        QuickSort(pivot, p, 0, int(p.size()) - 1);

                        std::vector<glm::vec2> chull(p.begin(), p.begin() + 3);

                        for (int i = 3; i < p.size(); ++i)
                        {

                            while (chull.size() >= 2 && cross(chull[chull.size() - 2], chull[chull.size() - 1], p[i]) <= 0)
                            {
                                chull.erase(chull.end() - 1);
                            }

                            chull.push_back(p[i]);
                        }

                        vml::geo2d::Polygon polygon;

                        polygon.Begin();

                        for (size_t i = 0; i < chull.size(); ++i)
                            polygon.AddVertex(chull[i]);

                        polygon.End();

                        return polygon;

                    }

                    // --------------------------------------
                    // ctor / dtor

                    GrahamScan()
                    {}

                    ~GrahamScan()
                    {}

            };

        } // end of hulls geo2d

    } // end of namespace geo2d

} // end of namespace vml
