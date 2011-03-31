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
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgText/Text>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_iterators.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "Player.h"
#include "Level.h"

#define MENU_FONT               "fonts/arial.ttf"
#define MENU_MUSIC_FILE         "resources/sound/48000_2chan.ogg"
#define LEVEL_SELECTOR_TEXTURE  "resources/textures/menu_background.png"
#define MENU_ITEM_HEIGHT        40
#define MENU_DETAIL_FONT_SIZE   25
#define MENU_BACKGROUND_MODEL   "resources/models/player_high.osg"

class MenuKeyboardHandler;

class LevelMenu : public osg::Group
{
private:
    std::vector<std::map<std::string, std::string> > _items;
    int _currentItemIndex;

	osg::ref_ptr<osg::Camera> _camera;
	osg::PositionAttitudeTransform *_menuPat;
    osg::PositionAttitudeTransform *_itemsPat;
    osg::MatrixTransform *_background;
	
    osg::ref_ptr<osgText::Text> _bestTimeText;
    osg::ref_ptr<osgText::Text> _completionsText;
    osg::ref_ptr<osgText::Text> _deathsText;
    
    MenuKeyboardHandler *_keyboardHandler;
    Level *_currentLevel;

public:
	LevelMenu();

	void initializeCamera();
    void initializeBackground();
    void initializeSelector();
    void loadLevels();
    void updateDetails();
    
    void selectPreviousItem();
    void selectNextItem();    
    void runSelectedLevel();
    void returnFromLevel();

    osg::Camera *getCamera() { return _camera; };  
    bool levelRunning() { return _currentLevel != NULL; }
};
