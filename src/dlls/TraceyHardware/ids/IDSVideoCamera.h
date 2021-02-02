#pragma once

#include "uEye.h"
#include <opencv2/core.hpp>
#include "libs/CommonLib/AppSettings.h"

class IDSVideoCamera
{
public:
  using Callback = std::function<void(const cv::Mat &)>;

  IDSVideoCamera(Callback &&callback);

  virtual ~IDSVideoCamera();

  bool Open();
  void Close();
  bool IsOpened() const
  {
    return m_hCam != 0;
  }

  enum class Prop { brightness, contrast, hue, saturation, exposure, gain };
  void SetProperty(Prop prop, double value);

  void OnCameraEvent(int id, HIDS hCam);

private:
  struct Memory
  {
    char *pcImageMemory{};
    INT   lMemoryId{};
    INT   lSequenceId{};
  };
  using MemVector = std::array<Memory, 3>;

  AppSettings m_appSettings;
  HIDS        m_hCam = 0;
  Callback    m_callback{};
  CAMINFO     m_cameraInfo;
  SENSORINFO  m_sensorInfo;
  int         m_width;
  int         m_height;
  time_t      m_startTime;
  MemVector   m_memVector;
  double      m_exposureRange[3];

  bool  Configure(HIDS hCam);
  void  OnSettingsChanged();
  bool  AllocateImgMem(int sizeX, int sizeY, int bitsPerPixel = 3 * 8);
  void  FreeImgMem();
  char *GetLastMem() const;
  bool  GetLastMem(char **ppLastMem, INT &lMemoryId, INT &lSequenceId) const;
  void  Experiments();
};
