#include "assets/anilistdatabasemanager.h"
#include "assets/appresourcekey.h"
// --------------------------------------------------------------------------------------------------------------------------
AnilistDatabaseManager::AnilistDatabaseManager()
{
    m_db = QSqlDatabase::database();
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::addListsToDB(const QList<Anime> &mediaList) {
    QSqlTableModel animeTable;
    animeTable.setTable("Anime");
    animeTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    QSqlTableModel entryTable;
    entryTable.setTable("Entry");
    entryTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    QSqlTableModel animeGenreTable;
    animeGenreTable.setTable("AnimeGenre");
    animeGenreTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    QSqlTableModel studioTable;
    studioTable.setTable("Studio");
    studioTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    m_dbManager.executeSqlScript(toString(AppResourceKey::SQLPopulateTables));

    if (!animeTable.select() || !entryTable.select() || !animeGenreTable.select() || !studioTable.select()) {
        qDebug() << "Failed to select one or more tables.";
        return;
    }

    for (const Anime& anime : mediaList) {
        insertAnime(animeTable, anime);
        insertEntry(entryTable, anime);
        for (const QString& genre : anime.genres) {
            insertAnimeGenre(animeGenreTable, anime, genre);
        }
        insertStudios(studioTable, anime);
    }

    if (!animeTable.submitAll()) {
        qDebug() << "Failed to submit all Anime records:" << animeTable.lastError();
    }
    if (!entryTable.submitAll()) {
        qDebug() << "Failed to submit all Entry records:" << entryTable.lastError();
    }
    if (!animeGenreTable.submitAll()) {
        qDebug() << "Failed to submit all AnimeGenre records:" << animeGenreTable.lastError();
    }
    if (!studioTable.submitAll()) {
        qDebug() << "Failed to submit all Studio records:" << studioTable.lastError();
    }
    qDebug() << "DONE";
}
// --------------------------------------------------------------------------------------------------------------------------
bool AnilistDatabaseManager::updateDatabase(const QList<Anime>& mediaList) {
    QSqlTableModel animeTable;
    animeTable.setTable("Anime");
    animeTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    QSqlTableModel entryTable;
    entryTable.setTable("Entry");
    entryTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    QSqlTableModel animeGenreTable;
    animeGenreTable.setTable("AnimeGenre");
    animeGenreTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    QSqlTableModel studioTable;
    studioTable.setTable("Studio");
    studioTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    if (!animeTable.select()) {
        qDebug() << "COULD NOT SELECT ANIME TABLE";
        return false;
    }

    for (const Anime& anime : mediaList) {
        updateAnime(animeTable, anime);
        updateEntry(entryTable, anime);
        updateAnimeGenres(animeGenreTable, anime);
        updateStudios(studioTable, anime);
    }

    return true;
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertAnime(QSqlTableModel &animeTable, const Anime &anime) {
    QSqlRecord newAnimeRecord = animeTable.record();
    auto animeInfo = anime.asHash();

    for (auto it = animeInfo.cbegin(); it != animeInfo.cend(); ++it) {
        newAnimeRecord.setValue(it.key(), it.value());
    }

    if (!animeTable.insertRecord(-1, newAnimeRecord)) {
        qDebug() << "Failed to insert Anime record:" << animeTable.lastError();
    } else {
        qDebug() << "Inserted record for Anime ID:" << anime.id;
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertEntry(QSqlTableModel &entryTable, const Anime &anime) {
    QSqlRecord newEntryRecord = entryTable.record();
    auto entryInfo = anime.myInfoAsHash();

    for (auto it = entryInfo.cbegin(); it != entryInfo.cend(); ++it) {
        newEntryRecord.setValue(it.key(), it.value());
    }

    if (!entryTable.insertRecord(-1, newEntryRecord)) {
        qDebug() << "Failed to insert Entry record:" << entryTable.lastError();
    } else {
        qDebug() << "Inserted record for Entry ID:" << anime.id;
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertStudios(QSqlTableModel &studioTable, const Anime &anime) {
    for (const Anime::Studio& studio : anime.studios) {
        // Set filter to check if the studio already exists
        studioTable.setFilter(QString("studioId = %1").arg(studio.studioId));
        studioTable.select();

        if (studioTable.rowCount() == 0) {
            // Studio does not exist, proceed with insertion
            QSqlRecord newStudioRecord = studioTable.record();
            newStudioRecord.setValue("studioId", studio.studioId);
            newStudioRecord.setValue("studioName", studio.studioName);

            if (!studioTable.insertRecord(-1, newStudioRecord)) {
                qDebug() << "Failed to insert Studio record:" << studioTable.lastError();
            } else {
                qDebug() << "Inserted record for Studio ID:" << studio.studioId << " " << studio.studioName;
            }
        } else {
            qDebug() << "Studio already exists for studioId:" << studio.studioId;
        }

        // Clear the filter after checking
        studioTable.setFilter("");
        studioTable.select();
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::updateAnime(QSqlTableModel &animeTable, const Anime &anime) {
    animeTable.setFilter(QString("id = %1").arg(anime.id));
    animeTable.select();

    if (animeTable.rowCount() > 0) {
        QSqlRecord animeRecord = animeTable.record(0);
        int dbAnilistModified = animeRecord.value("anilistModified").toInt();
        if (anime.anilistModified > dbAnilistModified) {
            qDebug() << "UPDATING ANIME: " << anime.id << " " << anime.titleRomaji;
            auto animeInfo = anime.asHash();

            for (auto it = animeInfo.cbegin(); it != animeInfo.cend(); ++it) {
                animeRecord.setValue(it.key(), it.value());
            }

            animeTable.setRecord(0, animeRecord);

            if (!animeTable.submitAll()) {
                qDebug() << "Failed to update record:" << animeTable.lastError();
            } else {
                qDebug() << "Updated record for Anime ID:" << anime.id;
            }
        }
    } else {
        qDebug() << "Anime ID not found in the table:" << anime.id;
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::updateEntry(QSqlTableModel &entryTable, const Anime &anime) {
    entryTable.setFilter(QString("mediaId = %1").arg(anime.id));
    entryTable.select();

    if (entryTable.rowCount() > 0) {
        QSqlRecord entryRecord = entryTable.record(0);
        int dbAniListEntryModified = entryRecord.value("anilistModified").toInt();

        if (anime.myInfo.anilistModified > dbAniListEntryModified) {
            qDebug() << "UPDATING ENTRY: " << " " << entryRecord.value("id") << " " << anime.titleRomaji;
            auto entryInfo = anime.myInfoAsHash();

            for (auto it = entryInfo.cbegin(); it != entryInfo.cend(); ++it) {
                entryRecord.setValue(it.key(), it.value());
            }

            entryTable.setRecord(0, entryRecord);

            if (!entryTable.submitAll()) {
                qDebug() << "Failed to update entry:" << entryTable.lastError();
            } else {
                qDebug() << "Updated entry for Entry ID:" << anime.id;
            }
        }
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::updateAnimeGenres(QSqlTableModel &animeGenreTable, const Anime &anime) {
    for (const QString& genre : anime.genres) {
        QString animeGenreFilter = QString("animeId = %1 AND genreId = %2").arg(anime.id).arg(anime.getGenreIndex(genre));
        animeGenreTable.setFilter(animeGenreFilter);
        animeGenreTable.select();

        if (animeGenreTable.rowCount() == 0) {
            insertAnimeGenre(animeGenreTable, anime, genre);
        }
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::updateStudios(QSqlTableModel &studioTable, const Anime &anime) {
    for (const Anime::Studio& studio : anime.studios) {
        studioTable.setFilter(QString("studioId = %1").arg(studio.studioId));
        studioTable.select();

        if (studioTable.rowCount() == 0) {
            QSqlRecord newStudioRecord;
            newStudioRecord.setValue("studioId", studio.studioId);
            newStudioRecord.setValue("studioName", studio.studioName);

            studioTable.setRecord(0, newStudioRecord);

            if (!studioTable.submitAll()) {
                qDebug() << "Failed to update Studio:" << studioTable.lastError();
            } else {
                qDebug() << "Updated entry for Studio ID:" << studio.studioId << " " << studio.studioName;
            }
        }
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertAnimeGenre(QSqlTableModel& animeGenreTable, const Anime& anime, const QString& genre) {
    // Create a new record based on the table's structure
    QSqlRecord newAnimeGenreRecord = animeGenreTable.record();

    // Set the values for the new record
    newAnimeGenreRecord.setValue("genreId", anime.getGenreIndex(genre));
    newAnimeGenreRecord.setValue("animeId", anime.id);

    // Insert the new record into the model at the end
    if (!animeGenreTable.insertRecord(-1, newAnimeGenreRecord)) {
        qDebug() << "Failed to insert record:" << animeGenreTable.lastError().text();
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::batchInsertAnimeGenres(QSqlTableModel& animeGenreTable, const QList<Anime>& animes) {
    animeGenreTable.database().transaction(); // Begin a transaction

    for (const Anime &anime : animes) {
        for (const QString &genre : anime.genres) {
            insertAnimeGenre(animeGenreTable, anime, genre);
        }
    }

    if (!animeGenreTable.submitAll()) {
        qDebug() << "Failed to submit all changes:" << animeGenreTable.lastError().text();
        animeGenreTable.database().rollback(); // Rollback transaction on failure
    } else {
        animeGenreTable.database().commit(); // Commit transaction on success
    }
}
// --------------------------------------------------------------------------------------------------------------------------
