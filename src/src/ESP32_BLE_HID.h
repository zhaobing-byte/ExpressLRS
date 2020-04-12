#pragma once
#include "CRSF.h"
#include "BleGamepad.h"

extern BleGamepad bleGamepad;

extern CRSF crsf;

void initBLEHID();

void updateBLEHID();