
CREATE OR REPLACE FUNCTION insert_into_inside_function_test() RETURNS VOID AS
$$
DECLARE
    entry entries%rowtype;
    singleSynonymEntry entries%rowtype;
    englishSynonyms text [];
    englishSynonym text;
    englishAttributes text [];
    englishAttribute text;
    germanAttributes text [];
    index int;
    currentRowNumber int;
BEGIN
    DELETE FROM no_English_synonym_entries;
    currentRowNumber := 0;
    FOR entry IN
        SELECT * FROM entries
    LOOP
        if mod(currentRowNumber, 1000) = 0 then
          RAISE notice 'current row #:% %', currentRowNumber, entry.German;
        end if;
        englishAttributes := tokenize_by_pipes(entry.English);
	--FOREACH englishAttribute IN ARRAY englishAttributes
	--LOOP
    	   INSERT INTO no_English_synonym_entries VALUES('germanAttributes', 'englishSynonym');
	--END LOOP;
        currentRowNumber := currentRowNumber + 1;
    END LOOP;
END
$$
LANGUAGE plpgsql;

select insert_into_inside_function_test();
