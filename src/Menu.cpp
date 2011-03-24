#include "Menu.h"

Menu::Menu(osgViewer::Viewer *viewer) :
    RenderingInstance(viewer)
{
    _windowManager = new osgWidget::WindowManager(
        viewer,
       	viewer->getCamera()->getViewport()->width(),
       	viewer->getCamera()->getViewport()->height(),
        MASK_2D
    );  

	_buttonList = new osgWidget::Box("menu", osgWidget::Box::VERTICAL);
	
	// position the menu in the window
	_buttonList->setAnchorVertical(osgWidget::Window::VA_CENTER);
    _buttonList->setAnchorHorizontal(osgWidget::Window::HA_CENTER);

  	_buttonList->getBackground()->setColor(MENU_BACKGROUND_COLOR);

    _buttonList->attachMoveCallback();
    _buttonList->attachScaleCallback();
    _buttonList->resize();

    _windowManager->addChild(_buttonList);
    _windowManager->resizeAllWindows();

	// create a camera which is orthogonal to the window for having a 2D view
    osg::Camera *camera = _windowManager->createParentOrthoCamera();
    getRootNode()->addChild(camera);
    
    getViewer()->addEventHandler(new osgWidget::MouseHandler(_windowManager));

	initializeBackground();
}

void Menu::initializeBackground()
{
	// rootNode -> transMatrix -> _rotate -> rotModel
	osg::Node* rotModel = osgDB::readNodeFile(PLAYER_MODEL_HIGH);
	if(!rotModel)
	{
		throw std::runtime_error("Unable to load player model file!");
	}

	_rotate = new osg::MatrixTransform;
	//_rotate->addChild(rotModel);

	osg::MatrixTransform* transMatrix = new osg::MatrixTransform;
	transMatrix->addChild(_rotate);

	transMatrix->setMatrix(osg::Matrix::translate(-2.0, 20.0, 0.0) * osg::Matrix::scale(1.0, 1.0, 1.0));

	MenuUpdater* menuUpdater = new MenuUpdater(this);
	_rotate->setUpdateCallback(menuUpdater);

	getRootNode()->addChild(transMatrix);


	//
	osg::Program *programObject = new osg::Program();
	osg::Shader *vertexObject = new osg::Shader(osg::Shader::VERTEX);
	osg::Shader *geometryObject = new osg::Shader(osg::Shader::GEOMETRY);
	programObject->addShader(vertexObject);
	programObject->addShader(geometryObject);
	vertexObject->loadShaderSourceFromFile("resources/shaders/test.vert");
	geometryObject->loadShaderSourceFromFile("resources/shaders/shrink.geom");

	osg::StateSet *test = rotModel->getOrCreateStateSet();
	test->setAttributeAndModes(programObject, osg::StateAttribute::ON);

	_rotate->addChild(rotModel);
}

osg::MatrixTransform *Menu::getRotate()
{
	return _rotate;
}

void Menu::addButton(const char* label, std::tr1::function<void ()> callback)
{
	MenuButton *button = new MenuButton(label, callback);
	
	_buttonList->addWidget(button);
}

void Menu::prepare(osgViewer::Viewer *viewer)
{
    
}

void Menu::cleanup(osgViewer::Viewer *viewer)
{
    
}

MenuUpdater::MenuUpdater(Menu *menu) :
    _menu(menu)
{

}

void MenuUpdater::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
	_menu->getRotate()->postMult(osg::Matrix::rotate(osg::inDegrees(0.5f),0.0f,0.0f,1.0f));
}