#ifndef MESH_H
#define MESH_H

#include <QString>
#include "volume.h"

class Mesh
{
public:
    Mesh();
    void toFile(QString filename);
	double volume() const;
private:
    QString _nom{"generation"};
    QList <QVector3D> _vertices;
	QList <Facet> _facets;
	QList <Volume> _volumes;
};

#endif // MESH_H
