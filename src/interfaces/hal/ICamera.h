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
    inline static auto EXPOSURE   = Guid("4f87a3ef-e690-484d-aa41-414048924ac7");
    inline static auto GAIN       = Guid("1de0f978-4880-4351-ada6-61b41137316d");
    inline static auto GAIN_RED   = Guid("272378a6-ad4b-432b-844f-a4ca868b734b");
    inline static auto GAIN_GREEN = Guid("b0c0c843-4b1b-41fd-909d-e933082ac1ac");
    inline static auto GAIN_BLUE  = Guid("b0c0c843-4b1b-41fd-909d-e933082ac1ac");
    //inline static auto HUE        = Guid("d9a7d06f-315d-46ce-9dbc-4e6485a2bb22");  // Unused
    //inline static auto SATURATION = Guid("80ad3417-b6f5-403f-a9a4-b2d6299908b7");  // Unused

    // aliases
    inline static auto BRIGHTNESS = EXPOSURE;
    inline static auto CONTRAST   = GAIN;

    enum class Mode {
      HIRES_COLOR,
      LORES_COLOR,
      HIRES_GRAY,
      LORES_GRAY,
    };

    //! Initialization
    virtual void Initialize(IUnknown *unk) = 0;

    //! Connect/Disconnect (returns the previous state
    virtual bool Connect(bool yes) = 0;

    //! Starts/Stops Capture
    using Callback = std::function<void(unsigned)>;

    virtual void StartCapture(Callback notify) = 0;
    virtual void StopCapture()                 = 0;

    //! Starts Frame transfer (simulate old camera driver)
    virtual void StartFrameTransfer() = 0;
    virtual void StopFrameTransfer()  = 0;

    //! Returns the most recent captured image
    virtual bool GetImage(cv::Mat & mat, Mode mode) const = 0;

    //! Returns true, if the device is connected
    virtual bool Connected(double *fps = nullptr) const = 0;
  };
} // namespace hal
