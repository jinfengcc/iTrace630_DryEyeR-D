#pragma once

// From Kenny Kerr's MSDN article: https://msdn.microsoft.com/en-us/magazine/dn879357.aspx
// NOTE: Did not bother with 'Inspectable'
//

template <typename... Interfaces>
class __declspec(novtable) ImplementsStatic : public Interfaces...
{

protected:
  virtual ~ImplementsStatic() noexcept
  {
  }

  virtual unsigned long __stdcall AddRef() noexcept override
  {
    return 1;
  }

  virtual unsigned long __stdcall Release() noexcept override
  {
    return 1;
  }

public:
  virtual HRESULT __stdcall QueryInterface(GUID const &id, void **object) noexcept override
  {
    *object = BaseQueryInterface<Interfaces...>(id);

    if (nullptr == *object) {
      return E_NOINTERFACE;
    }
    static_cast<::IUnknown *>(*object)->AddRef();
    return S_OK;
  }

private:
  template <typename First, typename... Rest>
  void *BaseQueryInterface(GUID const &id) noexcept
  {
    if (id == __uuidof(First) || id == __uuidof(::IUnknown)) {
      return static_cast<First *>(this);
    }

    return FindInterface<Rest...>(id);
  }

  template <typename First, typename... Rest>
  void *FindInterface(GUID const &id) noexcept
  {
    if (id == __uuidof(First)) {
      return static_cast<First *>(this);
    }
    return FindInterface<Rest...>(id);
  }

  template <int = 0>
  void *FindInterface(GUID const &) noexcept
  {
    return nullptr;
  }
};

template <typename... Interfaces>
class __declspec(novtable) Implements : public ImplementsStatic<Interfaces...>
{
public:
  virtual HRESULT __stdcall QueryInterface(GUID const &id, void **object) noexcept override
  {
    if (S_OK == ImplementsStatic<Interfaces...>::QueryInterface(id, object))
      return S_OK;

    return m_outer ? m_outer->QueryInterface(id, object) : E_NOINTERFACE;
  }

protected:
  //Implements(IUnknown *outer = nullptr) noexcept
  //  : m_name("<NoName>")
  //  , m_outer(outer)
  //{
  //}
  Implements(const char *name, IUnknown *outer = nullptr) noexcept
    : m_name(name)
    , m_outer(outer)
  {
  }
  virtual ~Implements() noexcept
  {
  }

  virtual unsigned long __stdcall AddRef() noexcept override
  {
    unsigned long res = InterlockedIncrement(&m_references);
    AddRefGate(res);
    return res;
  }

  virtual unsigned long __stdcall Release() noexcept override
  {
    unsigned long const remaining = InterlockedDecrement(&m_references);
    ReleaseGate(remaining);
    if (0 == remaining) {
      delete this;
    }
    return remaining;
  }

protected:
  const char *m_name;
#if defined(LOG_Diag) && defined(_DEBUG)
  virtual void AddRefGate(unsigned long after)
  {
    LOG_Diag("AddRef = {} ({})", after, m_name ? m_name : "");
  }
  virtual void ReleaseGate(unsigned long remaining)
  {
    LOG_Diag("Release = {} ({})", remaining, m_name ? m_name : "");
  }
#else
  virtual void AddRefGate(unsigned long after)
  {
    (void)after;
  }
  virtual void ReleaseGate(unsigned long remaining)
  {
    (void)remaining;
  }
#endif

private:
  CComPtr<IUnknown> m_outer;
  unsigned long     m_references = 0;
};
