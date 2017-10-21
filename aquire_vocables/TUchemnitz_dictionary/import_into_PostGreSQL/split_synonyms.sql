-- https://www.postgresql.org/docs/9.5/static/functions-string.html
-- https://www.w3resource.com/PostgreSQL/position-function.php
  
-- http://www.sqlines.com/postgresql/stored_procedures_functions
-- https://stackoverflow.com/questions/42631864/postgres-function-returning-array
CREATE OR REPLACE FUNCTION separate_English_semicolons(English TEXT) RETURNS TEXT []
   AS $$
    DECLARE
      pos INT;
      synonym_begin INT;
      num_chars INT;
      synonyms text[];
      synonym text;
    BEGIN
       synonym_begin := 0;
      LOOP
      -- TODO replace manual splitting with "regexp_split_to_array(...)" ?
      pos := position(';' in English);
      num_chars := pos - synonym_begin;
      IF pos > 0 THEN
         RAISE NOTICE 'pos > 0 : %', pos;
         synonym := substring(English from synonym_begin for num_chars);
         English := substring(English from pos + 1 ); 
         RAISE NOTICE 'English: %', English;
         RAISE NOTICE 'synonym: %', synonym;
         synonyms := array_append(synonyms, synonym);
         RAISE NOTICE 'synonyms: %', synonyms;
      ELSE
        RAISE NOTICE 'pos = 0 --> no more semicolon found from pos %', pos;
        EXIT;
      END IF;
      END LOOP;
         num_chars := char_length(English);
         RAISE NOTICE 'English: % # chars:%', English, num_chars;
         -- synonym := substring(English from 0 for num_chars);
         RAISE NOTICE 'synonym: %', English;
         synonyms := array_append(synonyms, English);      
         RAISE NOTICE 'synonyms: %', synonyms;
      RETURN synonyms;
    END; $$
    LANGUAGE PLPGSQL;

-- select separate_English_semicolons('sss|sss');

