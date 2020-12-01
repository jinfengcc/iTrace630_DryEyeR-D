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

  HIDS index = _index;

  // Release any existing resources.
  release();

  // Ask API to open camera.
  if (is_InitCamera(&index, nullptr) == IS_SUCCESS) {

    // If success set the camera ID field and fetch camera and sensor info
    m_hCam = index;
    is_GetCameraInfo(m_hCam, &m_cameraInfo);
    is_GetSensorInfo(m_hCam, &m_sensorInfo);

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
