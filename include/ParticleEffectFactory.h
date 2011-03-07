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
    osgParticle::RandomRateCounter *_counter;
    osgParticle::Placer *_placer;
    osgParticle::Shooter *_shooter;
    osgParticle::ModularProgram *_program;
    
    void configureTemplateParticle(osgParticle::Particle *particleTemplate = NULL);
    void configurePlacer(osgParticle::Placer *placer = NULL);
    void configureShooter(osgParticle::Shooter *shooter = NULL);
    
public:
    ParticleEffect(osgParticle::Particle *particleTemplate = NULL, osgParticle::Placer *placer = NULL, osgParticle::Shooter *shooter = NULL);
    
    osg::Group *getEffectRoot();
    
    void setRate(const double rate);
    void setColor(const osg::Vec4 &color);
    void setSize(const float size);
    
    void enable();
    void disable();
    void clearParticles();
};

class ParticleEffectFactory
{
private:
    osg::Geode *_geode;
    
public:
    ParticleEffectFactory(osg::Group *rootNode);
    
    static ParticleEffect *createRearEngineEffect();
    static ParticleEffect *createSteerEngineEffect();
};