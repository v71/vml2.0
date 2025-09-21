#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

////////////////////////////////////////////////////////////////////////////////////

// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

////////////////////////////////////////////////////////////////////////////////////

#include <vml2.0/os/common.h>
#include <vml2.0/math/mathutils.h>
#include <vml2.0/math/2d/geometry2d.h>
#include <vml2.0/math/3d/geometry3d.h>
#include <vml2.0/opengl/debugrendering/rgbutils.h>

/////////////////////////////////////////////////////////////////////////////////////
// stb image implementation

////////////////////////////////////////////////////////////////////////////////////
// string utils

#include <vml2.0\strings\stringconverter.h>
#include <vml2.0\strings\stringutils.h>
#include <vml2.0\strings\stringpath.h>

////////////////////////////////////////////////////////////////////////////////////

#include <vml2.0\utils\logger.h>

////////////////////////////////////////////////////////////////////////////////////
// asset store

#include <vml2.0\assetstore\assetstore.h>

////////////////////////////////////////////////////////////////////////////////////
// session

#include <vml2.0\os\session.h>

////////////////////////////////////////////////////////////////////////////////////
// opengl api

#include <vml2.0\opengl\glwindow.h>

////////////////////////////////////////////////////////////////////////////////////
// views

#include <vml2.0\opengl\view\view.h>

////////////////////////////////////////////////////////////////////////////////////
// shaders

#include <vml2.0\opengl\shaders\shader.h>
#include <vml2.0\opengl\shaders\shaderstore.h>
namespace vml { extern vml::shaders::ShaderStore *ShaderStore; }

////////////////////////////////////////////////////////////////////////////////////
// meshes

#include <vml2.0\core\mesh\procedural.h>
#include <vml2.0\core\mesh\mesh.h>
#include <vml2.0\core\mesh\meshstore.h>
namespace vml { extern vml::meshes::MeshStore* MeshStore; }
#include <vml2.0/core/mesh/removeduplicates.h>
#include <vml2.0/core/mesh/meshbuilder.h>
#include <vml2.0/libs/assimpimporter/importer.h>
#include <vml2.0/opengl/view/frustum3d.h>
#include <vml2.0/core/mesh/objectmanager2.h>

////////////////////////////////////////////////////////////////////////////////////
// texture

#include <vml2.0\libs\stb\stb_image.h>
#include <vml2.0\opengl\textures\texture.h>
#include <vml2.0\opengl\textures\texturestore.h>
namespace vml { extern vml::textures::TextureStore* TextureStore; }

////////////////////////////////////////////////////////////////////////////////////
// debug rendering

#include <vml2.0\opengl\debugrendering\debugrendering.h>
namespace vml { extern vml::OpenglDebugRender  *DebugRender; }

////////////////////////////////////////////////////////////////////////////////////

#include <vml2.0/core/octree/meshcollider.h>
#include <vml2.0/core/octree/octree.h>

////////////////////////////////////////////////////////////////////////////////////
// core class

#include <vml2.0\os\core.h>
