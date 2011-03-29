#include "Credits.h"

Credits::Credits(osgViewer::Viewer *viewer) :
    RenderingInstance(viewer)
{
	osgText::Text3D* text3D = new osgText::Text3D;
    text3D->setFont("fonts/arial.ttf");
    text3D->setDrawMode(osgText::Text3D::TEXT | osgText::Text3D::BOUNDINGBOX);
    text3D->setAxisAlignment(osgText::Text3D::XZ_PLANE);
    text3D->setText("Ingame Music: Andromeda from Libra");
	text3D->setPosition(osg::Vec3(900, 700, 0));

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(text3D);
    getRootNode()->addChild(geode);

	initializeBackground();
}

void Credits::initializeBackground()
{
	// rootNode -> transMatrix -> _rotate -> rotModel
	osg::Node* rotModel = osgDB::readNodeFile(PLAYER_MODEL_HIGH);
	if(!rotModel)
	{
		throw std::runtime_error("Unable to load player model file!");
	}

	_rotate = new osg::MatrixTransform;
	_rotate->addChild(rotModel);

	osg::MatrixTransform* transMatrix = new osg::MatrixTransform;
	transMatrix->addChild(_rotate);

	transMatrix->setMatrix(osg::Matrix::translate(-2.0, 20.0, 0.0) * osg::Matrix::scale(1.0, 1.0, 1.0));

	CreditsUpdater* creditsUpdater = new CreditsUpdater(this);
	_rotate->setUpdateCallback(creditsUpdater);

	getRootNode()->addChild(transMatrix);
}

osg::MatrixTransform *Credits::getRotate()
{
	return _rotate;
}

void Credits::prepare(osgViewer::Viewer *viewer)
{
    
}

void Credits::cleanup(osgViewer::Viewer *viewer)
{
    
}

CreditsUpdater::CreditsUpdater(Credits *credits) :
    _credits(credits)
{

}

void CreditsUpdater::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
	_credits->getRotate()->postMult(osg::Matrix::rotate(osg::inDegrees(0.5f),0.0f,0.0f,1.0f));
}