CREATE TABLE IF NOT EXISTS Anime (
    id INTEGER PRIMARY KEY, 
    idMal INTEGER,
    titleRomaji TEXT, 
    titleEnglish TEXT, 
    titleNative TEXT, 
    synopsis TEXT, 
    imageLink TEXT, 
    episodes INTEGER,
    mediaStatus INTEGER,
    mediaFormat INTEGER,
    anilistModified INTEGER,
    modified INTEGER,
    FOREIGN KEY (mediaStatus) REFERENCES MediaStatus(id)
    FOREIGN KEY (mediaFormat) REFERENCES MediaFormat(id)
);
