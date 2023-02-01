#ifndef MESH_H
#define MESH_H

#include <QString>
#include "volume.h"

class Mesh
{
public:
    Mesh();
	Mesh(const Mesh &m);
	virtual ~Mesh();
    void toFile(QString filename);
	double volume() const;
	double surface() const;
	double longueurMax() const;
    bool isIncluding(const QVector3D &p) const;
	Facet *nearestFacet(const QVector3D *p) const;
    void addVolume(Volume &v);

	const QString &nom() const;

	const QList<QVector3D *> &vertices() const;

	const QList<Facet *> &facets() const;

	const QList<Volume *> &volumes() const;
	void setVertice(int i, QVector3D *v);
private:
    QString _nom{"generation"};
	QList <QVector3D *> _vertices;
	QList <Facet *> _facets;
	QList <Volume *> _volumes;
};

#endif // MESH_H
