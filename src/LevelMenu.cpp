#include <sstream>

#include "LevelMenu.h"
#include "MenuKeyboardHandler.h"


extern osgViewer::Viewer viewer;

LevelMenu::LevelMenu() :
    _currentLevel(NULL),
    _currentItemIndex(0)
{
	_menuPat = new osg::PositionAttitudeTransform();
	_menuPat->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    _keyboardHandler = new MenuKeyboardHandler(this);
    viewer.addEventHandler(_keyboardHandler);

	initializeCamera();
    initializeHeader();
    initializeBackgroundAnimation();
    initializeSelector();
    loadLevels();
    updateDetails();
            
    viewer.getCamera()->setUpdateCallback(new LevelMenuUpdater(this));

    Sound::getInstance()->playInLoop(MENU_MUSIC_FILE);    
}

void LevelMenu::initializeHeader()
{
    osg::PositionAttitudeTransform *headerPat = new osg::PositionAttitudeTransform();
    osg::Geode *headerGeode = new osg::Geode();
    osg::Geometry *textureDrawable = new osg::Geometry();
    osg::Texture2D *texture;

    osg::Vec3Array *vertices = new osg::Vec3Array();
    {
        vertices->push_back(osg::Vec3(0, 0, 0));
        vertices->push_back(osg::Vec3(436, 0, 0));
        vertices->push_back(osg::Vec3(436, 75, 0));
        vertices->push_back(osg::Vec3(0, 75, 0));
    }

    textureDrawable->setVertexArray( vertices );

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
    headerGeode->addDrawable(textureDrawable);
    headerPat->addChild(headerGeode);
    _menuPat->addChild(headerPat);
    
    headerPat->setPosition(osg::Vec3(100, viewer.getCamera()->getViewport()->height() - 125, -0.01)); 
	
    osg::Image *image = osgDB::readImageFile(LEVEL_HEADER_TEXTURE);
    texture->setImage(image);
}

void LevelMenu::initializeBackgroundAnimation()
{
	osg::Node* rotModel = osgDB::readNodeFile(MENU_BACKGROUND_MODEL);

	if(!rotModel)
	{
		throw std::runtime_error("Unable to load player model file!");
	}

	_background = new osg::MatrixTransform;
	_background->addChild(rotModel);

	osg::MatrixTransform* transMatrix = new osg::MatrixTransform;
	transMatrix->addChild(_background);

	transMatrix->setMatrix(osg::Matrix::translate(-2.0, 20.0, -5.0) * osg::Matrix::scale(1.0, 1.0, 1.0));

	addChild(transMatrix);
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

void LevelMenu::resetCamera()
{
    viewer.setCameraManipulator(NULL); 
    viewer.getCamera()->setViewMatrixAsLookAt(MENU_CAMERA_HOME_EYE, MENU_CAMERA_HOME_CENTER, MENU_CAMERA_HOME_UP);
}

void LevelMenu::initializeSelector()
{   	
    osg::PositionAttitudeTransform *selectorPat = new osg::PositionAttitudeTransform();
    osg::Geode *selectorGeode = new osg::Geode();
    osg::Geometry *textureDrawable = new osg::Geometry();
    osg::Texture2D *texture;

    osg::Vec3Array *vertices = new osg::Vec3Array();
    {
        vertices->push_back(osg::Vec3(0, 0, 0));
        vertices->push_back(osg::Vec3(682, 0, 0));
        vertices->push_back(osg::Vec3(682, 172, 0));
        vertices->push_back(osg::Vec3(0, 172, 0));
    }

    textureDrawable->setVertexArray( vertices );

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
    
    selectorPat->setPosition(osg::Vec3(viewer.getCamera()->getViewport()->width() - 900, viewer.getCamera()->getViewport()->height() - 325, -0.01)); 
	
    osg::Image *image = osgDB::readImageFile(LEVEL_SELECTOR_TEXTURE);
    texture->setImage(image);
    
    //////////////////
    
    osg::PositionAttitudeTransform *detailsPat = new osg::PositionAttitudeTransform();

    // completions
    {
        osg::Geode *completionsNode = new osg::Geode();

	    _completionsText = new osgText::Text();
	    _completionsText->setFont(MENU_FONT);
	    _completionsText->setCharacterSize(MENU_DETAIL_FONT_SIZE);
	    _completionsText->setPosition(osg::Vec3(0, - MENU_ITEM_HEIGHT, 0));

        completionsNode->addDrawable(_completionsText);
        detailsPat->addChild(completionsNode);
    }
    
    // deaths
    {
        osg::Geode *deathsNode = new osg::Geode();

	    _deathsText = new osgText::Text();
	    _deathsText->setFont(MENU_FONT);
	    _deathsText->setCharacterSize(MENU_DETAIL_FONT_SIZE);
	    _deathsText->setPosition(osg::Vec3(0, - MENU_ITEM_HEIGHT * 2, 0));

        deathsNode->addDrawable(_deathsText);
        detailsPat->addChild(deathsNode);
    }
    
    // best time
    {
        osg::Geode *bestTimeNode = new osg::Geode();

	    _bestTimeText = new osgText::Text();
	    _bestTimeText->setFont(MENU_FONT);
	    _bestTimeText->setCharacterSize(MENU_DETAIL_FONT_SIZE);    	
	    _bestTimeText->setPosition(osg::Vec3(0, - MENU_ITEM_HEIGHT * 3, 0));

        bestTimeNode->addDrawable(_bestTimeText);
        detailsPat->addChild(bestTimeNode);
    }
    

    detailsPat->setPosition(osg::Vec3(viewer.getCamera()->getViewport()->width() - 860, viewer.getCamera()->getViewport()->height() - 170, 0));
    _menuPat->addChild(detailsPat);
}

void LevelMenu::loadLevels()
{
    _itemsPat = new osg::PositionAttitudeTransform();
	_itemsPat->setPosition(osg::Vec3(viewer.getCamera()->getViewport()->width() - 400, viewer.getCamera()->getViewport()->height() - 250, 0)); 
    
    // load XML document
    rapidxml::file<> mf(LEVEL_OVERVIEW_FILE);
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
            item["filename"] = it->first_attribute("filename")->value();
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
        updateDetails();
    }        
}

void LevelMenu::selectNextItem()
{
    if(_currentItemIndex < _items.size() - 1)
    {
        _itemsPat->setPosition(_itemsPat->getPosition() + osg::Vec3(0, MENU_ITEM_HEIGHT, 0)); 
        _currentItemIndex++;
        updateDetails();
    }   
}

void LevelMenu::updateDetails()
{
    _completionsText->setText("Completions: " + _items[_currentItemIndex]["completions"]);
    _deathsText->setText("Deaths: " + _items[_currentItemIndex]["deaths"]);

    if(_items[_currentItemIndex]["besttime"] == "")
        _bestTimeText->setText("Best Time: --:--:--");
    else
    {
        time_t t = (time_t)atol(_items[_currentItemIndex]["besttime"].c_str());
        
        // extract miliseconds, seconds and minutes
        time_t ms = t % 100;
        time_t  s = (t / 100) % 60;
        time_t  m = (t / 100 / 60) % 60;

        // construct time string
    	std::stringstream ss;
    	ss <<
    	    (m  < 10 ? "0" : "") << m << ":" <<
    	    (s  < 10 ? "0" : "") << s << ":" <<
            (ms < 10 ? "0" : "") << ms;

        _bestTimeText->setText("Best Time: " + ss.str());
    }
}

void LevelMenu::runSelectedLevel()
{
    _currentLevel = new Level(_items[_currentItemIndex]["filename"]);
    viewer.setSceneData(_currentLevel);
}

void LevelMenu::returnFromLevel()
{
   	
    if(_currentLevel->playerReachedFinish())
    {
        // update completions
        {
            std::stringstream ss;
            ss << atoi(_items[_currentItemIndex]["completions"].c_str()) + 1;
            _items[_currentItemIndex]["completions"] = ss.str();
        }
        
        // update best time
        {
            time_t t = _currentLevel->getTime();

            if(_items[_currentItemIndex]["besttime"] == "" | t < atol(_items[_currentItemIndex]["besttime"].c_str()))
            {
                std::stringstream ss;
                ss << t;
                _items[_currentItemIndex]["besttime"] =  ss.str();
            }
        }
    }

    // update number of deaths
    {
        std::stringstream ss;
        ss << atoi(_items[_currentItemIndex]["deaths"].c_str()) + _currentLevel->getNumDeaths();
        _items[_currentItemIndex]["deaths"] = ss.str();
    }
    
    viewer.setCameraManipulator(NULL); 
    viewer.getCamera()->setViewMatrixAsLookAt(MENU_CAMERA_HOME_EYE, MENU_CAMERA_HOME_CENTER, MENU_CAMERA_HOME_UP);
	
    _currentLevel->resetScene();
    viewer.setSceneData(this);
    _currentLevel = NULL;
    
    Player::getInstance()->reset();
    updateDetails();
    
    Sound::getInstance()->stop(LEVEL_MUSIC_FILE);
    Sound::getInstance()->playInLoop(MENU_MUSIC_FILE);
}

void LevelMenu::writeBackLevelFile()
{
    std::ofstream overviewFile;
    overviewFile.open(LEVEL_OVERVIEW_FILE);
        
    overviewFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    overviewFile << "<world name=\"Galactica\">" << std::endl;
    
    for(size_t i=0; i<_items.size(); i++)
    {
       overviewFile << "\t<road name=\"" << _items[i]["name"] 
                    << "\" filename=\""  << _items[i]["filename"] 
                    << "\" besttime=\""  << _items[i]["besttime"] 
                    << "\" completions=\""  << _items[i]["completions"] 
                    << "\" deaths=\""  << _items[i]["deaths"]
                    << "\" />" << std::endl;
    }
    
    overviewFile << "</world>" << std::endl;
    overviewFile.close();
}


LevelMenuUpdater::LevelMenuUpdater(LevelMenu *menu) :
    _menu(menu),
    _previousStepTime(0.0f)
{

}

void LevelMenuUpdater::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    double currentStepTime = viewer.getFrameStamp()->getSimulationTime();
    
    if(_menu->levelRunning())
    {
        if(_menu->getCurrentLevel()->playerReachedFinish())
            _menu->returnFromLevel();
    }
    else if(_previousStepTime > 0.0f) 
    {
            _menu->resetCamera();
	        _menu->getBackground()->postMult(osg::Matrix::rotate(osg::inDegrees((currentStepTime - _previousStepTime) * 20.0f),0.0f,0.0f,1.0f));
    }
    
    _previousStepTime = currentStepTime;
}


