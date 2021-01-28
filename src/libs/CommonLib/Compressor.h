#pragma once

namespace util {
  namespace compress {
    const unsigned MIN_COMPRESSION_SIZE = 157;

    using Version = std::uint32_t;
    using Data    = std::vector<std::uint8_t>;
    using path    = std::filesystem::path;

    struct VersionedData
    {
      Version version;
      Data    data;
    };

    VersionedData Decompress(const path &path);
    VersionedData Decompress(const void *data, unsigned size);
    Data          Compress(Version version, const void *data, unsigned size);
    void          Compress(const path &path, Version version, const void *data, unsigned size);
  } // namespace compress

  using compress::Compress;
  using compress::Decompress;

}; // namespace util
