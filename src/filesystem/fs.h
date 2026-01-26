#pragma once
#include <string>
#include <utility>

struct File {
    std::string path;
    std::string filename;
    std::string content;
    // File() = default;
};

File get_content(const std::string& path);
