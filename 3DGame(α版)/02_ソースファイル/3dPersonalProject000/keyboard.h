#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//====================================================================
// インクルードファイル
//====================================================================
#include "main.h"
#include "input.h"

//====================================================================
// マクロ定義
//====================================================================
#define NUM_KEY_MAX (256)//キーボードの最大数

//====================================================================
// キーボードクラス
//====================================================================
class CInputKeyboard :public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyPress(int nKey);
	bool GetKeyTrigger(int nKey);
	bool GetKeyRelease(int nKey);
private:
	BYTE m_akeyState[NUM_KEY_MAX]; //キーボードの入力情報ワーク
	BYTE m_akeyStateTrigger[NUM_KEY_MAX];
	BYTE m_akeyStateRelease[NUM_KEY_MAX];
};

#endif