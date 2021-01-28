#pragma once

#include <optional>
#include <libs/CommonLib/Guid.h>
#include <libs/CommonLib/CreateObj.h>
#include <interfaces/ITraceyConfig.h>

namespace CFG {
  const Guid IUNKNOWN  = Guid("372de483-37e1-4b9f-b31d-ac1ce34e1427"); // IUnknown
  const Guid PRINTING  = Guid("143612fa-2841-40d7-8980-4ddbc3f1b9a7"); // bool
  const Guid POLARGRID = Guid("ad3a8b88-1196-444d-86c3-dc5048ecb205"); // bool

  namespace CAMERA {
    const Guid NAME    = Guid("3fd7a017-38fd-4c5c-a262-5086149bb506"); // string
    const Guid SIZE_MM = Guid("9a93ffc6-3336-4418-86ca-84b89ed2c14f"); // cv::Size2d

  } // namespace CAMERA

}; // namespace CFG

class TraceyConfig
{
public:
  TraceyConfig(ITraceyConfig *cfg = nullptr)
  {
    if (cfg)
      m_cfg = cfg;
    else
      m_cfg = CreateObj<ITraceyConfig>();
  }

  template <class T>
  std::optional<T> Get(const Guid &key) noexcept
  {
    T data{};
    if (GetImpl<T>(key, data))
      return {data};
    else
      return {};
  }

  template <class T>
  T Get(const Guid &key, T defValue) noexcept
  {
    auto v = Get<T>(key);
    return v.has_value() ? v.value() : defValue;
  }

  template <class T>
  wil::com_ptr_t<T> GetObj()
  {
    if (auto d = Get<IUnknown *>(__uuidof(T)); d.has_value()) {
      wil::com_ptr_t<T> ptr;
      d.value()->QueryInterface(ptr.put());
      return ptr;
    }
    return {};
  }

private:
  using Key  = ITraceyConfig::Key;
  using Data = ITraceyConfig::Data;

  wil::com_ptr_t<ITraceyConfig> m_cfg;

  const ITraceyConfig::Data *GetConfig(const Key &key) const
  {
    return m_cfg ? m_cfg->GetConfigData(key) : nullptr;
  }

  template <class T>
  bool GetImpl(const Guid &key, T &data) noexcept
  {
    auto d = GetConfig(key);
    if (!d)
      return false;

    constexpr bool special_handling = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;
    if constexpr (special_handling) {
      if (std::holds_alternative<double>(*d)) {
        data = static_cast<T>(std::get<double>(*d));
        return true;
      }
      else if (std::holds_alternative<int>(*d)) {
        data = static_cast<T>(std::get<int>(*d));
        return true;
      }
      //else if (std::holds_alternative<bool>(*d)) {
      //  data = static_cast<T>(std::get<bool>(*d));
      //  return true;
      //}
      else {
        ATLASSERT(false);
      }
    }
    else {
      static_assert(variant_index<Data, T>() >= 0, "Config does not contain this data type");

      ATLASSERT(std::holds_alternative<T>(*d));
      data = std::get<T>(*d);
      return true;
    }

    return false;
  }

  // bool GetImpl(const Guid &key, int &data) noexcept
  //{
  //  if (auto d = GetConfig(key)) {
  //    if (std::holds_alternative<double>(*d)) {
  //      data = std::get<double>(*d);
  //      return true;
  //    }
  //    else if (std::holds_alternative<int>(*d)) {
  //      data = std::get<int>(*d);
  //      return true;
  //    }
  //    else {
  //      ATLASSERT(false);
  //      return false;
  //    }
  //  }
  //}

  template <typename T>
  static constexpr bool has_type()
  {
    return variant_index<Data, T>() >= 0;
  }

  template <typename VariantType, typename T, std::size_t index = 0>
  static constexpr int variant_index()
  {
    if constexpr (index == std::variant_size_v<VariantType>)
      return -1;
    else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>)
      return static_cast<int>(index);
    else
      return variant_index<VariantType, T, index + 1>();
  }
};
