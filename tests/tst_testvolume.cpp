#include "AutoTest.h"


#include "../volume.h"

// add necessary includes here

class TestVolume : public QObject
{
	Q_OBJECT

public:
	TestVolume();
	~TestVolume();

private slots:
    void isIncluding();

};

TestVolume::TestVolume()
{

}

TestVolume::~TestVolume()
{

}

void TestVolume::isIncluding()
{
    QVector3D p[4] = {QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(1, 0, 0), QVector3D(0, 0, 1)};
    Facet f[4] = {Facet(&p[0], &p[1], &p[2]), Facet(&p[1], &p[3], &p[2]), Facet(&p[0], &p[3], &p[2]), Facet(&p[1], &p[3], &p[0])};
    Volume v(&f[0], &f[1], &f[2], &f[3]);
    QVERIFY(!v.isIncluding(QVector3D(1, 1, 1)));
    //Test dessous
    QVERIFY(!v.isIncluding(QVector3D(0.1, 0.1, -rand()/(double)RAND_MAX)));
    //Test dessus
    QVERIFY(!v.isIncluding(QVector3D(0.1, 0.8, rand()/(double)RAND_MAX))+0.2);
    for (auto i = 0; i < 4; ++i)
    {
        QVERIFY(v.isIncluding(p[i]));
    }
}

DECLARE_TEST(TestVolume)

#include "tst_testvolume.moc"
