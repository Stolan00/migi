#pragma once

#include <QtSql>

class AnimeDetailModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit AnimeDetailModel(QObject *parent = nullptr);
    //Q_INVOKABLE void executeQuery(const QString &query);

    enum Roles {
        AnimeIdRole = Qt::UserRole,
        TitleEnglishRole,
        TitleRomajiRole,
        FormatRole,
        EpisodesRole,
        StatusRole,
        SeasonRole,
        //Genres?
        //Producers?
        ScoreRole,
        UserProgressRole,
        UserScoreRole,


        SortTitleRole
    };

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void setAnimeIdFilter(int animeId);

signals:
private:
    static void createTable();
};
