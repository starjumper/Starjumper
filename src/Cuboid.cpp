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
		
    osg::Geometry *_drawable = new osg::Geometry();

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
/*    
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 0 red
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) ); //index 1 green
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) ); //index 2 blue
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 3 white 
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 4 red
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 5 white 
    colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f) ); //index 6 red
    colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f) ); //index 7 red
    
    _drawable->setColorArray(colors);
    _drawable->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
*/
    
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
    
    // top
    {
        osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        face->push_back(0);
        face->push_back(1);
        face->push_back(5);
        face->push_back(4);
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
    
    osg::Vec2Array* texcoords = new osg::Vec2Array(8);
       (*texcoords)[0].set(width, 0.0f); // TOP
       (*texcoords)[1].set(0.0f, 0.0f); // TOP
       (*texcoords)[2].set(0.0f,  0.0f); // ""
       (*texcoords)[3].set(0.0f,  0.0f); // "" 
       (*texcoords)[4].set(width, depth / 3.0f); // TOP
       (*texcoords)[5].set(0.0f,  depth / 3.0f); // TOP
       (*texcoords)[6].set(0.0f,  0.0f); // ""
       (*texcoords)[7].set(0.0f,  0.0f); // "" 
       
       _drawable->setTexCoordArray(0,texcoords);
    
	_node = new osg::Geode();
	_node->addDrawable(_drawable);
	
	osg::StateSet* stateSet = new osg::StateSet();
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    
    getNode()->setStateSet(stateSet);
    getNode()->setNodeMask(RECEIVE_SHADOW_MASK);
    
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
}

osg::Node *Cuboid::getNode()
{
    return _node;
}

osg::Drawable *Cuboid::getDrawable()
{
    return _drawable;
}

btRigidBody *Cuboid::getRigidBody()
{
    return _rigidBody;
}

AccelerationCuboid::AccelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    Cuboid(from, size)
{    
    osg::Texture2D *texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); 

    osg::Image *image = osgDB::readImageFile(ACCELERATION_CUBOID_TEXTURE);
    texture->setImage(image);
    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT); 
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
        
    osg::StateSet* stateSet = getNode()->getStateSet();
    stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
}