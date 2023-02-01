#include "meshgenerator.h"

MeshGenerator::MeshGenerator()
{

}

Mesh *MeshGenerator::genere()
{
	Mesh *m = new Mesh();
	QVector3D * a = new QVector3D(rand() / (double)RAND_MAX-0.5,
								rand() / (double)RAND_MAX-0.5,
								rand() / (double)RAND_MAX-0.5);
	QVector3D * b = new QVector3D(rand() / (double)RAND_MAX-0.5,
								rand() / (double)RAND_MAX-0.5,
								rand() / (double)RAND_MAX-0.5);
	QVector3D * c = new QVector3D(rand() / (double)RAND_MAX-0.5,
								rand() / (double)RAND_MAX-0.5,
								rand() / (double)RAND_MAX-0.5);
	QVector3D * d = new QVector3D(rand() / (double)RAND_MAX-0.5,
								rand() / (double)RAND_MAX-0.5,
								rand() / (double)RAND_MAX-0.5);
	Facet *f1 = new Facet(a, b, c);
	Facet *f2 = new Facet(b, c, d);
	Facet *f3 = new Facet(d, a, b);
	Facet *f4 = new Facet(d, a, c);
	Volume v(f1, f2, f3, f4);
	m->addVolume(v);
	for (auto i = 0; i < 50; ++i)
	{
		QVector3D * point = nullptr;
		for (;!point || m->isIncluding(*point); point = new QVector3D(rand() / (double)RAND_MAX-0.5,
																	 rand() / (double)RAND_MAX-0.5,
																	 rand() / (double)RAND_MAX-0.5))
		{
		}
		*point += *m->vertices().last();
		Facet *f = m->nearestFacet(point);
		if (!f)
		{
			i --;
			delete point;
			continue;
		}
		Facet *f1 = new Facet(f->vertices()[0], f->vertices()[1], point);
		Facet *f2 = new Facet(f->vertices()[1], f->vertices()[2], point);
		Facet *f3 = new Facet(f->vertices()[2], f->vertices()[0], point);
		Volume v(f, f1, f2, f3);
		m->addVolume(v);
	}
    return m;
}
