#pragma once

#if __cpp_lib_jthread
#  include <thread>
#else
#  include <jthread.hpp>
#endif