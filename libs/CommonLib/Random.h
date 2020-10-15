#pragma once

#include <random>

struct random_seed_seq
{
  template <typename It>
  void generate(It begin, It end)
  {
    for (; begin != end; ++begin) {
      *begin = device();
    }
  }

  static random_seed_seq &get_instance()
  {
    static thread_local random_seed_seq result;
    return result;
  }

private:
  std::random_device device;
};

inline double random_double(double _low = 0.0, double _hi = 1.0)
{
  static thread_local std::mt19937_64 randomness(random_seed_seq::get_instance());
  static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return _low + _hi * distribution(randomness);
}

template <int _low, int _hi>
int random_int()
{
  static thread_local std::mt19937_64 randomness(random_seed_seq::get_instance());
  static thread_local std::uniform_int_distribution<int> distribution(_low, _hi);
  return distribution(randomness);
}
