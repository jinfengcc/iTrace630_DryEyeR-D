#include "pch.h"
#include "CameraHiresUtil.h"
#include <comutil.h>
#include <Dshow.h>

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "comsuppw.lib")

namespace hrc {
  void DumpSettings(const char *title, cv::VideoCapture &vc)
  {
    static std::array names = {
      "CAP_PROP_POS_MSEC            ", "CAP_PROP_POS_FRAMES          ", "CAP_PROP_POS_AVI_RATIO       ", "CAP_PROP_FRAME_WIDTH         ",
      "CAP_PROP_FRAME_HEIGHT        ", "CAP_PROP_FPS                 ", "CAP_PROP_FOURCC              ", "CAP_PROP_FRAME_COUNT         ",
      "CAP_PROP_FORMAT              ", "CAP_PROP_MODE                ", "CAP_PROP_BRIGHTNESS          ", "CAP_PROP_CONTRAST            ",
      "CAP_PROP_SATURATION          ", "CAP_PROP_HUE                 ", "CAP_PROP_GAIN                ", "CAP_PROP_EXPOSURE            ",
      "CAP_PROP_CONVERT_RGB         ", "CAP_PROP_WHITE_BALANCE_BLUE_U", "CAP_PROP_RECTIFICATION       ", "CAP_PROP_MONOCHROME          ",
      "CAP_PROP_SHARPNESS           ", "CAP_PROP_AUTO_EXPOSURE       ", "CAP_PROP_GAMMA               ", "CAP_PROP_TEMPERATURE         ",
      "CAP_PROP_TRIGGER             ", "CAP_PROP_TRIGGER_DELAY       ", "CAP_PROP_WHITE_BALANCE_RED_V ", "CAP_PROP_ZOOM                ",
      "CAP_PROP_FOCUS               ", "CAP_PROP_GUID                ", "CAP_PROP_ISO_SPEED           ", "CAP_PROP_BACKLIGHT           ",
      "CAP_PROP_PAN                 ", "CAP_PROP_TILT                ", "CAP_PROP_ROLL                ", "CAP_PROP_IRIS                ",
      "CAP_PROP_SETTINGS            ", "CAP_PROP_BUFFERSIZE          ", "CAP_PROP_AUTOFOCUS           ", "CAP_PROP_SAR_NUM             ",
      "CAP_PROP_SAR_DEN             ", "CAP_PROP_BACKEND             ", "CAP_PROP_CHANNEL             ", "CAP_PROP_AUTO_WB             ",
      "CAP_PROP_WB_TEMPERATURE      ", "CAP_PROP_CODEC_PIXEL_FORMAT  ", "CAP_PROP_BITRATE             ",
    };

    DILASCIA_TRACE_EX("CAMERA", fmt::format("------- {} -----------\n", title).c_str());
    for (int i = 0; i < 47; ++i) {
      auto x = vc.get(i);
      DILASCIA_TRACE_EX("CAMERA", fmt::format("{} = {}\n", names[i], (int)x).c_str());
    }
  }

  std::vector<std::string> GetCameraList();

  int GetDeviceId(std::span<const char *> names)
  {
    if (names.empty())
      return -1;

    try {
      auto cameras = GetCameraList();

      for (auto name : names) {
        auto pred = [name](const auto &w) { return _stricmp(w.c_str(), name) == 0; };
        if (auto i = std::find_if(cameras.begin(), cameras.end(), pred); i != cameras.end())
          return std::distance(cameras.begin(), i);
      }
    }
    catch (std::exception &) {
    }

    return -1;
  }

  std::string ReadStr(IPropertyBag *pPropBag, const wchar_t *name)
  {
    variant_t var;
    if (FAILED(pPropBag->Read(name, &var, nullptr)))
      return {};
    var.ChangeType(VT_BSTR);
    if (var.vt != VT_BSTR)
      return {};
    return utf8_convert((wchar_t *)var.bstrVal);
  }

  std::vector<std::string> GetCameraList()
  {
    wil::com_ptr_t<ICreateDevEnum> pDevEnum;

    auto hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, pDevEnum.put_void());
    TRACEY_THROW_IF(hr != S_OK || !pDevEnum, "Unable to create 'ICreateDevEnum'");

    wil::com_ptr_t<IEnumMoniker> pEnum;
    pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, pEnum.put(), 0);
    TRACEY_THROW_IF(!pEnum, "Unable to create VideoDevice enumerator");

    std::vector<std::string> devList;

    wil::com_ptr_t<IMoniker> pMoniker;
    while (pEnum->Next(1, pMoniker.put(), nullptr) == S_OK) {
      wil::com_ptr_t<IPropertyBag> pPropBag;
      hr = pMoniker->BindToStorage(nullptr, nullptr, IID_IPropertyBag, pPropBag.put_void());
      TRACEY_THROW_IF(hr != S_OK || !pPropBag, "Unable to create 'IPropertyBag'");

      if (auto str = ReadStr(pPropBag.get(), L"FriendlyName"); !str.empty())
        devList.push_back(str);
    }

    return devList;
  }
} // namespace hrc
