CREATE TABLE IF NOT EXISTS AnimeSynonym (
    synId INTEGER PRIMARY KEY,
    animeId INTEGER,
    synonymText TEXT,
    modified INTEGER,
    FOREIGN KEY (animeId) REFERENCES Anime(id)
);
