#include <osgViewer/Viewer>

#include "LevelMenu.h"
#include "Sound.h"

osgViewer::Viewer viewer;

int main(int argc, char *argv[])
{
    // disable escape key for quitting viewer
    // we use an own escape implementation
    viewer.setKeyEventSetsDone(0);

    // configure viewer to use the primary screen only
    viewer.setUpViewOnSingleScreen(0);

    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
	
	// set background color
	viewer.getCamera()->setClearColor(osg::Vec4( 0., 0., 0., 1. ));
    viewer.getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT);
         
    Sound::initSoundManager();
    
    osg::ref_ptr<LevelMenu> levelMenu = new LevelMenu();
	
    viewer.setSceneData(levelMenu);    
    viewer.run();
    
    Sound::shutdownSoundManager();
}