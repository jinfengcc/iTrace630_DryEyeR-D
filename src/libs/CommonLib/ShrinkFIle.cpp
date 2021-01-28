#include "pch.h"
#include "ShrinkFIle.h"

void ShrinkFile(const fs::path &path, std::uint32_t maxFileSize, const char *pDelim)
{
  if (maxFileSize == 0 || !fs::exists(path))
    return;

  auto dwFileSize = fs::file_size(path);
  if (dwFileSize < maxFileSize)
    return;

  CAtlFile file;
  if (file.Create(path.wstring().c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, OPEN_EXISTING) != S_OK)
    return;

  CAtlFileMapping map;
  if (map.MapFile(file, static_cast<SIZE_T>(dwFileSize), 0, PAGE_READWRITE | SEC_COMMIT, FILE_MAP_ALL_ACCESS) != S_OK)
    return;

  // 10% hysteresis
  auto hyst = maxFileSize - maxFileSize * 10 / 100;

  auto *begin = static_cast<char *>(map);
  auto *end   = strstr(begin + hyst, pDelim);
  if (end == nullptr)
    return;

  end += strlen(pDelim);
  auto skiplen = std::distance(begin, end);
  memcpy(begin, end, gsl::narrow<size_t>(dwFileSize - skiplen));

  map.Unmap();

  file.SetSize(dwFileSize - skiplen);
}
