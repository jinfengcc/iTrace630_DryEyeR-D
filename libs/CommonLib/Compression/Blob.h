#pragma once

#include <vector>
#include <filesystem>
#include <gsl/gsl>

class Blob
{
public:
  Blob(const std::filesystem::path &blobFile)
    : m_data(Load(blobFile))
  {
  }
  Blob(std::vector<uint8_t> &&data)
    : m_data(std::move(data))
  {
  }

  gsl::span<const uint8_t> Get() const
  {
    return {m_data};
  }

  template <class T>
  const T *GetNext()
  {
    return static_cast<const T *>(GetNext(sizeof(T)));
  }

  void Reset()
  {
    m_next = 0u;
  }

  const void *GetNext(unsigned size);
  cv::Mat     GetNextImage();

  static std::vector<uint8_t> Load(const std::filesystem::path &file);
  static bool                 Save(const std::filesystem::path &blobFile, const void *data, unsigned size);
  bool                        Save(const std::filesystem::path &blobFile) const
  {
    return m_data.empty() ? false : Save(blobFile, &m_data[0], m_data.size());
  }

private:
  std::vector<uint8_t> m_data;
  unsigned             m_next{};
};
