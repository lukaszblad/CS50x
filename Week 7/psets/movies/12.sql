SELECT m.title FROM movies m
JOIN stars s ON m.id = s.movie_id
JOIN people p ON s.person_id = p.id
WHERE p.name = 'Bradley Cooper' AND m.title IN
(SELECT m.title FROM movies m
JOIN stars s ON m.id = s.movie_id
JOIN people p ON s.person_id = p.id
WHERE p.name = 'Jennifer Lawrence');