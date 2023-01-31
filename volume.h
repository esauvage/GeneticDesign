#ifndef VOLUME_H
#define VOLUME_H

#include "facet.h"

#include <QSet>

class Volume
{
public:
	Volume(Facet *a, Facet *b, Facet *c, Facet *d);
	double volume() const;
    bool isIncluding(const QVector3D &p) const;
    QSet <QVector3D *> vertices() const;
	const QList<Facet *> &facets() const;
    void setFacet(int i, Facet *f);
private:
	QList<Facet *>_facets;
};

#endif // VOLUME_H
