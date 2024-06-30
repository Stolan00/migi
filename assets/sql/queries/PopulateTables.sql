INSERT INTO EntryStatus (statusId, statusName, modified) VALUES (0, 'CURRENT', 0);
INSERT INTO EntryStatus (statusId, statusName, modified) VALUES (1, 'PLANNING', 0);
INSERT INTO EntryStatus (statusId, statusName, modified) VALUES (2, 'COMPLETED', 0);
INSERT INTO EntryStatus (statusId, statusName, modified) VALUES (3, 'DROPPED', 0);
INSERT INTO EntryStatus (statusId, statusName, modified) VALUES (4, 'PAUSED', 0);
INSERT INTO EntryStatus (statusId, statusName, modified) VALUES (5, 'REPEATING', 0);

INSERT INTO MediaStatus (statusId, statusName, modified) VALUES (0, 'FINISHED', 0);
INSERT INTO MediaStatus (statusId, statusName, modified) VALUES (1, 'RELEASING', 0);
INSERT INTO MediaStatus (statusId, statusName, modified) VALUES (2, 'NOT_YET_RELEASED', 0);
INSERT INTO MediaStatus (statusId, statusName, modified) VALUES (3, 'CANCELLED', 0);
INSERT INTO MediaStatus (statusId, statusName, modified) VALUES (4, 'HIATUS', 0);

INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (0, 'TV', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (1, 'TV_SHORT', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (2, 'MOVIE', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (3, 'SPECIAL', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (4, 'OVA', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (5, 'ONA', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (6, 'MUSIC', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (7, 'MANGA', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (8, 'NOVEL', 0);
INSERT INTO MediaFormat (formatId, formatName, modified) VALUES (9, 'ONE_SHOT', 0);

INSERT INTO Genre (genreId, genreName, modified) VALUES (0, 'Action', 0);
INSERT INTO Genre (genreName) VALUES ('Adventure');
INSERT INTO Genre (genreName) VALUES ('Comedy');
INSERT INTO Genre (genreName) VALUES ('Drama');
INSERT INTO Genre (genreName) VALUES ('Ecchi');
INSERT INTO Genre (genreName) VALUES ('Fantasy');
INSERT INTO Genre (genreName) VALUES ('Hentai');
INSERT INTO Genre (genreName) VALUES ('Horror');
INSERT INTO Genre (genreName) VALUES ('Mahou Shoujo');
INSERT INTO Genre (genreName) VALUES ('Mecha');
INSERT INTO Genre (genreName) VALUES ('Music');
INSERT INTO Genre (genreName) VALUES ('Mystery');
INSERT INTO Genre (genreName) VALUES ('Psychological');
INSERT INTO Genre (genreName) VALUES ('Romance');
INSERT INTO Genre (genreName) VALUES ('Sci-Fi');
INSERT INTO Genre (genreName) VALUES ('Slice of Life');
INSERT INTO Genre (genreName) VALUES ('Sports');
INSERT INTO Genre (genreName) VALUES ('Supernatural');
INSERT INTO Genre (genreName) VALUES ('Thriller');

INSERT INTO Season (seasonId, seasonName, modified) VALUES (0, 'WINTER', 0);
INSERT INTO Season (seasonName) VALUES ('SPRING');
INSERT INTO Season (seasonName) VALUES ('SUMMER');
INSERT INTO Season (seasonName) VALUES ('FALL');