#include "TextClass.h"

TextClass::TextClass() {
}

bool TextClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, FileSrc, CustomRect);

	return true;
}

void TextClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

}