BEGIN TRANSACTION;
CREATE TEMPORARY TABLE perceptrons_backup(id, id_layer, id_nn, weights, bias, number_input, learning_rate);
INSERT INTO perceptrons_backup SELECT id, id_layer, id_nn, weights, bias, number_input, learning_rate FROM perceptrons;
DROP TABLE perceptrons;
CREATE TABLE perceptrons(id, id_layer, id_nn, weights, bias, number_input, learning_rate);
INSERT INTO perceptrons SELECT id, id_layer, id_nn, weights, bias, number_input, learning_rate FROM perceptrons_backup;
DROP TABLE perceptrons_backup;
COMMIT;