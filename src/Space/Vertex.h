#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QByteArray>

class Vertex
{
public:
  // Constructors
  Q_DECL_CONSTEXPR Vertex();
  Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position);
  Q_DECL_CONSTEXPR explicit Vertex(double x, double y, double z);

  // Accessors / Mutators
  Q_DECL_CONSTEXPR const QVector3D& position() const;
  void setPosition(const QVector3D& position);
  void setZ(const double z);
  QByteArray toByteArray();

  // OpenGL Helpers
  static const int PositionTupleSize = 3;
  static Q_DECL_CONSTEXPR int positionOffset();
  static Q_DECL_CONSTEXPR int stride();

private:
  QVector3D m_position;
  QVector3D m_color;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline Vertex::Vertex() {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position) : m_position(position) {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(double x, double y, double z) : m_position(QVector3D(x,y,z)) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const { return m_position; }
void inline Vertex::setPosition(const QVector3D& position) { m_position = position; }
void inline Vertex::setZ(const double z) { m_position.setZ(z); }

QByteArray inline Vertex::toByteArray()
{
	QByteArray result;

	float point[3] = {
		m_position.x(),
		m_position.y(),
		m_position.z()
	};

	for(int a = 0; a < 3; a++)
	{
		char *pointer = (char*)&point[a];
		for(int i = 0; i < 4; i++)
			result += pointer[i];
	}

	return result;
}

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }
Q_DECL_CONSTEXPR inline int Vertex::stride() { return sizeof(Vertex); }

#endif // VERTEX_H
