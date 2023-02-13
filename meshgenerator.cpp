#include "meshgenerator.h"
#include <Eigen/Geometry>

MeshGenerator::MeshGenerator()
{

}

Mesh *MeshGenerator::genere()
{
	Mesh *m = new Mesh();
    QVector3D * a = new QVector3D(0, 0, 0);
	QVector3D * b = new QVector3D(rand() * 0.2 / (double)RAND_MAX-0.1,
								rand() * 0.2 / (double)RAND_MAX-0.1,
								rand() * 0.2 / (double)RAND_MAX-0.1);
	QVector3D * c = new QVector3D(rand() * 0.2 / (double)RAND_MAX-0.1,
								rand() * 0.2 / (double)RAND_MAX-0.1,
								rand() * 0.2 / (double)RAND_MAX-0.1);
	QVector3D * d = new QVector3D(rand() * 0.2 / (double)RAND_MAX-0.1,
								rand() * 0.2 / (double)RAND_MAX-0.1,
								rand() * 0.2 / (double)RAND_MAX-0.1);
	Facet *f1 = new Facet(a, b, c);
	Facet *f2 = new Facet(b, c, d);
	Facet *f3 = new Facet(d, a, b);
	Facet *f4 = new Facet(d, a, c);
	Volume v(f1, f2, f3, f4);
	m->addVolume(v);
    auto nbFautes = 0;
	for (auto i = 0; i < 50; ++i)
	{
		QVector3D * point = nullptr;
		for (;!point || m->isIncluding(*point); )
		{
            if (point) delete point;
			point = new QVector3D(rand()*0.2 / (double)RAND_MAX-0.1,
								  rand()*0.2 / (double)RAND_MAX-0.1,
								  rand()*0.2 / (double)RAND_MAX-0.1);
            *point += *m->vertices().last();
        }
		Facet *f = m->nearestFacet(point);
		if (!f)
		{
			i --;
//			m->nearestFacet(point);
			delete point;
			point = nullptr;
			nbFautes++;
			if (nbFautes > 100)
			{
//				m->diviseVolume(m->volumes().last());
//				delete point;
                break;
			}
            continue;
		}
		QList <Facet *>newFacets;
		newFacets << new Facet(f->vertices()[0], f->vertices()[1], point);
		newFacets << new Facet(f->vertices()[1], f->vertices()[2], point);
		newFacets << new Facet(f->vertices()[2], f->vertices()[0], point);
		bool facesSecantes = false;
		for (auto & fRef : m->facets())
		{
			if (fRef == f)
				continue;
//			const auto &p = *(fRef->vertices()[0]);
//			const auto &n = fRef->normal();
			for (auto f1: newFacets)
			{
				if (f1->intersect(fRef))
				{
					facesSecantes = true;
					break;
				}
			}
			if (facesSecantes) break;
		}
		if (facesSecantes)
		{
			qDeleteAll(newFacets);
			delete point;
			point = nullptr;
			i --;
			nbFautes++;
			if (nbFautes > 100)
			{
				break;
			}
			continue;
		}
		nbFautes = 0;
		Volume v(f, newFacets[0], newFacets[1], newFacets[2]);
		m->addVolume(v);
	}
    return m;
}
