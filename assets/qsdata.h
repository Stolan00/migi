#pragma once

#include <QObject>

class QSData : public QObject
{
    Q_OBJECT

public:
    explicit QSData(QObject *parent = nullptr);
    QString animeInfo;

signals:
};


