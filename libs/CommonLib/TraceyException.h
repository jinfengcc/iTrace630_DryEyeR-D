#pragma once

#include <atlbase.h>
#include <stdexcept>
#include <string>
#include <interfaces/ILogger.h>

namespace tracey {
  struct Exception : public std::runtime_error
  {
  public:
    Exception(const char *msg = nullptr)
      : std::runtime_error(nullptr)
      , m_msg(msg)
    {
    }

    Exception(std::string &&msg)
      : std::runtime_error(nullptr)
      , m_msg(std::move(msg))
    {
    }

    template <typename... Args>
    Exception(const char *sz, Args... args)
      : std::runtime_error(nullptr)
    {
      m_msg = fmt::format(sz, args...);
    }

    const char *what() const override
    {
      return m_msg.c_str();
    }

  private:
    std::string m_msg;
  };

  struct ExceptionError : public Exception
  {
    ExceptionError(DWORD error = GetLastError())
      : Exception(Error(error))
    {
    }

  private:
    static std::string Error(DWORD error)
    {
      void *      m_lpMsgBuf{};
      const DWORD dw = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
      FormatMessageA(dw, NULL, error, 0, (char *)&m_lpMsgBuf, 0, NULL);

      std::string str;
      if (m_lpMsgBuf != nullptr) {
        str = static_cast<const char *>(m_lpMsgBuf);
        LocalFree(m_lpMsgBuf);
      }
      return str;
    }
  };

  struct FatalException
  {
  public:
    template <typename... Args>
    FatalException(const char *sz, Args... args)
    {
      ShowMessage(fmt::format(sz, args...));
    }

    const char *what() const
    {
      return m_msg.c_str();
    }

  private:
    std::string m_msg;

    static void ShowMessage(std::string_view msg)
    {
      ::MessageBoxA(nullptr, msg.data(), "Fatal Error", MB_OK | MB_APPLMODAL);
      ATLASSERT(FALSE);
      _exit(-1);
    }
  };

  namespace details {
    template <typename E>
    inline void ThrowEx(const char *file, int line, const E &e)
    {
      LOG_Throw("{}({}): {}", file, line, e.what());
      throw e;
    }

    template <typename E>
    inline void ThrowImpl(const char *file, int line, const char *msg)
    {
      ThrowEx(file, line, E(msg));
    }

    template <typename E, typename... Args>
    inline void ThrowImpl(const char *file, int line, const char *sz, Args... args)
    {
      // char msg[512];
      //_snprintf_s(msg, _TRUNCATE, sz, args...);
      ThrowEx(file, line, E(sz, args...));
    }
  } // namespace details
} // namespace tracey

#define TRACEY_THROW_EX_IF(cond, e)                                                                                                        \
  do {                                                                                                                                     \
    if (cond) {                                                                                                                            \
      LOG_Throw("{}({}): {}", __FILE__, __LINE__, e.what());                                                                               \
      throw e;                                                                                                                             \
    }                                                                                                                                      \
  }                                                                                                                                        \
  __pragma(warning(suppress : 4127)) while (0)

#define TRACEY_THROW_EX(e)                                                                                                                 \
  do {                                                                                                                                     \
    LOG_Throw("{}({}): {}", __FILE__, __LINE__, e.what());                                                                                 \
    throw e;                                                                                                                               \
  }                                                                                                                                        \
  __pragma(warning(suppress : 4127)) while (0)

#define TRACEY_THROW_FATAL(...)                                                                                                            \
  do {                                                                                                                                     \
    auto e = tracey::FatalException(__VA_ARGS__);                                                                                          \
    LOG_Fatal("{}({}): {}", __FILE__, __LINE__, e.what());                                                                                 \
    throw e;                                                                                                                               \
  }                                                                                                                                        \
  __pragma(warning(suppress : 4127)) while (0)

#define TRACEY_THROW_BUG(...)                                                                                                              \
  do {                                                                                                                                     \
    auto e = tracey::FatalException(__VA_ARGS__);                                                                                          \
    LOG_Bug("{}({}): {}", __FILE__, __LINE__, e.what());                                                                                   \
    throw e;                                                                                                                               \
  }                                                                                                                                        \
  __pragma(warning(suppress : 4127)) while (0)

#define TRACEY_THROW(...) TRACEY_THROW_EX(tracey::Exception(__VA_ARGS__))
#define TRACEY_THROW_IF(cond, ...) TRACEY_THROW_EX_IF(cond, tracey::Exception(__VA_ARGS__))
