#include "InputManager.h"

InputManager::InputManager(GameManager *manager) :
    _manager(manager)
{
    
}

bool InputManager::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    KeyState keyState;
    Key key;
	
	switch(ea.getEventType())
	{
		case osgGA::GUIEventAdapter::KEYDOWN:
			keyState = pressed;
			break;
		case osgGA::GUIEventAdapter::KEYUP:
			keyState = released;
			break;
		default:
			return false;
	}
		
	switch(ea.getKey())
	{
        case osgGA::GUIEventAdapter::KEY_Left:
            key = Left;
            break;
        case osgGA::GUIEventAdapter::KEY_Right:
            key = Right;
            break;
        case osgGA::GUIEventAdapter::KEY_Up:
            key = Up;
            break;
        case osgGA::GUIEventAdapter::KEY_Down:
            key = Down;
            break;
        case osgGA::GUIEventAdapter::KEY_Space:
            key = Space;
            break;
        case osgGA::GUIEventAdapter::KEY_Return:
            if(keyState == released)
                return false;
            key = Enter;
            break;
        case osgGA::GUIEventAdapter::KEY_Escape:
            if(keyState == released)
                return false;
            key = Escape;
            break;
		default:
			return false;
	}
	
	// let GameManager instance handle the input
    _manager->handleUserInput(key, keyState);
	
	return true;
}

void InputManager::accept(osgGA::GUIEventHandlerVisitor& v)
{
    v.visit(*this);
}