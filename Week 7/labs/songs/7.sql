SELECT AVG(energy) FROM songs s
JOIN artists a ON s.artist_id = a.id
WHERE a.name LIKE "%Drake%";