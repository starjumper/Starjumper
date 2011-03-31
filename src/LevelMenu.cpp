#include "LevelMenu.h"
#include "MenuKeyboardHandler.h"

extern osgViewer::Viewer viewer;

LevelMenu::LevelMenu() 
{
	_menuPat = new osg::PositionAttitudeTransform();
	_menuPat->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    viewer.addEventHandler(new MenuKeyboardHandler(this));

	initializeCamera();
    loadLevels();
    
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
        	text->setPosition(osg::Vec3(0, - (itemIndex * 40), 0));
            text->setText(name);

            itemNode->addDrawable(text);
            _itemsPat->addChild(itemNode);

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
    _itemsPat->setPosition(_itemsPat->getPosition() + osg::Vec3(0, 30, 0));
}

void LevelMenu::selectNextItem()
{
    _itemsPat->setPosition(_itemsPat->getPosition() - osg::Vec3(0, 30, 0));    
}

void LevelMenu::runSelectedLevel()
{
    viewer.setSceneData(new Level("resources/levels/level1.xml"));
}