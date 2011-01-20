#include "Game.h"

Game::Game(osgViewer::Viewer *viewer) :
	RenderingInstance(viewer)
{
    _level = new Level("resources/levels/level1.xml");
    _player = new Player();
    
    getRootNode()->addChild(_level->getNode());
    getRootNode()->addChild(_player->getNode());
}

void Game::configureViewer(osgViewer::Viewer *viewer)
{
    
}
