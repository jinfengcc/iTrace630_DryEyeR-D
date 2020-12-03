#pragma once

void LogToFile(const char *file, int lineNo, const char *str, ...);
void LogToFile(const char *file, int lineNo, const wchar_t *str, ...);

// Usage:
//    LOG(text, <optional_args>)
// The 'text' and 'args' are exactly like printf
// Examples:
//    LOG("Some text");
//    LOG("Size = %d", size);
//    LOG("String = %s, Size = %d", "abcd", size);
//

#define LOG(str, ...) LogToFile(__FILE__, __LINE__, str, __VA_ARGS__)

