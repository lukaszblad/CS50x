SELECT DISTINCT p.name FROM people p
JOIN stars s ON p.id = s.person_id
WHERE p.name IS NOT 'Kevin Bacon' AND s.movie_id IN
(SELECT s.movie_id FROM stars s
JOIN people p ON s.person_id = p.id
WHERE p.name = 'Kevin Bacon' AND p.birth = 1958);