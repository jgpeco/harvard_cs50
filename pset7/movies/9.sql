SELECT name FROM ((stars INNER JOIN movies ON stars.movie_id = movies.id) INNER JOIN people ON stars.person_id = people.id) WHERE year = 2004  GROUP BY person_id ORDER BY birth