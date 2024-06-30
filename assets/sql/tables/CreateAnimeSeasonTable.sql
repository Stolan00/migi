CREATE TABLE IF NOT EXISTS AnimeSeason (
    seasonId INTEGER,
    seasonYear INTEGER,
    animeId  INTEGER,
    modified INTEGER,
    PRIMARY KEY (seasonId, animeId),
    FOREIGN KEY (seasonId) REFERENCES Season(id),
    FOREIGN KEY (animeId) REFERENCES Anime(id)
);
