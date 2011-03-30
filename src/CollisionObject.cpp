#include "CollisionObject.h"

CollisionObject::CollisionObject()
{
    setNodeMask(RECEIVE_SHADOW_MASK);
}