DROP VIEW IF EXISTS to_delete;
CREATE VIEW to_delete AS 
SELECT id, id_combo, res, date_added FROM
states sss,
(
	SELECT 
	id_combo ic, 
	state  st,
	learning_set ls,
	SUM
	(
		CASE 
			WHEN score > 0 THEN 1 
			WHEN score < 0 THEN -1
			WHEN Score = 0 AND (
												SELECT deals_damage FROM combos WHERE id = sts.id_combo
											) = 'FALSE' THEN 1							
			WHEN Score = 0 AND (
												SELECT deals_damage FROM combos WHERE id = sts.id_combo
											) = 'TRUE' THEN -1 END
			
		) res
	FROM states sts
	GROUP BY ic, st, ls HAVING res <= 0 OR res >= 0 
)
WHERE
sss.id_combo = ic AND 
sss.state = st AND 
sss.learning_set = ls AND 
id IN
(
	SELECT id FROM states sss2 
	WHERE 
	sss2.id_combo = sss.id_combo AND 
	sss2.state = sss.state AND 
	sss2.learning_set = sss.learning_set ORDER BY date_added ASC
) 
ORDER BY res, date_added;

DECLARE @count INT = 0;
DECLARE @nb INT = (SELECT max(id) FROM combos);



SELECT * FROM to_delete;
