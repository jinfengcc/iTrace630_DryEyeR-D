#pragma once

#include <filesystem>

// Input Test Data
std::filesystem::path GetTestDataFolder(bool reportErrors = true);
void                  SetTestDataFolder(const std::filesystem::path &p);

// Results
std::filesystem::path GetTestResultsFolder();
void                  SetTestResultsFolder(const std::filesystem::path &p);
