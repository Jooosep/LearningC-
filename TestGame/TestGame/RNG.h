#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <bcrypt.h>
#include <string>
#include <iostream>

#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)

#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)

class RNG
{
public:
	RNG();
	UINT rng();
};