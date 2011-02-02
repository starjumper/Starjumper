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
		
	_shapeDrawable = new osg::ShapeDrawable(new osg::Box(center, width, depth, height));
	_shapeDrawable->setColor(DEFAULT_CUBOID_COLOR);

	_node = new osg::Geode();
	_node->addDrawable(_shapeDrawable);
}

osg::Node *Cuboid::getNode()
{
    return _node;
}

osg::ShapeDrawable *Cuboid::getShapeDrawable()
{
    return _shapeDrawable;
}

AccelerationCuboid::AccelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size) :
    Cuboid(from, size)
{
//    getShapeDrawable()->setColor(ACCELERATION_CUBOID_COLOR);
    
    osg::Texture2D *texture = new osg::Texture2D;

    // protect from being optimized away as static state:
    texture->setDataVariance(osg::Object::DYNAMIC); 

    // load an image by reading a file: 
    osg::Image *image = osgDB::readImageFile(ACCELERATION_CUBOID_TEXTURE);

    if (!image)
    {
        std::cout << " couldn't find texture, quiting." << std::endl;
        exit(1);
    }

    // Assign the texture to the image we read from file: 
    texture->setImage(image);
    
    osg::StateSet* stateSet = new osg::StateSet();
    
    stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
    
    getNode()->setStateSet(stateSet);
}