#include "Game.h"

Game::Game(osgViewer::Viewer *viewer) :
	RenderingInstance(viewer)
{
    _level = new Level("resources/levels/level1.xml");
    
    getRootNode()->addChild(_level->getNode());
}

void Game::configureViewer(osgViewer::Viewer *viewer)
{
    
}
