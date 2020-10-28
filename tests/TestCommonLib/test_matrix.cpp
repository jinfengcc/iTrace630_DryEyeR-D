#include "pch.h"
#if 0
#include <libs/LegacyLib/Structures.h>

#include <opencv2/opencv.hpp>

namespace {
  // clang-format off
  real_t values1[2 * 3] = {
     1.321, 2, 3,
    -4.321, 9, 2
  };

  int values2[2 * 3] = {
    11, 12, 13,
    14, 15, 16
  };

  real_t rvalues1[2 * 3] = {
    1, 2, 3,
    4, 5, 6
  };
  // clang-format on

  template <class T>
  void TestEqual(const Matrix<T> &mat, gsl::span<const T> data)
  {
    EXPECT_EQ(data.size(), mat.GetSize());
    for (int i = 0; i < mat.GetSize(); ++i) {
      EXPECT_EQ(data[i], mat[i]);
    }

    for (int row = 0; row < mat.GetNumRows(); ++row) {
      for (int col = 0; col < mat.GetNumCols(); ++col) {
        auto val = data[row * mat.GetNumCols() + col];
        EXPECT_EQ(val, mat(row, col));
      }
    }

    auto mmi = std::minmax_element(data.begin(), data.end());

    EXPECT_EQ(*mmi.first, mat.GetMin().first);
    //EXPECT_EQ(std::distance(data.begin(), mmi.first), mat.GetMin().second);

    EXPECT_EQ(*mmi.second, mat.GetMax().first);
    //EXPECT_EQ(std::distance(data.begin(), mmi.second), mat.GetMax().second);
  }
} // namespace

GTEST_TEST(Matrix, Create)
{
  Matrix<real_t> mat(2, 3, values1);

  EXPECT_EQ(2, mat.GetNumRows());
  EXPECT_EQ(3, mat.GetNumCols());
  //EXPECT_NE(values1, mat.GetMem());

  TestEqual(mat, {values1});
}

GTEST_TEST(Matrix, Copy)
{
  Matrix<real_t> mat(2, 3, values1);
  TestEqual(mat, {values1});

  mat.CopyMem(rvalues1);
  TestEqual(mat, {rvalues1});
}

GTEST_TEST(Matrix, Fill)
{
  const int fill = 17;

  Matrix<int> mat(12, 23);
  mat.Fill(fill);

  EXPECT_EQ(fill, mat.GetMin().first);
  EXPECT_EQ(fill, mat.GetMax().first);
}

GTEST_TEST(Matrix, Transpose)
{
  Matrix<real_t> mat(2, 3, rvalues1);
  auto           t = Transpose(mat);

  EXPECT_EQ(mat.GetNumRows(), t.GetNumCols());
  EXPECT_EQ(mat.GetNumCols(), t.GetNumRows());

  for (int row = 0; row < mat.GetNumRows(); ++row) {
    for (int col = 0; col < mat.GetNumCols(); ++col) {
      EXPECT_EQ(mat(row, col), t(col, row));
    }
  }
}

GTEST_TEST(Matrix, Multiply)
{
  const real_t rvalues2[] = {3, 2, 1};

  Matrix<real_t> mat1(2, 3, rvalues1);
  Matrix<real_t> mat2(3, 1, rvalues2);

  auto mat = Multiply(mat1, mat2);

  EXPECT_EQ(2, mat.GetNumRows());
  EXPECT_EQ(1, mat.GetNumCols());

  EXPECT_DOUBLE_EQ(3 * 1 + 2 * 2 + 1 * 3, mat(0, 0));
  EXPECT_DOUBLE_EQ(3 * 4 + 2 * 5 + 1 * 6, mat(1, 0));
}

GTEST_TEST(Matrix, Invert)
{
  Matrix<real_t> mat(2, 2, rvalues1);

  auto inv = Invert(mat);

  EXPECT_EQ(mat.GetNumRows(), inv.GetNumCols());
  EXPECT_EQ(mat.GetNumCols(), inv.GetNumRows());

  auto prod = Multiply(mat, inv);
  for (int row = 0; row < prod.GetNumRows(); ++row) {
    for (int col = 0; col < prod.GetNumCols(); ++col) {
      auto val = row == col ? 1.0 : 0.0;
      EXPECT_DOUBLE_EQ(val, prod(row, col));
    }
  }
}

GTEST_TEST(Matrix, LSM)
{
  cv::Mat_<real_t> cvA(2, 3);
  cv::Mat_<real_t> cvF(2, 1);

  cv::Mat_<double> x = (cv::Mat_<double>(3, 3) << 1, 22, 27, 22, 29, 36, 27, 36, 45);
  cv::Mat_<double> y = x.inv();


  memcpy(cvA.data, rvalues1, sizeof(rvalues1));

  std::cout << cvA;

  //cv::Mat_<real_t> cvX;
  //cv::solve(cvA, cvF, cvX);

   // {{1, 2, 3}, {4, 5, 6}};

  Matrix<real_t> mat1(2, 3, rvalues1);
  Matrix<real_t> mat2(3, 1, rvalues1);

  auto mat = LSM(mat1, mat2);

  EXPECT_EQ(3, mat.GetNumRows());
  EXPECT_EQ(3, mat.GetNumCols());
}
#endif
