// DEFINE
#pragma once
#if FOR_WIN
    #define PATH_SEPERATOR "\\"
#else
    #define PATH_SEPERATOR "/"
#endif


// INCLUDE
#include "../compact/compact.hpp"


// DATA
struct folder_item
{
    bool is_folder;
    string name;
    string path;
    uint32_t size = 0;
};


// CODE
string get_executable_path ();
string get_executable_folder_path ();
string get_absolute_path (string relative_path);
bool path_exists (string path);
void create_folder (string path);
vector<folder_item> read_folder (string path);
string read_file (string path);
void write_file (string path, string bytes, uint32_t byte_offset = 0);
void delete_path (string path);
