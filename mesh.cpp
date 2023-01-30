#include "mesh.h"

#include <QFile>
#include <QTextStream>

Mesh::Mesh()
{
	_vertices << new QVector3D(0, 0, 0);//0
	_vertices << new QVector3D(0, 1, 0);//1
	_vertices << new QVector3D(1, 1, 0);//2
	_vertices << new QVector3D(1, 0, 0);//3
	_vertices << new QVector3D(0, 0, 1);//4
//	_vertices.append(QVector3D(0, 1, 1));//5
//	_vertices.append(QVector3D(1, 1, 1));//6
//	_vertices.append(QVector3D(1, 0, 1));//7
	_facets << new Facet(_vertices[0], _vertices[1], _vertices[2]);
	_facets << new Facet(_vertices[0], _vertices[2], _vertices[4]);
	_facets << new Facet(_vertices[1], _vertices[0], _vertices[4]);
	_facets << new Facet(_vertices[2], _vertices[1], _vertices[4]);
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
	_volumes << new Volume(_facets[0], _facets[1], _facets[2], _facets[3]);
}

Mesh::Mesh(const Mesh &m)
	:_nom(m.nom()),
	  _vertices(m.vertices()),
	  _facets(m.facets()),
	  _volumes(m.volumes())
{

}

Mesh::~Mesh()
{
//	qDeleteAll(_volumes);
//	qDeleteAll(_facets);
//	qDeleteAll(_vertices);
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
		auto facetteNoyee = false;
		for (auto & v : _volumes)
		{
			if (v->facets().contains(f))
			{
				nbVolumes ++;
				if (nbVolumes > 1)
				{
					facetteNoyee = true;
					break;
				}
			}
		}
		if (facetteNoyee)
			continue;
		s += f->surface();
	}
	return s;
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
	for (auto &f : _facets)
    {
		auto noDist = false;
		auto nbVolumes = 0;
		for (auto &v : _volumes)
		{
			if (v->facets().contains(f))
			{
				nbVolumes ++;
				if (nbVolumes > 1)
				{
					noDist = true;
					break;
				}
			}
		}
		if (noDist)
		{
			++j;
			continue;
		}
		if (i < 0)
        {
			//On teste si on intersecte chaque future arête avec une facette
			for (auto &f2 : _facets)
            {
				if (f == f2)
					continue;
				for (auto &v : f->vertices())
                {
//                    auto line = p - *v;
					if (f2->intersect(*p, *v))
                    {
                        noDist = true;
                    }
                }
            }
			if (noDist)
			{
				++j;
				continue;
			}
			auto n = f->distanceTo(*p);
			d = n;
            i = j;
        }
        else
        {
			auto n = f->distanceTo(*p);
            if (n < d)
            {
                d = n;
                i = j;
            }
        }
        ++j;
    }
	if (i < 0)
		return nullptr;
    return _facets[i];
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
//		Facet *nF = new Facet(&_vertices[_vertices.indexOf(*(f->vertices()[0]))], &_vertices[_vertices.indexOf(*(f->vertices()[1]))], &_vertices[_vertices.indexOf(*(f->vertices()[2]))]);
		if (!_facets.contains(f))
        {
			_facets << f;
//			nFs << _facets.last();
        }
//		else
//		{
			nFs << f;//_facets[_facets.indexOf(f)];
//			delete nF;
//		}
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
