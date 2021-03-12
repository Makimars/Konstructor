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

  static QVector3D generateNormalVector(QVector3D objectCenter, const Vertex *vertex1, const Vertex *vertex2, const Vertex *vertex3);
  static QVector3D generateNormalVector(QVector3D objectCenter, QVector3D pos1, QVector3D pos2, QVector3D pos3);

private:
  QVector3D m_position;
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

inline QVector3D Vertex::generateNormalVector(QVector3D objectCenter, const Vertex *vertex1, const Vertex *vertex2, const Vertex *vertex3)
{
	return Vertex::generateNormalVector(objectCenter, vertex1->position(), vertex2->position(), vertex3->position());
}

inline QVector3D Vertex::generateNormalVector(QVector3D objectCenter, QVector3D pos1, QVector3D pos2, QVector3D pos3)
{
	QVector3D sideVector = pos2 - pos1;
	sideVector.normalize();

	QVector3D triangleCenter = QVector3D(
			(pos1.x() + pos2.x() + pos3.x()) / 3,
			(pos1.y() + pos2.y() + pos3.y()) / 3,
			(pos1.z() + pos2.z() + pos3.y()) / 3
				);

	QVector3D upwardVector = triangleCenter - objectCenter;
	upwardVector.normalize();

	std::vector<QVector3D> possibleVectors;

	possibleVectors.push_back(QVector3D(sideVector.x(), -sideVector.z(), sideVector.y()));
	possibleVectors.push_back(QVector3D(sideVector.x(), sideVector.z(), -sideVector.y()));

	possibleVectors.push_back(QVector3D(-sideVector.z(), sideVector.y(), sideVector.x()));
	possibleVectors.push_back(QVector3D(sideVector.z(), sideVector.y(), -sideVector.x()));

	possibleVectors.push_back(QVector3D(-sideVector.y(), sideVector.x(), sideVector.z()));
	possibleVectors.push_back(QVector3D(sideVector.y(), -sideVector.x(), sideVector.z()));

	QVector3D targetPoint = triangleCenter + upwardVector;

	std::vector<double> distances;
	for(uint32_t i = 0; i < possibleVectors.size(); i++)
	{
		distances.push_back(targetPoint.distanceToPoint(triangleCenter + possibleVectors.at(i).normalized()));
	}

	return possibleVectors.at(std::distance(std::begin(distances), std::min_element(std::begin(distances), std::end(distances))));
}

#endif // VERTEX_H
