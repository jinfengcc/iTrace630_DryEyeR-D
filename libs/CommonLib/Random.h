#pragma once

#include <random>

// from: https://probablydance.com/2016/12/29/random_seed_seq-a-small-utility-to-properly-seed-random-number-generators-in-c/

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

  std::uniform_real_distribution<double> distribution(_low, _hi);
  return distribution(randomness);
}

inline int random_int(int _low, int _hi)
{
  static thread_local std::mt19937_64 randomness(random_seed_seq::get_instance());

  std::uniform_int_distribution<int> distribution(_low, _hi);
  return distribution(randomness);
}
