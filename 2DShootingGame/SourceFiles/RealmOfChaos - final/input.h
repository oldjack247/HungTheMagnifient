#ifndef _INPUT_H_
#define _INPUT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
//クラス
//=============================================================================
class CInput
{//基本クラス
public:
	CInput();												//コンストラクタ
	virtual ~CInput() = 0;									//デストラクタ
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化処理
	virtual void Uninit(void);								//終了処理
	virtual void Update(void);								//更新処理
	LPDIRECTINPUT8 GetInput(void) { return m_pInput; }		//情報取得用
protected:
	static LPDIRECTINPUTDEVICE8 m_pDevice;
	static LPDIRECTINPUT8 m_pInput;							//Directinputオブジェクトへのポインタ
private:

};
#endif

