-- Trigger for Anime table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_Anime_Modified
AFTER INSERT ON Anime
BEGIN
    UPDATE Anime SET modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Anime table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_Anime_Modified
AFTER UPDATE ON Anime
BEGIN
    UPDATE Anime SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for AnimeGenre table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_AnimeGenre_Modified
AFTER INSERT ON AnimeGenre
BEGIN
    UPDATE AnimeGenre SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for AnimeGenre table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_AnimeGenre_Modified
AFTER UPDATE ON AnimeGenre
BEGIN
    UPDATE AnimeGenre SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Entry table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_Entry_Modified
AFTER INSERT ON Entry
BEGIN
    UPDATE Entry SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Entry table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_Entry_Modified
AFTER UPDATE ON Entry
BEGIN
    UPDATE Entry SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for EntryStatus table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_EntryStatus_Modified
AFTER INSERT ON EntryStatus
BEGIN
    UPDATE EntryStatus SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for EntryStatus table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_EntryStatus_Modified
AFTER UPDATE ON EntryStatus
BEGIN
    UPDATE EntryStatus SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Genre table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_Genre_Modified
AFTER INSERT ON Genre
BEGIN
    UPDATE Genre SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Genre table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_Genre_Modified
AFTER UPDATE ON Genre
BEGIN
    UPDATE Genre SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for MediaFormat table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_MediaFormat_Modified
AFTER INSERT ON MediaFormat
BEGIN
    UPDATE MediaFormat SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for MediaFormat table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_MediaFormat_Modified
AFTER UPDATE ON MediaFormat
BEGIN
    UPDATE MediaFormat SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for MediaStatus table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_MediaStatus_Modified
AFTER INSERT ON MediaStatus
BEGIN
    UPDATE MediaStatus SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for MediaStatus table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_MediaStatus_Modified
AFTER UPDATE ON MediaStatus
BEGIN
    UPDATE MediaStatus SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Studio table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_Studio_Modified
AFTER INSERT ON Studio
BEGIN
    UPDATE Studio SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Studio table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_Studio_Modified
AFTER UPDATE ON Studio
BEGIN
    UPDATE Studio SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for AnimeStudio table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_AnimeStudio_Modified
AFTER INSERT ON AnimeStudio
BEGIN
    UPDATE AnimeStudio SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for AnimeStudio table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_AnimeStudio_Modified
AFTER UPDATE ON AnimeStudio
BEGIN
    UPDATE AnimeStudio SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Season table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_Season_Modified
AFTER INSERT ON Season
BEGIN
    UPDATE Season SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for Season table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_Season_Modified
AFTER UPDATE ON Season
BEGIN
    UPDATE Season SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for AnimeSeason table (AFTER INSERT)
CREATE TRIGGER IF NOT EXISTS before_insert_AnimeSeason_Modified
AFTER INSERT ON AnimeSeason
BEGIN
    UPDATE AnimeSeason SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;

-- Trigger for AnimeSeason table (after update)
CREATE TRIGGER IF NOT EXISTS after_update_AnimeSeason_Modified
AFTER UPDATE ON AnimeSeason
BEGIN
    UPDATE AnimeSeason SET Modified = strftime('%s', 'now') WHERE rowid = NEW.rowid;
END;