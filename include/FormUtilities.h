#include <iostream>
#include <functional>

void form(const std::string& question, std::string& answer, const std::string& default_answer, std::function<bool(const std::string&)> const& test);

extern std::function<bool(const std::string&)> const& file_exist;
extern std::function<bool(const std::string&)> const& directory_exist;
extern std::function<bool(const std::string&)> const& is_number;
extern std::function<bool(const std::string&)> const& resize_function_exist;
extern std::function<bool(const std::string&)> const& similarity_function_exist;