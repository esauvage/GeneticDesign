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
    for (const auto &volume : _volumes)
		v += volume.volume();
    return v;
}

bool Mesh::isIncluding(const QVector3D &p) const
{
    for (const auto &volume : _volumes)
    {
        if (volume.isIncluding(p))
        {
            return true;
        }
    }
    return false;
}
//def isin(p,p1,p2,p3):
//    u = dif(p1,p2)
//    v = dif(p3,p2)
//    w = dif(p,p2)
//    l1 = np.dot(u,w)
//    if l1 < 0:
//        return False
//    l2 = np.dot(v,w)
//    if l2 < 0:
//        return False
//    u = dif(p1,p3)
//    v = dif(p2,p3)
//    w = dif(p,p3)
//    l1 = np.dot(u,w)
//    if l1 < 0:
//        return False
//    l2 = np.dot(v,w)
//    if l2 < 0:
//        return False
//    return True


//p1 = [10,0,0]
//p2 = [0,10,0]
//p3 = [0,0,10]
//q0 = [10,13,15]
//w  = [1,1,1]


u  = dif(p1,p2)
v  = dif(p3,p2)
n  = list(np.cross(u,v))
p0 = prod(add(add(p1,p2),p3),1/3.)
pi = add(q0,prod(w,-np.dot(dif(q0,p0),n)/np.dot(w,n)))

if isin(pi,p1,p2,p3):
    print(pi, " internal point")
else:
    print(pi, " external point")
const Facet &Mesh::nearestFacet(QVector3D p) const
{
    auto d = -1.;
    auto i = -1;
    auto j = 0;
    auto noDist = false;
    for (const auto &f : _facets)
    {
        if (i < 0)
        {
            auto n = f.distanceTo(p);
            for (const auto &f2 : _facets)
            {
                if (f == f2)
                    continue;
                for (const auto &v : f.vertices())
                {
                    auto line = p - v;
                    if (f2.intersect(p, *v))
                    {
                        noDist = true;
                    }
                }
            }
            d = n;
            i = j;
        }
        else
        {
            auto n = f.distanceTo(p);
            if (n < d)
            {
                d = n;
                i = j;
            }
        }
        ++j;
    }
    return _facets[i];
}

void Mesh::addVolume(Volume &v)
{
    for (const auto &p : v.vertices())
    {
        if (!_vertices.contains(*p))
        {
            _vertices << *p;
        }
    }
    QList<Facet *> nFs;
    for (const auto & f : v.facets())
    {
        Facet nF(&_vertices[_vertices.indexOf(*(f->vertices()[0]))], &_vertices[_vertices.indexOf(*(f->vertices()[1]))], &_vertices[_vertices.indexOf(*(f->vertices()[2]))]);
        if (!_facets.contains(nF))
        {
            _facets << nF;
            nFs << &_facets.last();
        }
        else
        {
            nFs << &_facets[_facets.indexOf(nF)];
        }
    }
    _volumes << Volume(nFs[0], nFs[1], nFs[2], nFs[3]);
}
