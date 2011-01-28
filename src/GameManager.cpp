#include "GameManager.h"

GameManager::GameManager() :
    _activeRenderingInstance(NULL)
{    
    // configure viewer to use the primary screen only
    _viewer.setUpViewOnSingleScreen(0);
    
    // avoids segfaults when updating the HUD
    _viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
	
	// set background color
	_viewer.getCamera()->setClearColor(osg::Vec4( 0., 0., 0., 1. )); 

	buildMenus();
    addRenderingInstance("game", new Game(&_viewer));

    selectRenderingInstance("main_menu");
}

void GameManager::addRenderingInstance(std::string name, RenderingInstance *instance)
{
    _renderingInstances[name] = instance;
}

void GameManager::selectRenderingInstance(std::string name)
{
    if(_activeRenderingInstance)
        _activeRenderingInstance->cleanup(&_viewer);
    
    _activeRenderingInstance = _renderingInstances[name];
    _activeRenderingInstance->prepare(&_viewer);
    _viewer.setSceneData(_activeRenderingInstance->getRootNode());
}

void GameManager::buildMenus()
{
	// main menu
	{
		Menu *menu = new Menu(&_viewer);

		menu->addButton("Quit", std::tr1::bind(&GameManager::quit, this));
		menu->addButton("Highscore", std::tr1::bind(&GameManager::quit, this));
		menu->addButton("Settings", std::tr1::bind(&GameManager::quit, this));
		menu->addButton("Start game", std::tr1::bind(&GameManager::startGame, this));

    	addRenderingInstance("main_menu", menu);
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

void GameManager::startGame()
{
    selectRenderingInstance("game");
}