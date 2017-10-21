-- Select English from entries;
-- separate_English_semicolons(English);

GRANT ALL PRIVILEGES ON TABLE entries TO postgres;
create table separate_English_semicolons(German text, English text);
GRANT ALL PRIVILEGES ON TABLE no_English_synonym_entries TO postgres;

-- from https://www.postgresql.org/docs/9.5/static/plpgsql-control-structures.html
CREATE OR REPLACE FUNCTION split_synonyms_into_() RETURNS VOID AS
-- RETURNS SETOF entries AS
$BODY$
DECLARE
    entry entries%rowtype;
    singleSynonymEntry entries%rowtype;
    englishSynonyms text [];
    englishSynonym text;
    englishAttributes text [];
    englishAttribute text;
    germanAttributes text [];
    -- firstTwoChars text;
    -- previousFirstTwoChars text;
    index int;
    currentRowNumber int;
BEGIN
    -- previousFirstTwoChars := 'Aa';
    currentRowNumber := 0;
    DELETE FROM no_English_synonym_entries;
    FOR entry IN
        SELECT * FROM entries
    LOOP
        -- can do some processing here
   	--  RAISE NOTICE 'english: %', singleSynonymEntry.English;      -- single quotes!

        englishAttributes := tokenize_by_pipes(entry.English);
	germanAttributes := tokenize_by_pipes(entry.German);
        -- firstTwoChars := left(entry.German, 2);
        -- if previousFirstTwoChars < firstTwoChars Then
        if mod(currentRowNumber, 1000) = 0 then
          RAISE notice 'current row #:% %', currentRowNumber, entry.German;
        end if;
	index := 1;
	FOREACH englishAttribute IN ARRAY englishAttributes
	LOOP
          englishSynonyms := separate_English_semicolons(englishAttribute);

	   	--  RAISE NOTICE 'englishSynonyms: %', englishSynonyms;      -- single quotes!
		-- https://stackoverflow.com/questions/10214392/iterating-over-integer-in-pl-pgsql
		-- FOR englishSynonym IN 1 .. array_upper(englishSynonyms, 1)
		-- https://stackoverflow.com/questions/41776633/how-to-loop-through-string-in-postgresql
		FOREACH englishSynonym IN ARRAY englishSynonyms
		LOOP
	   	  -- RAISE NOTICE 'englishSynonym: % singleSynonymEntry.German: %', englishSynonym, germanAttributes[index];
		  INSERT INTO no_English_synonym_entries VALUES(germanAttributes[index], englishSynonym);
		END LOOP;
		-- RETURN NEXT entry; -- return current row of SELECT
	  index := index + 1;
	END LOOP;
        -- previousFirstTwoChars := firstTwoChars;
	currentRowNumber := currentRowNumber + 1;
    END LOOP;
    -- RETURN;
END
$BODY$
LANGUAGE plpgsql;

-- select split_synonyms_into_();
-- select count(*) from no_English_synonym_entries;
