#include "read_memory.h"
#include "gui/thread.h"

Read_memory *q_instance = 0;
HWND Read_memory::hwnd = NULL;

Read_memory* Read_memory::instance()
{
    if(q_instance == NULL)
        q_instance = new Read_memory();
    return q_instance;
}

Read_memory::Read_memory()
{
    size_buffer = 0;
    count_integer = 0;
    address_str = 0;
    address_number = 0;
    k = 0;
    page_of_string = false;
    ready = initialize();
}

Read_memory::~Read_memory()
{
    CloseHandle(handle);
    buffer_list.clear();
}

// recopy
Read_memory::Read_memory(Read_memory &memory)
{
    this->buffer_list = memory.buffer_list;
}

// windows function
BOOL SetPrivilege(HANDLE hToken, LPCTSTR Privilege, BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
    TOKEN_PRIVILEGES tpPrevious;
    DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);

    if(!LookupPrivilegeValue( NULL, Privilege, &luid ))
        return FALSE;

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = 0;


    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious))
    {
        Thread::instance()->write((string)"error : " + std::to_string(GetLastError()), "console");
        return FALSE;
    }
    tpPrevious.PrivilegeCount       = 1;
    tpPrevious.Privileges[0].Luid   = luid;

    if(bEnablePrivilege)
    {
        tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    }
    else
    {
        tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tpPrevious.Privileges[0].Attributes);
    }

    if (!AdjustTokenPrivileges(hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL))
    {
        Thread::instance()->write((string)"error : " + std::to_string(GetLastError()), "console");
        return FALSE;
    }
    return TRUE;
}

// to access the process
bool Read_memory::initialize()
{
    string name = Thread::instance()->read("window_name");
    wstring name1(name.begin(), name.end());
    const wchar_t* name2 = name1.c_str();
    LPCTSTR window_name = name2;
    BOOL is_64bits;
    DWORD process_id;
    SYSTEM_INFO system_information;
    GetSystemInfo(&system_information);//GetSystemInfo at 32 bit
    hwnd = FindWindow(NULL, window_name);
    Thread::instance()->write("Information system found !", "console");
    proc_min_address = (int64_t) system_information.lpMinimumApplicationAddress;
    first_address = proc_min_address;
    proc_max_address = (int64_t) system_information.lpMaximumApplicationAddress;
    if(!hwnd)
    {
        Thread::instance()->write("Window not found !", "console");
        return false;
    }
    Thread::instance()->write("Window found", "console");
    GetWindowThreadProcessId(hwnd, &process_id);
    Thread::instance()->write((string)"process : " + std::to_string(process_id), "console");
    if(true)
    {
        if(true/*error == "5"*/)
        {
            if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &handle_token))
            {
                if (GetLastError() == ERROR_NO_TOKEN)
                {
                    if (!ImpersonateSelf(SecurityImpersonation))
                    {
                        Thread::instance()->write("ERROR 1 !!!", "console");
                        return false;
                    }

                    if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &handle_token))
                    {
                        Thread::instance()->write("ERROR 2 !!!", "console");
                        return false;
                    }
                }
                else
                {
                    Thread::instance()->write("ERROR 3 !!!", "console");
                    return false;
                }
            }
            if (!SetPrivilege(handle_token, SE_DEBUG_NAME, TRUE))
            {
                Thread::instance()->write((string)"error : " + std::to_string(GetLastError()), "console");
                Thread::instance()->write("Error in AdjustTokenPrivileges", "console");
                return FALSE;
            }
            else
                Thread::instance()->write("Privilege modify", "console");

        }
        else
        {
            Thread::instance()->write((string)"error : " + std::to_string(GetLastError()), "console");
            return false;
        }
    }
    handle = OpenProcess(PROCESS_ALL_ACCESS, false, process_id);
    if(!handle)
    {
        Thread::instance()->write((string)"error : " + std::to_string(GetLastError()), "console");
        Thread::instance()->write("SHIT !!!", "console");
        return false;
    }
    CloseHandle(handle_token);
    if(!IsWow64Process(handle, &is_64bits))
    {
        string error = (string)std::to_string(GetLastError());
        Thread::instance()->write("Could not use 64 bits process !", "console");
        Thread::instance()->write(error, "console");
        return false;
    }
    Thread::instance()->write("Use 32 bits process !", "console");
    Thread::instance()->write("Get handle !", "console");

    Thread::instance()->write((string)"Size of : " + std::to_string(sizeof(int64_t)), "console"); // 2^16
    Thread::instance()->write((string)"Min : " + std::to_string(proc_min_address), "console"); // 2^16
    Thread::instance()->write((string)"Max : " + std::to_string(proc_max_address), "console"); // 2^31 - 2^16
    SetForegroundWindow(hwnd);
    return true;
}

bool Read_memory::read_data() // base fonction
{

    buffer_list.clear();
    struct buffer_struct buffer_structure;
    int h;
    proc_min_address = first_address;

    while(proc_min_address < proc_max_address) // 2Go
    {
        VirtualQueryEx(handle, (void*)proc_min_address, &memory_information, sizeof(MEMORY_BASIC_INFORMATION));
        if(memory_information.Protect == PAGE_READWRITE && memory_information.State == MEM_COMMIT)
        {
            size_buffer = (int64_t)memory_information.RegionSize;
            address_page = (int64_t)memory_information.BaseAddress;
            if(size_buffer%page_size != 0)
            {
                Thread::instance()->write("Dimension error of the vector !", "console");
                return false;
            }
            for(h = 0; h < (int)size_buffer/page_size; h ++, address_page += page_size)
            {
                buffer_structure.address_page = address_page;

                //try
                //{
                    buffer_list.push_back(buffer_structure);
                //}
                //catch(const std::exception & e)
                //{
                     //Thread::instance()->write(e.what(), "console");
                     //break;
                //}

                if(!ReadProcessMemory(handle, (void*)buffer_list.back().address_page, buffer_list.back().buffer, sizeof(buffer_structure.buffer), NULL))
                {
                    string error = (string)"number error : " + std::to_string(GetLastError()); // 5 = ERROR_ACCESS_DENIED
                    Thread::instance()->write(error, "console");
                    return false;
                }
            }
        }
        proc_min_address += memory_information.RegionSize;
    }
    Thread::instance()->write((string)itoa((proc_min_address - 1), c, 10) + "/" + (string)itoa(proc_max_address, c, 10),"page_read");
    if(is_empty())
        return false;
    Thread::instance()->write("--------------------------------", "console");
    return true;
}

// refrech data in buffer
void Read_memory::refresh_data()
{
    struct buffer_struct buffer_structure;
    for(list<buffer_struct>::iterator iterator = buffer_list.begin(); iterator != buffer_list.end(); iterator ++)
    {
        if(!ReadProcessMemory(handle, (void*)(*iterator).address_page, buffer_structure.buffer, sizeof(buffer_structure.buffer), NULL))
        {
            /*Thread::instance()->write((string)"error " + std::to_string(GetLastError()) + " in refresh data "
                                      + " at address " + QString::number((*iterator).address_page, 16).toStdString(), "console");
            return;*/
        }
        else
        {
            for(k = 0; k < page_size; k += 4)
            {
                number[0] = (*iterator).buffer[k+0];
                number[1] = (*iterator).buffer[k+1];
                number[2] = (*iterator).buffer[k+2];
                number[3] = (*iterator).buffer[k+3];

                if(number[0] != (char)255
                || number[1] != (char)255
                || number[2] != (char)255
                || number[3] != (char)255)
                {
                    (*iterator).buffer[k+0] = buffer_structure.buffer[k+0];
                    (*iterator).buffer[k+1] = buffer_structure.buffer[k+1];
                    (*iterator).buffer[k+2] = buffer_structure.buffer[k+2];
                    (*iterator).buffer[k+3] = buffer_structure.buffer[k+3];
                }
            }
        }

    }
}

// for test
void Read_memory::process_data_file() // to much time
{
    file_string.open("data_string.txt");
    file_int.open("data_int.txt");
    file_float.open("data_float.txt");
    if(!file_string.is_open() || !file_int.is_open() || !file_float.is_open())
    {
        Thread::instance()->write("Could not open a file of data !", "console");
        return;
    }
    int count_letter = 0;
    int count_char = 0;
    string str = "";
    char *tab = new char[1];
    j = 0;
    address_str = buffer_list.front().address_page;
    for(list<buffer_struct>::iterator iterator = buffer_list.begin(); iterator != buffer_list.end(); iterator ++, j++)
    {
        for(i = 0; i < page_size; i++) // really too much time
        {
            tab[0] = (*iterator).buffer[i];
            str = str + tab;
            if(tab[0] > 31 && tab[0] < 127) // text
            {
                count_char ++;
                if((tab[0] >= 65 && tab[0] <= 90) || (tab[0] >= 97 && tab[0] <= 122))
                    count_letter ++;
            }
            else
            {
                if(tab[0] == 0 && count_letter > 2 && count_char > 3)
                {
                    file_string << "0x" << hex << address_str << " : " << str << endl;
                }
                str = "";
                count_letter = 0;
                count_char = 0;
                address_str =(*iterator).address_page + i + 1;
            }
        }
        for(k = 0; k < page_size; k += 4) // integer and floating point number
        {
            number[0] = (*iterator).buffer[k+0];
            number[1] = (*iterator).buffer[k+1];
            number[2] = (*iterator).buffer[k+2];
            number[3] = (*iterator).buffer[k+3];
            address_number = (*iterator).address_page + k;

            value_int = *(int*) number;
            if(value_int > -2100 && value_int < 2100 && value_int != 0)
                file_int << "0x" << hex << address_number << " : " << dec << value_int << endl;

            value_float = *(float*) number;
            if(value_float > -200.5f && value_float < 200.5f && value_float > 0.01 && value_float > -0.01)
                file_float << "0x" << hex << address_number << " : " << dec << value_float << endl;
        }
    }
    file_string.close();
    file_int.close();
    file_float.close();

}

// read line in .txt
void Read_memory::read_line_number(string line, int &address, int &value)
{
    char* str;
    str = strtok((char*)line.c_str(), "x");
    str = strtok(NULL, " ");
    address = (int)strtol(str, NULL, 16);
    value = atoi(line.substr(line.find(":")+2, 20).c_str());
}

// read line in .txt
void Read_memory::read_line_number(string line, int &address, float &value)
{
    char* str;
    str = strtok((char*)line.c_str(), "x");
    str = strtok(NULL, " ");
    address = (int)strtol(str, NULL, 16);
    value = atof(line.substr(line.find(":")+2, 20).c_str());
}

// find in .txt
void Read_memory::find_value(int value_reach, string file_begin, string file_result)
{
    ifstream file_b(file_begin.c_str());
    ofstream file_r(file_result.c_str());
    string line;
    string line_copy;
    int address;
    int value;
    while(getline(file_b, line))
    {
        line_copy = line.c_str();
        read_line_number(line, address, value);

        if(value == value_reach)
        {
            file_r << line_copy << endl;
        }
    }
}

// find in .txt
void Read_memory::find_value(float value_reach , string file_begin, string file_result)
{
    ifstream file_b(file_begin.c_str());
    ofstream file_r(file_result.c_str());
    string line;
    int address;
    float value;
    while(getline(file_b, line))
    {
        read_line_number(line, address, value);
        if(value == value_reach)
        {
            file_r << line << endl;
        }
    }
}

// return first value in buffer
int64_t Read_memory::return_address()
{
    for(list<buffer_struct>::iterator iterator = buffer_list.begin(); iterator != buffer_list.end(); iterator ++)
    {
        for(k = 0; k <= page_size-4; k += 4)
        {
            number[0] = (*iterator).buffer[k+0];
            number[1] = (*iterator).buffer[k+1];
            number[2] = (*iterator).buffer[k+2];
            number[3] = (*iterator).buffer[k+3];

            if(number[0] != (char)255
            || number[1] != (char)255
            || number[2] != (char)255
            || number[3] != (char)255)
            {
                return (int64_t)(*iterator).address_page + k;
            }
        }
    }
}

// save data in .txt
void Read_memory::save_data(string file_name)
{
    Thread::instance()->write("start of save", "console");
    if(is_empty())
        return;
    ofstream file(file_name.c_str());
    j = 0;
    address_number = 0;
    for(list<buffer_struct>::iterator iterator = buffer_list.begin(); iterator != buffer_list.end(); iterator ++)
    {
        for(k = 0; k < page_size; k += 4)
        {
            number[0] = (*iterator).buffer[k+0];
            number[1] = (*iterator).buffer[k+1];
            number[2] = (*iterator).buffer[k+2];
            number[3] = (*iterator).buffer[k+3];
            address_number = (*iterator).address_page + k;
            value_float = *(float*) number;

            if(number[0] != (char)255
            || number[1] != (char)255
            || number[2] != (char)255
            || number[3] != (char)255)
            {
                file << "0x" << hex << address_number << " : " << dec << value_float << endl;
            }
        }
    }
    file.close();
    Thread::instance()->write("end of save", "console");
}

// for test
void Read_memory::unit_test()
{
    string line;
    ifstream file("data_int.txt");
    ifstream file2("data_float.txt");
    int address;
    int value;
    float value2;
    int value_reach[] = {268, 42, 42, 0, 1, 2, 3, -7, 456, 456, 456, 777, 778, 779};
    float value_reach2[] = {2.5, 4.98, 7.65};
    if(file.is_open() && file2.is_open())
    {
        while(getline(file, line))
        {
            read_line_number(line, address, value);
            for(unsigned int f = 0; f < sizeof(value_reach)/sizeof(int); f++)
            {
                if(value == value_reach[f])
                {
                    value_reach[f] = 0;
                    break;
                }
            }
        }
        for(unsigned int f = 0; f < sizeof(value_reach)/sizeof(int); f++)
        {
            if(value_reach[f] != 0)
            {
                Thread::instance()->write("Echec of unit test 1", "console");
                return;
            }
        }

        while(getline(file2, line))
        {
            read_line_number(line, address, value2);
            for(unsigned int f = 0; f < sizeof(value_reach)/sizeof(float); f++)
            {
                if(value2 == value_reach2[f])
                {
                    value_reach2[f] = 0;
                    break;
                }
            }
        }
        for(unsigned int f = 0; f < sizeof(value_reach2)/sizeof(float); f++)
        {
            if(value_reach2[f] != 0)
            {
                Thread::instance()->write("Echec of unit test 2", "console");
                return;
            }
        }
    }
    else
    {
        Thread::instance()->write("Could not file for unit test", "console");
        return;
    }
    int value_reach3 = 2;
    string name3 = "data_int.txt";
    string name4 = "data_test_int.txt";
    ifstream file3(name3.c_str());
    ofstream file4(name4.c_str());
    if(file3.is_open() && file4.is_open())
    {
        find_value(value_reach3, name3, name4);
    }
    else
    {
        Thread::instance()->write("Could not file for unit test", "console");
        return;
    }
    ifstream file5(name4.c_str());
    if(file5.is_open())
    {
        while(getline(file5, line))
        {
            read_line_number(line, address, value);
            if (value != value_reach3)
            {
                Thread::instance()->write("Echec of unit test 3", "console");
                return;
            }
        }
    }
    else
    {
        Thread::instance()->write("Could not file for unit test", "console");
        return;
    }


    Thread::instance()->write("Success of unit test", "console");
}

bool Read_memory::is_empty()
{
    if(buffer_list.empty() || buffer_list.begin() == buffer_list.end())
    {
        Thread::instance()->write("Empty buffer", "console");
        return true;
    }
    return false;
}

void Read_memory::clear()
{
    buffer_list.clear();
}

void Read_memory::write_data_around_address_in_txt(string file_name, int64_t address, unsigned int number_values_around = 10)
{
    number_values_around *= 4; // 4 bits for 1 integer
    Thread::instance()->write("start of write in txt", "console");
    read_data();
    ofstream file(file_name.c_str());
    j = 0;
    address_number = 0;
    for(list<buffer_struct>::iterator iterator = buffer_list.begin(); iterator != buffer_list.end(); iterator ++)
    {
        if(((*iterator).address_page <= address - number_values_around
        &&  (*iterator).address_page + page_size > address - number_values_around)
        || ((*iterator).address_page <= address + number_values_around
        && (*iterator).address_page + page_size > address + number_values_around))
        {
            for(k = 0; k < page_size; k += 4)
            {
                if((*iterator).address_page + k <= address + number_values_around
                && (*iterator).address_page + k >= address - number_values_around)
                {
                    number[0] = (*iterator).buffer[k+0];
                    number[1] = (*iterator).buffer[k+1];
                    number[2] = (*iterator).buffer[k+2];
                    number[3] = (*iterator).buffer[k+3];
                    address_number = (*iterator).address_page + k;
                    value_int = *(int*) number;

                    if(number[0] != (char)255
                    || number[1] != (char)255
                    || number[2] != (char)255
                    || number[3] != (char)255)
                    {
                        if(address_number > address)
                            file << "+" << dec << address_number - address << " : " << dec << value_int << endl;

                        else if (address_number < address)
                            file << dec << address_number - address << " : " << dec << value_int << endl;
                        else
                            file << "0x" << hex << address_number << " : " << dec << value_int << endl;
                    }
                }
            }
        }
    }
    file.close();
    Thread::instance()->write("end of write in txt", "console");
}

int64_t Read_memory::find_data(int64_t start_address, vector<int> &values, vector<int> &position, int position_of_reached_value) // start_address %4 = 0
{
    int temp;
    list<buffer_struct>::iterator iterator;
    if(is_empty() == true && !read_data())
        return 0;
    if(values.size() != position.size())
        return 0;

    for(iterator = buffer_list.begin(); iterator != buffer_list.end(); iterator++)
    {
        if(start_address < (*iterator).address_page + page_size)
        {
            if(start_address > (*iterator).address_page)
            {
                k = start_address - (*iterator).address_page + 4;
            }
            else
                k = 0;

            for(; k < page_size; k += 4)
            {
                for(j = 0; j < values.size(); j++)
                {
                    k += position[j];

                    number[0] = (*iterator).buffer[k+0];
                    number[1] = (*iterator).buffer[k+1];
                    number[2] = (*iterator).buffer[k+2];
                    number[3] = (*iterator).buffer[k+3];
                    value_int = *(int*) number;

                    if(value_int == values[j])
                    {
                        if(j == 0)
                        {
                            temp = k;
                            address_number = (*iterator).address_page + k;
                        }
                        if(j == values.size()-1)
                            return address_number + position_of_reached_value;

                        if(k >= page_size)
                        {
                             k -= page_size;
                            if(iterator != buffer_list.end())
                                iterator++;
                            else
                                return 0;
                        }
                        else if(k < 0)
                        {
                            k = temp+4;
                            break;
                            /*k += page_size;
                            if(iterator != buffer_list.end())
                                iterator--;
                            else
                                return 0;*/
                        }
                    }
                    else
                    {
                        if(j > 0)
                            k = temp+4;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
