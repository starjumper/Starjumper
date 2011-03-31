#include "MenuKeyboardHandler.h"

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
                    _levelMenu->selectPreviousItem();
                    break;
                case K_DOWN:
                    _levelMenu->selectNextItem();
                    break;
                case K_RETURN:
                    _levelMenu->runSelectedLevel();
                    break;
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