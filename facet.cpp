#include "facet.h"

#include <QTextStream>

Facet::Facet(QVector3D *a, QVector3D *b, QVector3D *c)
{
    _vertices << a << b << c;
}

QVector3D Facet::normal() const
{
    return QVector3D(0., 0., -1.);
}

const QList<QVector3D *> &Facet::vertices() const
{
    return _vertices;
}

QTextStream &operator<<(QTextStream &out, const Facet &facet)
{
    out << "\tfacet normal " << facet.normal() << Qt::endl;
    out << "\t\touter loop" << Qt::endl;
    for (const auto &x : facet.vertices())
    {
        out << "\t\t\tvertex " << (*x) <<Qt::endl;
    }
    out << "\t\tendloop" << Qt::endl;
    out << "\tendfacet" << Qt::endl;
    return out;
}

QTextStream &operator<<(QTextStream &out, const QVector3D &vertex)
{
    out << vertex.x() << " " << vertex.y() << " " << vertex.z();
    return out;
}
