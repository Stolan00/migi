#pragma once

#include <QtSql>

class AnimeListModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit AnimeListModel(QObject *parent = nullptr);
    //Q_INVOKABLE void executeQuery(const QString &query);

    enum Roles {
        AnimeIdRole = Qt::UserRole, //256
        TitleEnglishRole, //257
        TitleRomajiRole, //258
        SortTitleRole, //259
        SynonymsRole, //260
        ProgressRole, //261
        EpisodesRole, //262
        ScoreRole, //263
        StatusRole, //264
        FormatRole, //265
        SeasonRole, //266
        SeasonYearRole //267
    };

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE void setStatusFilter(int statusId);
    Q_INVOKABLE void forceUpdate();

    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
private:
    static void createTable();
};
