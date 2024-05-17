#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QFile>
#include <QTextStream>
#include <QDebug> //temp

class FileWriter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON //not sure if i actually want this to be a singleton but for now it is

public:
    explicit FileWriter(QObject *parent = nullptr);
    QString readFile(const QString& path);

signals:

public slots:
    void writeFile(const QString path, const QString value);
    // ^ dont really like this is a slot (should have another caller function?) but too lazy to fix
    // also dont like that im forced to pass by value, will try to fix later
};
