SELECT * FROM Entry
JOIN Anime ON Entry.mediaId = Anime.id
-- WHERE Entry.status = {status}
ORDER BY Anime.titleEnglish ASC, titleRomaji ASC
