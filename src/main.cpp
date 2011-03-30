#include <osgViewer/Viewer>

#include "Player.h"

osgViewer::Viewer viewer;

int main(int argc, char *argv[])
{
    // disable escape key for quitting viewer
    // we use an own escape implementation
    viewer.setKeyEventSetsDone(0);

    // configure viewer to use the primary screen only
    viewer.setUpViewOnSingleScreen(0);
	
	// set background color
	viewer.getCamera()->setClearColor(osg::Vec4( 0., 0., 0., 1. ));
    
    viewer.setSceneData(Player::getInstance());
    viewer.run();
}