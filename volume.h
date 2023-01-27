#ifndef VOLUME_H
#define VOLUME_H

#include "facet.h"

class Volume
{
public:
	Volume(Facet *a, Facet *b, Facet *c, Facet *d);
	double volume() const;
private:
	QList<Facet *>_facets;
};

#endif // VOLUME_H
