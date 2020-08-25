#include <iostream>
#include <string>
#include <filesystem>

int main()
{
    std::string path = "/path/to/directory";
    for (const auto& entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;
}