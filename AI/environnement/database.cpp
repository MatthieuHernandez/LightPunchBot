#include "database.h"
#include "gui/thread.h"
#include "gui/mainwindow.h"

sqlite3 *Database::db;
bool Database::is_open = false;
bool Database::is_opening = false;

int Database::callback(void *a_param, int number_value, char ** value, char ** name_column)
{
    for (int i = 0; i < number_value; i++)
    {
        if(value[i])
        {
            Database::result_request.push_back(value[i]);
        }
        else
        {
            Database::result_request.push_back("NULL");
        }
    }
    return 0;
}

void Database::delete_all()
{
    if(write("DROP TABLE combos") && write("DROP TABLE states"))
    {
        MainWindow::instance()->write("DATABASE ALL DELETE","information");
    }
}

void Database::initialize(string file)
{
    //file = file + ":memory:";
    //cout << file << endl;
    if(sqlite3_open(file.c_str(), &db) != 0)
    {
        MainWindow::instance()->write((string)"Could not open data base : " + file.c_str(),"information");
    }
    else
    {
        is_opening = true;

        //write("CREATE TABLE combos(id INTEGER PRIMARY KEY, combo VARCHAR(127) UNIQUE)");

        //write("CREATE TABLE states(id INTEGER PRIMARY KEY, id_combo INTEGER, score INTEGER, random REAL, date_added DATETIME, FOREIGN KEY (id_combo) REFERENCES combos(id))");

        //write("CREATE TABLE neural_networks(id INTEGER, id_combo INTEGER, clustering_rate REAL, number_input INTEGER, number_hidden_layer INTEGER, number_neuron_layer INTEGER, "
        //      "positive_classifed_well INTEGER, positive_misclassified INTEGER, negative_classified_well INTEGER, negative_misclassified INTEGER"
        //      "FOREIGN KEY (id_combo) REFERENCES combos(id), PRIMARY KEY (id, id_combo))");

        //write("CREATE TABLE perceptrons(id INTEGER, id_layer INTEGER, id_nn INTEGER, id_combo INTERGER, weights VARCHAR(255), bias REAL, number_input INTEGER, learning_rate INTEGER, "
        //      "FOREIGN KEY (id_nn) REFERENCES neural_networks(id), FOREIGN KEY (id_combo) REFERENCES combos(id), PRIMARY KEY (id, id_layer, id_nn, id_combo))");

        //write("ALTER TABLE combos ADD COLUMN deals_damage BOOLEAN");
        //write("ALTER TABLE combos ADD COLUMN average_damage REAL");
        //write("ALTER TABLE states ADD COLUMN learning_set BOOLEAN");
        //write("ALTER table states ADD COLUMN count INTEGER");
        //write("ALTER table neural_networks ADD COLUMN number_output INTEGER");
        //write("ALTER table neural_networks ADD COLUMN batch_size INTEGER");
        /*for(int h = 1; h <= 2; h++)
        {
            for(int i = 0; i < Player::number_float; i++)
            {
                write((string)"ALTER TABLE states ADD COLUMN " + Player::instance(h)->data_float[i]->name + " REAL");
            }
            for(int i = 0; i < Player::number_int; i++)
            {
                write((string)"ALTER TABLE states ADD COLUMN " + Player::instance(h)->data_int[i]->name + " INTEGER");
            }
        }*/
        //write("ALTER TABLE neural_networks ADD COLUMN score REAL");
        //write("ALTER TABLE states DROP COLUMN is_useful");
        //write("ALTER TABLE states ADD COLUMN is_useful BOOLEAN");
        //write("UPDATE states SET is_useful = 1");
        //write("ALTER TABLE neural_networks ADD COLUMN other_information VARCHAR(255)");

        MainWindow::instance()->write((string)"Opened database " + to_string(database_number),"information");

        is_open = true;
    }
}

bool Database::write(string sql) // DISPLAY ERROR
{
    char *error = NULL;
    if(sqlite3_exec(db, sql.c_str(), callback, (void*)0, &error) != (SQLITE_OK || SQLITE_CONSTRAINT_UNIQUE))//can used callback function
    {
        MainWindow::instance()->write("Request execute with success", "current_information");
        return true;
    }
    else
    {
        MainWindow::instance()->write((string)"Error : " + error, "information");
        cout << sql << endl;
        return false;
    }
}

vector<string> Database::read(string sql)
{
    mutex.lock();
    result_request.clear();
    if(write(sql))
    {
        vector<string> temp = result_request;
        mutex.unlock();
        return temp;
    }
    else
    {
        mutex.unlock();
        return result_request;
    }
}

