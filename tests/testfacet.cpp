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
	//Distances de part et d'autre du triangle, projection sur le triangle
    for (int i = 0; i < 10; ++i)
    {
        auto x = rand()/(double)RAND_MAX;
		auto y = rand()/(double)RAND_MAX *(1-x);
		auto z = rand()%2 * 2 - 1;
		qDebug() << i << x << y << z;
		QVERIFY((x + y) <= 1.);
		QVector3D point(x, y, z);
        QCOMPARE(f.distanceTo(point), 1.);
    }
	//Distances sur les pointes
	QVector3D point0(0, 0, rand()%2 * 2 - 1);
	QCOMPARE(f.distanceTo(point0), 1.);
	QVector3D point1(1, 0, rand()%2 * 2 - 1);
    QCOMPARE(f.distanceTo(point1), 1.);
	QVector3D point2(0, 1, rand()%2 * 2 - 1);
    QCOMPARE(f.distanceTo(point2), 1.);
	//Distances aux arêtes
	QVector3D point3(0.5, -1, 0);
	QCOMPARE(f.distanceTo(point3), 1.);
	QVector3D point4(-1, 0.5, 0);
	QCOMPARE(f.distanceTo(point4), 1.);
	QVector3D point5(0.7, 0.7, 0);
	QVERIFY(fabs(fabs(f.distanceTo(point5) / (float)sqrt(0.08)) - 1.) < 0.0001);
}

void TestFacet::surface()
{
	QVector3D p[3] = {QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 1, 0)};
	Facet f(&p[0], &p[1], &p[2]);
	QCOMPARE(f.surface(), 0.5);
	QVector3D p1[3] = {QVector3D(0, 1, 0), QVector3D(2, 1, 0), QVector3D(0, 1, 2)};
	Facet f2(&p1[0], &p1[1], &p1[2]);
	QCOMPARE(f2.surface(), 2);
}


void TestFacet::coordLocale()
{
    auto x0 = rand()/(double)RAND_MAX;
    auto y0 = rand()/(double)RAND_MAX;
    auto z0 = rand()/(double)RAND_MAX;
	QVector3D p[3] = {QVector3D(x0, y0, z0), QVector3D(x0+1, y0, z0), QVector3D(x0, 1+y0, z0)};
	Facet f(&p[0], &p[1], &p[2]);
	auto x = rand()/(double)RAND_MAX;
	auto y = rand()/(double)RAND_MAX;
	auto z = rand()/(double)RAND_MAX;
	auto r = f.coordLocales(QVector3D(x, y, z))- QVector3D(x-x0, y-y0, z-z0);
	QVERIFY(r.x() < 0.0001);
	QVERIFY(r.y() < 0.0001);
	QVERIFY(r.z() < 0.0001);
    QVector3D p1[3] = {QVector3D(x0, y0, z0), QVector3D(x0, y0+1, z0), QVector3D(x0, y0, z0+1)};
    Facet f1(&p1[0], &p1[1], &p1[2]);
    r = f1.coordLocales(QVector3D(x, y, z));
    r = r - QVector3D(y-y0, z-z0, x-x0);
    QVERIFY(r.x() < 0.0001);
    QVERIFY(r.y() < 0.0001);
    QVERIFY(r.z() < 0.0001);
}

void TestFacet::coteUnique()
{
	auto x0 = rand()/(double)RAND_MAX;
	auto y0 = rand()/(double)RAND_MAX;
	auto z0 = rand()/(double)RAND_MAX;
	QVector3D p[3] = {QVector3D(x0, y0, z0), QVector3D(x0+1, y0, z0), QVector3D(x0, 1+y0, z0)};
	Facet f(&p[0], &p[1], &p[2]);
	auto x = rand()/(double)RAND_MAX;
	auto y = rand()/(double)RAND_MAX;
	auto z = rand()/(double)RAND_MAX;
	QVector3D p1[3] = {QVector3D(x0+x, y0, z0), QVector3D(x0+x, y0+y, z0), QVector3D(x0+x, y0, z0+z)};
	Facet f1(&p1[0], &p1[1], &p1[2]);
	QVERIFY(f.coteUnique(&f1));
	Facet f2(&p[0], &p1[1], &p1[2]);
	QVERIFY(f.coteUnique(&f2));
//	QVERIFY(f2.coteUnique(&f));
	f2.setVertice(1, &p[1]);
	QVERIFY(f.coteUnique(&f2));
	QVERIFY(f2.coteUnique(&f));
	f2.setVertice(2, &p[2]);
	QVERIFY(f.coteUnique(&f2));
	QVERIFY(f2.coteUnique(&f));
}

void TestFacet::intersectFacet()
{
	QVector3D p[3] = {QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 1, 0)};
	Facet f(&p[0], &p[1], &p[2]);
	QVector3D p1[3] = {QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 0, 1)};
	Facet f1(&p1[0], &p1[1], &p1[2]);
	QVERIFY(!f1.intersect(&f));
	QVERIFY(!f.intersect(&f1));
    QVector3D p2[3] = {QVector3D(0.5, 0, -0.5), QVector3D(0.5, 1, -0.5), QVector3D(0.5, 0, 0.5)};
	Facet f2(&p2[0], &p2[1], &p2[2]);
    QVERIFY(f.intersect(&f2));
    QVERIFY(f2.intersect(&f));
	//f et f4 se touchent par la pointe
	QVector3D p3[2] = {QVector3D(1, 0, 1), QVector3D(0, 1, 1)};
	Facet f3(&p3[0], &p[0], &p3[1]);
	QVERIFY(!f.intersect(&f3));
    QVERIFY(!f3.intersect(&f));
}

void TestFacet::equality()
{
    QVector3D p[3] = {QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(0, 1, 0)};
    Facet f(&p[0], &p[1], &p[2]);
    QVERIFY(f == Facet(&p[0], &p[1], &p[2]));
    QVERIFY(f == Facet(&p[0], &p[2], &p[1]));
    QVERIFY(f == Facet(&p[1], &p[2], &p[0]));
    QVERIFY(f == Facet(&p[0], &p[2], &p[1]));
}

#include "moc_testfacet.cpp"
