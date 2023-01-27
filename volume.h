#ifndef VOLUME_H
#define VOLUME_H

#include "facet.h"

#include <QSet>

class Volume
{
public:
    Volume(const Facet *a, const Facet *b, const Facet *c, const Facet *d);
	double volume() const;
    bool isIncluding(const QVector3D &p) const;
    QSet <QVector3D *> vertices() const;
    const QList<const Facet *> &facets() const;

private:
    QList<const Facet *>_facets;
};

#endif // VOLUME_H
