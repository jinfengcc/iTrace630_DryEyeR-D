# Introduction

Interface to the Jet Database

# Definition

Interface class:

```
struct __declspec(uuid("85BA1187-BBE6-461C-98F0-4A5D853D03D3")) IDatabaseJet : public IUnknown
{
  virtual void Open()                 = 0;
  virtual void Open(const char *json) = 0;
  virtual void Close()                = 0;
  virtual bool IsReadOnly() const     = 0;

  // Groups
  virtual auto GetGroups() const -> std::vector<PATGROUP>                = 0;
  virtual auto GetGroup(const GUID &id) const -> std::optional<PATGROUP> = 0;
  virtual bool SaveGroup(const PATGROUP &group)                          = 0;

  // Clinics
  virtual auto GetClinics() const -> std::vector<CLINIC> = 0;
  virtual bool SaveClinic(const CLINIC &clinic)          = 0;
  virtual bool DeleteClinic(const GUID &clinicID)        = 0;

  // Physicians
  virtual auto GetPhysicians() const -> std::vector<PHYSICIAN> = 0;
  virtual bool SavePhysician(const PHYSICIAN &physician)       = 0;

  // Operators
  virtual auto GetOperators() const -> std::vector<OPERATOR> = 0;
  virtual bool SaveOperator(const OPERATOR &oper)            = 0;

  // Physicians
  virtual auto GetPatients() const -> std::vector<PATIENT>                = 0;
  virtual auto GetPatient(const GUID &id) const -> std::optional<PATIENT> = 0;
  virtual bool SavePatient(const PATIENT &physician)                      = 0;
  virtual bool DeletePatient(const GUID &physicianID)                     = 0;

  // Exams
  virtual auto GetExams(const GUID &patientID) const -> std::vector<EXAM>        = 0;
  virtual bool SaveExam(const EXAM &exam)                                        = 0;
  virtual bool DeleteExam(const GUID &examID)                                    = 0;
  virtual auto GetExamBlob(const GUID &examId) const -> std::vector<BYTE>        = 0;
  virtual bool SaveExamBlob(const GUID &examId, const void *data, unsigned size) = 0;

  // Eyes
  virtual auto GetEyes(const GUID &patientID) const -> std::vector<EYEDATA> = 0;
  virtual bool SaveEye(const EYEDATA &exam)                                 = 0;

private:
  virtual bool DeletePhysician(const GUID &physicianID) = 0;
  virtual bool DeleteOperator(const GUID &operID)       = 0;
  virtual bool DeleteGoup(const GUID &groupID)          = 0;
  virtual bool DeleteEyeData(const GUID &eyeID)         = 0;
};
```

Data referenced in the interface class:

```
struct DATETIME
{
  int Year;
  int Month;
  int Day;
  int Hour;
  int Minute;
  int Second;
};

struct PATGROUP
{
  GUID   GroupID;
  String Name;
};

struct ADDRESS
{
  String Line1;
  String Line2;
  String City;
  String State; // Region/province for non-US addresses is stored here
  String ZIP;
  String Country;
};

struct CLINIC
{
  GUID    ClinicID;
  String  Name;
  ADDRESS Address;
};

struct NAME
{
  String Last;
  String First;
  String Middle;
};

struct PHYSICIAN
{
  GUID PhysicianID;
  NAME Name;
};

struct OPERATOR
{
  GUID OperatorID;
  NAME Name;
};

struct PATIENT
{
  GUID     PatientID;
  GUID     GroupID;
  String   CustomID;
  NAME     Name;
  DATETIME Birthday;
  int      Sex;
  ADDRESS  Address;
  String   HomePhone;
  String   WorkPhone;
  String   EMail;
  String   Note;
  DATETIME Registration;
};

struct EXAM
{
  GUID     ExamID;
  GUID     PatientID;
  GUID     ClinicID;
  GUID     PhysicianID;
  GUID     OperatorID;
  GUID     CalibrationID;
  int      Type;
  DATETIME Date;
  int      Eye;
  int      Preop;
  int      SeriesNumber;
  int      Reserved1;
  int      Reserved2;
  int      Reserved3;
  int      Reserved4;
  String   Note;
};

struct EYEDATA
{
  GUID   EyeID;
  GUID   PatientID;
  int    Type;
  double ManifestVertex;
  double ManifestSphere;
  double ManifestCylinder;
  int    ManifestAxis;
  double K1;
  double K2;
  int    K2Axis;
};

```

# Function description

Function | Description
---------| -----------



Simple description of each function is the header file

# Design notes (optional)

How each algorith is implemented

# Unit tests

List of the tests implemented and brief description

### Test Data (optional)

Data required for testing (reference to GIT project/folder)

### Test Results

After tests pass, include test results