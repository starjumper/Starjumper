#include "MenuKeyboardHandler.h"

extern osgViewer::Viewer viewer;

MenuKeyboardHandler::MenuKeyboardHandler(LevelMenu *levelMenu) :
    _levelMenu(levelMenu)
{
}

bool MenuKeyboardHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{    
    // check if key is pressed or released
    switch(ea.getEventType())
	{
	    case osgGA::GUIEventAdapter::KEYDOWN:
            switch(ea.getKey())
            {
                case K_UP:
                    if(!_levelMenu->levelRunning())
                        _levelMenu->selectPreviousItem();
                    break;
                case K_DOWN:
                    if(!_levelMenu->levelRunning())
                        _levelMenu->selectNextItem();
                    break;
                case K_RETURN:
                    if(!_levelMenu->levelRunning())
                        _levelMenu->runSelectedLevel();
                    break;
                case K_EXIT:
                    if(_levelMenu->levelRunning())
                        _levelMenu->returnFromLevel();
                    else
                    {
                        Sound::shutdownSoundManager();
                        _levelMenu->writeBackLevelFile();
                        exit(0);
                    }
						
                default:
                    return false;
            }
			break;
		default:
			return false;
	}



	return true;
}

void MenuKeyboardHandler::accept(osgGA::GUIEventHandlerVisitor &v)
{
    v.visit(*this);
}