#include "PlainTerrain.h"
#include <windows.h>
#include <io.h>
#include "debug.h"

void
CPlainTerrain::SetInitHM(char* pInitFile)
{
	HANDLE		hFile;
	unsigned char							HM[32][32];
	VERIFY_FATAL(!_access(pInitFile, 0x04), "Can't Acces to texture");
	VERIFY_FATAL(((hFile= CreateFile(pInitFile, GENERIC_READ, 0, 0, OPEN_EXISTING,0,0)) != INVALID_HANDLE_VALUE), "Can't Open File For Reading");

	unsigned long dwRead;
	::ReadFile(hFile, HM, 32*32, &dwRead, NULL);
	
	VERTEX3*				pV= m_vb.Lock();
	unsigned char*	pHM= (unsigned char*)HM;
	for(int i= 0; i < 32; ++i){for(int j= 0; j < 32; ++j){
		pV->orig[2]+= (*pHM);
		++pHM; ++pV;
	}}
}

