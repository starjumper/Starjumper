#include "Cuboid.h"

Cuboid::Cuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    _from(from),
    _size(size)
{
    _drawable = new osg::Geometry();

    osg::Vec3Array *pyramidVertices = new osg::Vec3Array();
    {
        pyramidVertices->push_back( from + osg::Vec3(0, 0, size.z()));
        pyramidVertices->push_back( from + osg::Vec3(size.x(), 0, size.z()));
        pyramidVertices->push_back( from + osg::Vec3(size.x(), 0, 0));
        pyramidVertices->push_back( from );

        pyramidVertices->push_back( from + osg::Vec3(0, size.y(), size.z()));
        pyramidVertices->push_back( from + osg::Vec3(size.x(), size.y(), size.z()));
        pyramidVertices->push_back( from + osg::Vec3(size.x(), size.y(), 0));
        pyramidVertices->push_back( from + osg::Vec3(0, size.y(), 0) );
    }

    _drawable->setVertexArray( pyramidVertices ); 

     // front
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(0);
        face->push_back(1);
        face->push_back(2);
        face->push_back(3);
        _drawable->addPrimitiveSet(face);
    }

     // back
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(4);
        face->push_back(5);
        face->push_back(6);
        face->push_back(7);
        _drawable->addPrimitiveSet(face);
    }

     // left
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(0);
        face->push_back(4);
        face->push_back(7);
        face->push_back(3);
        _drawable->addPrimitiveSet(face);
    }

     // right
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(1);
        face->push_back(5);
        face->push_back(6);
        face->push_back(2);
        _drawable->addPrimitiveSet(face);
    }

     // bottom
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(3);
        face->push_back(2);
        face->push_back(6);
        face->push_back(7);
        _drawable->addPrimitiveSet(face);
    }

    osg::Vec4Array* colors = new osg::Vec4Array;
    {
        colors->push_back(osg::Vec4(0.0f, 1.0f, 1.0f, 0.8f));
        colors->push_back(osg::Vec4(0.0f, 1.0f, 1.0f, 0.8f)); 
        colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 0.8f)); 
        colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 0.8f)); 
        colors->push_back(osg::Vec4(1.0f, 0.0f, 1.0f, 0.8f));
        colors->push_back(osg::Vec4(1.0f, 0.0f, 1.0f, 0.8f));
        colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 0.8f));
        colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 0.8f));

        _drawable->setColorArray(colors);
        _drawable->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    }
/*
    osg::StateSet* stateSet = new osg::StateSet();
    stateSet->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON); 
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    setStateSet(stateSet);*/
    addDrawable(_drawable);
}

btRigidBody *Cuboid::getRigidBody()
{
    btRigidBody *rigidBody;
   // create start transform for the cuboid rigid body
    btTransform shapeTransform;
    shapeTransform.setIdentity();
    shapeTransform.setOrigin(osgbBullet::asBtVector3(_from + (_size / 2.0f)));
    
    // create bounding box
    btBoxShape *bsCuboid = new btBoxShape(osgbBullet::asBtVector3(_size / 2.0f));
    
    // create MotionState for the cuboid
    btDefaultMotionState *msCuboid = new btDefaultMotionState(shapeTransform);
    
    // passing 0 as first and a null-vector as last argument means this object is immovable
    btRigidBody::btRigidBodyConstructionInfo rbciCuboid(0, msCuboid, bsCuboid, btVector3(0,0,0));
    
    // construct rigid body from previously specified construction info
	rigidBody = new btRigidBody(rbciCuboid);
	rigidBody->setUserPointer(this);
	
    return rigidBody;
}

DefaultCuboid::DefaultCuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    Cuboid(from, size)
{
      // top
     {
         osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
         face->push_back(0);
         face->push_back(1);
         face->push_back(5);
         face->push_back(4);
         _drawable->addPrimitiveSet(face);
     }
}

TexturedCuboid::TexturedCuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    Cuboid(from, size)
{
    _textureDrawable = new osg::Geometry();

    osg::Vec3Array *pyramidVertices = new osg::Vec3Array();
    {
        pyramidVertices->push_back( from + osg::Vec3(0, 0, size.z()));
        pyramidVertices->push_back( from + osg::Vec3(size.x(), 0, size.z()));
        pyramidVertices->push_back( from + osg::Vec3(size.x(), size.y(), size.z()));
        pyramidVertices->push_back( from + osg::Vec3(0, size.y(), size.z()));
    }

    _textureDrawable->setVertexArray( pyramidVertices );

    osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
    face->push_back(0);
    face->push_back(1);
    face->push_back(2);
    face->push_back(3);
    
    _textureDrawable->addPrimitiveSet(face);
        
    osg::Vec2Array* texcoords = new osg::Vec2Array(4);
    {
        (*texcoords)[0].set(size.x(), 0.0f);
        (*texcoords)[1].set(0.0f, 0.0f);
        (*texcoords)[3].set(size.x(), size.y() / 3.0f);
        (*texcoords)[2].set(0.0f,  size.y() / 3.0f); 

        _textureDrawable->setTexCoordArray(0, texcoords);
    }
             
    _texture = new osg::Texture2D;
    _texture->setDataVariance(osg::Object::DYNAMIC); 
    _texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT); 
    _texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

    osg::StateSet* stateSet = new osg::StateSet();
    stateSet->setTextureAttributeAndModes(0, _texture, osg::StateAttribute::ON);
    
    _textureDrawable->setStateSet(stateSet);
    addDrawable(_textureDrawable);
}

void TexturedCuboid::setTexture(osg::Image *image)
{
    _texture->setImage(image);
}

AccelerationCuboid::AccelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    TexturedCuboid(from, size)
{
    osg::Image *image = osgDB::readImageFile(ACCELERATION_CUBOID_TEXTURE);
    setTexture(image);   
    
    osg::Vec4Array* colors = new osg::Vec4Array;
    {
        colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 0.7f));

        _textureDrawable->setColorArray(colors);
        _textureDrawable->setColorBinding(osg::Geometry::BIND_OVERALL);
    }
}

void AccelerationCuboid::collide()
{
    Player *player = Player::getInstance();
    float speed = player->getPlayerState()->getSpeed();
    
    if(!player->getPlayerState()->requestDecelerate())
    {
        player->getPlayerState()->setSpeed(speed + 0.02 <= 1.0 ? speed + 0.02 : 1.0);
        player->setEngines(speed + 0.02 <= 1.0 ? speed + 0.02 : 1.0, ACCELERATE);
    } 
}

DecelerationCuboid::DecelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    TexturedCuboid(from, size)
{
    osg::Image *image = osgDB::readImageFile(DECELERATION_CUBOID_TEXTURE);
    setTexture(image);  
    
    osg::Vec4Array* colors = new osg::Vec4Array;
    {
        colors->push_back(osg::Vec4(1.0f, 0.8f, 0.0f, 0.7f));

        _textureDrawable->setColorArray(colors);
        _textureDrawable->setColorBinding(osg::Geometry::BIND_OVERALL);
    } 
}

void DecelerationCuboid::collide()
{
    Player *player = Player::getInstance();
    float speed = player->getPlayerState()->getSpeed();

    if(!player->getPlayerState()->requestAccelerate())
    {
        player->getPlayerState()->setSpeed(speed - 0.04 >= 0 ? speed - 0.04 : 0);
        player->setEngines(speed - 0.04 >= 0 ? speed - 0.04 : 0, DECELERATE);
    }    
}
