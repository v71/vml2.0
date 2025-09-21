#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2030 v71 
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <vml2.0\assetstore\assetstore.h>

namespace vml
{
	namespace meshes
	{

		class MeshStore : public vml::resources::SharedResourceStore<Mesh3d>
		{
			public:

				// -----------------------------------------------------------------
				// inits manager

				void Init()
				{
					// just in case
				}

				Mesh3d* Load(const std::string& screenname) 
				{
					Mesh3d* mesh= vml::resources::SharedResourceStore<Mesh3d>::Load(screenname);
					
					// release data , see mesh class for more info 

					mesh->ReleaseVertexBufferData();
					
					return mesh;
				}

				//---------------------------------------------------------------------
				// unloads mesh

				bool UnLoad(const std::string& filename)
				{
					return vml::resources::SharedResourceStore<Mesh3d>::UnLoad(filename);
				}

				//---------------------------------------------------------------------
				// ctor / dtor


				MeshStore(const std::string& name, unsigned int flags = vml::PreferencesFlags::VERBOSE) : vml::resources::SharedResourceStore<Mesh3d>(name,flags)
				{
				}

				~MeshStore()
				{
				}

		};

	} // end of meshes namespace

} // end of vml namespace

