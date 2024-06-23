CREATE VIEW IF NOT EXISTS animeListView AS
SELECT 
    a.id AS anime_id, 
    a.titleEnglish,
	a.titleRomaji,
    e.progress,
	e.score,
    mf.formatName as type
FROM 
    Anime a
JOIN 
    Entry e ON a.id = e.mediaId
JOIN
	MediaFormat mf on a.mediaFormat = mf.formatId