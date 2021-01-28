#include "pch.h"
#include "TraceyConfigImpl.h"

// ITraceyConfig

const TraceyConfigImpl::Data *TraceyConfigImpl::GetConfigData(const Key &key) const
{
  if (auto i = m_map.find(key); i != m_map.end())
    return &i->second;

  return m_parent ? m_parent->GetConfigData(key) : nullptr;
}

int TraceyConfigImpl::OnDataChanged(std::function<void()> func)
{
  auto id = m_signal.Connect(func);
  if (m_parent)
    m_parentSigId[id] = m_parent->OnDataChanged(func);

  return id;
}

void TraceyConfigImpl::CancelNotifications(int id)
{
  if (auto i = m_parentSigId.find(id); i != m_parentSigId.end()) {
    if (m_parent)
      m_parent->CancelNotifications(i->second);
    m_parentSigId.erase(i);
  }
  m_signal.Disconnect(id);
}

void TraceyConfigImpl::SetConfigData(const Key &key, const Data &data)
{
  if (auto i = m_map.find(key); i != m_map.end()) {
    if (i->second == data)
      return;
  }

  m_map[key] = data;
  m_signal();
}
