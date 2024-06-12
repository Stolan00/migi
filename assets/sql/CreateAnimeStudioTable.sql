CREATE TABLE IF NOT EXISTS AnimeStudio (
    animeId INTEGER,
    studioId INTEGER,
    modified INTEGER,
    PRIMARY KEY (animeId, studioId),
    FOREIGN KEY (studioId) REFERENCES Studio(studioId),
    FOREIGN KEY (animeId) REFERENCES Anime(id)
);
