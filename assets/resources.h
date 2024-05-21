#pragma once

#include "assets/appresourcekey.h"
#include <QObject>

class Resources : public QObject
{
    Q_OBJECT
public:
    explicit Resources(QObject *parent = nullptr);
    static QString value(const AppResourceKey key);


    QVariant readResource(const AppResourceKey& key);

signals:

private:
    QVariant readResourceFile(const QString &resourcePath);
};
