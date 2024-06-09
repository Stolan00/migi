CREATE TABLE IF NOT EXISTS Entry (
    id INTEGER PRIMARY KEY,
    mediaId INTEGER,
    status INTEGER,
    completedDay INTEGER,
    completedMonth INTEGER,
    completedYear INTEGER,
    notes TEXT,
    private BOOLEAN,
    progress INTEGER,
    repeat INTEGER,
    score INTEGER,
    startedDay INTEGER,
    startedMonth INTEGER,
    startedYear INTEGER,
    modified INTEGER,
    FOREIGN KEY (mediaId) REFERENCES Anime(id),
    FOREIGN KEY (status) REFERENCES EntryStatus(id)
);
