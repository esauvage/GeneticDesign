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
};

#endif // TESTFACET_H
