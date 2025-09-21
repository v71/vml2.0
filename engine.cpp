////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
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


#include <vml2.0\os\common.h>
#include <vml2.0\math\mathutils.h>
#include <vml2.0\opengl\debugrendering\rgbutils.h>
#include <vml2.0\strings\stringconverter.h>
#include <vml2.0\strings\stringutils.h>
#include <vml2.0\strings\stringformat.h>
#include <vml2.0\strings\stringpath.h>
#include <vml2.0\utils\logger.h>
#include <vml2.0\assetstore\assetstore.h>
#include <vml2.0\os\session.h>
#include <vml2.0\opengl\glwindow.h>
#include <vml2.0\core\mesh\mesh.h>
#include <vml2.0/core/mesh/meshstore.h>
#include <vml2.0\opengl\view\view.h>
#include <vml2.0\opengl\shaders\shader.h>
#include <vml2.0\opengl\shaders\shaderstore.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <vml2.0\libs\stb\stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <vml2.0/libs/stb/stb_image_write.h>

#include <vml2.0/opengl/textures/texture.h>
#include<vml2.0/opengl/textures/texturestore.h>
#include <vml2.0\opengl\debugrendering\debugrendering.h>

// singletons

namespace vml
{
	Logger   *Logger::Singleton   = nullptr;
	Session  *Session::Singleton  = nullptr;
}

// debug rendeirng

namespace vml 
{ 
	vml::OpenglDebugRender* DebugRender;
}

// stores

namespace vml
{
	vml::shaders::ShaderStore   *ShaderStore  = nullptr;
	vml::textures::TextureStore *TextureStore = nullptr;
	vml::meshes::MeshStore      *MeshStore    = nullptr;
}

// include core 

#include <vml2.0\os\core.h>
