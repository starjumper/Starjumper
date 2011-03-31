#include "LevelMenu.h"
#include "MenuKeyboardHandler.h"

extern osgViewer::Viewer viewer;

LevelMenu::LevelMenu() 
{
	_menuPat = new osg::PositionAttitudeTransform();
	_menuPat->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    viewer.addEventHandler(new MenuKeyboardHandler(this));

	initializeCamera();
    initializeSelector();
    loadLevels();
    
    _currentItemIndex = 0;
    
    Sound::switchBackgroundMusic(MENU_MUSIC_FILE, "MenuMusic");
}

void LevelMenu::initializeCamera()
{   	
	_camera = new osg::Camera();
	_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, viewer.getCamera()->getViewport()->width(), 0, viewer.getCamera()->getViewport()->height()));
	_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	_camera->setViewMatrix(osg::Matrix::identity());
	
	_camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    _camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	_camera->setRenderOrder(osg::Camera::PRE_RENDER);
	_camera->addChild(_menuPat);
    addChild(_camera);
}

void LevelMenu::initializeSelector()
{   	
    osg::PositionAttitudeTransform *selectorPat = new osg::PositionAttitudeTransform();
    osg::Geode *selectorGeode = new osg::Geode();
    osg::Geometry *textureDrawable = new osg::Geometry();
    osg::Texture2D *texture;

    osg::Vec3Array *pyramidVertices = new osg::Vec3Array();
    {
        pyramidVertices->push_back(osg::Vec3(0, 0, 0));
        pyramidVertices->push_back(osg::Vec3(682, 0, 0));
        pyramidVertices->push_back(osg::Vec3(682, 172, 0));
        pyramidVertices->push_back(osg::Vec3(0, 172, 0));
    }

    textureDrawable->setVertexArray( pyramidVertices );

    osg::DrawElementsUInt *face = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
    face->push_back(0);
    face->push_back(1);
    face->push_back(2);
    face->push_back(3);
    
    textureDrawable->addPrimitiveSet(face);
        
    osg::Vec2Array* texcoords = new osg::Vec2Array(4);
    {
        (*texcoords)[0].set(0.0f, 0.0f);
        (*texcoords)[1].set(1.0f, 0.0f); 
        (*texcoords)[2].set(1.0f, 1.0f);
        (*texcoords)[3].set(0.0f, 1.0f);

        textureDrawable->setTexCoordArray(0, texcoords);
    }
             
    texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); 
    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT); 
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

    osg::StateSet* stateSet = new osg::StateSet();
    stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
    
    textureDrawable->setStateSet(stateSet);
    selectorGeode->addDrawable(textureDrawable);
    selectorPat->addChild(selectorGeode);
    _menuPat->addChild(selectorPat);
    
    selectorPat->setPosition(osg::Vec3(viewer.getCamera()->getViewport()->width() - 1000, viewer.getCamera()->getViewport()->height() - 275, -0.01)); 
	
    osg::Image *image = osgDB::readImageFile(LEVEL_SELECTOR_TEXTURE);
    texture->setImage(image);    
}

void LevelMenu::loadLevels()
{
    _itemsPat = new osg::PositionAttitudeTransform();
	_itemsPat->setPosition(osg::Vec3(viewer.getCamera()->getViewport()->width() - 500, viewer.getCamera()->getViewport()->height() - 200, 0)); 
    
    // load XML document
    rapidxml::file<> mf("resources/levels/overview.xml");
    rapidxml::xml_document<> xml_doc;
    xml_doc.parse<0>(mf.data());
    
    int itemIndex = 0;
    
    // parse XML document
    for(rapidxml::node_iterator<char> it(xml_doc.first_node()); it.dereference() != NULL; ++it, ++itemIndex)
    {
        if(strcmp(it->name(), "road") == 0)
        {         
            std::string name = it->first_attribute("name")->value();

        	osg::Geode *itemNode = new osg::Geode();

        	osgText::Text *text = new osgText::Text();
        	text->setFont(MENU_FONT);
        	text->setPosition(osg::Vec3(0, - (itemIndex * MENU_ITEM_HEIGHT), 0));
            text->setText(name);

            itemNode->addDrawable(text);
            _itemsPat->addChild(itemNode);
            
            std::map<std::string, std::string> item;

            item["name"] = it->first_attribute("name")->value();
            item["besttime"] = it->first_attribute("besttime")->value();
            item["completions"] = it->first_attribute("completions")->value();
            item["deaths"] = it->first_attribute("deaths")->value();
            
            _items.push_back(item);
        }
        else
        {
            throw std::runtime_error("Error: Unrecognized element in level file!");
        }
    }
    
    _menuPat->addChild(_itemsPat);
}

void LevelMenu::selectPreviousItem()
{
    if(_currentItemIndex > 0)
    {
        _itemsPat->setPosition(_itemsPat->getPosition() - osg::Vec3(0, MENU_ITEM_HEIGHT, 0));
        _currentItemIndex--;
    }
        
}

void LevelMenu::selectNextItem()
{
    if(_currentItemIndex < _items.size() - 1)
    {
        _itemsPat->setPosition(_itemsPat->getPosition() + osg::Vec3(0, MENU_ITEM_HEIGHT, 0)); 
        _currentItemIndex++;
    }   
}

void LevelMenu::runSelectedLevel()
{
    viewer.setSceneData(new Level("resources/levels/level1.xml"));
}