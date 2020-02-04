#include "FormUtilities.h"

#include <string>
#include <algorithm>
#include <cctype>

#include <sys/stat.h>
#include <sys/types.h>

void form(const std::string& question, std::string& answer, const std::string& default_answer, std::function<bool(const std::string&)> const& check) {
    do {
        std::cout << question << " (default is \"" << default_answer << "\"): ";
        std::getline(std::cin, answer);
        if (answer.size() == 0) {
            answer = default_answer;
        }
    } while (!check(answer));
}

std::function<bool(const std::string&)> const& file_exist = [](const std::string& name) {
    // Check if file exist
    struct stat buffer;

    bool file_exist = (stat(name.c_str(), &buffer) == 0);
    if (!file_exist) {
        std::cout << "File don't exist !" << std::endl;
    }
    return file_exist;
};

std::function<bool(const std::string&)> const& directory_exist = [](const std::string& name) {
    // Check if directory exist
    struct stat info;

    if (stat(name.c_str(), &info) != 0)
        std::cout << "Cannot access to this directory !" << std::endl;
    else if (info.st_mode & S_IFDIR)
        return true;
    else
        std::cout << "Directory is not good a directory !" << std::endl;
    return false;
};

std::function<bool(const std::string&)> const& is_number = [](const std::string& s) {
    // Check if is a number
    bool is_number = !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    if (!is_number) {
        std::cout << "Is not a number !" << std::endl;
    }
    return is_number;
};

std::function<bool(const std::string&)> const& resize_function_exist = [](const std::string& name) {
    // Check if function name exist
    bool resize_function_exist = (name == "NormalCrop" || name == "CenterCrop" || name == "Resize" || name == "ResizeCrop");
    if (!resize_function_exist) {
        std::cout << "Is not a valid function !" << std::endl;
    }
    return resize_function_exist;
};

std::function<bool(const std::string&)> const& similarity_function_exist = [](const std::string& name) {
    // Check if function name exist
    bool similarity_function_exist = (name == "diffHisto" || name == "diffHistoZone");
    if (!similarity_function_exist) {
        std::cout << "Is not a valid function !" << std::endl;
    }
    return similarity_function_exist;
};