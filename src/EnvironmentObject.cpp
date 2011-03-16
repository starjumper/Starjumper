#include "EnvironmentObject.h"

EnvironmentObject::EnvironmentObject()
{
}

void EnvironmentObject::setRigidBody(btRigidBody *rigidBody)
{
	_rigidBody = rigidBody;
}

btRigidBody *EnvironmentObject::getRigidBody() const
{
    return _rigidBody;
}
