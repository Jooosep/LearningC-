#include "RNG.h"

RNG::RNG() {};

UINT RNG::rng()
{
	BCRYPT_ALG_HANDLE       hAlg = NULL;
	NTSTATUS                status = STATUS_UNSUCCESSFUL;
	UINT                    rand = 0;


	//open an algorithm handle
	if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(
		&hAlg,
		BCRYPT_RNG_ALGORITHM,
		NULL,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptOpenAlgorithmProvider\n", status);
	}

	BCryptCloseAlgorithmProvider(hAlg, 0);
	return rand;


}