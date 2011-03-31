#include "Sky.h"

extern osgViewer::Viewer viewer;

Sky::Sky() 
{
	_skyPat = new osg::PositionAttitudeTransform();
	_skyPat->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	initializeCamera();
	
	osg::Geode* geode = new osg::Geode();
    osg::Geometry *geometry = new osg::Geometry();
    geode->addDrawable(geometry);
    
    osg::Vec3Array *vertices = new osg::Vec3Array;

    int screenWidth = viewer.getCamera()->getViewport()->width();
    int screenHeight = viewer.getCamera()->getViewport()->height();
    
    vertices->push_back( osg::Vec3(0, 0, 0) );
    vertices->push_back( osg::Vec3(0, screenHeight, 0) );
    vertices->push_back( osg::Vec3(screenWidth, screenHeight, 0) ); 
    vertices->push_back( osg::Vec3(screenWidth, 0, 0) );
    
    geometry->setVertexArray(vertices);
    
    osg::DrawElementsUInt *rectangle = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
    
    rectangle->push_back(0);
    rectangle->push_back(1);
    rectangle->push_back(2);
    rectangle->push_back(3);
    
    geometry->addPrimitiveSet(rectangle);    
    
    osg::Vec2Array* texcoords = new osg::Vec2Array(4);

    (*texcoords)[0].set(0.0f, 0.0f);
    (*texcoords)[1].set(0.0f, 1.0f);
    (*texcoords)[2].set(1.0f, 1.0f);
    (*texcoords)[3].set(1.0f, 0.0f); 
    
    geometry->setTexCoordArray(0, texcoords);
    
    osg::Texture2D *texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); 

    osg::Image *image = osgDB::readImageFile(BACKGROUND_IMAGE);

    if (!image)
    {
        std::cout << " couldn't find texture, quiting." << std::endl;
        exit(0);
    }

    // assign image to texture
    texture->setImage(image); 
    
    osg::StateSet* stateOne = new osg::StateSet();

    stateOne->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);

    geode->setStateSet(stateOne);
    _skyPat->addChild(geode);  
}

void Sky::initializeCamera()
{   	
	_camera = new osg::Camera();
	_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, viewer.getCamera()->getViewport()->width(), 0, viewer.getCamera()->getViewport()->height()));
	_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	_camera->setViewMatrix(osg::Matrix::identity());
	
	_camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    _camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	_camera->setRenderOrder(osg::Camera::PRE_RENDER);
	_camera->addChild(_skyPat);
}

osg::Camera *Sky::getCamera()
{
    return _camera;
}
