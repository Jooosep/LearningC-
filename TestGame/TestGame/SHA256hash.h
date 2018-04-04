#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <bcrypt.h>
#include <string>
#include <fstream>

#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)

#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)
class SHA256hash
{
public:

	SHA256hash();

	int hash(const std::string string);
};