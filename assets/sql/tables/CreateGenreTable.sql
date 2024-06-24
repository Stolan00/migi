CREATE TABLE IF NOT EXISTS Genre (
    genreId INTEGER PRIMARY KEY,
    genreName TEXT UNIQUE,
    modified INTEGER
);
