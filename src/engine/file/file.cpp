// INCLUDE
#include "./file.hpp"


// CODE
string get_executable_path ()
{
    char buffer[PATH_MAX] = {0};
    uint32_t buffer_length = PATH_MAX;
    bool success = false;
    string os;

    #if FOR_MAC
        success = int(_NSGetExecutablePath(buffer, &buffer_length)) == 0;
        os = "MAC";
    #elif FOR_WIN
        success = int(GetModuleFileName(NULL, buffer, buffer_length)) != 0;
        os = "WINDOWS";
    #elif FOR_LIN
        success = int(readlink("/proc/self/exe", buffer, buffer_length)) != -1;
        os = "LINUX";
    #endif

    throw_error(!success, 
        "\n"
        "ERROR: Retrieval of path to executable was unsuccessful.\n"
        "       Executed get_absolute_path() on " + os + ".\n"
        "       The resulting buffer contains:\n"
        "       " + buffer
    );

    return string(buffer, buffer_length);
}

string get_executable_folder_path ()
{
    string path = get_executable_path();
    path = path.substr(0, path.find_last_of( PATH_SEPERATOR ));
    return path;
}

string get_absolute_path (string relative_path)
{
    #if FOR_WIN
        replace_characters(relative_path, "/", "\\");
    #else
        replace_characters(relative_path, "\\", "/");
    #endif

    return get_executable_folder_path() + PATH_SEPERATOR + relative_path;
}

bool path_exists (string path)
{
    return filesystem::exists(path);
}

void create_folder (string path)
{
    filesystem::create_directory(path);
}

vector<folder_item> read_folder (string path)
{
    vector<folder_item> items;

    for (auto const& folder_entry : filesystem::directory_iterator{path}) 
    {
        folder_item new_item;
        new_item.is_folder = folder_entry.is_directory();
        new_item.path = folder_entry.path();
        new_item.size = new_item.is_folder ? 0 : folder_entry.file_size();
        new_item.name = new_item.path.substr( new_item.path.find_last_of("/") + 1);
        items.push_back(new_item);
    }

    return items;
}

string read_file (string path)
{
    ifstream file(path, ios::ate | ios::binary);

    throw_error(!file.is_open(), 
        "\n"
        "ERROR: Opening file '" + path + "' failed.\n"
    );

    uint64_t file_size = (uint64_t) file.tellg();

    vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), file_size);
    file.close();

    return string(buffer.begin(), buffer.end());
}

void write_file (string path, string bytes, uint32_t byte_offset)
{
    ofstream file(path);
    file.seekp(byte_offset);
    file << bytes;
    file.close();
}

void delete_path (string path)
{
    throw_error(!path_exists(path),
        "\n"
        "ERROR: The filepath '" + path + "' can not be deleted since it doesn't exist."
    );

    filesystem::remove_all(path);
}
