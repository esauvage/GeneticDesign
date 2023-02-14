#ifndef FACET_H
#define FACET_H

#include <QVector3D>
#include <QList>

class QTextStream;

class Facet
{
public:
    Facet(QVector3D *a, QVector3D *b, QVector3D *c);
    QVector3D normal() const;
    const QList<QVector3D *> &vertices() const;
    double distanceTo(QVector3D point) const;
    bool operator==(const Facet &f) const;
	bool intersect(const QVector3D *a, const QVector3D *b) const;
	bool intersect(const Facet *f) const;
	bool isContaining(const QVector3D &p) const;
	double surface() const;
	void setVertice(int i, QVector3D *v);
	QVector3D coordLocales(const QVector3D &p) const;
	bool coteUnique(const Facet *f) const;
    QPair<QVector3D, QVector3D>commonLine(const Facet *f) const;

private:
    QList<QVector3D *> _vertices;
};

QTextStream &operator<<(QTextStream &out, const Facet &facet);
QTextStream &operator<<(QTextStream &out, const QVector3D &vertex);

#endif // FACET_H
