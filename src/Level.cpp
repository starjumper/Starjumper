#include "Level.h"

Level::Level(std::string mapfile)
{
    _level = new osg::PositionAttitudeTransform();
    
    loadMapFromFile(mapfile);
}

void Level::loadMapFromFile(std::string mapfile)
{
    // load XML document
    rapidxml::file<> mf(mapfile.c_str());
    rapidxml::xml_document<> xml_doc;
    xml_doc.parse<0>(mf.data());
    
    // parse XML document
    for(rapidxml::node_iterator<char> it(xml_doc.first_node()); ; ++it)
    {
        if(strcmp(it->name(), "cuboid") == 0)
            addCuboid(*it);
        else if(strcmp(it->name(), "tunnel") == 0)
            addTunnel(*it);
        else
            throw std::runtime_error("Error: Unrecognized element in level file!");
    }
}

osg::Vec3 Level::getFromVector(rapidxml::xml_node<> &node)
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

osg::Vec3 Level::getToVector(rapidxml::xml_node<> &node)
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

void Level::addCuboid(rapidxml::xml_node<> &cuboidNode)
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
}

void Level::addTunnel(rapidxml::xml_node<> &tunnelNode)
{
    // yet to be done
}