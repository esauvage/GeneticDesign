#include "meshgenerator.h"

MeshGenerator::MeshGenerator()
{

}

Mesh *MeshGenerator::genere()
{
	Mesh *m = new Mesh();
	for (auto i = 0; i < 300; ++i)
	{
		QVector3D * point = nullptr;
		for (;!point || m->isIncluding(*point); point = new QVector3D(rand() * 5. / (double)RAND_MAX-2.5,
																	 rand() * 5. / (double)RAND_MAX-2.5,
																	 rand() * 5. / (double)RAND_MAX-2.5))
		{
		}
		Facet *f = m->nearestFacet(point);
		if (!f)
			continue;
		Facet *f1 = new Facet(f->vertices()[0], f->vertices()[1], point);
		Facet *f2 = new Facet(f->vertices()[1], f->vertices()[2], point);
		Facet *f3 = new Facet(f->vertices()[2], f->vertices()[0], point);
		Volume v(f, f1, f2, f3);
		m->addVolume(v);
	}
    return m;
}
