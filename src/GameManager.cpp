#include "GameManager.h"

GameManager::GameManager()
{
    addRenderingInstance("menu", new Menu(&_viewer));
    addRenderingInstance("game", new Game(&_viewer));
    
    // configure viewer to use the primary screen only
    _viewer.setUpViewOnSingleScreen(0);
    
	// set background color
	_viewer.getCamera()->setClearColor(osg::Vec4( 0., 0., 0., 1. )); 
    
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