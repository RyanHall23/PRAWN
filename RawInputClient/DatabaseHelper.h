#pragma once

#include "Persistence.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <sqlext.h>
#include <string>

class CDatabaseHelper
{
public:
    CDatabaseHelper();
    ~CDatabaseHelper();
    bool UpdateDatabase(std::string strRegistrationPlate, std::string strOffence, std::string strLocation, std::string strDBLocation);
    bool TestDatabase();
private:
    CPersistence::DeviceProperties pDevProp;
};

