SELECT name FROM (((directors JOIN movies ON movies.id = directors.movie_id)JOIN ratings ON movies.id = ratings.movie_id) JOIN people ON people.id = person_id) WHERE rating >= 9.0 GROUP BY person_id