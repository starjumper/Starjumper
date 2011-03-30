#pragma once

#include <iostream>

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>

#define PLAYER_MASS 2.0f

class btCollisionShape;
class btRigidBody;
class btCollisionWorld;
class btCollisionDispatcher;
class btPairCachingGhostObject;

/**
 * The KinematicCharacterController - as the name suggests - is a customized version
 * of the btKinematicCharacterController provided by the Bullet Physics Library.
 * Basically it respects the worlds gravity for fall speed calculation and fixes jumping.
 *
 * For reference see http://www.bulletphysics.com/Bullet/BulletFull/classbtKinematicCharacterController.html
 */
class KinematicCharacterController : public btCharacterControllerInterface
{
protected:
	btScalar m_halfHeight;
	
	btPairCachingGhostObject *m_ghostObject;
	btConvexShape *m_convexShape; //is also in m_ghostObject, but it needs to be convex, so we store it here to avoid upcast
	
	btScalar m_fallSpeed;
	btScalar m_jumpSpeed;
	btScalar m_maxJumpHeight;

	btScalar m_turnAngle;
	btScalar m_stepHeight;
	btScalar m_addedMargin;

	///this is the desired walk direction, set by the user
	btVector3 m_walkDirection;
	btVector3 m_normalizedDirection;

	//some internal variables
	btVector3 m_currentPosition;
	btScalar m_currentStepOffset;
	btVector3 m_targetPosition;

	///keep track of the contact manifolds
	btManifoldArray	m_manifoldArray;

	bool m_touchingContact;
	btVector3 m_touchingNormal;

	bool m_useGhostObjectSweepTest;
	bool m_useWalkDirection;
	float m_velocityTimeInterval;
	int m_upAxis;
	
    bool m_onGround;
    bool m_forwardHit;
    void *m_groundObject;
	
	btVector3 computeReflectionDirection(const btVector3 &direction, const btVector3 &normal);
	btVector3 parallelComponent(const btVector3 &direction, const btVector3 &normal);
	btVector3 perpindicularComponent(const btVector3 &direction, const btVector3 &normal);

	bool recoverFromPenetration(btCollisionWorld *collisionWorld);
	void stepUp(btCollisionWorld *collisionWorld);
	void updateTargetPositionBasedOnCollision(const btVector3 &hit_normal, btScalar tangentMag = btScalar(0.0), btScalar normalMag = btScalar(1.0));
	void stepForwardAndStrafe(btCollisionWorld *collisionWorld, const btVector3 &walkMove);
	void stepDown(btCollisionWorld *collisionWorld, btScalar dt);
	
public:
	KinematicCharacterController(btPairCachingGhostObject *ghostObject, btConvexShape *convexShape, btScalar stepHeight, int upAxis = 1);
	~KinematicCharacterController();
	
	virtual void updateAction(btCollisionWorld *collisionWorld, btScalar deltaTime)
	{
		preStep(collisionWorld);
		playerStep(collisionWorld, deltaTime);
	}
	
	void debugDraw(btIDebugDraw *debugDrawer);

	void setUpAxis(int axis)
	{
		if (axis < 0)
			axis = 0;
		if (axis > 2)
			axis = 2;
		m_upAxis = axis;
	}

	virtual void setWalkDirection(const btVector3 &walkDirection);
	virtual void setVelocityForTimeInterval(const btVector3 &velocity, btScalar timeInterval);

	void reset();
	void warp(const btVector3 &origin);

	void preStep(btCollisionWorld *collisionWorld);
	void playerStep(btCollisionWorld *collisionWorld, btScalar dt);

	void setFallSpeed(btScalar fallSpeed);
	void setJumpSpeed(btScalar jumpSpeed);
	void setMaxJumpHeight(btScalar maxJumpHeight);
	
	bool canJump() const;
    bool frontalHit();
	void jump();

	btPairCachingGhostObject* getGhostObject();
	
    void setUseGhostSweepTest(bool useGhostObjectSweepTest);

	bool onGround () const;
    void *getGroundObject();
};
