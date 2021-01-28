#pragma once

#include <variant>
#include <opencv2/opencv.hpp>
#include <libs/CommonLib/Guid.h>
#include "TraceyObj.h"

TRACEYOBJ("5ddc9e4f-5c44-473b-b45b-a9f45ba7402e")
ITraceyConfig : public IUnknown
{
  using Key  = Guid;
  using Data = std::variant<IUnknown *, bool, int, double, std::string, cv::Size2d, cv::Size2i>;

  virtual const Data *GetConfigData(const Key &key) const       = 0;
  virtual int         OnDataChanged(std::function<void()> func) = 0;
  virtual void        CancelNotifications(int id)               = 0;
};

TRACEYOBJ("5ddc9e4f-5c44-473b-b45b-a9f45ba7402e")
ITraceyConfigWriter : public IUnknown
{
  virtual void SetConfigData(const ITraceyConfig::Key &key, const ITraceyConfig::Data &com) = 0;
};
