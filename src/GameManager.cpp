#include "GameManager.h"

GameManager::GameManager()
{
    addRenderingInstance("menu", new Menu());
    addRenderingInstance("game", new Game());
    
    // configure viewer to use the primary screen only
    _viewer.setUpViewOnSingleScreen(0);
    
	// set background color
	_viewer.getCamera()->setClearColor(osg::Vec4( 0., 0., 0., 1. )); 

    // setup InputManager to track user inputs via keyboard
    _inputManager = new InputManager(this);
    _viewer.addEventHandler(_inputManager);
    
    selectRenderingInstance("menu");
}

void GameManager::addRenderingInstance(std::string name, RenderingInstance *instance)
{
    _renderingInstances[name] = instance;
}

void GameManager::selectRenderingInstance(std::string name)
{
    _activeRenderingInstance = _renderingInstances[name];
    _activeRenderingInstance->configureViewer(&_viewer);
    _viewer.setSceneData(_activeRenderingInstance->getRootNode());
}

void GameManager::run()
{
    _viewer.run();
}

void GameManager::handleUserInput(Key key, KeyState keyState)
{
    if(key == Escape && keyState == pressed)
    {
        // toggle in-game menu
    }
    else
    {
        // if key does require a global action, let the active RenderingInstance handle it
        _activeRenderingInstance->handleUserInput(key, keyState);
    }
    
}