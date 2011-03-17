#include "Level.h"

Level::Level(const std::string &mapfile)
{
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
        else if(strcmp(it->name(), "cuboidtunnel") == 0)
            addCuboidTunnel(*it);
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
	
    addChild(cuboid);
    _collisionObjects.push_back(cuboid->getRigidBody());
    
    // save minimal x values for buckets of close y values
    // needed for detection whether player is falling to dead    
    int yBucketIndex = (int)((from.y() + size.y()) / 20.0f);
	
	while(_minZValues.size() <= yBucketIndex)
        _minZValues.push_back(from.z());
    
    // if current cuboid is lower then z -> adjust bucket value
    if(from.z() < _minZValues[yBucketIndex])
        _minZValues[yBucketIndex] = from.z();
}

void Level::addTunnel(const rapidxml::xml_node<> &tunnelNode)
{
	osg::Vec3 position = getVectorFromXMLNode("position", tunnelNode);
	Tunnel *tunnel = new Tunnel(position, atof(tunnelNode.first_attribute("length")->value()));

	addChild(tunnel->getNode());
	_collisionObjects.push_back(tunnel->getRigidBody());
}

void Level::addCuboidTunnel(const rapidxml::xml_node<> &tunnelNode)
{
	osg::Vec3 position = getVectorFromXMLNode("position", tunnelNode);
	CuboidTunnel *tunnel = new CuboidTunnel(position, atof(tunnelNode.first_attribute("length")->value()));

	addChild(tunnel->getNode());
	_collisionObjects.push_back(tunnel->getRigidBody());
}

std::vector<btRigidBody *> Level::getCollisionObjects() const
{
    return _collisionObjects;
}

const std::vector<float> *Level::getMinZValues() const
{
    return &_minZValues;
}
