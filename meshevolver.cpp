#include "meshevolver.h"
#include <Eigen/Geometry>

#include <QDebug>

MeshEvolver::MeshEvolver()
{

}

void MeshEvolver::evolve(Mesh *m)
{
//    srand(time(NULL));
	for (int i = 0; i < m->vertices().size()*0.2; i += 1)
	{
		int j;
		QVector3D *ref;
		QVector3D *p = nullptr;
		auto fMemeCote = true;
		auto fFacetteCroisees = false;
		do {
			j = rand()%m->vertices().size();
			ref = m->vertices()[j];
			fMemeCote = true;
			fFacetteCroisees = false;
            do{
                if (p)
                    delete p;
                p = new QVector3D(*ref);
                *p+= QVector3D(rand()*0.02/(double)RAND_MAX - 0.01,
                            rand()*0.02/(double)RAND_MAX - 0.01,
                            rand()*0.02/(double)RAND_MAX - 0.01);
            }while (m->isIncluding(*p));
			for (auto & f : m->facets())
			{
				if (!f->vertices().contains(ref))
				{
					auto pDotN = p->dotProduct(*p - *(f->vertices()[0]), f->normal());
					auto refDotN = ref->dotProduct(*ref - *(f->vertices()[0]), f->normal());
					if (pDotN * refDotN < 0)
					{
						fMemeCote = false;
						break;
					}
				}
			}
			if (!fMemeCote)
				continue;
//			QList <Facet *>newFacets;
			for (auto f : m->facets())
			{
				if (f->vertices().contains(ref))
				{
					auto f1 = new Facet(f->vertices()[0], f->vertices()[1], f->vertices()[2]);
					f1->setVertice(f1->vertices().indexOf(ref), p);
					bool facesSecantes = false;
					for (auto & fRef : m->facets())
					{
						if (fRef == f)
							continue;
						if (f1->intersect(fRef))
						{
							facesSecantes = true;
							break;
						}
					}
					delete f1;
					fFacetteCroisees |= facesSecantes;
					if (fFacetteCroisees)
						break;
				}
			}
        }while (!fMemeCote || fFacetteCroisees);
		m->setVertice(j, p);
		delete p;
	}
}
