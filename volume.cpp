#include "volume.h"

Volume::Volume(const Facet *a, const Facet *b, const Facet *c, const Facet *d)
{
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
		return -1;
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
//  u0 = 1−u1−u2−u3;
//  u1(P1−P0)+u2(P2−P0)+u3(P3−P0)=(P−P0)
    QVector3D PP0 = p - (*P0);
    QVector3D P1P0 = (*P1) - (*P0);
    QVector3D P2P0 = (*P2) - (*P0);
    QVector3D P3P0 = (*P3) - (*P0);
    double u1, u2, u3, u0;
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
        for (const auto & v: f->vertices())
        {
            r << v;
        }
    }
    return r;
}

const QList<const Facet *> &Volume::facets() const
{
    return _facets;
}
