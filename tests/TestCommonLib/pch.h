//
// pch.h
// Header for standard system include files.
//

#pragma once

#pragma warning(push)
#pragma warning(disable: 5054)
#include <opencv2/opencv.hpp>
#pragma warning(pop)

#define GTEST_DONT_DEFINE_TEST 1
#include <gtest/gtest.h>

#include <type_traits>
#include <filesystem>

namespace fs = std::filesystem;

#include <atlbase.h>

#include <fmt/format.h>
#include <gsl/gsl>

#include <libs/CommonLib/TestSupport/TestDataFolder.h>
