#include "assets/animedetailmodel.h".h"
// --------------------------------------------------------------------------------------------------------------------------
void AnimeDetailModel::createTable()
{
    // QSqlDatabase db = QSqlDatabase::database();
    // if (!db.isOpen()) {
    //     qDebug() << "Database is not open!";
    //     return;
    // }

    // // Check if the view exists
    // QSqlQuery query(db);
    // query.prepare("SELECT name FROM sqlite_master WHERE type='view' AND name='AnimeListView'");
    // if (!query.exec()) {
    //     qDebug() << "Failed to check for view: " << query.lastError().text();
    //     return;
    // }

    // if (query.next()) {
    //     qDebug() << "VIEW EXISTS";
    //     return;
    // }

    // qDebug() << "DIDNT RETURN";
    // if (!query.exec(
    //         R"(
    //         CREATE VIEW IF NOT EXISTS AnimeListView AS
    //         SELECT
    //             a.id AS anime_id,
    //             a.titleEnglish,
    //             a.titleRomaji,
    //             e.progress,
    //             e.score,
    //             e.status,
    //             mf.formatName as type,
    //             COALESCE(a.titleEnglish, a.titleRomaji) as sortTitle
    //         FROM
    //             Anime a
    //         JOIN
    //             Entry e ON a.id = e.mediaId
    //         JOIN
    //             MediaFormat mf ON a.mediaFormat = mf.formatId
    //         JOIN
    //             EntryStatus es ON e.status = es.statusId
    //         )"
    //         )) {
    //     qDebug() << "QUERY FAILED: " << query.lastError().text();
    //     qFatal("Failed to create view AnimeListView: %s", qPrintable(query.lastError().text()));
    // } else {
    //     qDebug() << "View AnimeListView created successfully";
    // }

    // // Create trigger if it does not exist
    // if (!query.exec(
    //         R"(
    //     CREATE TRIGGER IF NOT EXISTS update_animeListView INSTEAD OF UPDATE ON animeListView
    //     BEGIN
    //         -- Update progress
    //         UPDATE Entry
    //         SET progress = NEW.progress
    //         WHERE mediaId = NEW.anime_id AND OLD.progress != NEW.progress;

    //         -- Update score
    //         UPDATE Entry
    //         SET score = NEW.score
    //         WHERE mediaId = NEW.anime_id AND OLD.score != NEW.score;
    //     END;
    //     )"
    //         )) {
    //     qDebug() << "TRIGGER QUERY FAILED: " << query.lastError().text();
    //     qFatal("Failed to create trigger update_animeListView: %s", qPrintable(query.lastError().text()));
    // } else {
    //     qDebug() << "Trigger update_animeListView created successfully";
    // }
}

// --------------------------------------------------------------------------------------------------------------------------
AnimeDetailModel::AnimeDetailModel(QObject *parent)
    : QSqlTableModel(parent)
{
    // createTable();
    // setTable("AnimeListView");
    // setEditStrategy(QSqlTableModel::OnManualSubmit);
    // select();
    // qDebug() << "ALM CREATED";
}
// --------------------------------------------------------------------------------------------------------------------------
QHash<int, QByteArray> AnimeDetailModel::roleNames() const  {
    // QHash<int, QByteArray> roles;
    // roles[AnimeIdRole] = "anime_id";
    // roles[TitleEnglishRole] = "titleEnglish";
    // roles[TitleRomajiRole] = "titleRomaji";
    // roles[UserProgressRole] = "progress";
    // roles[ScoreRole] = "score";
    // roles[FormatRole] = "format";
    // roles[StatusRole] = "status";
    // roles[SortTitleRole] = "sortTitle";

    // return roles;
}
// --------------------------------------------------------------------------------------------------------------------------
QVariant AnimeDetailModel::data(const QModelIndex &index, int role) const {
    // if (!index.isValid())
    //     return QVariant();

    // if (role < Qt::UserRole)
    //     return QSqlTableModel::data(index, role);

    // const QSqlRecord record = this->record(index.row());
    // if (role == AnimeIdRole)
    //     return record.value("anime_id");
    // else if (role == TitleEnglishRole)
    //     return record.value("titleEnglish");
    // else if (role == TitleRomajiRole)
    //     return record.value("titleRomaji");
    // else if (role == UserProgressRole)
    //     return record.value("progress");
    // else if (role == ScoreRole)
    //     return record.value("score");
    // else if (role == FormatRole)
    //     return record.value("format");
    // else if (role == StatusRole)
    //     return record.value("status");
    // else if (role == SortTitleRole)
    //     return record.value("sortTitle");

    // return QVariant();
}
// --------------------------------------------------------------------------------------------------------------------------
void AnimeDetailModel::setAnimeIdFilter(int statusId) {
    // setFilter(QString("status = %1").arg(statusId));
    // setSort(SortTitleRole - Qt::UserRole, Qt::AscendingOrder);
    // select();
}
// --------------------------------------------------------------------------------------------------------------------------
