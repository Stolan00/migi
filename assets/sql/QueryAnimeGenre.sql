SELECT 
    Anime.id AS animeId,
    Anime.titleEnglish,
    Anime.titleRomaji,
    Genre.genreId,
    Genre.genreName
FROM 
    Anime
JOIN 
    AnimeGenre ON Anime.id = AnimeGenre.animeId
JOIN 
    Genre ON AnimeGenre.genreId = Genre.genreId
ORDER BY Anime.id ASC, titleEnglish ASC, titleRomaji ASC, Genre.genreId ASC;