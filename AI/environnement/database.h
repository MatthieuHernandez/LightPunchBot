#ifndef DATA_BASE_H
#define DATA_BASE_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <QMutex>
#include "sqlite3.h"

using namespace std;

class Database // ALTERN WRITE()
{
    private:

        static sqlite3 *db;
        static int callback(void *a_param, int number_value, char ** value, char ** name_column);
        static vector<string> result_request;
        static QMutex mutex;

    public:

        static bool is_open;
        static bool is_opening;
        static void delete_all();
        static void initialize(string file);
        static bool write(string sql);
        static vector<string> read(string sql);
};
#endif // DATA_BASE_H


