#include <QtTest>

// add necessary includes here

class TestVolume : public QObject
{
	Q_OBJECT

public:
	TestVolume();
	~TestVolume();

private slots:
	void test_case1();

};

TestVolume::TestVolume()
{

}

TestVolume::~TestVolume()
{

}

void TestVolume::test_case1()
{

}

//QTEST_APPLESS_MAIN(TestVolume)

#include "tst_testvolume.moc"
