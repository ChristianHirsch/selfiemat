#ifndef selfiematTEST_H
#define selfiematTEST_H

#include <QtCore/QObject>
#include <QtTest/QTest>

class selfiematTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void someTest();
};

#endif // selfiematTEST_H
