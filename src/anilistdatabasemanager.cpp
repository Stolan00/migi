#include "assets/anilistdatabasemanager.h"
#include "assets/appresourcekey.h"
// --------------------------------------------------------------------------------------------------------------------------
AnilistDatabaseManager::AnilistDatabaseManager()
{
    m_db = QSqlDatabase::database();
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::addListsToDB(const QList<Anime> &mediaList) {
    QList<QHash<QString, QVariant>> mediaValuesList;
    QList<QHash<QString, QVariant>> myInfoValuesList;
    QList<QHash<QString, QVariant>> animeGenreList;
    QList<QHash<QString, QVariant>> studioList;
    QList<QHash<QString, QVariant>> animeStudioList;

    QSet<int> studioSet;
    QSet<QString> animeStudioSet;

    for (const Anime& anime : mediaList) {
        mediaValuesList.append(anime.asHash());
        myInfoValuesList.append(anime.myInfoAsHash());

        for (const QString& genre : anime.genres) {
            QHash<QString, QVariant> animeGenre {
                { "genreId", anime.getGenreIndex(genre) },
                { "animeId", anime.id }
            };
            animeGenreList.append(animeGenre);
        }

        for (const Anime::Studio& studio : anime.studios) {
            int studioKey = studio.studioId;
            QString animeStudioKey = QString("%1-%2").arg(anime.id).arg(studio.studioId);

            if (!studioSet.contains(studioKey)) {
                QHash<QString, QVariant> studioHash {
                    { "studioId", studio.studioId },
                    { "studioName", studio.studioName },
                    { "isMain", studio.isMain }
                };
                studioList.append(studioHash);
                studioSet.insert(studioKey);
            }

            if (!animeStudioSet.contains(animeStudioKey)) {
                QHash<QString, QVariant> animeStudio {
                    { "studioId", studio.studioId },
                    { "animeId", anime.id },
                    { "isMain", studio.isMain }
                };
                animeStudioList.append(animeStudio);
                animeStudioSet.insert(animeStudioKey);
            } else {
                qDebug() << "DUPLICATE FOUND" << animeStudioKey;
            }
        }
    }

    m_dbManager.executeSqlScript(toString(AppResourceKey::SQLPopulateTables));

    if ( !m_dbManager.bulkInsertIntoTable("Anime", mediaValuesList) ) {
        qDebug() << "Bulk insert failed.";
    }

    if ( !m_dbManager.bulkInsertIntoTable("Entry", myInfoValuesList) ) {
        qDebug() << "Bulk insert failed.";
    }

    if ( !m_dbManager.bulkInsertIntoTable("AnimeGenre", animeGenreList) ) {
        qDebug() << "Bulk insert failed.";
    }

    if ( !m_dbManager.bulkInsertIntoTable("Studio", studioList) ) {
        qDebug() << "Bulk insert failed.";
    }

    if ( !m_dbManager.bulkInsertIntoTable("AnimeStudio", animeStudioList) ) {
        qDebug() << "Bulk insert failed.";
    }
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

    QSqlTableModel animeStudioTable;
    animeStudioTable.setTable("Studio");
    animeStudioTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    if (!animeTable.select()) {
        qDebug() << "COULD NOT SELECT ANIME TABLE";
        return false;
    }

    // Loop over mediaList
    for (const Anime& anime : mediaList) {

        // Set filter to find the corresponding entry in the Anime table
        animeTable.setFilter(QString("id = %1").arg(anime.id));
        animeTable.select();  // Re-select to apply the filter

        entryTable.setFilter(QString("mediaId = %1").arg(anime.id));
        entryTable.select();

        if (animeTable.rowCount() > 0) {
            QSqlRecord animeRecord = animeTable.record(0);  // Get the first (and only) record

            // Compare anilistModified values
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
            // Handle case where the entry is not found in the table
            qDebug() << "Anime ID not found in the table:" << anime.id;
        }

        for (const QString& genre : anime.genres) {
            QString animeGenreFilter = QString("animeId = %1 AND genreId = %2").arg(anime.id).arg(anime.getGenreIndex(genre));

            animeGenreTable.setFilter(animeGenreFilter);

            animeGenreTable.select();

            if (animeGenreTable.rowCount() == 0) {
                insertAnimeGenre(animeGenreTable, anime, genre);
            }
        }

        if (entryTable.rowCount() > 0) {
            QSqlRecord entryRecord = entryTable.record(0);

            int dbAniListEntryModified = entryRecord.value("anilistModified").toInt();

            if(anime.myInfo.anilistModified > dbAniListEntryModified && anime.id == 20666) {
                qDebug() << "UPATING ENTRY: " << " " << entryRecord.value("id") << " " << anime.titleRomaji;

                auto entryInfo = anime.myInfoAsHash();

                for (auto it = entryInfo.cbegin(); it != entryInfo.cend(); ++it) {
                    entryRecord.setValue(it.key(), it.value());
                }

                entryTable.setRecord(0, entryRecord);

                if (!entryTable.submitAll()) {
                    qDebug() << "Failed to update entry:" << entryTable.lastError();
                }

                else {
                    qDebug() << "Updated entry for Entry ID:" << anime.id;
                }
            }
        }

        for (const Anime::Studio& studio : anime.studios) {
            studioTable.setFilter( QString("studioId = %1").arg(studio.studioId) );
            studioTable.select();

            if (studioTable.rowCount() == 0 ) {
                QSqlRecord newStudioRecord;

                newStudioRecord.setValue("studioId", studio.studioId);
                newStudioRecord.setValue("studioName", studio.studioName);

                studioTable.setRecord(0, newStudioRecord);

                if (!studioTable.submitAll()) {
                    qDebug() << "Failed to update Studio:" << studioTable.lastError();
                }

                else {
                    qDebug() << "Updated entry for Studio ID:" << studio.studioId << " " << studio.studioName;
                }
            }
        }
    }

    // Reset filter after the operation (necessary?)
    animeTable.setFilter("");
    animeTable.select();

    entryTable.setFilter("");
    entryTable.select();

    return true;
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
