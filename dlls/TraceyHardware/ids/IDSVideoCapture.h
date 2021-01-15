#pragma once

#include <opencv2/videoio.hpp>
#include "uEye.h"

/*
  Extends OpenCV's VideoCapture to fetch camera frames from an IDS uEye
  camera.
*/
class IDSVideoCapture : public cv::VideoCapture
{
public:
  IDSVideoCapture();
  IDSVideoCapture(int index);
  virtual ~IDSVideoCapture();

  bool open(int index, int apiPreference = cv::CAP_ANY) override;
  void release() override;
  bool isOpened() const override;
  bool grab() override;
  bool retrieve(cv::OutputArray image, int flag = 0) override;

  /*
    Get camera property. Implemented values are:
    -	`CAP_PROP_POS_MSEC`
    -	`CAP_PROP_POS_FRAMES`
    -	`CAP_PROP_POS_AVI_RATIO`
    -	`CAP_PROP_FRAME_WIDTH`
    -	`CAP_PROP_FRAME_HEIGHT`
    -	`CAP_PROP_FPS`
    -	`CAP_PROP_FOURCC`
    -	`CAP_PROP_FORMAT`
  */
  double get(int propid) const override;

  /*
    WARNING: not implemented
  */
  bool set(int propId, double value) override;

private:
  struct Memory
  {
    char *pcImageMemory{};
    INT   lMemoryId{};
    INT   lSequenceId{};
  };
  using MemVector = std::array<Memory, 3>;

  HIDS       m_hCam = 0;
  CAMINFO    m_cameraInfo;
  SENSORINFO m_sensorInfo;
  int        m_width = 0;
  int        m_height = 0;
  time_t     m_startTime;
  MemVector  m_memVector;
  double     m_exposureRange[3];

  // Time stamp for camera recording start.
  // char *     m_lockedMemory                 = nullptr;   // Pointer to currently locked frame.
  // char *     m_imageMemoryAddr[NUM_BUFFERS] = {nullptr}; // Pointers to all buffers in memory that are given to uEye API.
  // int        m_imageMemoryId[NUM_BUFFERS];               // API references to image memory.

  bool  Configure(HIDS hCam);
  bool  AllocateImgMem(int sizeX, int sizeY, int bitsPerPixel = 3 * 8);
  void  FreeImgMem();
  char *GetLastMem() const;
  bool  GetLastMem(char **ppLastMem, INT &lMemoryId, INT &lSequenceId) const;
  void  Experiments();
};
