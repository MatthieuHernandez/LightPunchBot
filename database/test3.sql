DELETE FROM states WHERE id IN
(
	SELECT id FROM to_delete td WHERE id_combo = 50 AND 
	1 = 
	CASE 
		WHEN score > 0 AND res > 0 THEN 1 
		WHEN score < 0 AND res < 0  THEN 1
		
		WHEN Score = 0 AND (SELECT deals_damage FROM combos WHERE id = 50) = 'FALSE' AND res > 0 THEN 1							
		WHEN Score = 0 AND (SELECT deals_damage FROM combos WHERE id = 50) = 'TRUE' AND res < 0 THEN 1
		ELSE 0 END
	ORDER BY date_added
	LIMIT (SELECT CAST(ABS(res-4) AS INTEGER) FROM to_delete WHERE id_combo = 50 LIMIT 1)
)