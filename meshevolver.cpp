#include "meshevolver.h"

MeshEvolver::MeshEvolver()
{

}

void MeshEvolver::evolve(Mesh *m)
{
	for (int i = 0; i < 5; ++i)
	{
		auto j = rand()%m->vertices().size();
		QVector3D *p = new QVector3D(*m->vertices()[j]);
		do {
		*p+= QVector3D(rand()/(double)RAND_MAX - 0.5,
					rand()/(double)RAND_MAX - 0.5,
					rand()/(double)RAND_MAX - 0.5);
		}while (m->isIncluding(*p));
		m->setVertice(i, p);
	}
}
