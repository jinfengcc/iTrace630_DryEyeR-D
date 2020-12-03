#include "pch.h"
#include "IDSVideoCapture.h"
#include <opencv2/core.hpp>

IDSVideoCapture::IDSVideoCapture()
{
}

IDSVideoCapture::IDSVideoCapture(int index)
{
  open(index);
}

IDSVideoCapture::~IDSVideoCapture()
{
  release();
}

bool IDSVideoCapture::open(int _index, int apiPreferenece)
{
  (void)apiPreferenece;

  HIDS hCam = _index;

  // Release any existing resources.
  release();

  // Ask API to open camera.
  if (is_InitCamera(&hCam, nullptr) == IS_SUCCESS) {

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
    for (int i = 0; i < NUM_BUFFERS; ++i) {
      is_AllocImageMem(m_hCam, m_sensorInfo.nMaxWidth, m_sensorInfo.nMaxHeight, 8 * 3, &m_imageMemoryAddr[i], &m_imageMemoryId[i]);
      is_AddToSequence(m_hCam, m_imageMemoryAddr[i], m_imageMemoryId[i]);
    }

    // Tell API to load frames into memory and set the format
    is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
    is_SetColorMode(m_hCam, IS_CM_BGR8_PACKED);

    // Initialize start time and start the camera running
    m_startTime = time(nullptr);
    is_CaptureVideo(m_hCam, IS_DONT_WAIT);

    // Run experiments
    Experiments();
  }

  return isOpened();
}

void IDSVideoCapture::release()
{
  if (isOpened()) {

    // Stop the camera
    is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);

    // Release memory
    is_ClearSequence(m_hCam);
    for (int i = 0; i < NUM_BUFFERS; ++i) {
      if (m_imageMemoryAddr[i] != nullptr) {
        is_FreeImageMem(m_hCam, m_imageMemoryAddr[i], m_imageMemoryId[i]);
      }
    }

    // Close the camera handle
    is_ExitCamera(m_hCam);
    m_hCam = 0;
  }
}

bool IDSVideoCapture::isOpened() const
{
  return m_hCam > 0;
}

bool IDSVideoCapture::grab()
{
  if (isOpened()) {

    // Unlock image memory to allow the API to write to the previously
    // locked buffer
    if (m_lockedMemory != nullptr) {
      is_UnlockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, m_lockedMemory);
    }

    // Grab the last (and not current) image buffer
    is_GetActSeqBuf(m_hCam, nullptr, nullptr, &m_lockedMemory);
    is_LockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, m_lockedMemory);

    return true;
  }
  else {
    return false;
  }
}

bool IDSVideoCapture::retrieve(cv::OutputArray image, int flag)
{
  if (isOpened()) {
    // Initialize Mat using locked memory and then duplicate into output
    cv::Mat ref(m_height, m_width, CV_8UC3, (void *)m_lockedMemory);
    ref.copyTo(image);
    return true;
  }
  else {
    return false;
  }
}

double IDSVideoCapture::get(int propid) const
{
  using namespace cv;

  switch (propid) {

  case CAP_PROP_POS_MSEC: {
    return (double)(time(nullptr) - m_startTime) * 1e2;
  }

  case CAP_PROP_POS_FRAMES: {
    long count;
    is_GetVsyncCount(m_hCam, &count, nullptr);
    return (double)count;
  }

  case CAP_PROP_POS_AVI_RATIO:
    return 0.0;

  case CAP_PROP_FRAME_WIDTH:
    return m_width;

  case CAP_PROP_FRAME_HEIGHT:
    return m_height;

  case CAP_PROP_FPS: {
    double fps;
    is_GetFramesPerSecond(m_hCam, &fps);
    return fps;
  }

  case CAP_PROP_FOURCC:
    return CAP_FFMPEG;

  case CAP_PROP_FORMAT:
    return CV_8UC3;
  }

  return 0.0;
}

bool IDSVideoCapture::set(int propId, double value)
{
  using namespace cv;

  IS_SIZE_2D imageSize;
  is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void *)&imageSize, sizeof(imageSize));

  switch (propId) {

  case CAP_PROP_FRAME_WIDTH:
    imageSize = {.s32Width = (int)value, .s32Height = m_height};
    is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void *)&imageSize, sizeof(imageSize));
    return true;

  case CAP_PROP_FRAME_HEIGHT:
    imageSize = {.s32Width = m_width, .s32Height = (int)value};
    is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void *)&imageSize, sizeof(imageSize));
    return true;
  }

  return false;
}

bool IDSVideoCapture::Configure(HIDS hCam)
{
  INT  nRet = IS_SUCCESS;

  UINT pixelClock = 118;
  nRet = is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void *)&pixelClock, sizeof(pixelClock));
  if (nRet != IS_SUCCESS)
    return false;

  nRet = is_PixelClock(hCam, IS_PIXELCLOCK_CMD_GET, (void *)&pixelClock, sizeof(pixelClock));

  double dExposure = 26.129135;
  nRet = is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void *)&dExposure, sizeof(dExposure));
  if (nRet != IS_SUCCESS)
    return false;

  nRet = is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void *)&dExposure, sizeof(dExposure));

  double dDummy;
  nRet = is_SetFrameRate(hCam, 25.0, &dDummy);

// Enable auto gain control:

  double dEnable = 1;
  nRet = is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_GAIN, &dEnable, 0);

  // Set brightness setpoint to 128:
  double nominal = 128;
  nRet = is_SetAutoParameter(hCam, IS_SET_AUTO_REFERENCE, &nominal, 0);

  // Return shutter control limit:
  double maxShutter;
  nRet = is_SetAutoParameter(hCam, IS_GET_AUTO_SHUTTER_MAX, &maxShutter, 0);

  //auto nRet = is_ParameterSet(hCam, IS_PARAMETERSET_CMD_LOAD_FILE, (void *)LR"(C:\1\ids_settings.ini)", NULL);
  //if (nRet != IS_SUCCESS)
  //  return false;

  return true;
}

void IDSVideoCapture::Experiments()
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
  nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void *)dblRange, sizeof(dblRange));
  auto dMin = dblRange[0];
  auto dMax = dblRange[1];
  auto dInt = dblRange[2];

  nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void *)&dMax, sizeof(dMax));

  // and remember the value for later use
  double m_ExposureTime;
  nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void *)&m_ExposureTime, sizeof(m_ExposureTime));

// Load parameters from file
  //nRet = is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, (void *)LR"(C:\1\ids_settings.ini)", NULL);
}
