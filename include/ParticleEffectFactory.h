#include <osg/Group>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>

class ParticleEffect : public osg::PositionAttitudeTransform
{
private:
    osg::Group *_effectRoot;
    
    osgParticle::ParticleSystem *_particleSystem;
    osgParticle::ParticleSystemUpdater *_updater;
    
    osgParticle::Particle _templateParticle;
    osgParticle::ModularEmitter *_emitter;
    osgParticle::Counter *_counter;
    osgParticle::Placer *_placer;
    osgParticle::Shooter *_shooter;
    osgParticle::ModularProgram *_program;
    
    void configureTemplateParticle(osgParticle::Particle *particleTemplate = NULL);
    void configureCounter(osgParticle::Counter *counter = NULL);
    void configurePlacer(osgParticle::Placer *placer = NULL);
    void configureShooter(osgParticle::Shooter *shooter = NULL);
    
public:
    ParticleEffect(osgParticle::Particle *particleTemplate = NULL, osgParticle::Counter *counter = NULL, osgParticle::Placer *placer = NULL, osgParticle::Shooter *shooter = NULL);
    
    osg::Group *getEffectRoot();
};

class ParticleEffectFactory
{
private:
    osg::Geode *_geode;
    
public:
    ParticleEffectFactory(osg::Group *rootNode);
    
    static ParticleEffect *createRearEngineEffect();
};