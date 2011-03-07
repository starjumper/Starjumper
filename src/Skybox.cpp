//---------------------------------------------------------------------------
// (c) 2006-2008   Jean-Sébastien Guay
// This code is distributed in the hope it will be useful, but without 
// any warranty whatsoever. You may use it in any project, as well as modify
// it without restriction, as long as this notice is retained in the 
// relevant files.
//---------------------------------------------------------------------------

#include "Skybox.h"

#include <iostream>
#include <cassert>

#include <osg/Geometry>
#include <osg/Depth>
#include <osg/TextureCubeMap>
#include <osg/TexEnv>
#include <osgDB/ReadFile>
#include <osgUtil/CullVisitor>

#include <iostream>

class SkyboxTransform : public osg::Transform
{
    public:
        // Get the transformation matrix which moves from local coords to world coords.
        virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,
            osg::NodeVisitor* nv) const
        {
            osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
            if (cv)
            {
                osg::Vec3 eyePointLocal = cv->getEyeLocal();
                matrix.preMult(osg::Matrix::translate(eyePointLocal));
            }

            return true;
        }

        // Get the transformation matrix which moves from world coords to local coords.
        virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,
            osg::NodeVisitor* nv) const
        {
            osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
            if (cv)
            {
                osg::Vec3 eyePointLocal = cv->getEyeLocal();
                matrix.postMult(osg::Matrix::translate(-eyePointLocal));
            }

            return true;
        }
};

osg::TextureCubeMap* loadCubeMap() {
	osg::TextureCubeMap* cubemap = new osg::TextureCubeMap;

    osg::Image* image = osgDB::readImageFile(BACKGROUND_IMAGE);

    if (image)
    {
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, image);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, image);
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, image);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, image);
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, image);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, image);

        cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

        cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
        cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    }

    return cubemap;
}

osg::Node* createSkyBoxCubeMap(osg::StateSet* stateset)
{
    float radius = 100.0f;

    if (!stateset)
        stateset = new osg::StateSet;

    // Set texture mode to REPLACE
    osg::TexEnv* te = new osg::TexEnv;
    te->setMode(osg::TexEnv::REPLACE);
    stateset->setTextureAttributeAndModes(0, te, osg::StateAttribute::ON);

    // Turn off lighting and cull face
    stateset->setMode(GL_LIGHTING,  osg::StateAttribute::OFF);
    stateset->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);

    // Basic principle of skyboxes: render it last, and with a depth func
    // that only sets pixels at the far plane. That way, if the sky is not
    // visible at all for example, the sky won't be drawn (and possibly
    // fragment shaders will not be called) whereas rendering it first will
    // cause all pixels to be written, then overwritten by objects, and
    // possibly fragment shaders will have been called for nothing.

    // Clear the depth to the far plane.
    osg::Depth* depth = new osg::Depth(osg::Depth::LEQUAL, 1.0, 1.0);
    stateset->setAttributeAndModes(depth, osg::StateAttribute::ON);

    // Make sure it is drawn last
    stateset->setRenderBinDetails(1000, "RenderBin");

    // Create a drawable for the skybox
    osg::Geometry* drawable = new osg::Geometry;


    // Create vertices for box
    osg::Vec3Array *verts = new osg::Vec3Array;
    verts->push_back(osg::Vec3(-radius, -radius,  radius));
    verts->push_back(osg::Vec3(-radius,  radius,  radius));
    verts->push_back(osg::Vec3( radius,  radius,  radius));
    verts->push_back(osg::Vec3( radius, -radius,  radius));
    verts->push_back(osg::Vec3(-radius, -radius, -radius));
    verts->push_back(osg::Vec3(-radius,  radius, -radius));
    verts->push_back(osg::Vec3( radius,  radius, -radius));
    verts->push_back(osg::Vec3( radius, -radius, -radius));
    drawable->setVertexArray(verts);

    // Create texture coordinates for cubemaps
    osg::Vec3Array *coords = new osg::Vec3Array;
    coords->push_back(osg::Vec3(-1,  1, -1));
    coords->push_back(osg::Vec3(-1, -1, -1));
    coords->push_back(osg::Vec3( 1, -1, -1));
    coords->push_back(osg::Vec3( 1,  1, -1));
    coords->push_back(osg::Vec3(-1,  1,  1));
    coords->push_back(osg::Vec3(-1, -1,  1));
    coords->push_back(osg::Vec3( 1, -1,  1));
    coords->push_back(osg::Vec3( 1,  1,  1));

    drawable->setTexCoordArray(0,coords);

    // Create an index array for the box
    osg::ref_ptr<osg::UIntArray> indices = new osg::UIntArray;

    // Front face
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(3);

    // Back face
    indices->push_back(4);
    indices->push_back(5);
    indices->push_back(6);
    indices->push_back(7);

    // Right face
    indices->push_back(6);
    indices->push_back(7);
    indices->push_back(3);
    indices->push_back(2);

    // Left face
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(5);
    indices->push_back(4);

    // Top face
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(6);
    indices->push_back(5);

    // Bottom face
    indices->push_back(0);
    indices->push_back(3);
    indices->push_back(7);
    indices->push_back(4);

    drawable->addPrimitiveSet(
        new osg::DrawElementsUInt(GL_QUADS,
            indices->size(), &(indices->front())));


    // Create a geode for the skybox
    osg::Geode* geode = new osg::Geode;
    geode->setName("Skybox");

    // Disable culling
    geode->setCullingActive(false);

    // Set the stateset
    geode->setStateSet(stateset);

    // Add the skybox
    geode->addDrawable(drawable);


    // Load the texture
    osg::ref_ptr<osg::TextureCubeMap> texture = loadCubeMap();
    if (texture.valid())
    {
        texture->setResizeNonPowerOfTwoHint(false);
        texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);

		texture->setTextureHeight(5);
    }

    // Create a transform and set it to absolute so it does not influence
    // the viewer's bounds computation (for initial camera settings).
    osg::Transform* transform = new SkyboxTransform;
    transform->setCullingActive(false);
    transform->addChild(geode);
    transform->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    transform->setName("SkyboxTransform");

    return transform;
}