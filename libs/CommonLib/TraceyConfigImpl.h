#pragma once

#include <wil/com.h>
#include <libs/CommonLib/Signal.h>
#include <interfaces/ITraceyConfig.h>
#include "Implements.h"
#include "CreateObj.h"

class TraceyConfigImpl : public ImplementsStatic<ITraceyConfig, ITraceyConfigWriter>
{
public:
  TraceyConfigImpl()
  {
    m_parent = CreateObj<ITraceyConfig>();
  }

  TraceyConfigImpl(ITraceyConfig *tc)
    : m_parent(tc)
  {
  }

  operator ITraceyConfig *()
  {
    return static_cast<ITraceyConfig *>(this);
  }

  template <class T>
  void Set(Key key, const T &data) noexcept
  {
    SetConfigData(key, {data});
  }

  template <class T>
  void SetObj(T *obj)
  {
    SetConfigData(__uuidof(T), {obj});
  }

  // ITraceyConfig
  const Data *GetConfigData(const Key &key) const override;
  int         OnDataChanged(std::function<void()> func) override;
  void        CancelNotifications(int id) override;

  // ITraceyConfigWriter
  void SetConfigData(const Key &key, const Data &data) override;

private:
  wil::com_ptr_t<ITraceyConfig> m_parent;
  std::map<Key, Data>           m_map;
  sig::Signal<>                 m_signal;
  std::map<int, int>            m_parentSigId;

};
