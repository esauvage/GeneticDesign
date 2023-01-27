#include "meshgenerator.h"

MeshGenerator::MeshGenerator()
{

}

Mesh MeshGenerator::genere()
{
    Mesh m;
    QVector3D * point = nullptr;
    for (;!point || m.isIncluding(*point); point = new QVector3D(rand() * 5. / (double)RAND_MAX, rand() * 5. / (double)RAND_MAX, rand() * 5. / (double)RAND_MAX))
    {
    }
    const Facet &f = m.nearestFacet(*point);
    Facet f1(f.vertices()[0], f.vertices()[1], point);
    Facet f2(f.vertices()[1], f.vertices()[2], point);
    Facet f3(f.vertices()[2], f.vertices()[0], point);
    Volume v(&f, &f1, &f2, &f3);
    m.addVolume(v);
    return m;
}
