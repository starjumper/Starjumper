#include "GameManager.h"

GameManager::GameManager()
{
    addRenderingInstance("menu", new Menu());
    addRenderingInstance("game", new Game());
    
    // configure viewer to use the primary screen only
    _viewer.setUpViewOnSingleScreen(0);
    
    // TODO: add keyboard/mouse event handler to the viewer
    
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