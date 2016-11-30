// decryptplug.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Plugin_decrypt(long _Offset, DWORD _Key, PDWORD InDataBuf, PDWORD OutDataBuf)
{
	DWORD Offset;
	DWORD OutData;
	DWORD Key;
	DWORD OutDataRotate;

	Offset = _Offset;
	Key = _Key;
	Offset += _lrotr(Offset, 7);
	Offset += _lrotr(Offset, 2);
	OutDataRotate = 0;

	for (int i=0; i<128; i++)
	{
		OutData = InDataBuf[i] ^ OutDataRotate ^ Key ^ Offset;
		Key = _lrotr(Key, 10);
		OutDataRotate = _lrotr(InDataBuf[i], 15);
		Offset += 0x561A9C1A;
		OutDataBuf[i] = OutData;
	}
}

int mm_Plugin(TCHAR *plugName, TCHAR *rawName)
{
	int Result;
	DWORD Seed;
	DWORD inData[128];
	DWORD outData[128];
	FILE *fp_in;
	FILE *fp_out;

	fp_in = _tfopen(plugName, TEXT("rb"));
	if (fp_in)
	{
		fp_out = _tfopen(rawName, TEXT("wb"));
		if (fp_out)
		{
			fread(&Seed, 1, sizeof(Seed), fp_in);
			fseek(fp_in, 0, SEEK_SET);
			Seed ^= 0x8B12BAB6;

			while (TRUE)
			{
				long in_pos = ftell(fp_in);
				DWORD Key = Seed ^ ((in_pos >> 16) + in_pos + (in_pos >> 8) + (in_pos >> 24));
				memset(inData, 0, sizeof(inData));
				size_t n = fread(inData, 1, sizeof(inData), fp_in);
				if (!n)
					break;
				Plugin_decrypt(in_pos, Key, inData, outData);
				fwrite(outData, 1, n, fp_out);
			}
			fclose(fp_in);
			fclose(fp_out);
			Result = 0;
		}
		else
		{
			fclose(fp_in);
			Result = -2;
		}
	}
	else
	{
		Result = -1;
	}

	return Result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int ErrRet = 0;
	if (argc == 3)
	{
		ErrRet = mm_Plugin(argv[1], argv[2]);
		switch (ErrRet)
		{
		case -1:
			_tprintf(TEXT("can't open: %s\n"), argv[1]);
			break;
		case -2:
			_tprintf(TEXT("can't create: %s\n"), argv[2]);
			break;
		default:			
			_tprintf(TEXT("generate output file: %s success\n"), argv[2]);
			break;
		}
	}
	else
	{
		_tprintf(TEXT("argument error...\n"));
		_tprintf(TEXT("correct formate is: decryptplug plug_source raw_output\n"));
	}

	return 0;
}

