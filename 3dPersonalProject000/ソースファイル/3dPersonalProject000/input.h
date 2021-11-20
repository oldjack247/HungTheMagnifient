#ifndef _INPUT_H_
#define _INPUT_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//====================================================================
// インプットクラス
//====================================================================
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pInputDevice;

};

#endif // !_INPUT_H_
