#include "Camera.h"

const QVector3D Camera::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera::LocalRight(1.0f, 0.0f, 0.0f);

void Camera::translate(const QVector3D dt)
{
	expired = true;
	translation += dt;
}

void Camera::rotate(const QQuaternion dr)
{
	expired = true;
	rotation = dr * rotation;
}

void Camera::setTranslation(const QVector3D t)
{
	expired = true;
	translation = t;
}

void Camera::setRotation(const QQuaternion r)
{
	expired = true;
	rotation = r;
}

const QMatrix4x4 Camera::toMatrix()
{
	if (expired)
	{
	  expired = false;
	  world.setToIdentity();
	  world.rotate(rotation.conjugate());
	  world.translate(-translation);
	}
	return world;
}

QVector3D Camera::forward() const
{
	return rotation.rotatedVector(LocalForward);
}

QVector3D Camera::right() const
{
	return rotation.rotatedVector(LocalRight);
}

QVector3D Camera::up() const
{
	return rotation.rotatedVector(LocalUp);
}
