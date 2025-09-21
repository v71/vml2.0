#pragma once

namespace vml
{
	namespace geo2d
	{

			///////////////////////////////////////////////////
			// triangulate a simple triangle ( non intersectiong )
			// using the ear clipping algorithm 
			// no holes supported

			class EarClipTriangulate
			{
				
				private:

					// -------------------------------------------------------------
					// cuts a contour given extremal points of a triangle

					bool Snip(const std::vector<glm::vec2> &contour, int u, int v, int w, int n, int *V, float eps = vml::math::EPSILON)
					{
						glm::vec2 r = contour[V[v]] - contour[V[u]];
						glm::vec2 s = contour[V[w]] - contour[V[u]];

						if (r.x*s.y - r.y*s.x < eps) return false;

						for (size_t p = 0; p < n; p++)
						{
							if ((p == u) || (p == v) || (p == w)) continue;

							if (vml::geo2d::distances::IsPointInTriangle(contour[V[p]], contour[V[u]], contour[V[v]], contour[V[w]]))
							{
								return false;
							}
						}

						return true;
					}
					
				public:
					
					// -------------------------------------------------------------
					// triangulate a contour/polygon, places results in STL vector
					// as series of triangles.

					bool Triangulate(const std::vector<glm::vec2> &contour, std::vector<glm::vec2> &result)
					{
						// allocate and initialize list of Vertices in polygon 

						int n = (int)contour.size();

						if (n < 3) return false;

						int *V = new int[n];

						// we want a counter-clockwise polygon in V 

						float area = 0.0f;
						
						for (int p = n - 1, q = 0; q<n; p = q++)
							area += contour[p].x*contour[q].y - contour[q].x*contour[p].y;
						
						area *= 0.5f;

						if (area >0.0f)
							for (int v = 0; v<n; v++) V[v] = v;
						else
							for (int v = 0; v<n; v++) V[v] = (n - 1) - v;

						int nv = n;

						//  remove nv-2 Vertices, creating 1 triangle every time 

						int count = 2 * nv;   // error detection 

						for (int m = 0, v = nv - 1; nv>2;)
						{
							// if we loop, it is probably a non-simple polygon 

							if (0 >= (count--))
							{
								// Triangulate: ERROR - probable bad polygon!
								return false;
							}

							// three consecutive vertices in current polygon, <u,v,w> 

							int u = v;
							if (nv <= u) u = 0;		// previous 
							v = u + 1;
							if (nv <= v) v = 0;		// new v    
							int w = v + 1;
							if (nv <= w) w = 0;     // next     

							if (Snip(contour, u, v, w, nv, V))
							{
								int a, b, c, s, t;

								// true names of the vertices 

								a = V[u];
								b = V[v];
								c = V[w];

								// output Triangle 

								result.emplace_back(contour[a]);
								result.emplace_back(contour[b]);
								result.emplace_back(contour[c]);

								m++;

								// remove v from remaining polygon 

								for (s = v, t = v + 1; t<nv; s++, t++) V[s] = V[t]; nv--;

								// resest error detection counter 

								count = 2 * nv;
							}
						}

						delete[] V;

						return true;
					}
					
				public:
					
					// -------------------------------------------------------------
					// triangulate a contour/polygon, places results in STL vector
					// as series of triangles.

					std::vector< vml::geo2d::Polygon > Go(const std::vector<glm::vec2> &vertices)
					{
						std::vector<glm::vec2> result;
						Triangulate(vertices,result);
						std::vector< vml::geo2d::Polygon > polygons;
						for (size_t i = 0; i<result.size() / 3; ++i)
							polygons.emplace_back(vml::geo2d::Polygon({ result[i * 3],result[i * 3 + 1],result[i * 3 + 2] }));
						return polygons;
					}
					
					// --------------------------------------------------------------
					// ctor / dtor

					EarClipTriangulate()
					{}

					~EarClipTriangulate()
					{}

			};

	} // end of geo2d namespace

} // end of vml namepsace
