#include "Cuboid.h"

Cuboid::Cuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    _from(from),
    _size(size)
{
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

        drawable->setColorArray(colors);
        drawable->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    }

    osg::Vec2Array* texcoords = new osg::Vec2Array(8);
    {
        (*texcoords)[0].set(size.x(), 0.0f); // TOP
        (*texcoords)[1].set(0.0f, 0.0f); // TOP
        (*texcoords)[2].set(0.0f,  0.0f); // ""
        (*texcoords)[3].set(0.0f,  0.0f); // "" 
        (*texcoords)[4].set(size.x(), size.y() / 3.0f); // TOP
        (*texcoords)[5].set(0.0f,  size.y() / 3.0f); // TOP
        (*texcoords)[6].set(0.0f,  0.0f); // ""
        (*texcoords)[7].set(0.0f,  0.0f); // "" 

        drawable->setTexCoordArray(0,texcoords);
    }

    osg::StateSet* stateSet = new osg::StateSet();
    stateSet->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON); 
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    setStateSet(stateSet);
    addDrawable(drawable);
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

void Cuboid::collide()
{
    
}
