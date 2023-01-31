#include "meshevolver.h"

#include <QDebug>

MeshEvolver::MeshEvolver()
{

}

void MeshEvolver::evolve(Mesh *m)
{
//    srand(time(NULL));
    for (int i = 0; i < 1; ++i)
	{
		auto j = rand()%m->vertices().size();
        QVector3D *ref = m->vertices()[j];
        QVector3D *p = new QVector3D(*ref);
//		do {
//		*p+= QVector3D(rand()/(double)RAND_MAX - 0.5,
//					rand()/(double)RAND_MAX - 0.5,
//					rand()/(double)RAND_MAX - 0.5);
//		}while (m->isIncluding(*p));
        m->setVertice(j, p);
        bool error = false;
        if (m->vertices().contains(ref))
            error = true;
        for (auto &f : m->facets())
        {
            if (f->vertices().contains(ref))
                error = true;
        }
        for (auto &v : m->volumes())
        {
            for (auto &f : v->facets())
            {
                if (f->vertices().contains(ref))
                    error = true;
            }
        }
        if (error)
            qDebug() << "MegaErreur";
	}
}
