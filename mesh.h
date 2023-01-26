#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QList>
#include "facet.h"

class Mesh
{
public:
    Mesh();
    void toFile(QString filename);
private:
    QString _nom{"generation"};
    QList <Facet> _facets;
    QList <QVector3D> _vertices;
};

#endif // MESH_H
