#ifndef CAMERA_H
#define CAMERA_H

#include "Transform3D.h"

class Camera
{
public:
	Camera();

	static const QVector3D LocalForward;
	static const QVector3D LocalUp;
	static const QVector3D LocalRight;

	//transform by
	void translate(const QVector3D dt);
	void translate(float dx, float dy, float dz);
	void rotate(const QQuaternion dr);
	void rotate(float angle, const QVector3D axis);
	void rotate(float angle, float ax, float ay, float az);

	//transform to
	void setTranslation(const QVector3D t);
	void setTranslation(float x, float y, float z);
	void setRotation(const QQuaternion r);
	void setRotation(float angle, const QVector3D axis);
	void setRotation(float angle, float ax, float ay, float az);

	const QVector3D getTranslation();
	const QQuaternion getRotation();
	const QMatrix4x4 toMatrix();

	QVector3D forward() const;
	QVector3D right() const;
	QVector3D up() const;

private:
  bool expired;
  QVector3D translation;
  QQuaternion rotation;
  QMatrix4x4 world;
};


// Constructors
inline Camera::Camera() : expired(true) {}

// Transform By (Add/Scale)
inline void Camera::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void Camera::rotate(float angle, const QVector3D axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Transform To (Setters)
inline void Camera::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void Camera::setRotation(float angle, const QVector3D axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

inline const QVector3D Camera::getTranslation() { return translation; }
inline const QQuaternion Camera::getRotation() { return rotation; }


#endif // CAMERA_H
