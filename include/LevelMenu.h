#pragma once

#include <osg/Geode>
#include <osg/Depth>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osg/Drawable>
#include <osg/Texture2D>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osgText/Text>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_iterators.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "Level.h"

#define MENU_FONT           "fonts/arial.ttf"
#define MENU_MUSIC_FILE     "resources/sound/48000_2chan.ogg"

class MenuKeyboardHandler;

class LevelMenu : public osg::Referenced
{
private:
	osg::ref_ptr<osg::Camera> _camera;
	osg::PositionAttitudeTransform *_menuPat;
    osg::PositionAttitudeTransform *_itemsPat;

public:
	LevelMenu();

	void initializeCamera();
    void loadLevels();
    
    void selectPreviousItem();
    void selectNextItem();    
    void runSelectedLevel();

    osg::Camera *getCamera() { return _camera; };    
};
