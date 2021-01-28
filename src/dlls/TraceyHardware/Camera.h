#pragma once

class CameraImpl;

class Camera : public Implements<hal::ICamera>
{
public:
  Camera(THW *thw);

private:
  THW *              m_thw;
  static CameraImpl *m_pimpl;

  // ICamera
  bool     LoadImage() override;
  bool     GetImage(cv::Mat &mat) const override;
  bool     Connected() const override;
  bool     Power() const override;
  void     Power(bool power) override;
  SETTINGS Settings() const override;
  void     Settings(const SETTINGS &settings) override;
};
