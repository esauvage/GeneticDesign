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
private:
    QList<QVector3D *> _vertices;
};

QTextStream &operator<<(QTextStream &out, const Facet &facet);
QTextStream &operator<<(QTextStream &out, const QVector3D &vertex);

#endif // FACET_H
