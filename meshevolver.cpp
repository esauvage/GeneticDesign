#include "meshevolver.h"

#include <QDebug>

MeshEvolver::MeshEvolver()
{

}

void MeshEvolver::evolve(Mesh *m)
{
//    srand(time(NULL));
	for (int i = 0; i < m->vertices().size()*0.2; i += 1)
	{
		auto j = rand()%m->vertices().size();
        QVector3D *ref = m->vertices()[j];
        QVector3D *p = new QVector3D(*ref);
		auto fMemeCote = true;
		do {
		*p+= QVector3D(rand()*0.02/(double)RAND_MAX - 0.01,
					rand()*0.02/(double)RAND_MAX - 0.01,
					rand()*0.02/(double)RAND_MAX - 0.01);
		for (auto & f : m->facets())
		{
			if (p->dotProduct(*p - *(f->vertices()[0]), f->normal()) * ref->dotProduct(*ref - *(f->vertices()[0]), f->normal()) < 0)
			{
				fMemeCote = false;
				break;
			}
		}
		}while (m->isIncluding(*p) && !fMemeCote);
		m->setVertice(j, p);
		delete p;
	}
}
