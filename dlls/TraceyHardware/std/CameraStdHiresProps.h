#pragma once

class CameraStdHiresProps
{
public:
  void Initialize();

  static bool HasStdHiresCamera()
  {
    CameraStdHiresProps c;
    c.Initialize();
    return c.GetDeviceId() >= 0;
  }

  int GetDeviceId() const
  {
    return m_deviceId;
  }

  SIZE GetResolution() const
  {
    return m_nativeResolution;
  }

  double TranslatePropery(int propId, int propValue) const;

private:
  using Range     = std::pair<double, double>;
  using PropRange = std::map<int, Range>;

  int       m_deviceId{-1};
  SIZE      m_nativeResolution{};
  PropRange m_propRange;

  static auto GetCameraList() -> std::vector<std::string>;
};
