#include <windows.h>
#include "GameWin.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{

	CGameWin gameWin(hInstance);

	if (!gameWin.InitGame())
		return 0;

	return gameWin.BeginGame();
}