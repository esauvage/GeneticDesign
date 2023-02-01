#include "meshevolver.h"

#include <QDebug>

MeshEvolver::MeshEvolver()
{

}

void MeshEvolver::evolve(Mesh *m)
{
//    srand(time(NULL));
    for (int i = rand()%m->vertices().size()*5; i < m->vertices().size(); i = rand()%m->vertices().size()*5)
	{
		auto j = rand()%m->vertices().size();
        QVector3D *ref = m->vertices()[j];
        QVector3D *p = new QVector3D(*ref);
		do {
		*p+= QVector3D(rand()/(double)RAND_MAX - 0.5,
					rand()/(double)RAND_MAX - 0.5,
					rand()/(double)RAND_MAX - 0.5);
		}while (m->isIncluding(*p));
		m->setVertice(j, p);
		delete p;
	}
}
