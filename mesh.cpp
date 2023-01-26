#include "mesh.h"

#include <QFile>
#include <QTextStream>

Mesh::Mesh()
{

}

void Mesh::toFile(QString filename)
{
    QFile fichier(filename);
    if (fichier.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream sortie(&fichier);
        sortie << "solid " << _nom << Qt::endl;

        //    facet normal ni nj nk
        //        outer loop
        //            vertex v1x v1y v1z
        //            vertex v2x v2y v2z
        //            vertex v3x v3y v3z
        //        endloop
        //    endfacet

        sortie << "endsolid " << _nom << Qt::endl;
    }
    fichier.close();
}
