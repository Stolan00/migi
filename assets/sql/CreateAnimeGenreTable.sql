CREATE TABLE IF NOT EXISTS AnimeGenre (
    genreId INTEGER,
    animeId INTEGER,
    modified INTEGER,
    PRIMARY KEY (genreId, animeId),
    FOREIGN KEY (genreId) REFERENCES Genre(id),
    FOREIGN KEY (animeId) REFERENCES Anime(id)
);
