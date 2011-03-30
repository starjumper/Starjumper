//---------------------------------------------------------------------------
// (c) 2006-2008   Jean-Sébastien Guay
// 
// This code will create a skybox in OpenSceneGraph. The core of it comes
// from Farshid Lashkari <flashk_at_gmail.com> - see 
// http://openscenegraph.org/archiver/osg-users/2005-June/0581.html
//
// I added the functionality to be able to extract the six cube face 
// textures from a vertical cross texture such as the ones on
// http://www.debevec.org/Probes/.
//
// This code is distributed in the hope it will be useful, but without 
// any warranty whatsoever. You may use it in any project, as well as modify
// it without restriction, as long as this notice is retained in the 
// relevant files.
//---------------------------------------------------------------------------

#pragma once

#define BACKGROUND_IMAGE   "resources/textures/space.jpg"

#include <string>
#include <osg/Node>
#include <osg/TextureCubeMap>

osg::Node* createSkyBoxCubeMap(osg::StateSet* stateset = 0);
osg::TextureCubeMap* loadCubeMap();