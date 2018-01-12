SELECT * FROM states where (SELECT COUNT(*) FROM states GROUP BY id_combo AND state ) > 0

SELECT
SUM(CASE WHEN score > 0 THEN 1 ELSE 0 END) - SUM(CASE WHEN score > 0 THEN 1 ELSE 0 END) result,
SUM(CASE WHEN score > 0 THEN 1 ELSE 0 END) positive,
Sum(CASE WHEN score <= 0 THEN 1 ELSE 0 END) negative
FROM states GROUP BY id_combo, learning_set