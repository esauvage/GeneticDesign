#ifndef TESTFACET_H
#define TESTFACET_H

#include <QtTest/QtTest>

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
};

#endif // TESTFACET_H
