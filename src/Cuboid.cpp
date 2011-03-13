#include "Cuboid.h"

Cuboid::Cuboid(const osg::Vec3 &from, const osg::Vec3 &size)
{
    osg::Vec3 to = from + size;
    osg::Vec3 center = osg::Vec3(
							0.5 * (from.x() + to.x()),
							0.5 * (from.y() + to.y()),
							0.5 * (from.z() + to.z()));

	float width = fabs(from.x() - to.x());
	float depth = fabs(from.y() - to.y());
	float height = fabs(from.z() - to.z());
		
    osg::Geometry *drawable = new osg::Geometry();

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
    
    drawable->setVertexArray( pyramidVertices ); 
    
    // front
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(0);
        face->push_back(1);
        face->push_back(2);
        face->push_back(3);
        drawable->addPrimitiveSet(face);
    }
    
    // back
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(4);
        face->push_back(5);
        face->push_back(6);
        face->push_back(7);
        drawable->addPrimitiveSet(face);
    }
    
    // left
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(0);
        face->push_back(4);
        face->push_back(7);
        face->push_back(3);
        drawable->addPrimitiveSet(face);
    }
    
    // right
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(1);
        face->push_back(5);
        face->push_back(6);
        face->push_back(2);
        drawable->addPrimitiveSet(face);
    }
    
    // top
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(0);
        face->push_back(1);
        face->push_back(5);
        face->push_back(4);
        drawable->addPrimitiveSet(face);
    }
    
    // bottom
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(3);
        face->push_back(2);
        face->push_back(6);
        face->push_back(7);
        drawable->addPrimitiveSet(face);
    }
    
    osg::Vec2Array* texcoords = new osg::Vec2Array(8);
       (*texcoords)[0].set(width, 0.0f); // TOP
       (*texcoords)[1].set(0.0f, 0.0f); // TOP
       (*texcoords)[2].set(0.0f,  0.0f); // ""
       (*texcoords)[3].set(0.0f,  0.0f); // "" 
       (*texcoords)[4].set(width, depth / 3.0f); // TOP
       (*texcoords)[5].set(0.0f,  depth / 3.0f); // TOP
       (*texcoords)[6].set(0.0f,  0.0f); // ""
       (*texcoords)[7].set(0.0f,  0.0f); // "" 
       
       drawable->setTexCoordArray(0,texcoords);
	    
    _texture = new osg::Texture2D;
    _texture->setDataVariance(osg::Object::DYNAMIC); 
    _texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT); 
    _texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    
    osg::StateSet* stateSet = new osg::StateSet();
    stateSet->setTextureAttributeAndModes(0, _texture, osg::StateAttribute::ON);
    
    setStateSet(stateSet);
    setNodeMask(RECEIVE_SHADOW_MASK);
    addDrawable(drawable);
	    
    constructRigidBody(center, width, depth, height);
}

void Cuboid::constructRigidBody(const osg::Vec3 &center, const float width, const float depth, const float height)
{
    // create start transform for the cuboid rigid body
    btTransform shapeTransform;
    shapeTransform.setIdentity();
    shapeTransform.setOrigin(osgbBullet::asBtVector3(center));
    
    // create bounding box
    btBoxShape *bsCuboid = new btBoxShape(osgbBullet::asBtVector3(osg::Vec3(width, depth, height) / 2.0f));
    
    // create MotionState for the cuboid
    btDefaultMotionState *msCuboid = new btDefaultMotionState(shapeTransform);
    
    // passing 0 as first and a null-vector as last argument means this object is immovable
    btRigidBody::btRigidBodyConstructionInfo rbciCuboid(0, msCuboid, bsCuboid, btVector3(0,0,0));
    
    // construct rigid body from previously specified construction info
    _rigidBody = new btRigidBody(rbciCuboid);
    _rigidBody->setUserPointer(this);
}

void Cuboid::setTexture(osg::Image *image)
{
    _texture->setImage(image);
}

btRigidBody *Cuboid::getRigidBody() const
{
    return _rigidBody;
}

AccelerationCuboid::AccelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    Cuboid(from, size)
{    
    osg::Image *image = osgDB::readImageFile(ACCELERATION_CUBOID_TEXTURE);
    setTexture(image);
}

void AccelerationCuboid::applyTo(Player *player)
{
    float speed = player->getPlayerState()->getSpeed();
    
    if(!player->getPlayerState()->requestDecelerate())
    {
        player->getPlayerState()->setSpeed(speed + 0.02 <= 1.0 ? speed + 0.02 : 1.0);
        player->setEnginesAccelerating(speed + 0.02 <= 1.0 ? speed + 0.02 : 1.0);
    }
}

DecelerationCuboid::DecelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    Cuboid(from, size)
{    
    osg::Image *image = osgDB::readImageFile(DECELERATION_CUBOID_TEXTURE);
    setTexture(image);
}

void DecelerationCuboid::applyTo(Player *player)
{
    float speed = player->getPlayerState()->getSpeed();

    if(!player->getPlayerState()->requestAccelerate())
    {
        player->getPlayerState()->setSpeed(speed - 0.04 >= 0 ? speed - 0.04 : 0);
        player->setEnginesDecelerating(speed - 0.04 >= 0 ? speed - 0.04 : 0);
    }
}