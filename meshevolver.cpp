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
		auto j = rand()%m->vertices().size();
        QVector3D *ref = m->vertices()[j];
        QVector3D *p = new QVector3D(*ref);
		auto fMemeCote = true;
		auto fFacetteCroisees = false;
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
			QList <Facet *>newFacets;
			for (auto f : m->facets())
			{
				if (f->vertices().contains(ref))
				{
					auto f1 = new Facet(f->vertices()[0], f->vertices()[1], f->vertices()[2]);
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
						}
						cotes *= n.dotProduct(*(f1->vertices()[2]) - p, n);
						if ((cotes < 0 && positif) || (cotes > 0 && !positif))
						{
							facesSecantes = true;
						}
						if (facesSecantes)
						{
							facesSecantes = false;
							//On calcule la droite d'intersection des plans des triangles
							QVector3D origin(0, 0, 0);
							auto n1 = f1->normal();
							auto pH = origin.distanceToPlane(p, n);
							auto p1 = origin.distanceToPlane(*(f1->vertices()[0]), n);
							auto lineDir = n.crossProduct(n, n1);
			//				n.x() * x + n.y() * y = -pH
			//				n1.x() * x + n1.y() * y = -p1
							//				n1.y() * n.x() * x + n1.y() * n.y() * y = -n1.y() * pH
							//				n1.x() * n.y() * x + n1.y() * n.y() * y = -n.y() * p1
							//				(n1.y() * n.x() - n1.x() * n.y()) * x = n.y() * p1 - n1.y() * pH
							//				n1.x() * n.y() * x + n1.y() * n.y() * y = -n.y() * p1
							double x = (n.y() * p1 - n1.y() * pH) / (n1.y() * n.x() - n1.x() * n.y());
							double y = (-pH - n.x() * x)/n.y();
							QVector3D linePoint(x, y, 0.);
							//Est-ce que les triangles se croisent sur cette ligne ?
							using Line2 = Eigen::Hyperplane<float,2>;
							using Vec2  = Eigen::Vector2f;
							QList<double> listPoints;
							auto f1LineDir = f1->coordLocales(lineDir);
							auto f1LineOrig = f1->coordLocales(linePoint);
							Vec2 linedir(f1LineDir.x(), f1LineDir.y());
							Vec2 linepoint(f1LineOrig.x(), f1LineOrig.y());
							Vec2 b(0, 0);
							Vec2 d(1, 0);

							Line2 ac = Line2(linedir,linepoint);
							Line2 bd = Line2::Through(b,d);
							auto pIntersect = ac.intersection(bd);
							if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
							{
								listPoints << ac.signedDistance(pIntersect);
			//					listPoints << QVector3D(pIntersect[0], pIntersect[1], 0) * ;
							}
							b = Vec2(0, 0);
							d = Vec2(0, 1);

							bd = Line2::Through(b,d);
							pIntersect = ac.intersection(bd);
							if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
							{
								listPoints << ac.signedDistance(pIntersect);
			//					listPoints << QVector3D(pIntersect[0], pIntersect[1], pIntersect[2]);
							}
							b = Vec2(0, 1);
							bd = Line2::Through(b,d);
							pIntersect = ac.intersection(bd);
							if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
							{
								listPoints << ac.signedDistance(pIntersect);
			//					listPoints << QVector3D(pIntersect[0], pIntersect[1], pIntersect[2]);
							}
							if (listPoints.size() < 2)
								continue;
							auto fRefLineDir = fRef->coordLocales(lineDir);
							auto fRefLineOrig = fRef->coordLocales(linePoint);
							QList<double> listPoints2;
							linedir = Vec2(fRefLineDir.x(), fRefLineDir.y());
							linepoint = Vec2(fRefLineOrig.x(), fRefLineOrig.y());
							b = Vec2(0, 0);
							d = Vec2(1, 0);

							ac = Line2(linedir,linepoint);
							bd = Line2::Through(b,d);
							pIntersect = ac.intersection(bd);
							if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
							{
								listPoints2 << ac.signedDistance(pIntersect);
			//					listPoints << QVector3D(pIntersect[0], pIntersect[1], 0) * ;
							}
							b = Vec2(0, 0);
							d = Vec2(0, 1);

							bd = Line2::Through(b,d);
							pIntersect = ac.intersection(bd);
							if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
							{
								listPoints2 << ac.signedDistance(pIntersect);
			//					listPoints << QVector3D(pIntersect[0], pIntersect[1], pIntersect[2]);
							}
							b = Vec2(0, 1);
							bd = Line2::Through(b,d);
							pIntersect = ac.intersection(bd);
							if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
							{
								listPoints2 << ac.signedDistance(pIntersect);
			//					listPoints << QVector3D(pIntersect[0], pIntersect[1], pIntersect[2]);
							}
							if (listPoints2.size() < 2)
								continue;
							std::sort(listPoints.begin(), listPoints.end());
							std::sort(listPoints2.begin(), listPoints2.end());
							if (listPoints[1] <= listPoints2[0])
								continue;
							if (listPoints2[1] <= listPoints[0])
								continue;
							facesSecantes = true;
							if (facesSecantes)
								break;
						}
					}
					delete f1;
					fFacetteCroisees &= facesSecantes;
					if (fFacetteCroisees)
						break;
				}
			}
		}while (m->isIncluding(*p) && !fMemeCote && fFacetteCroisees);
		m->setVertice(j, p);
		delete p;
	}
}
