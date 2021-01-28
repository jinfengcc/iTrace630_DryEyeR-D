#pragma once

#include "libs/CommonLib/Singleton.h"
#include <vector>
#include <functional>

class PatientServices : public Singleton<PatientServices>
{
public:
  using Data = std::vector<GUID>;
  enum class Type { WF, CT};

  PatientServices() = default;

  template <class Func>
  void RegisterPatientCallback(Func &&func)
  {
    m_patFunc = std::move(func);
  }
  template <class Func>
  void RegisterExamCallback(Func &&func)
  {
    m_examFunc = std::move(func);
  }

  Data GetSelectedPatientIDs() const
  {
    return m_patFunc();
  }

  Data GetSelectedExamIDs(EYE eye, Type type) const
  {
    return m_examFunc(eye, type);
  }

  Data GetSelectedExamIDs(Type type) const
  {
    auto od = GetSelectedExamIDs(EYE::OD, type);
    auto os = GetSelectedExamIDs(EYE::OS, type);

    od.insert(od.end(), os.begin(), os.end());

    return od;
  }


private:
  using PatFunction  = std::function<Data()>;
  using ExamFunction = std::function<Data(EYE, Type)>;

  PatFunction  m_patFunc;
  ExamFunction m_examFunc;
};

