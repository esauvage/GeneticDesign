#include "mesh.h"

#include <QFile>
#include <QTextStream>

Mesh::Mesh()
{
//	_vertices << new QVector3D(0, 0, 0);//0
//	_vertices << new QVector3D(0, 1, 0);//1
//	_vertices << new QVector3D(1, 1, 0);//2
//	_vertices << new QVector3D(1, 0, 0);//3
//	_vertices << new QVector3D(0, 0, 1);//4
//	_facets << new Facet(_vertices[0], _vertices[1], _vertices[2]);
//	_facets << new Facet(_vertices[0], _vertices[2], _vertices[4]);
//	_facets << new Facet(_vertices[1], _vertices[0], _vertices[4]);
//	_facets << new Facet(_vertices[2], _vertices[1], _vertices[4]);
//	_volumes << new Volume(_facets[0], _facets[1], _facets[2], _facets[3]);
}

Mesh::Mesh(const Mesh &m)
{
	for (auto & v: m.vertices())
	{
		_vertices << new QVector3D(v->x(), v->y(), v->z());
	}
	for (auto & f: m.facets())
	{
		_facets << new Facet(_vertices[m.vertices().indexOf(f->vertices()[0])],
				_vertices[m.vertices().indexOf(f->vertices()[1])],
				_vertices[m.vertices().indexOf(f->vertices()[2])]);
	}
	for (auto &v: m.volumes())
	{
		_volumes << new Volume(_facets[m.facets().indexOf(v->facets()[0])],
				_facets[m.facets().indexOf(v->facets()[1])],
				_facets[m.facets().indexOf(v->facets()[2])],
				_facets[m.facets().indexOf(v->facets()[3])]);
	}
}

Mesh::~Mesh()
{
	qDeleteAll(_volumes);
	qDeleteAll(_facets);
	qDeleteAll(_vertices);
}

void Mesh::toFile(QString filename)
{
    QFile fichier(filename);
    if (fichier.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream sortie(&fichier);
        sortie << "solid " << _nom << Qt::endl;
        for (auto facet : _facets)
        {
			sortie << *facet;
        }
        sortie << "endsolid " << _nom << Qt::endl;
    }
	fichier.close();
}

double Mesh::volume() const
{
	double v = 0;
	for (auto &volume : _volumes)
		v += volume->volume();
	return v;
}

double Mesh::surface() const
{
	double s = 0;
	for (auto &f : _facets)
	{
		auto nbVolumes = 0;
//		auto facetteNoyee = false;
		for (auto & v : _volumes)
		{
			if (v->facets().contains(f))
			{
				nbVolumes ++;
				if (nbVolumes > 1)
				{
//					facetteNoyee = true;
					break;
				}
			}
		}
		if (nbVolumes != 1)
			continue;
		s += f->surface();
	}
	return s;
}

double Mesh::longueurMax() const
{
	double max = 0;
	for (auto &v1 : _vertices)
	{
		for (auto &v2 : _vertices)
		{
			if (v2==v1) continue;
			max = qMax(max, (*v2-*v1).lengthSquared());
		}
	}
	return max;
}

bool Mesh::isIncluding(const QVector3D &p) const
{
	for (auto &volume : _volumes)
    {
		if (volume->isIncluding(p))
        {
            return true;
        }
    }
    return false;
}

Facet *Mesh::nearestFacet(const QVector3D *p) const
{
    auto d = -1.;
    auto i = -1;
    auto j = 0;
    auto curVol = _volumes.last();
    for (auto &f : curVol->facets())
    {
        //On exclut les facettes internes du volume
        auto nbVolumes = 0;
        for (auto &v : _volumes)
        {
            if (v->facets().contains(f))
            {
                nbVolumes ++;
                if (nbVolumes > 1)
                {
                    break;
                }
            }
        }
        if (nbVolumes != 1)
        {
            ++j;
            continue;
        }
        //On cherche le point du tétraèdre qui n'est pas sur la facette
        QVector3D lastPoint;
        for (auto x : curVol->vertices())
        {
            if (!f->vertices().contains(x))
            {
                lastPoint = *x;
                break;
            }
        }
        QVector3D n = f->normal().normalized();
        lastPoint -= *f->vertices()[0];
        auto coteLast = n.dotProduct(n, lastPoint.normalized());
        auto coteNouveau = n.dotProduct(n, (*p - *f->vertices()[0]).normalized());
        if ((coteLast * coteNouveau) >= 0)
        {
            continue;
        }
        lastPoint = *p - *f->vertices()[0];
        d = n.dotProduct(n, lastPoint);
        if (f->isContaining(lastPoint - n*d))
            return f;
    }
    return nullptr;
//	for (auto &f : _facets)
//    {
//		auto noDist = false;
//		auto nbVolumes = 0;
//		for (auto &v : _volumes)
//		{
//			if (v->facets().contains(f))
//			{
//				nbVolumes ++;
//				if (nbVolumes > 1)
//				{
//					break;
//				}
//			}
//		}
//        if (nbVolumes != 1)
//		{
//			++j;
//			continue;
//		}
//		if (i < 0)
//        {
//            //On teste si on intersecte chaque future arête avec une facette
//            for (auto &f2 : _facets)
//            {
//				if (f == f2)
//					continue;
//				for (auto &v : f->vertices())
//                {
////                    auto line = p - *v;
//                    if (f2->intersect(p, v))
//                    {
//                        noDist = true;
//                        break;
//                    }
//                }
//                if (noDist)
//                    break;
//            }
//			if (noDist)
//			{
//				++j;
//				continue;
//			}
//			auto n = f->distanceTo(*p);
//			d = n;
//            i = j;
//        }
//        else
//        {
//			auto n = f->distanceTo(*p);
//            if (n < d)
//            {
//                d = n;
//                i = j;
//            }
//        }
//        ++j;
//    }
//	if (i < 0)
//		return nullptr;
//    return _facets[i];
}

void Mesh::addVolume(Volume &v)
{
	for (auto &p : v.vertices())
    {
		if (!_vertices.contains(p))
        {
			_vertices << p;
        }
    }
    QList<Facet *> nFs;
	for (auto & f : v.facets())
    {
		if (!_facets.contains(f))
        {
			_facets << f;
        }
		nFs << f;
    }
	_volumes << new Volume(nFs[0], nFs[1], nFs[2], nFs[3]);
}

const QString &Mesh::nom() const
{
	return _nom;
}

const QList<QVector3D *> &Mesh::vertices() const
{
	return _vertices;
}

const QList<Facet *> &Mesh::facets() const
{
	return _facets;
}

const QList<Volume *> &Mesh::volumes() const
{
	return _volumes;
}

void Mesh::setVertice(int i, QVector3D *v)
{
	_vertices[i]->setX(v->x());
	_vertices[i]->setY(v->y());
	_vertices[i]->setZ(v->z());

//    for (auto fIndex = 0; fIndex < _facets.size(); ++fIndex)
//	{
//        auto &f = _facets[fIndex];
//        if (!f->vertices().contains(_vertices[i]))
//            continue;
//		auto j = f->vertices().indexOf(_vertices[i]);
//		Facet * newFacet = new Facet(f->vertices()[0], f->vertices()[1], f->vertices()[2]);
////		newFacet->setVertice(j, v);
////		for (auto vIndex = 0; vIndex < _volumes.size(); ++vIndex)
////		{
////			auto v = _volumes[vIndex];
////			if (!v->facets().contains(f))
////				continue;
////			j = v->facets().indexOf(f);
////			v->setFacet(j, newFacet);
////		}
//		_facets[fIndex] = newFacet;
//    }
//	_vertices[i] = v;
}
