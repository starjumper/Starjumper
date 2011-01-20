#include "Level.h"

//Level::Level(btDynamicsWorld *world, const std::string mapfile) :
//    _world(world)
Level::Level(const std::string mapfile)
{
    _level = new osg::PositionAttitudeTransform();
    
    loadMapFromFile(mapfile);
    
    // setup physics components
    _collisionShapes = new btCompoundShape();
}

void Level::loadMapFromFile(const std::string mapfile)
{
    // load XML document
    rapidxml::file<> mf(mapfile.c_str());
    rapidxml::xml_document<> xml_doc;
    xml_doc.parse<0>(mf.data());
    
    // parse XML document
    for(rapidxml::node_iterator<char> it(xml_doc.first_node()); it.dereference() != NULL; ++it)
    {
        if(strcmp((*it).name(), "cuboid") == 0)
        {
            addCuboid(*it);
        }
        else if(strcmp((*it).name(), "tunnel") == 0)
        {
            addTunnel(*it);
        }
		else
        {
            throw std::runtime_error("Error: Unrecognized element in level file!");
        }
    }
}

osg::Vec3 Level::getFromVector(const rapidxml::xml_node<> &node) const
{
    rapidxml::xml_node<> *position = node.first_node("position");
    
    if(!position)
        throw std::runtime_error("Error: Level element missing position node!");
    
    float x, y, z;
    
    try
    {
        x = atof(position->first_attribute("x")->value());
        y = atof(position->first_attribute("y")->value());
        z = atof(position->first_attribute("z")->value());
    }
    catch(...)
    {
        throw std::runtime_error("Error: position node missing either x, y or z attribute!");
    }
    
    return osg::Vec3(x, y, z);
}

osg::Vec3 Level::getToVector(const rapidxml::xml_node<> &node) const
{
    rapidxml::xml_node<> *size = node.first_node("size");
    
    if(!size)
        throw std::runtime_error("Error: Level element missing size node!");
    
    float x, y, z;
    
    try
    {
        x = atof(size->first_attribute("x")->value());
        y = atof(size->first_attribute("y")->value());
        z = atof(size->first_attribute("z")->value());
    }
    catch(...)
    {
        throw std::runtime_error("Error: size node missing either x, y or z attribute!");
    }
    
    return getFromVector(node) + osg::Vec3(x, y, z);
}

void Level::addCuboid(const rapidxml::xml_node<> &cuboidNode)
{
    osg::Vec3 from = getFromVector(cuboidNode);
    osg::Vec3 to = getToVector(cuboidNode);
    
    osg::ShapeDrawable *cuboid;
    
    osg::Vec3 center = osg::Vec3(
							0.5 * (from.x() + to.x()),
							0.5 * (from.y() + to.y()),
							0.5 * (from.z() + to.z()));

	float width = fabs(from.x() - to.x());
	float depth = fabs(from.y() - to.y());
	float height = fabs(from.z() - to.z());
	
	cuboid = new osg::ShapeDrawable(new osg::Box(center, width, depth, height));
	cuboid->setColor(DEFAULT_COLOR);

	osg::Geode *geode = new osg::Geode();
	geode->addDrawable(cuboid);	
	
    _level->addChild(geode);
    
    // create Bullet bounding box
    /*btBoxShape *bsCuboid = new btBoxShape(osgbBullet::asBtVector3(osg::Vec3(width, depth, height) / 2.0f));
    
    btTransform shapeTransform;
    shapeTransform.setIdentity();
    shapeTransform.setOrigin(osgbBullet::asBtVector3(center));
    
    _collisionShapes->addChildShape(shapeTransform, bsCuboid);*/
}

void Level::addTunnel(const rapidxml::xml_node<> &tunnelNode)
{
	osg::Node *tunnelModel = osgDB::readNodeFile(TUNNEL_MODEL_FILE);
	osg::PositionAttitudeTransform* tunnelTransform = new osg::PositionAttitudeTransform();

	tunnelTransform->addChild(tunnelModel);
	tunnelTransform->setPosition(getFromVector(tunnelNode));
	tunnelTransform->setScale(osg::Vec3f(1.0f, atof(tunnelNode.first_node("size")->first_attribute("y")->value()) / 8.0f, 1.0f));
	
	_level->addChild(tunnelTransform);
    /*
	btConvexTriangleMeshShape* mesh = osgbBullet::btConvexTriMeshCollisionShapeFromOSG(tunnelPat);

	btTransform shapeTransform;
	shapeTransform.setIdentity();
	cs->addChildShape(shapeTransform, mesh);*/
}

osg::PositionAttitudeTransform *Level::getNode()
{
    return _level;
}