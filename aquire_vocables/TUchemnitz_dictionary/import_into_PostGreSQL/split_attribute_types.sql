-- https://www.postgresql.org/docs/9.5/static/functions-string.html
-- https://www.w3resource.com/PostgreSQL/position-function.php
  
-- http://www.sqlines.com/postgresql/stored_procedures_functions
-- https://stackoverflow.com/questions/42631864/postgres-function-returning-array
CREATE OR REPLACE FUNCTION tokenize_by_pipes(English TEXT) RETURNS TEXT []
   AS $$
    DECLARE
      synonyms text[];
    BEGIN
      -- https://www.postgresql.org/docs/9.1/static/functions-string.html
      synonyms := regexp_split_to_array(English, E'\\|');
      -- RAISE NOTICE 'synonyms: %', synonyms;
      RETURN synonyms;
    END; $$
    LANGUAGE PLPGSQL;

-- select separate_English_semicolons('sss|sss');

