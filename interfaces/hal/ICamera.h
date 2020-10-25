#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include <libs/CommonLib/Guid.h>
#include <interfaces/TraceyObj.h>

namespace hal {
  TRACEYOBJ("a8b10028-92bb-4737-9ba6-89b686b4a5d0") ICamera : public IUnknown
  {
    //! Loads a recently captured image
    //*** VERY IMPORTANT: Don't call this while holding a mutex lock
    virtual bool LoadImage() = 0;

    //! Returns the most recent loaded image
    virtual bool GetImage(cv::Mat & mat) const = 0;

    //! Returns true, if the device is connected
    virtual bool Connected() const = 0;

    //! Returns the current camera power setting
    virtual bool Power() const = 0;

    //! Sets the camera power on/off
    virtual void Power(bool power) = 0;

    struct SETTINGS
    {
      bool color;
      SIZE offset;
      int  brightness;
      int  contrast;
      int  hue;
      int  saturation;
    };

    //! Set/Get Settings
    virtual SETTINGS Settings() const                   = 0;
    virtual void     Settings(const SETTINGS &settings) = 0;
  };

  TRACEYOBJ("2e50fcd1-7ad8-4bb8-a657-2ca15e5140ce") ICameraHires : public IUnknown
  {
    inline static auto BRIGHTNESS = Guid("0e96163a-47f5-4d30-8f50-c59cf53de05e");
    inline static auto CONTRAST   = Guid("9cc9f2f2-e56e-4093-a368-d3cd482efe13");
    inline static auto HUE        = Guid("272378a6-ad4b-432b-844f-a4ca868b734b");
    inline static auto SATURATION = Guid("b0c0c843-4b1b-41fd-909d-e933082ac1ac");
    inline static auto GAIN       = Guid("1de0f978-4880-4351-ada6-61b41137316d");
    inline static auto EXPOSURE   = Guid("4f87a3ef-e690-484d-aa41-414048924ac7");

    enum class Mode {HIRES, LORES};

    //! Initialization
    virtual void Initialize(IUnknown * unk) = 0;

    //! Starts Capture
    virtual bool StartCapture(std::function<void(cv::Mat)> notify) = 0;
    virtual void StopCapture()                                     = 0;

    //! Returns the most recent captured image, or reads an image from the camera
    virtual bool GetImage(cv::Mat & mat, Mode mode = Mode::HIRES) const = 0;

    //! Returns true, if the device is connected
    virtual bool Connected(double *fps = nullptr) const = 0;
  };
} // namespace hal