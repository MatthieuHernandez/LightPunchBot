SELECT combo, deals_damage, COUNT(*) as count from states, combos WHERE combos.id = states.id_combo AND combos.deals_damage = 'FALSE' GROUP BY combo ORDER BY count ASC