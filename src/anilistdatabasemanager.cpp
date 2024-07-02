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

    QSqlTableModel animeStudioTable;
    animeStudioTable.setTable("AnimeStudio");
    animeStudioTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    QSqlTableModel animeSynonymTable;
    animeSynonymTable.setTable("AnimeSynonym");
    animeSynonymTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    QSqlTableModel animeSeasonTable;
    animeSeasonTable.setTable("AnimeSeason");
    animeSeasonTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

    m_dbManager.executeSqlScript(toString(AppResourceKey::SQLPopulateTables));

    if (!animeTable.select() || !entryTable.select() || !animeGenreTable.select() || !studioTable.select()) {
        qDebug() << "Failed to select one or more tables.";
        return;
    }

    if (!m_db.transaction()) {
        qDebug() << "Failed to start transaction:" << m_db.lastError();
        return;
    }

    try {
        for (const Anime& anime : mediaList) {
            insertAnime(animeTable, anime);

            insertEntry(entryTable, anime);

            insertAnimeSeason(animeSeasonTable, anime);

            for (const QString& genre : anime.genres) {
                insertAnimeGenre(animeGenreTable, anime, genre);
            }

            for (const QString& synonym : anime.synonyms) {
                insertAnimeSynonym(animeSynonymTable, anime, synonym);
            }

            for (const Anime::Studio& studio : anime.studios) {
                insertStudio(studioTable, studio);

                insertAnimeStudio(animeStudioTable, anime.id, studio);
            }
        }

        if (!animeTable.submitAll()) {
            qDebug() << "Failed to submit all Anime records:" << animeTable.lastError();
            throw std::runtime_error("Failed to submit Anime records");
        }
        if (!entryTable.submitAll()) {
            qDebug() << "Failed to submit all Entry records:" << entryTable.lastError();
            throw std::runtime_error("Failed to submit Entry records");
        }
        if (!animeGenreTable.submitAll()) {
            qDebug() << "Failed to submit all AnimeGenre records:" << animeGenreTable.lastError();
            throw std::runtime_error("Failed to submit AnimeGenre records");
        }
        if (!studioTable.submitAll()) {
            qDebug() << "Failed to submit all Studio records:" << studioTable.lastError();
            throw std::runtime_error("Failed to submit Studio records");
        }
        if (!animeSeasonTable.submitAll()) {
            qDebug() << "Failed to submit all Season records:" << animeSeasonTable.lastError();
            throw std::runtime_error("Failed to submit Season records");
        }
        if (!animeSynonymTable.submitAll()) {
            qDebug() << "Failed to submit all synonym records:" << animeSynonymTable.lastError();
            throw std::runtime_error("Failed to submit synonym records");
        }


        if (!m_db.commit()) {
            qDebug() << "Failed to commit transaction:" << m_db.lastError();
        } else {
            qDebug() << "Transaction committed successfully.";
        }
    } catch (const std::runtime_error& e) {
        qDebug() << "Error occurred, rolling back transaction:" << e.what();
        m_db.rollback();
    }

    qDebug() << "DONE";
}
// --------------------------------------------------------------------------------------------------------------------------
bool AnilistDatabaseManager::updateDatabase(const QList<Anime>& mediaList) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        qDebug() << "Failed to start transaction:" << db.lastError();
        return false;
    }

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

    try {
        for (const Anime& anime : mediaList) {
            // Update anime record if modified
            updateAnime(animeTable, anime);

            // Update entry record if modified
            updateEntry(entryTable, anime);

            // Update anime genres
            updateAnimeGenres(animeGenreTable, anime);

            // Update studios
            updateStudios(studioTable, anime);
        }

        // Commit transaction
        if (!db.commit()) {
            qDebug() << "Failed to commit transaction:" << db.lastError();
            return false;
        } else {
            qDebug() << "Transaction committed successfully.";
        }
    } catch (const std::runtime_error& e) {
        qDebug() << "Error occurred, rolling back transaction:" << e.what();
        db.rollback();
        return false;
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
        throw std::runtime_error("Database insertion error");
    } else {
        //qDebug() << "Inserted record for Anime ID:" << anime.id;
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
        throw std::runtime_error("Database insertion error");
    } else {
        //qDebug() << "Inserted record for Entry ID:" << anime.id;
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertStudio(QSqlTableModel &studioTable, const Anime::Studio &studio) {
    // Set filter to check if the studio already exists
    studioTable.setFilter(QString("studioId = %1").arg(studio.studioId));
    if (!studioTable.select()) {
        qDebug() << "Failed to select Studio table with filter for studioId:" << studio.studioId << studioTable.lastError();
        return;
    }

    //qDebug() << "Row count after select with filter for studioId:" << studio.studioId << studioTable.rowCount();

    if (studioTable.rowCount() == 0) {
        // Studio does not exist, proceed with insertion
        QSqlRecord newStudioRecord = studioTable.record();
        newStudioRecord.setValue("studioId", studio.studioId);
        newStudioRecord.setValue("studioName", studio.studioName);

        if (!studioTable.insertRecord(-1, newStudioRecord)) {
            qDebug() << "Failed to insert Studio record:" << studioTable.lastError();
            throw std::runtime_error("Database insertion error");
        } else {
            //qDebug() << "Inserted record for Studio ID:" << studio.studioId << studio.studioName;
        }

        // Submit changes immediately after insertion
        if (!studioTable.submitAll()) {
            qDebug() << "Failed to submit changes to Studio table:" << studioTable.lastError();
            throw std::runtime_error("Database submission error");
        }
    } else {
        //qDebug() << "Studio already exists for studioId:" << studio.studioId;
    }

    // Clear the filter and refresh table state
    studioTable.setFilter("");
    if (!studioTable.select()) {
        qDebug() << "Failed to select Studio table after clearing filter:" << studioTable.lastError();
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertAnimeStudio(QSqlTableModel &animeStudioTable, int animeId, const Anime::Studio &studio) {
    // Set filter to check if the anime-studio relationship already exists
    animeStudioTable.setFilter(QString("animeId = %1 AND studioId = %2")
                                   .arg(animeId)
                                   .arg(studio.studioId));

    if (!animeStudioTable.select()) {
       // qDebug() << "Failed to select AnimeStudio table with filter for animeId:" << animeId << "and studioId:" << studio.studioId << animeStudioTable.lastError();
        return;
    }

  //  qDebug() << "Row count after select with filter for animeId:" << animeId << "and studioId:" << studio.studioId << animeStudioTable.rowCount();

    if (animeStudioTable.rowCount() == 0) {
        // Anime-Studio relationship does not exist, proceed with insertion
        QSqlRecord newAnimeStudioRecord = animeStudioTable.record();
        newAnimeStudioRecord.setValue("animeId", animeId);
        newAnimeStudioRecord.setValue("studioId", studio.studioId);
        newAnimeStudioRecord.setValue("isMain", studio.isMain);

        if (!animeStudioTable.insertRecord(-1, newAnimeStudioRecord)) {
           // qDebug() << "Failed to insert AnimeStudio record:" << animeStudioTable.lastError();
            throw std::runtime_error("Database insertion error");
        } else {
           // qDebug() << "Inserted record for AnimeStudio with animeId:" << animeId << "and studioId:" << studio.studioId;
        }

        // Submit changes immediately after insertion
        if (!animeStudioTable.submitAll()) {
          //  qDebug() << "Failed to submit changes to AnimeStudio table:" << animeStudioTable.lastError();
            throw std::runtime_error("Database submission error");
        }
    } else {
      //  qDebug() << "AnimeStudio relationship already exists for animeId:" << animeId << "and studioId:" << studio.studioId;
    }

    // Clear the filter and refresh table state
    animeStudioTable.setFilter("");
    if (!animeStudioTable.select()) {
       // qDebug() << "Failed to select AnimeStudio table after clearing filter:" << animeStudioTable.lastError();
    }
}
// TODO: a lot of these are very similar--use template instead?
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertAnimeSeason(QSqlTableModel& animeSeasonTable, const Anime &anime) {
    QSqlRecord newAnimeSeasonRecord = animeSeasonTable.record();
    newAnimeSeasonRecord.setValue("seasonId",  static_cast<int>(anime.season));
    newAnimeSeasonRecord.setValue("seasonYear", anime.seasonYear);
    newAnimeSeasonRecord.setValue("animeId", anime.id);

    if (!animeSeasonTable.insertRecord(-1, newAnimeSeasonRecord)) {
        qDebug() << "Failed to insert record:" << animeSeasonTable.lastError().text();
        throw std::runtime_error("Database insertion error");
    } else {
        //qDebug() << "Inserted record for Season: for Anime ID:" << anime.id;
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertAnimeGenre(QSqlTableModel& animeGenreTable, const Anime& anime, const QString& genre) {
    QSqlRecord newAnimeGenreRecord = animeGenreTable.record();
    newAnimeGenreRecord.setValue("genreId", anime.getGenreIndex(genre));
    newAnimeGenreRecord.setValue("animeId", anime.id);

    if (!animeGenreTable.insertRecord(-1, newAnimeGenreRecord)) {
        qDebug() << "Failed to insert record:" << animeGenreTable.lastError().text();
        throw std::runtime_error("Database insertion error");
    } else {
        //qDebug() << "Inserted record for Genre ID:" << anime.getGenreIndex(genre) << " for Anime ID:" << anime.id;
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void AnilistDatabaseManager::insertAnimeSynonym(QSqlTableModel& animeSynonymTable, const Anime& anime, const QString &synonym) {
    // Create a filter to check for existing records with the same animeId and synonym
    QString filter = QString("animeId = %1 AND synonym = '%2'")
                         .arg(anime.id)
                         .arg(synonym);

    // Set the filter on the table model
    animeSynonymTable.setFilter(filter);
    animeSynonymTable.select();

    // Check if any records exist with the same animeId and synonym
    if (animeSynonymTable.rowCount() > 0) {
        // Debug statement for when a duplicate is found
        //qDebug() << "Duplicate synonym found for Anime ID:" << anime.id << "Synonym:" << synonym;
    } else {
        // Clear the filter before inserting a new record
        animeSynonymTable.setFilter("");

        // Create a new record
        QSqlRecord newAnimeSynonymRecord = animeSynonymTable.record();
        newAnimeSynonymRecord.setValue("animeId", anime.id);
        newAnimeSynonymRecord.setValue("synonym", synonym);

        // Insert the new record
        if (!animeSynonymTable.insertRecord(-1, newAnimeSynonymRecord)) {
            qDebug() << "Failed to insert record:" << animeSynonymTable.lastError().text();
            throw std::runtime_error("Database insertion error");
        } else {
            qDebug() << "Inserted record for Synonym:" << synonym << " for Anime ID:" << anime.id;
        }
    }

    if (!animeSynonymTable.submitAll()) {
        //  qDebug() << "Failed to submit changes to AnimeStudio table:" << animeStudioTable.lastError();
        throw std::runtime_error("Database submission error");
    }
    // Clear the filter after operation
    animeSynonymTable.setFilter("");
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

            // Update AnimeStudio table for each studio associated with the anime
            QSqlTableModel animeStudioTable;
            animeStudioTable.setTable("AnimeStudio");
            animeStudioTable.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);

            for (const Anime::Studio& studio : anime.studios) {
                updateAnimeStudio(animeStudioTable, anime.id, studio);
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
void AnilistDatabaseManager::updateAnimeStudio(QSqlTableModel &animeStudioTable, int animeId, const Anime::Studio &studio) {
    QString filter = QString("animeId = %1 AND studioId = %2").arg(animeId).arg(studio.studioId);
    animeStudioTable.setFilter(filter);
    animeStudioTable.select();

    QSqlRecord studioRecord;
    bool isUpdateNeeded = false;

    if (animeStudioTable.rowCount() > 0) {
        studioRecord = animeStudioTable.record(0);

        // Check if values are different before updating
        if (studioRecord.value("isMain").toInt() != studio.isMain) {
            studioRecord.setValue("isMain", studio.isMain);
            isUpdateNeeded = true;
        }
    } else {
        // No existing record, insert new one
        studioRecord = animeStudioTable.record();
        studioRecord.setValue("animeId", animeId);
        studioRecord.setValue("studioId", studio.studioId);
        studioRecord.setValue("isMain", studio.isMain);
        isUpdateNeeded = true;  // Need to insert new record
    }

    // Perform the update or insert
    if (isUpdateNeeded) {
        animeStudioTable.setRecord(0, studioRecord);

        if (!animeStudioTable.submitAll()) {
            qDebug() << "Failed to update AnimeStudio:" << animeStudioTable.lastError();
        } else {
            if (animeStudioTable.rowCount() > 0) {
                qDebug() << "Updated AnimeStudio for Anime ID:" << animeId << " and Studio ID:" << studio.studioId;
            } else {
                qDebug() << "Inserted new AnimeStudio for Anime ID:" << animeId << " and Studio ID:" << studio.studioId;
            }
        }
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
