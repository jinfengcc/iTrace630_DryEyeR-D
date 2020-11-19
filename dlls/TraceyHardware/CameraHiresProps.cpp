#include "pch.h"
#include "CameraHiresProps.h"
#include <comutil.h>
#include <Dshow.h>

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "comsuppw.lib")

namespace {
  struct PropRange
  {
    int    id;
    double minValue;
    double maxValue;
  };

  struct CamProp
  {
    const char *           name;
    int                    cx, cy;
    std::vector<PropRange> props;
  };

  // clang-format off
  const CamProp camProps[] = {
    {
      "UVC Video Device", 1600, 1200,
      {
        { cv::CAP_PROP_BRIGHTNESS,     0, 256},  // Range [   0, 256]
        { cv::CAP_PROP_CONTRAST  ,     0,  64},  // Range [   0,  64]
        { cv::CAP_PROP_HUE       ,  -128, 128},  // Range [-180, 180]
        { cv::CAP_PROP_SATURATION,     0,  20},  // Range [   0,  20]
        { cv::CAP_PROP_GAIN      ,     0, 100},  // Range [   0, 100]
      }
    },
    {
      "HD USB Camera", 1280, 960,
      {
        { cv::CAP_PROP_BRIGHTNESS,   -64,   64},  // Range [  -64,   64]
        { cv::CAP_PROP_CONTRAST  ,     0,   95},  // Range [    0,   95]
        { cv::CAP_PROP_HUE       ,  -128,  128},  // Range [-2000, 2000]
        { cv::CAP_PROP_SATURATION,     0,  128},  // Range [    0,  128]
        { cv::CAP_PROP_GAIN      ,     0,  100},  // Range [    0,  100]
      }
    }
  };
  // clang-format on

} // namespace

void CameraHiResProps::Initialize()
{
  try {
    m_propRange.clear();

    auto cameras = GetCameraList();

    for (auto p : camProps) {
      auto pred = [p](const auto &w) { return _stricmp(w.c_str(), p.name) == 0; };
      if (auto i = std::find_if(cameras.begin(), cameras.end(), pred); i != cameras.end()) {
        m_deviceId = std::distance(cameras.begin(), i);
        m_nativeResolution.cx = p.cx;
        m_nativeResolution.cy = p.cy;
        for (auto &k : p.props)
          m_propRange[k.id] = std::make_pair(k.minValue, k.maxValue);
      }
    }
  }
  catch (std::exception &) {
    m_deviceId = -1;
  }
}

double CameraHiResProps::TranslatePropery(int propId, int propValue) const
{
  double x = propValue;

  if (auto i = m_propRange.find(propId); i != m_propRange.end()) {
    auto [minValue, maxValue] = i->second;

    x = minValue + (x / 256.0) * (maxValue - minValue);
  }
  return std::round(x);
}

std::vector<std::string> CameraHiResProps::GetCameraList()
{
  auto ReadStr = [](IPropertyBag *pPropBag, const wchar_t *name) -> std::string {
    variant_t var;
    if (FAILED(pPropBag->Read(name, &var, nullptr)))
      return {};
    var.ChangeType(VT_BSTR);
    if (var.vt != VT_BSTR)
      return {};
    return utf8_convert((wchar_t *)var.bstrVal);
  };

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
