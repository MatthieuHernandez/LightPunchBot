#ifndef READ_MEMORY_H
#define READ_MEMORY_H

#include <string>
#include <string.h>
#include <vector>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <list>
#include <typeinfo.h>
#include <aclapi.h>
#include <accctrl.h>

using namespace std;
class Read_memory
{
    public:
        static const int page_size = 4096;

    private:
        char c[32];
        uint64_t size_buffer;
        int64_t address_str;
        int64_t address_number;
        int64_t address_page;

        int count_integer;
        char number[4];
        int value_int;
        float value_float;
        int i, j, k;
        ofstream file_string, file_int, file_float;
        bool page_of_string;
        int64_t first_address;
        int64_t proc_min_address;
        int64_t proc_max_address;

        HANDLE handle;
        HANDLE handle_token;
        MEMORY_BASIC_INFORMATION memory_information;

        struct buffer_struct
        {
            byte buffer[page_size];
            int64_t address_page;
        };

        list<buffer_struct> buffer_list;
        bool initialize();
        bool is_empty();
        void read_line_number(string line, int &address, int &value);
        void read_line_number(string line, int &address, float &value);

    public:
        static HWND hwnd;

        bool ready = false;
        static Read_memory* instance();
        Read_memory();
        ~Read_memory();
        Read_memory(Read_memory &memory);
        void process_data_file(); // for test
        bool read_data();
        void refresh_data();
        void unit_test();
        int64_t return_address();
        void clear();

        int64_t find_data(int64_t start_address, vector<int> &values, vector<int> &position, int position_of_reached_value = 0);

        void write_data_around_address_in_txt(string file_name, int64_t address, unsigned int number_values_around);

        //reach in file
        void find_value(int value_reach , string file_begin, string file_result);
        void find_value(float value_reach , string file_begin, string file_result);

        // reach in buffer_list
        template <typename T>
        bool reach_data(string symbol_of_test, T value, T value2 = (T)0, bool is_reset = false)
        {
            list<buffer_struct>::iterator iterator;
            if(is_empty() == true || is_reset == true)
            {
                if(!read_data())
                    return false;
            }
            else
            {
                refresh_data();
            }
            j = 0;
            char test;
            if(symbol_of_test == "==")
                test = 0;
            else if(symbol_of_test == "!=")
                test = 1;
            else if(symbol_of_test == "<")
                test = 2;
            else if(symbol_of_test == ">")
                test = 3;
            else if(symbol_of_test == "<>")
                test = 4;
            else
            {
                //MainWindow::instance()->write("Error of type inside reach_data", "console");
                return false;
            }

            bool contains_useful_values = false;
            if(typeid(T).name() == typeid(int).name())
            {
                for(iterator = buffer_list.begin(); iterator != buffer_list.end();) // iterator ++ ////////////////////////////////////////////////////
                {
                    for(k = 0; k < page_size; k += 4)
                    {
                        number[0] = (*iterator).buffer[k+0];
                        number[1] = (*iterator).buffer[k+1];
                        number[2] = (*iterator).buffer[k+2];
                        number[3] = (*iterator).buffer[k+3];
                        address_number = (*iterator).address_page + k;

                        value_int = *(int*) number;

                        if( (test == 0 && value_int == value)
                        ||  (test == 4 && value_int > value && value_int < value2)
                        ||  (test == 1 && value_int != value)
                        ||  (test == 2 && value_int < value)
                        ||  (test == 3 && value_int > value))
                        {
                            if(contains_useful_values == false)
                                contains_useful_values = true;
                        }
                        else // biggest integer value
                        {
                            (*iterator).buffer[k+0] = (char)255;
                            (*iterator).buffer[k+1] = (char)255;
                            (*iterator).buffer[k+2] = (char)255;
                            (*iterator).buffer[k+3] = (char)255;
                        }
                    }
                    if(contains_useful_values == false)
                    {
                        iterator = buffer_list.erase(iterator);
                    }
                    else
                    {
                        ++ iterator;
                         contains_useful_values = false;
                    }
                }
            }
            if(typeid(T).name() == typeid(float).name())
            {
                for(iterator = buffer_list.begin(); iterator != buffer_list.end();) // iterator ++ //////////////////////////////////////////////////////
                {
                    for(k = 0; k < page_size; k += 4)
                    {
                        number[0] = (*iterator).buffer[k+0];
                        number[1] = (*iterator).buffer[k+1];
                        number[2] = (*iterator).buffer[k+2];
                        number[3] = (*iterator).buffer[k+3];
                        address_number = (*iterator).address_page + k;

                        value_float = *(float*) number;

                        if( (test == 0 && value_float == value)
                        ||  (test == 4 && value_float > value && value_float < value2)
                        ||  (test == 1 && value_float != value)
                        ||  (test == 2 && value_float < value)
                        ||  (test == 3 && value_float > value))
                        {
                            if(contains_useful_values == false)
                                contains_useful_values = true;
                        }
                        else // biggest floating value
                        {
                            (*iterator).buffer[k+0] = (char)255;
                            (*iterator).buffer[k+1] = (char)255;
                            (*iterator).buffer[k+2] = (char)255;
                            (*iterator).buffer[k+3] = (char)255;
                        }
                    }
                    if(contains_useful_values == false)
                    {
                        iterator = buffer_list.erase(iterator);
                    }
                    else
                    {
                        ++ iterator;
                         contains_useful_values = false;
                    }
                }
            }
            return true;
        }

        // read value
        template <typename T>
        void refresh_value(int64_t &address, T &value)
        {
            if(ReadProcessMemory(handle, (LPCVOID)address, (LPVOID)number, sizeof(value), NULL))
            {
                if(typeid(T).name() == typeid(int).name())
                {
                    value_int = *(int*) number;
                    value = value_int;
                }
                else if(typeid(T).name() == typeid(float).name())
                {
                    value_float = *(float*) number;
                    value = value_float;
                }

            }
            else
            {
                return;
            }
        }

        void save_data(string file_name);

};

#endif // READ_MEMORY_H
