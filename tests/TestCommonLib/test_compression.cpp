#include "pch.h"
#include <libs/CommonLib/Compressor.h>
#include <libs/CommonLib/Compression/Compression.h>
#include <libs/CommonLib/TempFile.h>
#include <libs/CommonLib/TestSupport/TestDataFolder.h>
#include <libs/CommonLib/Random.h>

namespace {
  using util::compress::MIN_COMPRESSION_SIZE;

  const char oldExam[]  = R"(jet\Exams\{00D92B11-0137-4C5F-A587-4FEC862A7684}.exam)";
  const int  testData[1024] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
} // namespace

GTEST_TEST(Compressor, CompressSize1)
{
  EXPECT_ANY_THROW(util::Compress(1u, testData, sizeof(testData)));
  EXPECT_ANY_THROW(util::Compress(0u, nullptr, sizeof(testData)));
  EXPECT_ANY_THROW(util::Compress(0u, testData, MIN_COMPRESSION_SIZE - 1));
  EXPECT_NO_THROW(util::Compress(0u, testData, util::compress::MIN_COMPRESSION_SIZE));

  //EXPECT_TRUE()
  //for (unsigned size = sizeof(testData); --size > 0; ) {
  //  auto com = util::Compress(0U, testData, size);
  //  auto dec = util::Decompress(&com[0], com.size());

  //  ASSERT_EQ(size, dec.data.size()) << "Unable to compress a buffer less than " << size << " bytes";
  //}
}

auto GetRandomData(int n = 0)
{
  auto size = n > 0 ? n : random_int(40'000, 100'000);
  auto td   = std::vector<std::uint8_t>(size, 0);

  std::for_each(td.begin(), td.end(), [](auto &v) { v = (std::uint8_t)random_int(0, 255); });
  std::fill(td.end() - size / 20, td.end(), 0);

  return td;
}

GTEST_TEST(Compression, Compress)
{
  for (int i = 0; i < 100; ++i) {

    auto td = GetRandomData();

    //std::fill(td.end() - 1000, td.end(), 0);

    auto c0 = Compress(&td[0], td.size());
    auto d0 = Decompress(&c0[0], c0.size());

    EXPECT_EQ(GetDecompressedSize(&c0[0], c0.size()), d0.size());
    EXPECT_EQ(d0, td);


    //// Compress into a file
    //auto temp = util::GetTempFile(".zip");
    //util::Compress(temp, 0U, &td[0], td.size());

    //// Decompress and confirm
    //auto dec = util::Decompress(temp);
    //EXPECT_EQ(0u, dec.version);
    //EXPECT_EQ(td, dec.data);

    if (td != d0)
      DebugBreak();
  }
}

GTEST_TEST(Compressor, CompressSize2)
{
  const unsigned sizes[] = {
    sizeof(testData),
    MIN_COMPRESSION_SIZE,
    MIN_COMPRESSION_SIZE + 1,
    MIN_COMPRESSION_SIZE + 10,
  };


  const char temp[] = R"(C:\Users\thank\AppData\Local\Temp\TRACEY\A51D0658-DB88-43DD-B16E-4AAF7ADEB4C4.zip)";

  auto dec = util::Decompress(temp);

  for (auto size : sizes) {
    auto com = util::Compress(0U, testData, size);
    ASSERT_NE(0u, com.size());

    auto dec = util::Decompress(&com[0], com.size());
    EXPECT_EQ(0u, dec.version);
    ASSERT_EQ(size, dec.data.size());
    EXPECT_TRUE(memcmp(testData, &dec.data[0], size) == 0);
  }
}

GTEST_TEST(Compressor, CompressRandomBuffer)
{
  for (int i = 0; i < 100; ++i) {

    // Create a random sized array and fill it with random data
    auto td = GetRandomData();

    // Compress into a buffer
    auto c = util::Compress(0, &td[0], td.size());

    // Decompress and confirm
    auto dec = util::Decompress(&c[0], c.size());
    EXPECT_EQ(0u, dec.version);
    EXPECT_EQ(td, dec.data);

    if (td != dec.data)
      DebugBreak();
  }
}

GTEST_TEST(Compressor, DecompressOldDbFiles)
{
}


GTEST_TEST(Compressor, CompressRandomFile)
{
  for (int i = 0; i < 100; ++i) {

    // Create a random sized array and fill it with random data
    auto td = GetRandomData();

    // Compress into a file
    auto temp = util::GetTempFile(".zip");
    util::Compress(temp, 0U, &td[0], td.size());

    // Decompress and confirm
    auto dec = util::Decompress(temp);
    EXPECT_EQ(0u, dec.version);
    EXPECT_EQ(td, dec.data);

    if (td != dec.data)
      DebugBreak();
  }
}

GTEST_TEST(Compressor, Compress4)
{
  auto file = GetTestDataFolder() / oldExam;
  auto dec0 = util::Decompress(file);
  auto temp = util::GetTempFile(".zip");

  util::Compress(temp, dec0.version, &dec0.data[0], dec0.data.size());

  auto dec1 = util::Decompress(temp);
  EXPECT_EQ(dec0.version, dec1.version);
  EXPECT_EQ(dec0.data, dec1.data);
}

GTEST_TEST(Compressor, DecompressOldVersion1)
{
  auto file = GetTestDataFolder() / oldExam;
  auto dec = util::Decompress(file);

  EXPECT_EQ(0u, dec.version);
  EXPECT_EQ(697'774u, dec.data.size());
}

GTEST_TEST(Compressor, DecompressDbFiles)
{
  const auto folder = fs::path(R"(C:\Tracey\Data)");
  if (!fs::exists(folder))
    return;

  int count = 0, errors = 0;
  for (fs::recursive_directory_iterator i(folder), e; i != e; ++i) {
    auto p = i->path();
    if (p.extension() == ".cal" || p.extension() == ".exam") {
      auto data = util::Decompress(p);
      EXPECT_TRUE(!data.data.empty()) << "File " << p << " failed to load";

      ++count;
      if (data.data.empty())
        ++errors;
    }
  }

  std::cout << "Scanned " << count << " with " << errors << "errors\n";
}

GTEST_TEST(Compressor, DecompressPreviouslyCreated)
{
  const auto folder = fs::path(R"(D:\_Proj\_Active\Tracey\data\TestData\badMD5)");
  if (!fs::exists(folder))
    return;

  auto file = folder / "{F60FAC92-6789-4B53-B4C4-C902C45F7F84}.exam";
  auto data = util::Decompress(file);

  for (fs::recursive_directory_iterator i(folder), e; i != e; ++i) {
    auto p = i->path();
    if (p.extension() == ".cal" || p.extension() == ".exam") {
      auto data = util::Decompress(p);
      EXPECT_TRUE(!data.data.empty()) << "File " << p << " failed to load";
    }
  }
}
