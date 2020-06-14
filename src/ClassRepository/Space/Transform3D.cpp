#include "Transform3D.h"

// Transform By (Add/Scale)
void Transform3D::translate(const QVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}

void Transform3D::scale(const QVector3D &ds)
{
  m_dirty = true;
  m_scale *= ds;
}

void Transform3D::rotate(const QQuaternion &dr)
{
  m_dirty = true;
  m_rotation = dr * m_rotation;
}

void Transform3D::grow(const QVector3D &ds)
{
  m_dirty = true;
  m_scale += ds;
}

// Transform To (Setters)
void Transform3D::setTranslation(const QVector3D &t)
{
  m_dirty = true;
  m_translation = t;
}

void Transform3D::setScale(const QVector3D &s)
{
  m_dirty = true;
  m_scale = s;
}

void Transform3D::setRotation(const QQuaternion &r)
{
  m_dirty = true;
  m_rotation = r;
}

// Accessors
const QMatrix4x4 &Transform3D::toMatrix()
{
  if (m_dirty)
  {
	m_dirty = false;
	m_world.setToIdentity();
	m_world.translate(m_translation);
	m_world.rotate(m_rotation);
	m_world.scale(m_scale);
  }
  return m_world;
}
