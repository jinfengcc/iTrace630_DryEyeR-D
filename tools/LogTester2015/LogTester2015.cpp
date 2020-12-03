// LogTester2015.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "LogFile.h"

int main()
{
  LOG("aaaa");

  LOG("abc = %d", 1);
  LOG(L"Wide = %d", 1);

  LOG("LogTester2015.cpp : This file contains the 'main' function. Program execution begins and ends there. LogTester2015.cpp : This file contains the 'main' function. Program execution begins and ends there. LogTester2015.cpp : This file contains the 'main' function. Program execution begins and ends there. = %d", 1);
}
