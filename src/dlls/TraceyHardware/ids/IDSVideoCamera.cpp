#include "pch.h"
#include "IDSVideoCamera.h"

#define Check(x)                                                                                                                           \
  if (x != IS_SUCCESS)                                                                                                                     \
  return false

namespace {
  // Hidden window implementation
  IDSVideoCamera *idsCamera = nullptr;

  LRESULT CALLBACK HiddenWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    if (uMsg == IS_UEYE_MESSAGE) {
      if (idsCamera)
        idsCamera->OnCameraEvent(static_cast<int>(wParam), static_cast<HIDS>(lParam));
      return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  HWND CreateHiddenWindow()
  {
    static const TCHAR *class_name = _T("WindowClass_a0f9b46d-4cec-4fa3-917f-01b55ca78194");

    WNDCLASSEX wx    = {sizeof(WNDCLASSEX)};
    wx.lpfnWndProc   = &HiddenWndProc;
    wx.hInstance     = HINST_THISCOMPONENT;
    wx.lpszClassName = class_name;
    if (!RegisterClassEx(&wx))
      return nullptr;

    return CreateWindowEx(0, class_name, class_name, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr);
  }

} // namespace

IDSVideoCamera::IDSVideoCamera(Callback &&callback)
  : m_callback(std::move(callback))
{
  ATLASSERT(idsCamera == nullptr);
  idsCamera = this;

  m_appSettings.SetCallback([this]() { OnSettingsChanged(); });
}

IDSVideoCamera::~IDSVideoCamera()
{
  Close();

  ATLASSERT(idsCamera == this);
  idsCamera = nullptr;
}

bool IDSVideoCamera::Open()
{
  static HWND hWnd = CreateHiddenWindow();

  HIDS hCam{};

  // Release any existing resources.
  Close();

  // Ask API to open camera.
  if (is_InitCamera(&hCam, hWnd) == IS_SUCCESS) {
    // If success set the camera ID field and fetch camera and sensor info
    is_GetCameraInfo(hCam, &m_cameraInfo);
    is_GetSensorInfo(hCam, &m_sensorInfo);

    if (!Configure(hCam)) {
      return false;
    }

    m_hCam = hCam;

    // Use maximum dimensions
    m_width  = m_sensorInfo.nMaxWidth;
    m_height = m_sensorInfo.nMaxHeight;

    // Allocate memory for API to load frames into
    AllocateImgMem(m_sensorInfo.nMaxWidth, m_sensorInfo.nMaxHeight);

    // Tell API to load frames into memory and set the format
    is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
    is_SetColorMode(m_hCam, IS_CM_BGR8_PACKED);

    is_EnableMessage(m_hCam, IS_DEVICE_REMOVED, hWnd);
    is_EnableMessage(m_hCam, IS_DEVICE_RECONNECTED, hWnd);
    is_EnableMessage(m_hCam, IS_FRAME, hWnd);

    // Initialize start time and start the camera running
    m_startTime = time(nullptr);
    is_CaptureVideo(m_hCam, IS_DONT_WAIT);

    // Run experiments
    Experiments();
  }

  return IsOpened();
}

void IDSVideoCamera::Close()
{
  if (IsOpened()) {
    is_EnableMessage(m_hCam, IS_DEVICE_REMOVED, nullptr);
    is_EnableMessage(m_hCam, IS_DEVICE_RECONNECTED, nullptr);
    is_EnableMessage(m_hCam, IS_FRAME, nullptr);

    // Stop the camera
    is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);

    // Release memory
    FreeImgMem();

    // Close the camera handle
    is_ExitCamera(m_hCam);
    m_hCam = 0;
  }
}

void IDSVideoCamera::SetProperty(Prop prop, double value)
{
  switch (prop) {
  case Prop::brightness:
    value = m_exposureRange[0] + value * (m_exposureRange[1] - m_exposureRange[0]) / 100.0;
    value = std::clamp(value, m_exposureRange[0], m_exposureRange[1]);
    is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void *)&value, sizeof(value));
    break;
  case Prop::contrast:
    break;
  case Prop::hue:
    break;
  case Prop::saturation:
    break;
  case Prop::exposure:
    break;
  case Prop::gain:
    value = value * 100 / 255;
    is_SetHardwareGain(m_hCam, static_cast<int>(value), IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    break;
  default:
    break;
  }
}

void IDSVideoCamera::OnCameraEvent(int id, HIDS hCam)
{
  if (id == IS_FRAME) {
    if (auto pLast = GetLastMem()) {
      auto nRet = is_LockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, pLast);

      if (IS_SUCCESS == nRet) {
        cv::Mat ref(m_height, m_width, CV_8UC3, pLast);
        m_callback(ref);
        is_UnlockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, pLast);
      }
    }
  }
}

bool IDSVideoCamera::Configure(HIDS hCam)
{
  auto settings = AppSettings(m_appSettings, "hal.ids");

  auto pixelClock = settings.Get<int>("pixelClock", 118);
  auto dExposure  = settings.Get<double>("exposure", 26.129135);
  auto fps        = settings.Get<double>("fps", 25.0);

  double autoGain      = 1;
  double autoReference = 128;

  Check(is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void *)&pixelClock, sizeof(pixelClock)));
  Check(is_PixelClock(hCam, IS_PIXELCLOCK_CMD_GET, (void *)&pixelClock, sizeof(pixelClock)));
  Check(is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void *)&dExposure, sizeof(dExposure)));
  Check(is_SetFrameRate(hCam, fps, &fps));

  Check(is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_GAIN, &autoGain, nullptr));
  Check(is_SetAutoParameter(hCam, IS_SET_AUTO_REFERENCE, &autoReference, nullptr));

  double maxShutter;
  Check(is_SetAutoParameter(hCam, IS_GET_AUTO_SHUTTER_MAX, &maxShutter, 0));
  Check(is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void *)&dExposure, sizeof(dExposure)));
  Check(is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void *)m_exposureRange, sizeof(m_exposureRange)));

  return true;
}

void IDSVideoCamera::OnSettingsChanged()
{
  if (IsOpened()) {
    Configure(m_hCam);
  }
}

bool IDSVideoCamera::AllocateImgMem(int sizeX, int sizeY, int bitsPerPixel)
{
  for (auto &m : m_memVector) {
    auto err = is_AllocImageMem(m_hCam, sizeX, sizeY, bitsPerPixel, &m.pcImageMemory, &m.lMemoryId);

    if (err == IS_SUCCESS)
      err = is_AddToSequence(m_hCam, m.pcImageMemory, m.lMemoryId);

    if (err != IS_SUCCESS) {
      FreeImgMem();
      return false;
    }
  }

  return true;
}

void IDSVideoCamera::FreeImgMem()
{
  is_ClearSequence(m_hCam);
  for (auto &m : m_memVector) {
    if (m.pcImageMemory)
      (void)is_FreeImageMem(m_hCam, m.pcImageMemory, m.lMemoryId);
    m = {};
  }
}

char *IDSVideoCamera::GetLastMem() const
{
  char *pLast       = nullptr;
  INT   lMemoryId   = 0;
  INT   lSequenceId = 0;

  return GetLastMem(&pLast, lMemoryId, lSequenceId) ? pLast : nullptr;
}

bool IDSVideoCamera::GetLastMem(char **ppLastMem, INT &lMemoryId, INT &lSequenceId) const
{
  int   dummy = 0;
  char *pLast = nullptr;
  char *pMem  = nullptr;

  auto nRet = is_GetActSeqBuf(m_hCam, &dummy, &pMem, &pLast);

  if ((IS_SUCCESS == nRet) && (nullptr != pLast)) {
    nRet = IS_NO_SUCCESS;

    for (auto &m : m_memVector) {
      if (pLast == m.pcImageMemory) {
        *ppLastMem  = m.pcImageMemory;
        lMemoryId   = m.lMemoryId;
        lSequenceId = m.lSequenceId;
        nRet        = IS_SUCCESS;

        break;
      }
    }
  }

  return nRet == IS_SUCCESS;
}

void IDSVideoCamera::Experiments()
{
  /*
    In general, the pixel clock is set once when opening the camera and will not be changed. Note that, if you change the pixel clock, the
    setting ranges for frame rate and exposure time also changes. If you change a parameter, the following order is recommended:

    1.Change pixel clock.
    2.Query frame rate range and, if applicable, set new value.
    3.Query exposure time range and, if applicable, set new value.

    If one parameter is changed, the following parameters have to be adjusted due to the dependencies.
  */
  return;

  INT nRet;

  double fps;
  nRet = is_GetFramesPerSecond(m_hCam, &fps);

  // Get current pixel clock
  UINT nPixelClock;
  nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_GET, (void *)&nPixelClock, sizeof(nPixelClock));

  // Get default pixel clock
  UINT nPixelClockDefault;
  nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_GET_DEFAULT, (void *)&nPixelClockDefault, sizeof(nPixelClockDefault));

  if (nRet == IS_SUCCESS)
    nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_SET, (void *)&nPixelClockDefault, sizeof(nPixelClockDefault));

  double minr, maxr, intr;
  nRet = is_GetFrameTimeRange(m_hCam, &minr, &maxr, &intr);

  double dDummy;
  nRet = is_SetFrameRate(m_hCam, 25.0, &dDummy);

  double dblRange[3];
  nRet      = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void *)dblRange, sizeof(dblRange));
  auto dMin = dblRange[0];
  auto dMax = dblRange[1];
  auto dInt = dblRange[2];

  nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void *)&dMax, sizeof(dMax));

  // and remember the value for later use
  double m_ExposureTime;
  nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void *)&m_ExposureTime, sizeof(m_ExposureTime));

  // Load parameters from file
  // nRet = is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, (void *)LR"(C:\1\ids_settings.ini)", NULL);
}
