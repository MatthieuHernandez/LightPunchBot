#ifndef PARAMETERS
#define PARAMETERS

#define TAB_START 0  // 2

#define RESET true

#define LIGHT  1
#define MEDIUM 2
#define HEAVY  3
#define EX     4

#define FORWARD 1
#define BACK    2

#define LOW true
#define HIGH false

#define DATABASE ".\\..\\..\\..\\database\\database.db"
//#define DATABASE "C:\\Programming\\Light Punch\\database\\database.db"
#define TEMP_PATH ".\\..\\..\\..\\temp\\"

#define SAVE_FILE "save.ini"

#define VK_B 0x42
#define VK_N 0x4E
#define VK_M 0x4D
#define VK_G 0x47
#define VK_H 0x48
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_COMMA VK_OEM_COMMA

#define VK_LK 0x42
#define VK_MK 0x4E
#define VK_HK 0x4D
#define VK_3K VK_OEM_COMMA
#define VK_LP 0x47
#define VK_MP 0x48
#define VK_HP 0x4A
#define VK_3P 0x4B


#define READ_DATA       0
#define FIND_DATA       1
#define SAVE_DATA       2
#define LOAD_DATA       3
#define LEARNING        4
#define START_DATABASE  5
#define DELETE_DATABASE 6
#define TEST            7
#define START_NN        8
#define SAVE_NN         9
#define LOAD_NN         10
#define ADD_NN          11
#define DELETE_NN       12
#define TEST_NN         13
#define CLEAN_NN        14
#define TRAIN_NN        15
#define FIGHT           16
#define START_THREAD    17
#define TRAIN_ALL_NN    18
#define IS_CLOSED       20
#define WRITE_DATA      21

#define NUMBER_ACTION 30

#include <string>
#include <iomanip>
using namespace std;

class Parameters
{
    public:
        static string read_window;
        static int id_nn;
        static int choice_function;
        static bool is_close;
        static bool display_update;
        static bool shuffle_database;
        static bool is_single_nn;
        static bool is_score;
        static int number_single_nn;
        static int64_t address_to_write;
        static string name_of_file;
        static int number_of_thread;
        static bool is_display_values;

        static bool is_refreshing_neural_network_display;

        static int random_between(int a, int b)  // WARNING TO : b excluded
        {
            return rand()%(b-a)+a;
        }

        static int stoi(string str)
        {
            if(str == "NULL" || str == "")
                return 0;
            else
                return std::stoi(str);
        }

        static float stof(string str)
        {
            if(str == "NULL" || str == "")
                return 0;
            else
                return std::stof(str);
        }

        Parameters()
        {
            string read_window = "";
            //int choice_function = 0;
        }

        template <typename T>
        static std::string to_string_with_precision(const T a_value, const int n = 6)
        {
            std::ostringstream out;
            out << std::setprecision(n) << a_value;
            return out.str();
        }
};


#endif // PARAMETERS
