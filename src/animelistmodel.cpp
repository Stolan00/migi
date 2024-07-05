#include "assets/animelistmodel.h"
#include "anime.h"
// --------------------------------------------------------------------------------------------------------------------------
void AnimeListModel::createTable()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    // Check if the view exists
    QSqlQuery query(db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='view' AND name='AnimeListView'");
    if (!query.exec()) {
        qDebug() << "Failed to check for view: " << query.lastError().text();
        return;
    }

    if (query.next()) {
        qDebug() << "VIEW EXISTS";
        return;
    }

    if (!query.exec(
            R"(
            CREATE VIEW IF NOT EXISTS AnimeListView AS
            SELECT
                a.id AS anime_id,
                a.titleEnglish,
                a.titleRomaji,
                COALESCE(a.titleEnglish, a.titleRomaji) as sortTitle,
                ans.synonyms,
                e.progress,
                a.episodes,
                e.score,
                es.statusName AS status,
                es.statusId,
                mf.formatName AS type,
                s.seasonName AS season,
                ans_season.seasonYear,
                e.id AS entryId
            FROM
                Anime a
            JOIN
                Entry e ON a.id = e.mediaId
            JOIN
                MediaFormat mf ON a.mediaFormat = mf.formatId
            JOIN
                EntryStatus es ON e.status = es.statusId
            JOIN
                (SELECT animeId, GROUP_CONCAT(synonym, CHAR(31)) AS synonyms
                 FROM AnimeSynonym
                 GROUP BY animeId) ans ON ans.animeId = a.id
            JOIN
                AnimeSeason ans_season ON ans_season.animeId = a.id
            JOIN
                Season s ON s.seasonId = ans_season.seasonId
            GROUP BY
                a.id, a.titleEnglish, a.titleRomaji, e.progress, e.score,
                es.statusName, es.statusId, mf.formatName, s.seasonName, ans_season.seasonYear;
            )"
            )) {
        qDebug() << "QUERY FAILED: " << query.lastError().text();
        qFatal("Failed to create view AnimeListView: %s", qPrintable(query.lastError().text()));
    } else {
        qDebug() << "View AnimeListView created successfully";
    }

    // Create trigger if it does not exist
    if (!query.exec(
            R"(
        CREATE TRIGGER IF NOT EXISTS update_animeListView INSTEAD OF UPDATE ON animeListView
        BEGIN
            -- Update progress
            UPDATE Entry
            SET progress = NEW.progress
            WHERE mediaId = NEW.anime_id AND OLD.progress != NEW.progress;

            -- Update score
            UPDATE Entry
            SET score = NEW.score
            WHERE mediaId = NEW.anime_id AND OLD.score != NEW.score;
        END;
        )"
            )) {
        qDebug() << "TRIGGER QUERY FAILED: " << query.lastError().text();
        qFatal("Failed to create trigger update_animeListView: %s", qPrintable(query.lastError().text()));
    } else {
        qDebug() << "Trigger update_animeListView created successfully";
    }
}

// --------------------------------------------------------------------------------------------------------------------------
AnimeListModel::AnimeListModel(QObject *parent)
    : QSqlTableModel(parent)
{
    createTable();
    setTable("AnimeListView");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
    qDebug() << "Number of rows selected: " << rowCount();
    qDebug() << "ALM CREATED";
}
// --------------------------------------------------------------------------------------------------------------------------
QHash<int, QByteArray> AnimeListModel::roleNames() const  {
    QHash<int, QByteArray> roles;
    roles[AnimeIdRole] = "anime_id";
    roles[TitleEnglishRole] = "titleEnglish";
    roles[TitleRomajiRole] = "titleRomaji";
    roles[ProgressRole] = "progress";
    roles[ScoreRole] = "score";
    roles[FormatRole] = "type";
    roles[StatusRole] = "status";
    roles[SortTitleRole] = "sortTitle";
    roles[SeasonRole] = "season";
    roles[SeasonYearRole] = "seasonYear";
    roles[SynonymsRole] = "synonyms";
    roles[EpisodesRole] = "episodes";
    roles [EntryIdRole] = "entryId";

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
        return record.value("type");
    else if (role == StatusRole)
        return record.value("status");
    else if (role == SortTitleRole)
        return record.value("sortTitle");
    else if (role == SeasonRole)
        return record.value("season");
    else if (role == SeasonYearRole)
        return record.value("seasonYear");
    else if (role == SynonymsRole)
        return record.value("synonyms");
    else if (role == EpisodesRole)
        return record.value("episodes");
    else if (role == EntryIdRole)
        return record.value("entryId");
    return QVariant();
}
// --------------------------------------------------------------------------------------------------------------------------
// TODO: TRIGGER ANILIST MUTATION SO CHANGES ARE REFLECTED ON ACCOUNT
bool AnimeListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;

    QString columnName = roleNames().value(role);
    if (columnName.isEmpty())
        return false;

    // Get the anime_id for the current row
    int animeId = data(index.sibling(index.row(), 0), AnimeIdRole).toInt();

    QSqlQuery query;
    query.prepare(QString("UPDATE animeListView SET %1 = :value WHERE anime_id = :id").arg(columnName));
    query.bindValue(":value", value);
    query.bindValue(":id", animeId);

    qDebug() << "Executing query:" << query.lastQuery();
    qDebug() << "Anime ID:" << animeId << "New value:" << value;

    if (!query.exec()) {
        qWarning() << "Failed to update database:" << query.lastError();
        return false;
    }

    // Update the internal data
    if (QSqlTableModel::setData(index, value, role)) {
        // Emit dataChanged for the entire row
        emit dataChanged(index.sibling(index.row(), 0), index.sibling(index.row(), columnCount() - 1), QVector<int>() << role << Qt::DisplayRole);
        return true;
    }
    return false;
}
// --------------------------------------------------------------------------------------------------------------------------
void AnimeListModel::forceUpdate() {
    beginResetModel();
    endResetModel();
}
// --------------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags AnimeListModel::flags(const QModelIndex &index) const {
    Q_UNUSED(index)
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
// --------------------------------------------------------------------------------------------------------------------------
void AnimeListModel::setStatusFilter(int statusId) {

    beginResetModel();

    setFilter(QString("statusId = %1").arg(statusId));

    setSort(SortTitleRole - Qt::UserRole, Qt::AscendingOrder);

    endResetModel();

    emit layoutChanged();

    select();
}
// --------------------------------------------------------------------------------------------------------------------------
