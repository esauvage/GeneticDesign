#ifndef TESTFACET_H
#define TESTFACET_H

#include "AutoTest.h"

class TestFacet : public QObject
{
    Q_OBJECT
public:
//    explicit TestFacet(QObject *parent = nullptr);

private slots:
    void normal();
    void distanceTo();
    void surface();
	void coordLocale();
	void coteUnique();
	void intersectFacet();
    void equality();
};

DECLARE_TEST(TestFacet)

#endif // TESTFACET_H
