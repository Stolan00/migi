CREATE TRIGGER update_modified_timestamp
AFTER UPDATE ON Entry
FOR EACH ROW
BEGIN
    UPDATE Entry SET modified = strftime('%s', 'now') WHERE id = NEW.id;
END;
