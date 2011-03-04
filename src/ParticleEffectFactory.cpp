#include "ParticleEffectFactory.h"

// ##############
// ParticleEffect
// ##############

ParticleEffect::ParticleEffect(osgParticle::Particle *particleTemplate, osgParticle::Counter *counter, osgParticle::Placer *placer, osgParticle::Shooter *shooter)
{
    _effectRoot = new osg::Group;
    
    configureTemplateParticle(particleTemplate);
    
    _particleSystem = new osgParticle::ParticleSystem;
    _particleSystem->setDefaultAttributes("resources/particles/circle.png", false, false);
    _particleSystem->setDefaultParticleTemplate(_templateParticle);
    
    _updater = new osgParticle::ParticleSystemUpdater;
    _updater->addParticleSystem(_particleSystem);
    
    osg::Geode *particleSystemGeode = new osg::Geode;
    particleSystemGeode->addDrawable(_particleSystem);
    
    configureCounter(counter);
    configurePlacer(placer);
    configureShooter(shooter);
    
    // create a modular emitter using the previously defined counter, placer and shooter
    _emitter = new osgParticle::ModularEmitter;
    _emitter->setParticleSystem(_particleSystem);
    _emitter->setCounter(_counter);
    _emitter->setPlacer(_placer);
    _emitter->setShooter(_shooter);
    
    _program = new osgParticle::ModularProgram;
    _program->setParticleSystem(_particleSystem);

    osgParticle::AccelOperator *op1 = new osgParticle::AccelOperator;
    op1->setAcceleration(osg::Vec3(0.0, -0.5, 0.0));
    _program->addOperator(op1);

    // add particle system, program and updater to scene
    _effectRoot->addChild(particleSystemGeode);
    _effectRoot->addChild(_program);
    _effectRoot->addChild(_updater);
    
    // add emitter to this so it can be positioned
    addChild(_emitter);
}

void ParticleEffect::configureTemplateParticle(osgParticle::Particle *templateParticle)
{
    if(templateParticle)
    {
        _templateParticle = *templateParticle;
        return;
    }
    
    _templateParticle.setLifeTime(3);
    _templateParticle.setSizeRange(osgParticle::rangef(0.1f, 0.2f));
    _templateParticle.setAlphaRange(osgParticle::rangef(1.0f, 0.0f));
    _templateParticle.setColorRange(osgParticle::rangev4(osg::Vec4(1, 0.5f, 0.3f, 1.0f), osg::Vec4(0, 0.7f, 1.0f, 0.0f)));
    _templateParticle.setRadius(0.01f);
    _templateParticle.setMass(0.00f);
}

void ParticleEffect::configureCounter(osgParticle::Counter *counter)
{
    if(counter)
    {
        _counter = counter;
        return;
    }
    
    _counter = new osgParticle::RandomRateCounter;
    ((osgParticle::RandomRateCounter*)_counter)->setRateRange(200, 250);
}

void ParticleEffect::configurePlacer(osgParticle::Placer *placer)
{
    if(placer)
    {
        _placer = placer;
        return;
    }
    
    _placer = new osgParticle::SectorPlacer;
    ((osgParticle::SectorPlacer*)_placer)->setCenter(0, 0, 0);
    ((osgParticle::SectorPlacer*)_placer)->setRadiusRange(0.5, 0.5);
    ((osgParticle::SectorPlacer*)_placer)->setPhiRange(0, 2 * osg::PI);
}

void ParticleEffect::configureShooter(osgParticle::Shooter *shooter)
{
    if(shooter)
    {
        _shooter = shooter;
        return;
    }
    
    _shooter = new osgParticle::RadialShooter;
    ((osgParticle::RadialShooter*)_shooter)->setInitialSpeedRange(0.5, 0.5);
}

osg::Group *ParticleEffect::getEffectRoot()
{
    return _effectRoot;
}

// #####################
// ParticleEffectFactory
// #####################

ParticleEffectFactory::ParticleEffectFactory(osg::Group *rootNode)
{
    _geode = new osg::Geode;
}

ParticleEffect *ParticleEffectFactory::createRearEngineEffect()
{
    return new ParticleEffect();
}