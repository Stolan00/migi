#pragma once

#include <QtSql>
#include "assets/anime.h"
#include "assets/databasemanager.h"

class AnilistDatabaseManager
{
public:
        AnilistDatabaseManager();

        void addListsToDB(const QList<Anime> &mediaList);
        bool updateDatabase(const QList<Anime>& mediaList);
        void insertAnimeGenre(QSqlTableModel &animeGenreTable, const Anime& anime, const QString& genre);
        void batchInsertAnimeGenres(QSqlTableModel& animeGenreTable, const QList<Anime>& animes);

    private:
        void updateAnime(QSqlTableModel &animeTable, const Anime &anime);
        void updateEntry(QSqlTableModel &entryTable, const Anime &anime);
        void updateAnimeGenres(QSqlTableModel &animeGenreTable, const Anime &anime);
        void updateStudios(QSqlTableModel &studioTable, const Anime &anime);
        void updateAnimeStudio(QSqlTableModel &animeStudioTable, int animeId, const Anime::Studio &studio);

        void insertAnime(QSqlTableModel &animeTable, const Anime &anime);
        void insertEntry(QSqlTableModel &entryTable, const Anime &anime);
        void insertStudio(QSqlTableModel &studioTable, const Anime::Studio &studio);
        void insertAnimeStudio(QSqlTableModel &animeStudioTable, int animeId, const Anime::Studio &studio);

        QSqlDatabase m_db;
        DatabaseManager& m_dbManager = DatabaseManager::instance(); //TODO: DELETE
};
