#pragma once

#include <QtSql>

class AnimeListModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit AnimeListModel(QObject *parent = nullptr);
    //Q_INVOKABLE void executeQuery(const QString &query);

    enum Roles {
        AnimeIdRole = Qt::UserRole,
        TitleEnglishRole,
        TitleRomajiRole,
        ProgressRole,
        ScoreRole,
        FormatRole
    };

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void setStatusFilter(int statusId);

signals:
};
