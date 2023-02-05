#include "meshgenerator.h"

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
		Facet *f1 = new Facet(f->vertices()[0], f->vertices()[1], point);
		Facet *f2 = new Facet(f->vertices()[1], f->vertices()[2], point);
		Facet *f3 = new Facet(f->vertices()[2], f->vertices()[0], point);
		bool facesSecantes = false;
		for (auto & fRef : m->facets())
		{
			if (fRef == f)
				continue;
			const auto &p = *(fRef->vertices()[0]);
			const auto &n = fRef->normal();
			auto cotes = 0.;
			bool positif = true;
			cotes = n.dotProduct(*(f1->vertices()[0]) - p, n);
			if (cotes < 0)
			{
				positif = false;
			}
			cotes *= n.dotProduct(*(f1->vertices()[1]) - p, n);
			//Les points d'une nouvelle facette intersectent une ancienne facette
			if (cotes < 0)
			{
				facesSecantes = true;
				break;
			}
			cotes *= n.dotProduct(*(f1->vertices()[2]) - p, n);
			if ((cotes < 0 && positif) || (cotes > 0 && !positif))
			{
				facesSecantes = true;
				break;
			}
			if (facesSecantes)
			{
				QVector3D origin(0, 0, 0);
				auto n1 = f1->normal();
				auto pH = origin.distanceToPlane(p, n);
				auto p1 = origin.distanceToPlane(*(f1->vertices()[0]), n);
				auto lineDir = n.crossProduct(n, f1->normal());
//				n.x() * x + n.y() * y = -pH
//				n1.x() * x + n1.y() * y = -p1
				//				n1.y() * n.x() * x + n1.y() * n.y() * y = -n1.y() * pH
				//				n1.x() * n.y() * x + n1.y() * n.y() * y = -n.y() * p1
				//				(n1.y() * n.x() - n1.x() * n.y()) * x = n.y() * p1 - n1.y() * pH
				//				n1.x() * n.y() * x + n1.y() * n.y() * y = -n.y() * p1
				double x = (n.y() * p1 - n1.y() * pH) / (n1.y() * n.x() - n1.x() * n.y());
				double y = (-pH - n.x() * x)/n.y();
				QVector3D linePoint = (x, 0, 0);

			}
//			cotes = 0.;
//			positif = true;
//			cotes = n.dotProduct(*(f2->vertices()[0]) - p, n);
//			if (cotes < 0)
//			{
//				positif = false;
//			}
//			cotes *= n.dotProduct(*(f2->vertices()[1]) - p, n);
//			//Les points d'une nouvelle facette intersectent une ancienne facette
//			if (cotes < 0)
//			{
//				facesSecantes = true;
//				break;
//			}
//			cotes *= n.dotProduct(*(f2->vertices()[2]) - p, n);
//			if ((cotes < 0 && positif) || (cotes > 0 && !positif))
//			{
//				facesSecantes = true;
//				break;
//			}
//			cotes = 0.;
//			positif = true;
//			cotes = n.dotProduct(*(f3->vertices()[0]) - p, n);
//			if (cotes < 0)
//			{
//				positif = false;
//			}
//			cotes *= n.dotProduct(*(f3->vertices()[1]) - p, n);
//			//Les points d'une nouvelle facette intersectent une ancienne facette
//			if (cotes < 0)
//			{
//				facesSecantes = true;
//				break;
//			}
//			cotes *= n.dotProduct(*(f3->vertices()[2]) - p, n);
//			if ((cotes < 0 && positif) || (cotes > 0 && !positif))
//			{
//				facesSecantes = true;
//				break;
//			}
		}
		if (facesSecantes)
		{
			delete f1;
			delete f2;
			delete f3;
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
		Volume v(f, f1, f2, f3);
		m->addVolume(v);
	}
    return m;
}
