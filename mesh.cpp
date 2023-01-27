#include "mesh.h"

#include <QFile>
#include <QTextStream>

Mesh::Mesh()
{
	_vertices.append(QVector3D(0, 0, 0));//0
	_vertices.append(QVector3D(0, 1, 0));//1
	_vertices.append(QVector3D(1, 1, 0));//2
	_vertices.append(QVector3D(1, 0, 0));//3
	_vertices.append(QVector3D(0, 0, 1));//4
//	_vertices.append(QVector3D(0, 1, 1));//5
//	_vertices.append(QVector3D(1, 1, 1));//6
//	_vertices.append(QVector3D(1, 0, 1));//7
	_facets.append(Facet(&_vertices[0], &_vertices[1], &_vertices[2]));
	_facets.append(Facet(&_vertices[0], &_vertices[2], &_vertices[4]));
	_facets.append(Facet(&_vertices[1], &_vertices[0], &_vertices[4]));
	_facets.append(Facet(&_vertices[2], &_vertices[1], &_vertices[4]));
//    //Bottom
//	_facets.append(Facet(&_vertices[0], &_vertices[1], &_vertices[2]));
//	_facets.append(Facet(&_vertices[0], &_vertices[2], &_vertices[3]));
//    //Top
//	_facets.append(Facet(&_vertices[7], &_vertices[5], &_vertices[6]));
//	_facets.append(Facet(&_vertices[4], &_vertices[5], &_vertices[7]));

//	_facets.append(Facet(&_vertices[0], &_vertices[5], &_vertices[1]));
//	_facets.append(Facet(&_vertices[0], &_vertices[5], &_vertices[4]));

//	_facets.append(Facet(&_vertices[0], &_vertices[3], &_vertices[7]));
//	_facets.append(Facet(&_vertices[4], &_vertices[7], &_vertices[0]));

//	_facets.append(Facet(&_vertices[1], &_vertices[2], &_vertices[5]));
//	_facets.append(Facet(&_vertices[2], &_vertices[6], &_vertices[5]));

//	_facets.append(Facet(&_vertices[6], &_vertices[2], &_vertices[7]));
//	_facets.append(Facet(&_vertices[7], &_vertices[3], &_vertices[2]));
	_volumes.append(Volume(&_facets[0], &_facets[1], &_facets[2], &_facets[3]));
}

void Mesh::toFile(QString filename)
{
    QFile fichier(filename);
    if (fichier.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream sortie(&fichier);
        sortie << "solid " << _nom << Qt::endl;
        for (auto facet : _facets)
        {
            sortie << facet;
        }
        sortie << "endsolid " << _nom << Qt::endl;
    }
	fichier.close();
}

double Mesh::volume() const
{
	double v = 0;
	for (auto volume : _volumes)
		v += volume.volume();
	return v;
}
