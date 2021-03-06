#include <LinearMath/btIDebugDraw.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btDefaultMotionState.h>

#include "KinematicCharacterController.h"

static btVector3 upAxisDirection[3] = { btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 1.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f) };


// static helper method
static btVector3
getNormalizedVector(const btVector3& v)
{
	btVector3 n = v.normalized();
	if (n.length() < SIMD_EPSILON) {
		n.setValue(0, 0, 0);
	}
	return n;
}

class KinematicClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	KinematicClosestNotMeConvexResultCallback (btCollisionObject* me) : btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
	{
		m_me = me;
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == m_me)
			return 1.0;

		return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
	}
protected:
	btCollisionObject* m_me;
};

/*
 * Returns the reflection direction of a ray going 'direction' hitting a surface with normal 'normal'
 *
 * from: http://www-cs-students.stanford.edu/~adityagp/final/node3.html
 */
btVector3 KinematicCharacterController::computeReflectionDirection (const btVector3& direction, const btVector3& normal)
{
	return direction - (btScalar(2.0) * direction.dot(normal)) * normal;
}

/*
 * Returns the portion of 'direction' that is parallel to 'normal'
 */
btVector3 KinematicCharacterController::parallelComponent (const btVector3& direction, const btVector3& normal)
{
	btScalar magnitude = direction.dot(normal);
	return normal * magnitude;
}

/*
 * Returns the portion of 'direction' that is perpindicular to 'normal'
 */
btVector3 KinematicCharacterController::perpindicularComponent (const btVector3& direction, const btVector3& normal)
{
	return direction - parallelComponent(direction, normal);
}

KinematicCharacterController::KinematicCharacterController(btPairCachingGhostObject* ghostObject,btConvexShape* convexShape,btScalar stepHeight, int upAxis)
{
	m_upAxis = upAxis;
	m_addedMargin = 0.02f;
	m_walkDirection.setValue(0,0,0);
	m_useGhostObjectSweepTest = true;
	m_ghostObject = ghostObject;
	m_stepHeight = stepHeight;
	m_turnAngle = btScalar(0.0);
	m_convexShape=convexShape;	
	m_useWalkDirection = true;	// use walk direction by default, legacy behavior
	m_velocityTimeInterval = 0.0;
    m_forwardHit = false;

    reset();
}

KinematicCharacterController::~KinematicCharacterController ()
{

}

btPairCachingGhostObject* KinematicCharacterController::getGhostObject()
{
	return m_ghostObject;
}

bool KinematicCharacterController::recoverFromPenetration ( btCollisionWorld* collisionWorld)
{

	bool penetration = false;

	collisionWorld->getDispatcher()->dispatchAllCollisionPairs(m_ghostObject->getOverlappingPairCache(), collisionWorld->getDispatchInfo(), collisionWorld->getDispatcher());

	m_currentPosition = m_ghostObject->getWorldTransform().getOrigin();
	
	btScalar maxPen = btScalar(0.0);
	for (int i = 0; i < m_ghostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
	{
		m_manifoldArray.resize(0);

		btBroadphasePair* collisionPair = &m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];
		
		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);

		
		for (int j=0;j<m_manifoldArray.size();j++)
		{
			btPersistentManifold* manifold = m_manifoldArray[j];
			btScalar directionSign = manifold->getBody0() == m_ghostObject ? btScalar(-1.0) : btScalar(1.0);
			for (int p=0;p<manifold->getNumContacts();p++)
			{
				const btManifoldPoint&pt = manifold->getContactPoint(p);

				if (pt.getDistance() < 0.0)
				{
					if (pt.getDistance() < maxPen)
					{
						maxPen = pt.getDistance();
						m_touchingNormal = pt.m_normalWorldOnB * directionSign;//??

					}
					m_currentPosition += pt.m_normalWorldOnB * directionSign * pt.getDistance() * btScalar(0.2);
					penetration = true;
				} else {
					//printf("touching %f\n", pt.getDistance());
				}
			}
			
			//manifold->clearManifold();
		}
	}
	btTransform newTrans = m_ghostObject->getWorldTransform();
	newTrans.setOrigin(m_currentPosition);
	m_ghostObject->setWorldTransform(newTrans);
//	printf("m_touchingNormal = %f,%f,%f\n",m_touchingNormal[0],m_touchingNormal[1],m_touchingNormal[2]);
	return penetration;
}

void KinematicCharacterController::stepUp ( btCollisionWorld* world)
{
	// phase 1: up
	btTransform start, end;
	m_targetPosition = m_currentPosition + upAxisDirection[m_upAxis] * m_stepHeight;

	start.setIdentity ();
	end.setIdentity ();

	/* FIXME: Handle penetration properly */
	start.setOrigin (m_currentPosition + upAxisDirection[m_upAxis] * btScalar(0.1f));
	end.setOrigin (m_targetPosition);

	KinematicClosestNotMeConvexResultCallback callback (m_ghostObject);
	callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;
	
	if (m_useGhostObjectSweepTest)
	{
		m_ghostObject->convexSweepTest (m_convexShape, start, end, callback, world->getDispatchInfo().m_allowedCcdPenetration);
	}
	else
	{
		world->convexSweepTest (m_convexShape, start, end, callback);
	}
	
	if (callback.hasHit())
	{
		// we moved up only a fraction of the step height
		m_currentStepOffset = m_stepHeight * callback.m_closestHitFraction;
		m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition, callback.m_closestHitFraction);
	} else {
		m_currentStepOffset = m_stepHeight;
		m_currentPosition = m_targetPosition;
	}
}

void KinematicCharacterController::updateTargetPositionBasedOnCollision (const btVector3& hitNormal, btScalar tangentMag, btScalar normalMag)
{
	btVector3 movementDirection = m_targetPosition - m_currentPosition;
	btScalar movementLength = movementDirection.length();
	if (movementLength>SIMD_EPSILON)
	{
		movementDirection.normalize();

		btVector3 reflectDir = computeReflectionDirection (movementDirection, hitNormal);
		reflectDir.normalize();

		btVector3 parallelDir, perpindicularDir;

		parallelDir = parallelComponent (reflectDir, hitNormal);
		perpindicularDir = perpindicularComponent (reflectDir, hitNormal);

		m_targetPosition = m_currentPosition;
		if (0)//tangentMag != 0.0)
		{
			btVector3 parComponent = parallelDir * btScalar (tangentMag*movementLength);
//			printf("parComponent=%f,%f,%f\n",parComponent[0],parComponent[1],parComponent[2]);
			m_targetPosition +=  parComponent;
		}

		if (normalMag != 0.0)
		{
			btVector3 perpComponent = perpindicularDir * btScalar (normalMag*movementLength);
//			printf("perpComponent=%f,%f,%f\n",perpComponent[0],perpComponent[1],perpComponent[2]);
			m_targetPosition += perpComponent;
		}
	} else
	{
//		printf("movementLength don't normalize a zero vector\n");
	}
}

void KinematicCharacterController::stepForwardAndStrafe ( btCollisionWorld* collisionWorld, const btVector3& walkMove)
{
	btTransform start, end;
	m_targetPosition = m_currentPosition + walkMove;
	start.setIdentity ();
	end.setIdentity ();
	
	btScalar fraction = 1.0;
	btScalar distance2 = (m_currentPosition-m_targetPosition).length2();
//	printf("distance2=%f\n",distance2);

	if (m_touchingContact)
	{
		if (m_normalizedDirection.dot(m_touchingNormal) > btScalar(0.0))
			updateTargetPositionBasedOnCollision (m_touchingNormal);
	}

	int maxIter = 10;

	while (fraction > btScalar(0.01) && maxIter-- > 0)
	{
		start.setOrigin (m_currentPosition);
		end.setOrigin (m_targetPosition);

		KinematicClosestNotMeConvexResultCallback callback (m_ghostObject);
		callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
		callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;


		btScalar margin = m_convexShape->getMargin();
		m_convexShape->setMargin(margin + m_addedMargin);


		if (m_useGhostObjectSweepTest)
		{
			m_ghostObject->convexSweepTest (m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		} else
		{
			collisionWorld->convexSweepTest (m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		}
		
		m_convexShape->setMargin(margin);

		
		fraction -= callback.m_closestHitFraction;

		if (callback.hasHit())
		{
            m_forwardHit = true;
			// we moved only a fraction
			btScalar hitDistance = (callback.m_hitPointWorld - m_currentPosition).length();
			if (hitDistance<0.f)
			{
//				printf("neg dist?\n");
			}

			/* If the distance is farther than the collision margin, move */
			if (hitDistance > m_addedMargin)
			{
//				printf("callback.m_closestHitFraction=%f\n",callback.m_closestHitFraction);
				m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition, callback.m_closestHitFraction);
			}

			updateTargetPositionBasedOnCollision (callback.m_hitNormalWorld);
			btVector3 currentDir = m_targetPosition - m_currentPosition;
			distance2 = currentDir.length2();
			if (distance2 > SIMD_EPSILON)
			{
				currentDir.normalize();
				/* See Quake2: "If velocity is against original velocity, stop ead to avoid tiny oscilations in sloping corners." */
				if (currentDir.dot(m_normalizedDirection) <= btScalar(0.0))
				{
					break;
				}
			} else
			{
//				printf("currentDir: don't normalize a zero vector\n");
				break;
			}
		} else {
			// we moved whole way
			m_currentPosition = m_targetPosition;
		}

	//	if (callback.m_closestHitFraction == 0.f)
	//		break;

	}
}

void KinematicCharacterController::stepDown ( btCollisionWorld* collisionWorld, btScalar dt)
{
	btTransform start, end;
	
    btScalar gravity;
    btDynamicsWorld *dynamicsWorld = dynamic_cast<btDynamicsWorld *>(collisionWorld);
    if(dynamicsWorld)
        gravity = dynamicsWorld->getGravity().getZ();
    else
        gravity = -10.0f;

	m_targetPosition -= upAxisDirection[m_upAxis] * (m_currentStepOffset - m_fallSpeed * dt);

	start.setIdentity ();
	end.setIdentity ();

	start.setOrigin (m_currentPosition);
	end.setOrigin (m_targetPosition);

	KinematicClosestNotMeConvexResultCallback callback (m_ghostObject);
	callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;
	
	if (m_useGhostObjectSweepTest)
	{
		m_ghostObject->convexSweepTest (m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
	}
	else
	{
		collisionWorld->convexSweepTest (m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
	}

	if (callback.hasHit())
	{
        m_groundObject = callback.m_hitCollisionObject->getUserPointer();
                     
		// we dropped a fraction of the height -> hit floor
		m_currentPosition.setInterpolate3(m_currentPosition, m_targetPosition, callback.m_closestHitFraction);
        m_onGround = true;
        m_fallSpeed = 0.0f;        
	}
	else
	{
		// we dropped the full height
		m_groundObject = NULL;
		m_currentPosition = m_targetPosition;
        m_onGround = false;
        m_fallSpeed += gravity * dt;
	}
}



void KinematicCharacterController::setWalkDirection
(
const btVector3& walkDirection
)
{
	m_useWalkDirection = true;
	m_walkDirection = walkDirection;
	m_normalizedDirection = getNormalizedVector(m_walkDirection);
}



void KinematicCharacterController::setVelocityForTimeInterval
(
const btVector3& velocity,
btScalar timeInterval
)
{
	m_useWalkDirection = false;
	m_walkDirection = velocity;
	m_normalizedDirection = getNormalizedVector(m_walkDirection);
	m_velocityTimeInterval = timeInterval;
}



void KinematicCharacterController::reset ()
{
    m_jumpSpeed = 20.0f;
    m_fallSpeed = 0.0f;
    m_onGround = false;
    m_groundObject = NULL;
}

void KinematicCharacterController::warp (const btVector3& origin)
{
	btTransform xform;
	xform.setIdentity();
	xform.setOrigin (origin);
	m_ghostObject->setWorldTransform (xform);
}


void KinematicCharacterController::preStep (  btCollisionWorld* collisionWorld)
{
	
	int numPenetrationLoops = 0;
	m_touchingContact = false;
	while (recoverFromPenetration (collisionWorld))
	{
		numPenetrationLoops++;
		m_touchingContact = true;
		if (numPenetrationLoops > 4)
		{
//			printf("character could not recover from penetration = %d\n", numPenetrationLoops);
			break;
		}
	}

	m_currentPosition = m_ghostObject->getWorldTransform().getOrigin();
	m_targetPosition = m_currentPosition;
//	printf("m_targetPosition=%f,%f,%f\n",m_targetPosition[0],m_targetPosition[1],m_targetPosition[2]);

	
}

void KinematicCharacterController::playerStep (  btCollisionWorld* collisionWorld, btScalar dt)
{
//	printf("playerStep(): ");
//	printf("  dt = %f", dt);

	// quick check...
	if (!m_useWalkDirection && m_velocityTimeInterval <= 0.0) {
//		printf("\n");
		return;		// no motion
	}

	btTransform xform;
	xform = m_ghostObject->getWorldTransform ();

//	printf("walkDirection(%f,%f,%f)\n",walkDirection[0],walkDirection[1],walkDirection[2]);
//	printf("walkSpeed=%f\n",walkSpeed);

	stepUp (collisionWorld);
	if (m_useWalkDirection) {
		stepForwardAndStrafe (collisionWorld, m_walkDirection);
	} else {
		//printf("  time: %f", m_velocityTimeInterval);
		// still have some time left for moving!
		btScalar dtMoving =
		   (dt < m_velocityTimeInterval) ? dt : m_velocityTimeInterval;
		m_velocityTimeInterval -= dt;

		// how far will we move while we are moving?
		btVector3 move = m_walkDirection * dtMoving;

		// printf("  dtMoving: %f", dtMoving);

		// okay, step
		stepForwardAndStrafe(collisionWorld, move);
	}
	stepDown (collisionWorld, dt);

	// printf("\n");

	xform.setOrigin (m_currentPosition);
	m_ghostObject->setWorldTransform (xform);
}

void KinematicCharacterController::setFallSpeed (btScalar fallSpeed)
{
	m_fallSpeed = fallSpeed;
}

void KinematicCharacterController::setJumpSpeed (btScalar jumpSpeed)
{
	m_jumpSpeed = jumpSpeed;
}

void KinematicCharacterController::setMaxJumpHeight (btScalar maxJumpHeight)
{
	m_maxJumpHeight = maxJumpHeight;
}

bool KinematicCharacterController::canJump () const
{
	return onGround();
}

void KinematicCharacterController::jump ()
{
	if (!canJump())
		return;

    m_fallSpeed = m_jumpSpeed;
}

bool KinematicCharacterController::onGround() const
{
	return m_onGround;
}

bool KinematicCharacterController::frontalHit()
{
    bool result = m_forwardHit;
    m_forwardHit = false;
    return result;
}

void *KinematicCharacterController::getGroundObject()
{
    return m_groundObject;
}

void KinematicCharacterController::debugDraw(btIDebugDraw* debugDrawer)
{
}

void KinematicCharacterController::setUseGhostSweepTest(bool useGhostObjectSweepTest)
{
	m_useGhostObjectSweepTest = useGhostObjectSweepTest;
}