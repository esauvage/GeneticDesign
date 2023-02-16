#include "meshevolver.h"
#include <Eigen/Geometry>

#include <QDebug>

MeshEvolver::MeshEvolver()
{

}

void MeshEvolver::evolve(Mesh *m)
{
	for (int i = 0; i < m->vertices().size()*0.2; i += 1)
	{
		if (!(rand()%4))
		{
			addFacet(m);
			continue;
		}
		int j;
		QVector3D *ref;
		QVector3D *p = nullptr;
		auto fMemeCote = true;
		auto fFacetteCroisees = false;
		int k = 0;
		do {
			k++;
			if (k>100)
				break;
			fMemeCote = true;
			fFacetteCroisees = false;
            do{
                if (p)
                    delete p;
				j = rand()%m->vertices().size();
				ref = m->vertices()[j];
				p = new QVector3D(*ref);
				*p+= QVector3D(rand()*0.04/(double)RAND_MAX - 0.02,
							rand()*0.04/(double)RAND_MAX - 0.02,
							rand()*0.04/(double)RAND_MAX - 0.02);
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
						if (fRef->vertices().contains(ref))
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
		}while (k < 100 || !fMemeCote || fFacetteCroisees);
		m->setVertice(j, p);
		delete p;
	}
}

void MeshEvolver::addFacet(Mesh *m)
{
	QList <QPair <Facet *, Facet * > > bifaces;
	for (const auto & v : m->vertices())
	{
		for (const auto & f: m->facets())
		{
			if (f->vertices().contains(v))
			{
				for (const auto & v1: f->vertices())
				{
					if (v1 == v)
					{
						continue;
					}
					for (const auto & f1: m->facets())
					{
						if (f==f1)
						{
							continue;
						}
						if (f1->vertices().contains(v)&&f1->vertices().contains(v1))
						{
						// on a trouvé une 2ème facette
							QList<int> fVertice {0, 1, 2};
							QList<int> f1Vertice {0, 1, 2};
							fVertice.removeAll(f->vertices().indexOf(v));
							fVertice.removeAll(f->vertices().indexOf(v1));
							f1Vertice.removeAll(f1->vertices().indexOf(v));
							f1Vertice.removeAll(f1->vertices().indexOf(v1));
							auto v3 = f->vertices().at(fVertice.at(0));
							auto v4 = f1->vertices().at(f1Vertice.at(0));
                            bool isInVolume = false;
                            for (const auto & f2: m->facets())
							{
								if (f==f2|| f1==f2)
								{
									continue;
								}
								if (f2->vertices().contains(v)&&f2->vertices().contains(v3)&&f2->vertices().contains(v4))
								{
                                    isInVolume = false;
									//On a trouvé 3 facettes reliées
									//Espérons qu'on ne trouve pas de volume qui les contienne
									for (const auto &vol : m->volumes())
									{
										if (vol->facets().contains(f) && vol->facets().contains(f1) && vol->facets().contains(f2))
										{
											isInVolume = true;
											break;
										}
									}
									if (!isInVolume)
									{
//										qDebug() << "Manque une face pour faire un volume !";
									}
								}
							}
                            if (isInVolume)
                                break;
							for (const auto &vol : m->volumes())
							{
								if (vol->facets().contains(f) && vol->facets().contains(f1))
								{
									isInVolume = true;
									break;
								}
							}
							if (!isInVolume)
							{
								QPair<Facet*, Facet *>r(f, f1);
								if (!bifaces.contains(r))
									bifaces << r;
//								qDebug() << "Manque 2 faces pour faire un volume !";
							}
						}
					}
				}
			}
		}
	}
	bool fCoupe = false;
	Facet * f2 = nullptr;
	Facet * f3 = nullptr;
	Volume *V = nullptr;
	do {
		fCoupe  = false;
		auto i = rand()%bifaces.size();
		Facet * f0 = bifaces[i].first;
		Facet * f1 = bifaces[i].second;
		bifaces.removeAt(i);
		QVector3D * v0 = nullptr;
		QVector3D * v1 = nullptr;
		for (const auto &v : f0->vertices())
		{
			if (f1->vertices().contains(v))
			{
				v0 = v;
				break;
			}
		}
		for (const auto &v : f0->vertices())
		{
			if (v == v0)
			{
				continue;
			}
			if (f1->vertices().contains(v))
			{
				v1 = v;
				break;
			}
		}
		QList<int> index {0, 1, 2};
		index.removeAll(f0->vertices().indexOf(v0));
		index.removeAll(f0->vertices().indexOf(v1));
		auto v2= f0->vertices().at(index[0]);
		QList<int> i2 {0, 1, 2};
		i2.removeAll(f1->vertices().indexOf(v0));
		i2.removeAll(f1->vertices().indexOf(v1));
		auto v3= f1->vertices().at(i2[0]);
        try
        {
            f2 = new Facet(v0, v2, v3);
        } catch (const std::runtime_error& e) {
			fCoupe = true;
			delete f2;
			continue;
		}
		for (auto f : m->facets())
		{
			if (*f == *f2 || f2->intersect(f))
			{
				fCoupe = true;
				delete f2;
				break;
			}
		}
		if (fCoupe)
		{
			continue;
		}
		try
        {
            f3 = new Facet(v1, v2, v3);
        } catch (const std::runtime_error& e) {
            fCoupe = true;
            delete f2;
            delete f3;
            continue;
        }
		for (auto f : m->facets())
		{
			if (*f == *f3 || f3->intersect(f))
			{
				fCoupe = true;
				delete f2;
				delete f3;
				break;
			}
		}
		if (fCoupe)
		{
			continue;
		}
		V = new Volume(f0, f1, f2, f3);
		//Là il faut vérifier qu'aucun des points n'est dans ce volume...
		for (auto v : m->vertices())
		{
            if (v == v0 || v == v1 || v == v2 || v == v3)
			{
				continue;
			}
			if (V->isIncluding(*v))
			{
				fCoupe = true;
				delete V;
				V = nullptr;
				delete f2;
				delete f3;
				break;
			}
		}
	}while (fCoupe && bifaces.size());
	//Et puis traiter le cas du Triface.
	if (!V)
		return;
	m->addVolume(*V);
	delete V;
}
