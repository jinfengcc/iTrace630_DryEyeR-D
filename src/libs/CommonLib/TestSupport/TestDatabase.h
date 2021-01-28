#pragma once

#include <interfaces/IDatabase.h>
#include <wil/com.h>

class TestDatabase
{
public:
  TestDatabase(bool readOnly);

  explicit operator bool() const
  {
    return !!m_db;
  }
  IDatabaseJet *get()
  {
    return m_db.get();
  }
  IDatabaseJet *operator->()
  {
    return m_db.get();
  }

private:
  wil::com_ptr_t<IDatabaseJet> m_db;

  std::string GetDbConfigJson(bool readOnly);
};
