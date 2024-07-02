CREATE TABLE IF NOT EXISTS AnimeSeason (
    animeId  INTEGER,
    seasonId INTEGER,
    seasonYear INTEGER,
    modified INTEGER,
    PRIMARY KEY (seasonId, animeId),
    FOREIGN KEY (seasonId) REFERENCES Season(id),
    FOREIGN KEY (animeId) REFERENCES Anime(id)
);
