#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define NUM_KEY_MAX (256)//キーの最大数

//=============================================================================
//クラス
//=============================================================================
class CInputKeyboard :public CInput
{//派生クラス
public:
	CInputKeyboard();								//コンストラクタ	
	~CInputKeyboard();								//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	bool GetKeyboardPress(int nKey);				//押し続けているとき
	bool GetKeyboardTrigger(int nKey);				//押したとき
	bool GetKeyboardRelease(int nKey);				//離したとき
private:
	static BYTE m_aKeyState[NUM_KEY_MAX];			//キーボードの情報入力ワーク
	static BYTE m_aKeyStateRelease[NUM_KEY_MAX];	//キーボードのリリース情報
	static BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
};
#endif