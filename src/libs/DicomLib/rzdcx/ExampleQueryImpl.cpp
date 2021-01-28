#include "pch.h"
#include "ExampleQueryImpl.h"
#include "Libs/CommonLib/StringSplit.h"

using rzdcxLib::IDCXREQ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define AddTagS(a)                                                                                                                         \
  {                                                                                                                                        \
    el->Init((int)a);                                                                                                                      \
    p->insertElement(el.get());                                                                                                            \
  }
#define AddTagS2(a)                                                                                                                        \
  {                                                                                                                                        \
    el->Init((int)a);                                                                                                                      \
    p2->insertElement(el.get());                                                                                                           \
  }
#define AddTag(a)                                                                                                                          \
  {                                                                                                                                        \
    el->Init((int)a);                                                                                                                      \
    o->insertElement(el.get());                                                                                                            \
  }
#define AddTagV(a, v)                                                                                                                      \
  {                                                                                                                                        \
    el->Init((int)a);                                                                                                                      \
    el->Value = v;                                                                                                                         \
    o->insertElement(el.get());                                                                                                            \
  }

void rzdcx::ExampleQueryImpl(const RZDCX *rzdcx)
{
  using namespace rzdcxLib;

  auto req = rzdcx->CreateInstance<IDCXREQ>        ();
  auto el  = rzdcx->CreateInstance<IDCXELM>        ();
  auto o   = rzdcx->CreateInstance<IDCXOBJ>        ();
  auto it  = rzdcx->CreateInstance<IDCXOBJIterator>();

  AddTag(StudyDate);
  AddTag(AccessionNumber);
  AddTag(ReferringPhysiciansName);

  {
    auto p   = rzdcx->CreateInstance<IDCXOBJ>        ();
    auto it1 = rzdcx->CreateInstance<IDCXOBJIterator>();

    AddTagS(ReferencedSOPClassUID);
    AddTagS(ReferencedSOPInstanceUID);
    it1->Insert(p.get());
    el->Init(ReferencedStudySequence);
    auto v = _variant_t((IUnknown *)it1.get(), false);
    el->PutValue(&v);
    o->insertElement(el.get());
  }
  {
    auto p   = rzdcx->CreateInstance<IDCXOBJ>        ();
    auto it1 = rzdcx->CreateInstance<IDCXOBJIterator>();

    AddTagS(ReferencedSOPClassUID);
    AddTagS(ReferencedSOPInstanceUID);
    it1->Insert(p.get());
    el->Init(ReferencedPatientSequence);
    auto v = _variant_t((IUnknown *)it1.get(), false);
    el->PutValue(&v);
    o->insertElement(el.get());
  }

  AddTagV(PatientsName, "*");
  // AddTagV( patientID, "3456");
  AddTag(PatientsBirthDate);
  AddTag(PatientsSex);
  AddTag(OtherPatientIDs);
  AddTag(PatientsSize);
  AddTag(PatientsWeight);
  AddTag(AdditionalPatientHistory);
  AddTag(PatientComments);
  AddTag(studyInstanceUID);
  AddTag(RequestingPhysician);

  {
    auto p   = rzdcx->CreateInstance<IDCXOBJ>        ();
    auto it1 = rzdcx->CreateInstance<IDCXOBJIterator>();

    AddTagS(CodeValue);
    AddTagS(CodingSchemeDesignator);
    AddTagS(CodeMeaning);
    it1->Insert(p.get());
    el->Init(RequestedProcedureCodeSequence);
    auto v = _variant_t((IUnknown *)it1.get(), false);
    el->PutValue(&v);
    o->insertElement(el.get());
  }

  AddTag(AdmissionID);
  AddTag(CurrentPatientLocation);
  AddTag(PatientState);

  {
    auto p   = rzdcx->CreateInstance<IDCXOBJ>        ();
    auto it1 = rzdcx->CreateInstance<IDCXOBJIterator>();

    el->Init(Modality);
    // el->Value = "";
    p->insertElement(el.get());

    AddTagS(ScheduledStationAETitle);
    AddTagS(ScheduledProcedureStepStartDate);
    AddTagS(ScheduledProcedureStepStartTime);
    AddTagS(ScheduledPerformingPhysiciansName);
    AddTagS(ScheduledProcedureStepDescription);

    {
      auto p2  = rzdcx->CreateInstance<IDCXOBJ>        ();
      auto it2 = rzdcx->CreateInstance<IDCXOBJIterator>();

      AddTagS2(CodeValue);
      AddTagS2(CodingSchemeDesignator);
      AddTagS2(CodeMeaning);
      it2->Insert(p2.get());
      el->Init(ScheduledProtocolCodeSequence);
      auto v = _variant_t((IUnknown *)it1.get(), false);
      el->PutValue(&v);
      p->insertElement(el.get());
    }

    AddTagS(ScheduledProcedureStepID);
    AddTagS(ScheduledStationName);
    AddTagS(ScheduledProcedureStepLocation);
    AddTagS(CommentsOnTheScheduledProcedureStep);
    AddTagV(ScheduledProcedureStepStartDate, "20100427-20100430");

    it1->Insert(p.get());
    el->Init(ScheduledProcedureStepSequence);
    auto v = _variant_t((IUnknown *)it1.get(), false);
    el->PutValue(&v);
    o->insertElement(el.get());
  }

  AddTag(RequestedProcedureID);
  AddTag(ConfidentialityConstraintOnPatientDataDescription);

  it = req->Query("COMMON", "DSRSVC", "localhost", 104, "1.2.840.10008.5.1.4.31" /* The MWL SOP Class UID */, o.get());

  // Iterate over the query results
  // for (; !it.AtEnd(); it.Next())
  for (; !it->AtEnd(); it->Next()) {
    auto r = it->Get();

    _bstr_t bs;
    auto e = r->GetElement(RequestedProcedureID);
    if (e && e->Value) {
      bs = e->Value.bstrVal;
      //printf("%s	", s1);
    }

    e = r->GetElement(Modality);
    if (e) {
      bs = e->Value.bstrVal;
      //printf("%s	", s1);
    }

    e = r->GetElement(RequestedProcedureDescription);
    if (e && e->Value) {
      bs = e->Value.bstrVal;
      //printf("%s	", s2);
    }

    e = r->GetElement(PatientsName);
    if (e) {
      bs = e->Value.bstrVal;
      //printf("%s	", s3);
    }

    e = r->GetElement(PatientsBirthDate);
    if (e) {
      _variant_t v  = e->Value;
      bs = e->Value.bstrVal;
     // printf("%s	\n\n", s4);
    }
  }
}
