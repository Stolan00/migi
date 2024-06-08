CREATE TABLE IF NOT EXISTS Anime (
    id INTEGER PRIMARY KEY, 
    titleRomaji TEXT, 
    titleEnglish TEXT, 
    titleNative TEXT, 
    synopsis TEXT, 
    imageLink TEXT, 
    episodes INTEGER,
    mediaStatus INTEGER,
    FOREIGN KEY (mediaStatus) REFERENCES MediaStatus(id)
);
