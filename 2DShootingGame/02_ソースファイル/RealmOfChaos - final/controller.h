#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
// 外部変数
//=============================================================================
extern LPDIRECTINPUT8		pDInput;
extern LPDIRECTINPUTDEVICE8	pDIKeyboard;
extern BYTE					diKeyState[256];
extern LPDIRECTINPUTDEVICE8	pDIJoypad;
extern DWORD				diJoyState;
extern LPDIRECTINPUTDEVICE8	pDIMouse;
extern DIMOUSESTATE			diMouseState;

//*****************************************************************************
// Joypad用設定値
//*****************************************************************************
#define DEADZONE		(2500)			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		(10000)			// 有効範囲の最大値
#define RANGE_MIN		(-10000)			// 有効範囲の最小値
#define	MAX_CONTROLLER	(4)				//つなぐコントローラーの最大数 

//=============================================================================
//マクロ定義
//=============================================================================
#define UP     (0)	    //十字キー上
#define DOWN   (18000)	//十字キー下
#define LEFT   (27000)	//十字キー左
#define RIGHT  (9000)	//十字キー右

#define CROSSZONE (4500)        //十字キーの許容範囲

//=============================================================================
//クラス
//=============================================================================
class CInputController :public CInput
{//派生クラス
public:
	typedef struct
	{
		LPDIRECTINPUTDEVICE8 pDIJoypad;				// コントローラーデバイス
		DIJOYSTATE2 State;							//コントローラーのプレス情報
		DIJOYSTATE2 Trigger;						//コントローラーのトリガー情報
		DIJOYSTATE2 Release;						//コントローラーのプレス情報
	}CONTROLLER;

	typedef enum
	{
		BUTTON_X = 0,					//X
		BUTTON_Y,						//Y
		BUTTON_A,						//X
		BUTTON_B,						//Y
		BUTTON_L1,						//LB
		BUTTON_R1,						//RB
		BUTTON_L2,						//BACK
		BUTTON_R2,						//START
		BUTTON_L3,						//L押し込み
		BUTTON_R3,						//R押し込み
		BUTTON_BACK,					//BACK
		BUTTON_START,					//START
		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_M,
		MAX_BUTTON
	}BUTTON;//ボタンの数

	CInputController();									//コンストラクタ				
	~CInputController();								//デストラクタ
	HRESULT CreateJoypad(LPCDIDEVICEINSTANCE lpddi);	//生成処理
	HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd);//初期化処理
	void UpdateJoypad(void);							//更新処理
	void ReleaseDInput(void);							//リリース処理
	bool GetJoypadPress(int nButton);					//押しているとき
	bool GetJoypadTrigger(int nButton);					//押したとき
	bool GetJoypadRelease(int nButton);					//離したとき
	DIJOYSTATE2 GetController(void);					//コントローラーの情報取得関数
private:
	LPDIRECTINPUT8			m_pDInput;					// DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8	m_pDIJoypad;				// コントローラーデバイス
	static DIJOYSTATE2 m_ControllerState;				//コントローラーのプレス情報
	static DIJOYSTATE2 m_ControllerStateTrigger;		//コントローラーのトリガー情報
	CONTROLLER m_Controller[MAX_CONTROLLER];
};
#endif
