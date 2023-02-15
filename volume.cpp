#include "volume.h"
#include <Eigen/Geometry>

using Line2 = Eigen::Hyperplane<float,2>;
using Vec3  = Eigen::Vector3f;
using Mat3 = Eigen::Matrix3f;

Volume::Volume(Facet *a, Facet *b, Facet *c, Facet *d)
{
    if (!a || !b || !c || !d)
    {
        throw std::runtime_error("at least one Facet is a nullptr");
    }
    if (a == b || a == c || a == d || b == c || b == d || c == d)
    {
        throw std::runtime_error("at least 2 facets are duplicated");
    }
	_facets << a << b << c << d;
}

double Volume::volume() const
{
	double v = 0;//produit mixte de 3 vecteurs
	QVector3D *a = _facets[0]->vertices()[0];
	QVector3D *b = _facets[0]->vertices()[1];
	QVector3D *c = _facets[0]->vertices()[2];
	QVector3D *d = nullptr;
	for (auto i : _facets[1]->vertices())
	{
		if (i != a && i != b && i != c)
		{
			d = i;
			break;
		}
	}
	if (!d)
    {
        throw std::out_of_range("couldn't find a 4th point in the volume.");
		return -1;
    }
	QVector3D e = *b - *a;
	QVector3D f = *c - *a;
	e = e.crossProduct(e, f);
	f = *d - *a;
	v = fabs(e.dotProduct(e, f));
	v/=6.;
	return v;
}

bool Volume::isIncluding(const QVector3D &p) const
{
	Vec3 p0(_facets[0]->vertices().at(0)->x(), _facets[0]->vertices().at(0)->y(), _facets[0]->vertices().at(0)->z());
	Vec3 p1(_facets[0]->vertices().at(1)->x(), _facets[0]->vertices().at(1)->y(), _facets[0]->vertices().at(1)->z());
	Vec3 p2(_facets[0]->vertices().at(2)->x(), _facets[0]->vertices().at(2)->y(), _facets[0]->vertices().at(2)->z());
	QVector3D *P0 = _facets[0]->vertices()[0];
    QVector3D *P1 = _facets[0]->vertices()[1];
    QVector3D *P2 = _facets[0]->vertices()[2];
    QVector3D *P3 = nullptr;
    for (auto i : _facets[1]->vertices())
    {
        if (i != P0 && i != P1 && i != P2)
        {
            P3 = i;
            break;
        }
    }
    if (!P3)
        return false;
	Vec3 p3(P3->x(), P3->y(), P3->z());
	Vec3 pp(p.x(), p.y(), p.z());
	p1 -= p0;
	p2 -= p0;
	p3 -= p0;
	pp -= p0;
	Mat3 A;
	A.col(0) << p1;
	A.col(1) << p2;
	A.col(2) << p3;
	Vec3 u = A.colPivHouseholderQr().solve(pp);
	double u0 = 1 - u[0] - u[1] - u[2];
	return !(u[0] < 0. || u[1] < 0. || u[2] < 0. || u0 < 0.);
//  u0 = 1−u1−u2−u3;
//  u1(P1−P0)+u2(P2−P0)+u3(P3−P0)=(P−P0)
    QVector3D PP0 = p - (*P0);
    QVector3D P1P0 = (*P1) - (*P0);
    QVector3D P2P0 = (*P2) - (*P0);
    QVector3D P3P0 = (*P3) - (*P0);
	double u1, u2, u3;
//    u1 * P1P0.x() + u2 * P2P0.x() + u3 * P3P0.x() = PP0.x();
//    u1 * P1P0.y() + u2 * P2P0.y() + u3 * P3P0.y() = PP0.y();
//    u1 * P1P0.z() + u2 * P2P0.z() + u3 * P3P0.z() = PP0.z();
    u0 = (((P3P0.y() * P1P0.x() - P1P0.y() * P3P0.x()) * (P2P0.x() * P3P0.z() - P3P0.x() * P2P0.z())) - (P3P0.y() * P2P0.x() - P2P0.y() * P3P0.x()) * (P1P0.x() * P3P0.z() - P3P0.x() * P1P0.z()));
    if (u0)
    {
        u1 = (PP0.x() * P3P0.y() - PP0.y() * P3P0.x()) * (P2P0.x() * P3P0.z() - P3P0.x() * P2P0.z()) - (PP0.x() * P3P0.z() - PP0.z() * P3P0.x()) * (P3P0.y() * P2P0.x() - P2P0.y() * P3P0.x());
        u1 /= u0;
    }
    else
    {
        u0 = (((P3P0.z() * P1P0.y() - P1P0.z() * P3P0.y()) * (P2P0.y() * P3P0.x() - P3P0.y() * P2P0.x())) - (P3P0.z() * P2P0.y() - P2P0.z() * P3P0.y()) * (P1P0.y() * P3P0.x() - P3P0.y() * P1P0.x()));
        if (u0)
        {
            u1 = (PP0.y() * P3P0.z() - PP0.z() * P3P0.y()) * (P2P0.y() * P3P0.x() - P3P0.y() * P2P0.x()) - (PP0.y() * P3P0.x() - PP0.x() * P3P0.y()) * (P3P0.z() * P2P0.y() - P2P0.z() * P3P0.y());
            u1 /= u0;
        }
        else
        {
            u1 = (PP0.z() * P3P0.x() - PP0.x() * P3P0.z()) * (P2P0.z() * P3P0.y() - P3P0.z() * P2P0.y()) - (PP0.z() * P3P0.y() - PP0.y() * P3P0.z()) * (P3P0.x() * P2P0.z() - P2P0.x() * P3P0.z());
            u1 /= (((P3P0.x() * P1P0.z() - P1P0.x() * P3P0.z()) * (P2P0.z() * P3P0.y() - P3P0.z() * P2P0.y())) - (P3P0.x() * P2P0.z() - P2P0.x() * P3P0.z()) * (P1P0.z() * P3P0.y() - P3P0.z() * P1P0.y()));
        }
    }
    u0 = P2P0.x() * P3P0.y() - P3P0.x() * P2P0.y();
    if (u0)
    {
        u2 = (PP0.x() * P3P0.y()) - (PP0.y() * P3P0.x()) - u1 * (P1P0.x() * P3P0.y() - P3P0.x() * P1P0.y());
        u2 /= u0;
    }
    else
    {
        u0 = P2P0.y() * P3P0.z() - P3P0.y() * P2P0.z();
        if (u0)
        {
            u2 = (PP0.y() * P3P0.z()) - (PP0.z() * P3P0.y()) - u1 * (P1P0.y() * P3P0.z() - P3P0.y() * P1P0.z());
            u2 /= u0;
        }
        else
        {
            u2 = (PP0.z() * P3P0.x()) - (PP0.x() * P3P0.z()) - u1 * (P1P0.z() * P3P0.x() - P3P0.z() * P1P0.x());
            u2 /= (P2P0.z() * P3P0.x() - P3P0.z() * P2P0.x());
        }
    }
    if (P3P0.x() != 0)
    {
        u3 = (PP0.x() - u1 * P1P0.x() - u2 * P2P0.x());
        u3 /= P3P0.x();
    }
    else
    {
        u3 = (PP0.y() - u1 * P1P0.y() - u2 * P2P0.y());
        if (P3P0.y() != 0)
        {
            u3 /= P3P0.y();
        }
        else
        {
            u3 = (PP0.z() - u1 * P1P0.z() - u2 * P2P0.z());
            u3 /= P3P0.z();
        }
    }
    u0 = 1-u1-u2-u3;
    return !(u0 < 0. || u1 < 0. || u2 < 0. || u3 < 0.);
}

QSet<QVector3D *> Volume::vertices() const
{
    QSet<QVector3D *> r;
    for (const auto & f: _facets)
    {
		for (auto & v: f->vertices())
		{
			r << v;
		}
	}
    return r;
}

const QList<Facet *> &Volume::facets() const
{
    return _facets;
}

void Volume::setFacet(int i, Facet *f)
{
    _facets[i] = f;
}

bool Volume::isIntersecting(Volume *v)
{
    QList <QVector3D> pointsY;
    for (auto &x : vertices())
    {
        pointsY << *x;
    }
    QList <QVector3D> pointsV;
    for (auto &x : v->vertices())
    {
        pointsV << *x;
    }
    //On considère que ce tétraèdre est aligné avec les axes de R3
    //On va transformer v pour être défini dans cette transformation
    //D'abord on translate tout au point d'origine
    for (int i = 0; i < 4; ++i)
    {
        pointsV[i] -= pointsY[0];
        pointsY[i] -= pointsY[0];
    }
//    QMatrix3x3 m;
//    for (int i = 0; i < 3; ++i)
//    {
//        for (int j = 0; j < 3; ++j)
//        {
//            m(i, j) = pointsY[i+1][j];
//        }
//    }
    return false;
}
