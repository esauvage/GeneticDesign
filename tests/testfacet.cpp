#include "testfacet.h"

#include "../facet.h"

//TestFacet::TestFacet(QObject *parent)
//    : QObject{parent}
//{

//}

void TestFacet::normal()
{
    //Test face horizontale
    QVector3D p[] = {QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 1, 0)};
    Facet f(&p[0], &p[1], &p[2]);
    QCOMPARE(f.normal(), QVector3D(0, 0, 1));
    //Test face verticale
    QVector3D p2[] = {QVector3D(0, 1, 0), QVector3D(0, 0, 1), QVector3D(0, 0, 0)};
    Facet f2(&p2[0], &p2[1], &p2[2]);
    QCOMPARE(f2.normal(), QVector3D(1, 0, 0));
    //Test norme
    QVector3D p3[] = {QVector3D(0, 2, 0), QVector3D(0, 0, 20), QVector3D(0, 0, 0)};
    Facet f3(&p3[0], &p3[1], &p3[2]);
    QCOMPARE(f3.normal(), QVector3D(1, 0, 0));
}

void TestFacet::distanceTo()
{
    QVector3D p[3] = {QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 1, 0)};
    Facet f(&p[0], &p[1], &p[2]);
    for (int i = 0; i < 10; ++i)
    {
        auto x = rand()/(double)RAND_MAX;
        auto y = rand()/(double)RAND_MAX * x;
        QVERIFY(x >= y);
        qDebug() << x << y << i;
        QVector3D point(x, y, 1);
        QCOMPARE(f.distanceTo(point), 1.);
    }
    QVector3D point0(0, 0, 1);
    QCOMPARE(f.distanceTo(point0), 1.);
    QVector3D point1(1, 0, -1);
    QCOMPARE(f.distanceTo(point1), 1.);
    QVector3D point2(0, 1, -1);
    QCOMPARE(f.distanceTo(point2), 1.);
    QVector3D point3(0, -1, 0);
    QCOMPARE(f.distanceTo(point3), 1.);
}

void TestFacet::surface()
{
    QVector3D p[3] = {QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 1, 0)};
    Facet f(&p[0], &p[1], &p[2]);
    QCOMPARE(f.surface(), 0.5);
}

QTEST_MAIN(TestFacet)
#include "moc_testfacet.cpp"
