#include "LevelKeyboardHandler.h"

LevelKeyboardHandler::LevelKeyboardHandler()
{
}

bool LevelKeyboardHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    bool keyState;

    // check if key is pressed or released
    switch(ea.getEventType())
	{
	    case osgGA::GUIEventAdapter::KEYDOWN:
			keyState = K_PRESSED;
			break;
		case osgGA::GUIEventAdapter::KEYUP:
			keyState = K_RELEASED;
			break;
		default:
			return false;
	}

    // find out which key was pressed
	switch(ea.getKey())
	{
		case K_LEFT:
            Player::getInstance()->getPlayerState()->setRequestMoveLeft(keyState);
			break;
		case K_RIGHT:
			Player::getInstance()->getPlayerState()->setRequestMoveRight(keyState);
			break;
		case K_UP:
			Player::getInstance()->getPlayerState()->setRequestAccelerate(keyState);
			break;
		case K_DOWN:
			Player::getInstance()->getPlayerState()->setRequestDecelerate(keyState);
			break;
		case K_JUMP:
			Player::getInstance()->getPlayerState()->setRequestJump(keyState);
			break;
		default:
			return false;
	}

	return true;
}

void LevelKeyboardHandler::accept(osgGA::GUIEventHandlerVisitor &v)
{
    v.visit(*this);
}