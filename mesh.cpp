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
	for (auto &f : facettesExternes())
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
	Facet *r = nullptr;
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
		auto coteLast = n.dotProduct(n, lastPoint);
		auto coteNouveau = n.dotProduct(n, (*p - *f->vertices()[0]));
        if ((coteLast * coteNouveau) >= 0)
        {
            continue;
        }
        lastPoint = *p - *f->vertices()[0];
		auto dist = n.dotProduct(n, lastPoint);
		if (f->isContaining(lastPoint - n*dist) || f->isContaining(lastPoint + n*dist))
		{
			if ((d < 0) || (dist < d))
			{
				d = dist;
				r = f;
			}
		}
    }
	return r;
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

void Mesh::diviseVolume(Volume *v)
{
	QList<Facet *>facets = v->facets();
	QList<QVector3D *> points;
	for (auto & f : facets)
	{
		for (auto & v : f->vertices())
		{
			if (!points.contains(v))
			{
				points << v;
			}
		}
	}
	QList <QVector3D *> newPoints;
	QList <Facet *> newFacets;
	QList <Facet *> volumeInterne;
	for (auto & p : points)
	{
		for (auto & p2 : points)
		{
			if (p == p2)
				continue;
			newPoints << new QVector3D((*p + *p2)/2);
		}
		newFacets << new Facet(p, newPoints[0], newPoints[1]);
		newFacets << new Facet(p, newPoints[1], newPoints[2]);
		newFacets << new Facet(p, newPoints[2], newPoints[0]);
		newFacets << new Facet(newPoints[0], newPoints[1], newPoints[2]);
		volumeInterne << newFacets.last();
		_volumes << new Volume(newFacets[0], newFacets[1], newFacets[2], newFacets[3]);
//		_points
		_facets << newFacets;
	}
	_volumes << new Volume(volumeInterne[0], volumeInterne[1], volumeInterne[2], volumeInterne[3]);
}

QList<Facet *> Mesh::facettesExternes() const
{
	QList <Facet *>r;
	for (auto &v : _volumes)
	{
		for (Facet *f : v->facets())
		{
			if (r.contains(f))
			{
				r.remove(r.indexOf(f));
			}
			else
			{
				r << f;
			}
		}
	}
	return r;
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
