#pragma once

#include <filesystem>
#include <string>

void LogInit(std::filesystem::path path);
void LogAppend(std::string& message, std::filesystem::path path);
