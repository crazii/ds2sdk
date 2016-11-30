// makeplug.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void Plugin_encrypt(long _Offset, DWORD _Key, PDWORD InDataBuf, PDWORD OutDataBuf)
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
		OutDataRotate = _lrotr(OutData, 15);
		Offset += 0x561A9C1A;
		OutDataBuf[i] = OutData;
	}
}

int mm_Plugin(TCHAR *binName, TCHAR *firmwareName, TCHAR *plgName, unsigned long Addr1, unsigned long Addr2)
{
	int Result;
	FILE *fp_in;
	FILE *fp_out;
	FILE *fp_firmware;	
	long binSize;
	long firmwareSize;
	DWORD inData[128];
	DWORD outData[128];
		
	fp_in = _tfopen(binName, TEXT("rb"));
	if (fp_in)
	{
		fp_firmware = _tfopen(firmwareName, TEXT("rb"));
		if (fp_firmware)
		{
			fp_out = _tfopen(TEXT(".0temp0"), TEXT("wb"));
			if (fp_out)
			{
				fseek(fp_in, 0, SEEK_END);
				binSize = ftell(fp_in);
				fseek(fp_firmware, 0, SEEK_END);
				firmwareSize = ftell(fp_firmware);
				fseek(fp_in, 0, SEEK_SET);
				fseek(fp_firmware, 0, SEEK_SET);

				memset(inData, 0, sizeof(inData));				
				inData[0] = 0x8B12BAB6;
				inData[1] = 0x93D7DE9B;
				inData[2] = 0xCDD8D0D2;
				inData[3] = 0x7E5DEB16;
				inData[4]  = 0x200;
				inData[5]  = binSize;
				inData[6]  = Addr1;
				inData[7]  = Addr2;
				inData[8]  = (binSize + 0x3FF) & 0xFFFFFE00;
				inData[9]  = firmwareSize;
				fwrite(inData, 1, sizeof(inData), fp_out);

				while (TRUE)
				{
					size_t n = fread(outData, 1, sizeof(outData), fp_in);
					if (!n)
						break;
					fwrite(outData, 1, n, fp_out);
				}

				long patSize = inData[8] - ftell(fp_out);
				memset(outData, 0, sizeof(outData));
				fwrite(outData, 1, patSize, fp_out);
				
				while (TRUE)
				{
					size_t n = fread(outData, 1, sizeof(outData), fp_firmware);
					if (!n)
						break;
					fwrite(outData, 1, n, fp_out);
				}
				fclose(fp_in);
				fclose(fp_firmware);
				fclose(fp_out);

				fp_in = _tfopen(TEXT(".0temp0"), TEXT("rb"));
				if (fp_in)
				{
					fp_out = _tfopen(plgName, TEXT("wb"));
					if (fp_out)
					{
						fseek(fp_in, 0, SEEK_SET);
						fseek(fp_out, 0, SEEK_SET);
						srand((unsigned int) time(0));
						int Seed = rand() ^ 0x79FA8917;
						while (TRUE)
						{
							long in_pos = ftell(fp_in);
							DWORD Key = Seed ^ ((in_pos >> 16) + in_pos + (in_pos >> 8) + (in_pos >> 24));
							memset(inData, 0, sizeof(inData));
							size_t n = fread(inData, 1, sizeof(inData), fp_in);
							if (!n)
								break;
							Plugin_encrypt(in_pos, Key, inData, outData);
							fwrite(outData, 1, n, fp_out);
						}
						fclose(fp_in);
						fclose(fp_out);
						_tunlink(TEXT(".0temp0"));
						Result = 0;
					}
					else
					{
						fclose(fp_in);
						Result = -5;
					}
				}
				else
				{
					Result = -4;
				}
			}
			else
			{
				fclose(fp_in);
				fclose(fp_firmware);
				Result = -3;
			}
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
	TCHAR firmwareName[MAX_PATH];
	TCHAR binName[MAX_PATH];
	TCHAR plgName[MAX_PATH];

	_tprintf(TEXT("Start\n"));

	if (argc == 3)
	{
		_tcscpy(binName, argv[1]);
		_tcscpy(plgName, argv[2]);

		const TCHAR *pExtName = _tcsrchr(plgName, TEXT('.'));

		if (pExtName)
		{
			if (_tcsicmp(pExtName, TEXT(".plg")))
			{
				pExtName = NULL;
				_tcscat(plgName, TEXT(".plg"));
			}
		}
		else
		{
			_tcscat(plgName, TEXT(".plg"));
		}
#if !defined(_DEBUG)
		_tcscpy(firmwareName, argv[0]);
		TCHAR *pSlash = _tcsrchr(firmwareName, TEXT('\\'));
		if (pSlash)
			_tcscpy(pSlash+1, TEXT("ds2_firmware.dat"));
		else
#endif
			_tcscpy(firmwareName, TEXT("ds2_firmware.dat"));

		int ErrRet = mm_Plugin(binName, firmwareName, plgName, 0x80002000, 0x80002000);

		switch (ErrRet)
		{
		case -1:
			_tprintf(TEXT("can't open: %s\n"), binName);
			break;
		case -2:
			_tprintf(TEXT("can't open: %s\ncheck if it on the same directory as makeplug program\n"), firmwareName);
			break;
		case -3:
			_tprintf(TEXT("can't creat temporary file, dose the disk have enough space?\n"));
			break;
		case -4:
			_tprintf(TEXT("Strange? a temporary file had created, but can't open now...\n"));
			break;
		case -5:
			_tprintf(TEXT("can't creat output file: %s\n"), plgName);
			break;
		default:			
			_tprintf(TEXT("generate output file: %s success\n"), plgName);
			break;
		}
	}
	else
	{
		_tprintf(TEXT("argument error...\n"));
		_tprintf(TEXT("correct formate is: makeplug source output.plg\n"));
		_tprintf(TEXT("\"output\" is your wanted name\n"));
		_tprintf(TEXT("and make sure \"ds2_firmware.dat\" on the same directory as makeplug\n"));
	}

	return 0;
}

