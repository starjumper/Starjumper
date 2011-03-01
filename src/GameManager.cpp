#include "GameManager.h"

GameManager::GameManager() :
    _activeRenderingInstance(NULL)
{    
    // disable escape key for quitting viewer
    // we use an own escape implementation
    _viewer.setKeyEventSetsDone(0);

    // configure viewer to use the primary screen only
    _viewer.setUpViewOnSingleScreen(0);
    // avoids segfaults when updating the HUD
    _viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
	
	// set background color
	_viewer.getCamera()->setClearColor(osg::Vec4( 0., 0., 0., 1. )); 

	// window view for testing purpose, uncomment if needed
	// push with fullscreen ONLY!!!
	// _viewer.setUpViewInWindow(40, 40, 800, 600, 0);

    addRenderingInstance("game", new Game(&_viewer));
	buildMenus();

    selectRenderingInstance("main_menu");
    
    _keyboardHandler = new GameManagerKeyboardHandler(this);
    _viewer.addEventHandler(_keyboardHandler);
}

void GameManager::addRenderingInstance(std::string name, RenderingInstance *instance)
{
    _renderingInstances[name] = instance;
}

void GameManager::selectRenderingInstance(std::string name)
{
    if(_activeRenderingInstance)
	{
        _activeRenderingInstance->cleanup(&_viewer);
	}
    
    _activeRenderingInstanceName = name;
    _activeRenderingInstance = _renderingInstances[name];
    _activeRenderingInstance->prepare(&_viewer);
    _viewer.setSceneData(_activeRenderingInstance->getRootNode());
}

std::string GameManager::getActiveRenderingInstanceName()
{
    return _activeRenderingInstanceName;
}


void GameManager::buildMenus()
{
	// main menu
	{
		Menu *menu = new Menu(&_viewer);

		menu->addButton("Quit", std::tr1::bind(&GameManager::quit, this));
		menu->addButton("Highscore", std::tr1::bind(&GameManager::quit, this));
		menu->addButton("Settings", std::tr1::bind(&GameManager::quit, this));
		menu->addButton("Start game", std::tr1::bind(&GameManager::roadSelectMenu, this));

    	addRenderingInstance("main_menu", menu);
	}
	
	// road select menu
	{
	    LevelOverview *menu = new LevelOverview(&_viewer);
		
		// load XML document
        rapidxml::file<> mf("resources/levels/overview.xml");
        rapidxml::xml_document<> xml_doc;
        xml_doc.parse<0>(mf.data());

        // parse XML document
        for(rapidxml::node_iterator<char> it(xml_doc.first_node()); it.dereference() != NULL; ++it)
        {
            if(strcmp(it->name(), "road") == 0)
            {         
                std::string mapfile = it->first_attribute("filename")->value();
                menu->addButton(it->first_attribute("name")->value(), std::tr1::bind(&GameManager::runLevel, this, mapfile));
            }
    		else
    		{
                throw std::runtime_error("Error: Unrecognized element in level file!");
    		}
        }
		
	 	addRenderingInstance("road_select_menu", menu);   
	}	
}

void GameManager::run()
{
    _viewer.run();
}

void GameManager::quit()
{
	exit(0);
}

void GameManager::runLevel(const std::string &mapFile)
{
    selectRenderingInstance("game");
    ((Game *)_renderingInstances["game"])->runLevel(mapFile);
}

void GameManager::roadSelectMenu()
{
    selectRenderingInstance("road_select_menu");
}

// Handler for keyboard input

GameManagerKeyboardHandler::GameManagerKeyboardHandler(GameManager *gameManager) :
    _gameManager(gameManager)
{

}

bool GameManagerKeyboardHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    
    std::string activeRenderingInstanceName = _gameManager->getActiveRenderingInstanceName();

    // check if key is pressed or released
    switch(ea.getEventType())
	{
		case osgGA::GUIEventAdapter::KEYDOWN:
            // find out which key was pressed
        
	        switch(ea.getKey())
	        {
		        case K_EXIT:
            
                    if(activeRenderingInstanceName == "game")
                        _gameManager->selectRenderingInstance("road_select_menu");
                    else if(activeRenderingInstanceName == "road_select_menu")
                        _gameManager->selectRenderingInstance("main_menu");
                    else if(activeRenderingInstanceName == "main_menu")
                        exit(0);
                
                    break;
                    
		        default:
			        return false;
			}

            break;
            
		default:
			return false;
	}

	return false;
}

void GameManagerKeyboardHandler::accept(osgGA::GUIEventHandlerVisitor &v)
{
    v.visit(*this);
}