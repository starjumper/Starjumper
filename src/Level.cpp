#include "Level.h"

//Level::Level(btDynamicsWorld *world, const std::string mapfile) :
//    _world(world)
Level::Level(const std::string &mapfile)
{
    _level = new osg::PositionAttitudeTransform();

    loadMapFromFile(mapfile);
}

void Level::loadMapFromFile(const std::string &mapfile)
{
    // load XML document
    rapidxml::file<> mf(mapfile.c_str());
    rapidxml::xml_document<> xml_doc;
    xml_doc.parse<0>(mf.data());
    
    // parse XML document
    for(rapidxml::node_iterator<char> it(xml_doc.first_node()); it.dereference() != NULL; ++it)
    {
        if(strcmp(it->name(), "cuboid") == 0)
            addCuboid(*it);
        else if(strcmp(it->name(), "tunnel") == 0)
            addTunnel(*it);
		else
            throw std::runtime_error("Error: Unrecognized element in level file!");
    }
}

osg::Vec3 Level::getVectorFromXMLNode(const std::string &name, const rapidxml::xml_node<> &node) const
{
    rapidxml::xml_node<> *vectorNode = node.first_node(name.c_str());
    
    if(!vectorNode)
    {
        throw std::runtime_error("Error: Level element missing position node!");
    }
        
    float x, y, z;
    
    try
    {
        x = atof(vectorNode->first_attribute("x")->value());
        y = atof(vectorNode->first_attribute("y")->value());
        z = atof(vectorNode->first_attribute("z")->value());
    }
    catch(...)
    {
        throw std::runtime_error("Error: " + name + " node missing either x, y or z attribute!");
    }
    
    return osg::Vec3(x, y, z);   
};

void Level::addCuboid(const rapidxml::xml_node<> &cuboidNode)
{    
    osg::Vec3 from = getVectorFromXMLNode("position", cuboidNode);
    osg::Vec3 size = getVectorFromXMLNode("size", cuboidNode);
 
    Cuboid *cuboid = NULL;
    
    if(cuboidNode.first_attribute("type") != 0)
    {
        std::string type = cuboidNode.first_attribute("type")->value();

        if(type == "accelerate")
            cuboid = new AccelerationCuboid(from, size);
        else if(type == "decelerate")
            cuboid = new DecelerationCuboid(from, size);
    }

    if(cuboid == NULL)
    {
        cuboid = new Cuboid(from, size);
    }
	
    _level->addChild(cuboid->getNode());
    _collisionObjects.push_back(cuboid->getRigidBody());
}

void Level::addTunnel(const rapidxml::xml_node<> &tunnelNode)
{
	osg::Node *tunnelModel = osgDB::readNodeFile(TUNNEL_MODEL_FILE);
	osg::PositionAttitudeTransform *tunnelTransform = new osg::PositionAttitudeTransform();
    osg::Vec3 position = getVectorFromXMLNode("position", tunnelNode);

	tunnelTransform->addChild(tunnelModel);
	tunnelTransform->setPosition(position);
	tunnelTransform->setScale(osg::Vec3f(1.0f, atof(tunnelNode.first_attribute("length")->value()), 1.0f));
	
	tunnelTransform->setNodeMask(RECEIVE_SHADOW_MASK);
	_level->addChild(tunnelTransform);    
	
    /*
	btConvexTriangleMeshShape* mesh = osgbBullet::btConvexTriMeshCollisionShapeFromOSG(tunnelPat);

	btTransform shapeTransform;
	shapeTransform.setIdentity();
	cs->addChildShape(shapeTransform, mesh);
	*/
}

osg::PositionAttitudeTransform *Level::getNode() const
{
    return _level;
}

std::vector<btRigidBody *> Level::getCollisionObjects() const
{
    return _collisionObjects;
}