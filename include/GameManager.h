#pragma once

#include <map>
#include <string>

#if defined (_WIN32)
    #include <functional>
#else
	#include <tr1/functional>
#endif

#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_iterators.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "types.h"
#include "RenderingInstance.h"
#include "Sound.h"
#include "Menu.h"
#include "LevelOverview.h"
#include "Game.h"

#define GAME_MUSIC_FILE "resources/sound/andromeda.ogg"
#define MENU_MUSIC_FILE "resources/sound/48000_2chan.ogg"

class GameManagerKeyboardHandler;

class GameManager
{
private:
    osgViewer::Viewer _viewer;
    std::map<std::string, RenderingInstance *> _renderingInstances;
    RenderingInstance *_activeRenderingInstance;
    std::string _activeRenderingInstanceName;
    GameManagerKeyboardHandler *_keyboardHandler;    
    
    void addRenderingInstance(std::string name, RenderingInstance *instance);
    
	void buildMenus();

public:
    GameManager();

    void selectRenderingInstance(std::string name);
    std::string getActiveRenderingInstanceName();
    
    void run();
	void quit();
    void runLevel(const std::string &mapFile);
    void roadSelectMenu();
};

class GameManagerKeyboardHandler : public osgGA::GUIEventHandler
{
private:
    GameManager *_gameManager;
    
public:
    GameManagerKeyboardHandler(GameManager *gameManager);
    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    virtual void accept(osgGA::GUIEventHandlerVisitor &v);
};