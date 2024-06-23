#include "assets/animelistmodel.h"
// --------------------------------------------------------------------------------------------------------------------------
static void createTable()
{
    if (QSqlDatabase::database().tables().contains(QStringLiteral("AnimeListView"))) {
        // The table already exists; we don't need to do anything.
        return;
    }

    QSqlQuery query;
    if (!query.exec(
            R"(
            CREATE VIEW IF NOT EXISTS AnimeListView AS
            SELECT
                a.id AS anime_id,
                a.titleEnglish,
                a.titleRomaji,
                e.progress,
                e.score,
                e.status,
                mf.formatName as type
            FROM
                Anime a
            JOIN
                Entry e ON a.id = e.mediaId
            JOIN
                MediaFormat mf on a.mediaFormat = mf.formatId
            JOIN
                EntryStatus es on e.status = es.statusId
            )"
        )) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }

    query.exec(
        R"(
        CREATE TRIGGER update_animeListView INSTEAD OF UPDATE ON animeListView
        BEGIN
            -- Update progress
            UPDATE Entry
            SET progress = NEW.progress
            WHERE mediaId = NEW.anime_id AND OLD.progress != NEW.progress;

            -- Update score
            UPDATE Entry
            SET score = NEW.score
            WHERE mediaId = NEW.anime_id AND OLD.score != NEW.score;
        END;)
        )"
    );
}
// --------------------------------------------------------------------------------------------------------------------------
AnimeListModel::AnimeListModel(QObject *parent)
    : QSqlTableModel(parent)
{
    createTable();
    setTable("animeListView");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
}
// --------------------------------------------------------------------------------------------------------------------------
QHash<int, QByteArray> AnimeListModel::roleNames() const  {
    QHash<int, QByteArray> roles;
    roles[AnimeIdRole] = "anime_id";
    roles[TitleEnglishRole] = "titleEnglish";
    roles[TitleRomajiRole] = "titleRomaji";
    roles[ProgressRole] = "progress";
    roles[ScoreRole] = "score";
    roles[FormatRole] = "format";
    return roles;
}
// --------------------------------------------------------------------------------------------------------------------------
QVariant AnimeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role < Qt::UserRole)
        return QSqlTableModel::data(index, role);

    const QSqlRecord record = this->record(index.row());
    if (role == AnimeIdRole)
        return record.value("anime_id");
    else if (role == TitleEnglishRole)
        return record.value("titleEnglish");
    else if (role == TitleRomajiRole)
        return record.value("titleRomaji");
    else if (role == ProgressRole)
        return record.value("progress");
    else if (role == ScoreRole)
        return record.value("score");
    else if (role == FormatRole)
        return record.value("format");

    return QVariant();
}
// --------------------------------------------------------------------------------------------------------------------------
void AnimeListModel::setStatusFilter(int statusId) {
    setFilter(QString("status = %1").arg(statusId));
    select();
}
// --------------------------------------------------------------------------------------------------------------------------
