CREATE TABLE IF NOT EXISTS Entry (
    id INTEGER PRIMARY KEY,
    MediaId INTEGER,
    Status INTEGER,
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
    FOREIGN KEY (MediaId) REFERENCES Anime(id),
    FOREIGN KEY (Status) REFERENCES EntryStatus(id)
);
