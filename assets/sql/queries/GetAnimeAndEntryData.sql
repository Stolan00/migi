SELECT * FROM Entry
JOIN Anime ON Entry.mediaId = Anime.id
ORDER BY Anime.titleRomaji ASC