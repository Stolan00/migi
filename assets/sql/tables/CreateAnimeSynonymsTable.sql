CREATE TABLE IF NOT EXISTS AnimeSynonym (
    animeId INTEGER,
    synonym TEXT,
    PRIMARY KEY (animeId, synonym),
    FOREIGN KEY (animeId) REFERENCES Anime(id)
);