#pragma once

#include <string>
#include <filesystem>
#include "TraceyObj.h"
#include "data/Dicom.h"

//////////////////////////////////////////////////////////////////////////

TRACEYOBJ("eee9c50f-7306-42b2-8398-1c6258ef681e") ITraceyDicomConfig : public IUnknown
{
  using Data = dicom::Config;

  virtual Data Get() const                            = 0;
  virtual void Set(const Data &d)                     = 0;
  virtual bool IsValid(const Data *d = nullptr) const = 0;
  virtual bool Change(HWND hWndParent)                = 0;
};

TRACEYOBJ("54ac4456-27ed-4366-b2de-afed429125a4")
ITraceyDicomConfigRZDCX : public IUnknown
{
  virtual bool IsActivated() const = 0;
  virtual bool Activate()          = 0;
};

//////////////////////////////////////////////////////////////////////////

TRACEYOBJ("4f0f960b-81a5-4149-bbc0-c73267eb589a") ITraceyDicomQuery : public IUnknown
{
  using Patient = dicom::Patient;
  using Work    = dicom::Work;

  virtual bool           Query(HWND hWndParent)                           = 0;
  virtual bool           Query(ITraceyDicomConfig * cfg, HWND hWndParent) = 0;
  virtual const Patient *GetPatient() const                               = 0;
  virtual const Work *   GetWork() const                                  = 0;
};

TRACEYOBJ("14a40038-ef02-4a77-9d02-43627b5828c5") ITraceyDicomSend : public IUnknown
{
  using Patient = dicom::Patient;
  using Work    = dicom::Work;
  using Path    = std::filesystem::path;

  virtual bool SetPatient(const Patient &pt)                                                           = 0;
  virtual bool SendToServer(const Work &work, const cv::Mat &image)                                    = 0;
  virtual bool SendToServer(ITraceyDicomConfig * cfg, const Work &work, const cv::Mat &image)          = 0;
  virtual bool SendToFile(const Path &path, const Patient &pt, const Work &work, const cv::Mat &image) = 0;
};
