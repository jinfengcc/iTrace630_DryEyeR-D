#pragma once

#include "DatabaseJet.h"

//template <class T, class U>
//T Cast(const U &u) = delete;

//template <>
//db::GROUP Cast<db::GROUP, TDB_GROUP>(const TDB_GROUP &u)
//{
//  return {u.GroupID, u.Name};
//}
//template <>
//TDB_GROUP Cast<TDB_GROUP, db::GROUP>(const db::GROUP &u)
//{
//  TDB_GROUP t{u.GroupID};
//  strcpy_s(t.Name, u.Name.c_str());
//
//  return t;
//}

//template <>
//db::ADDRESS Cast<db::ADDRESS, TDB_ADDRESS>(const TDB_ADDRESS &u)
//{
//  return {u.Line1, u.Line2, u.City, u.State, u.ZIP, u.Country};
//}
//template <>
//TDB_ADDRESS Cast<TDB_ADDRESS, db::ADDRESS>(const db::ADDRESS &u)
//{
//  TDB_ADDRESS a{};
//  strcpy_s(a.Line1, u.Line1.c_str());
//  strcpy_s(a.Line2, u.Line2.c_str());
//  strcpy_s(a.City, u.City.c_str());
//  strcpy_s(a.State, u.State.c_str());
//  strcpy_s(a.ZIP, u.ZIP.c_str());
//  strcpy_s(a.Country, u.Country.c_str());
//  return a;
//}

//template <>
//db::CLINIC Cast<db::CLINIC, TDB_CLINIC>(const TDB_CLINIC &u)
//{
//  return {u.ClinicID, u.Name, Cast<db::ADDRESS>(u.Address)};
//}
//
//template <>
//TDB_CLINIC Cast<TDB_CLINIC, db::CLINIC>(const db::CLINIC &u)
//{
//  TDB_CLINIC clinic{u.ClinicID};
//  strcpy_s(clinic.Name, u.Name.c_str());
//  clinic.Address = Cast<TDB_ADDRESS>(u.Address);
//  return clinic;
//}

//template <class T, class U>
//std::vector<T> Cast(const std::vector<U> &u)
//{
//  std::vector<T> v;
//  for (auto &i : u)
//    v.push_back(Cast<T, U>(i));
//  return v;
//}
