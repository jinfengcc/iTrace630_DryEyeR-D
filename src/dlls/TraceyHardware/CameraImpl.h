#pragma once

class CameraImpl
{
public:
  using SETTINGS = hal::ICamera::SETTINGS;

  CameraImpl(THW *tdw);

  bool     Connected() const;
  bool     LoadImage();
  bool     GetImage(cv::Mat &mat) const;
  cv::Mat  GetEmptyImage() const;
  SETTINGS Settings() const;
  void     Settings(const SETTINGS &settings);

private:
  enum { IMG_ROWS = 468, IMG_COLS = 624 };
  cv::Mat3b m_image;
  cv::Mat3b m_empty;
  SETTINGS  m_settings{};

  bool    TransferFrame();
  bool    LoadFromFile(const char *fileName);
  cv::Mat ConvertColor();
  cv::Mat ConvertGray();

  // Threading
private:
  struct FRAME
  {
    BOOL             available;
    BOOL             color;
    thw::VIDEO_FRAME vframe{};
    cv::Mat          image;
  };

  FRAME m_frame;

private:
  // THW.dll interface
  typedef BOOL(_cdecl *TDllIsConnected)(void);
  typedef BOOL(_cdecl *TDllSetVideoBrightness)(int);
  typedef BOOL(_cdecl *TDllSetVideoContrast)(int);
  typedef BOOL(_cdecl *TDllSetVideoHue)(int);
  typedef BOOL(_cdecl *TDllSetVideoSaturation)(int);
  typedef BOOL(_cdecl *TDllGetMostRecentVideoFrame)(BOOL, int, int, BOOL *, thw::VIDEO_FRAME *);

  TDllIsConnected             m_connectedFunc;
  TDllSetVideoBrightness      m_setBrightnessFunc;
  TDllSetVideoContrast        m_setContrastFunc;
  TDllSetVideoHue             m_setHueFunc;
  TDllSetVideoSaturation      m_setSaturationFunc;
  TDllGetMostRecentVideoFrame m_getMostRecentFrameFunc;
};
