#include "volume.h"

Volume::Volume(Facet *a, Facet *b, Facet *c, Facet *d)
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
