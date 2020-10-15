#include "pch.h"
#include "Blob.h"
#include "Compression.h"

const void *Blob::GetNext(unsigned size)
{
  auto p = (m_next + size <= m_data.size()) ? nullptr : &m_data[m_next];
  if (p) {
    m_next += size;
    return p;
  }
  else {
    return nullptr;
  }
}

cv::Mat Blob::GetNextImage()
{
  auto *size  = GetNext<int>();
  auto *image = &m_data[m_next];
  m_next += *size;

  return cv::imdecode({image, *size}, cv::IMREAD_UNCHANGED);
}

std::vector<uint8_t> Blob::Load(const std::filesystem::path &blobFile)
{
  CAtlFile file;
  if (file.Create(blobFile.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) == S_OK) {
    CAtlFileMapping<std::uint8_t> mmf;
    if (mmf.MapFile(file) == S_OK) {
      return Decompress(mmf, mmf.GetMappingSize());
    }
  }
  return {};
}

bool Blob::Save(const std::filesystem::path &blobFile, const void *data, unsigned size)
{
  auto compressed = Compress(data, size);

  CAtlTemporaryFile file;
  file.Create();

  if (file.Write(compressed.data(), compressed.size()) != S_OK)
    return false;

  return file.Close(blobFile.wstring().c_str()) == S_OK;
}
