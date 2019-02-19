#include "D3DXHeader.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszClass, int nCmdShow) {
	if (SystemClass::GetInst()->Init()) {
		SystemClass::GetInst()->Run();
	}
	SystemClass::GetInst()->Shutdown();

	return 0;
}