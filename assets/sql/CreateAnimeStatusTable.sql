CREATE TABLE IF NOT EXISTS AnimeStatus (
    statusId INTEGER,
    animeId INTEGER,
    modified INTEGER,
    PRIMARY KEY (statusId, animeId),
    FOREIGN KEY (statusId) REFERENCES AnimeStatus(id),
    FOREIGN KEY (animeId) REFERENCES Anime(id)
);
