#include "Game.h"

Game::Game(osgViewer::Viewer *viewer) :
	RenderingInstance(viewer)
{
    _cameraManipulator = new LazyCameraManipulator();
    _level = new Level("resources/levels/level1.xml");
    _player = new Player();
    
    _cameraManipulator->setTrackNode(_player->getNode());
    
    getRootNode()->addChild(_level->getNode());
    getRootNode()->addChild(_player->getNode());
    
    getViewer()->setCameraManipulator(_cameraManipulator);
}

void Game::configureViewer(osgViewer::Viewer *viewer)
{
    
}
