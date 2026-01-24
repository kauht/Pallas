#pragma once
#include <string>

struct File {
    const std::string path;
    const std::string filename;
    const std::string content;
};

File get_content(const std::string& path);
