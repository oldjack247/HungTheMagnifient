//=============================================================================
//インクルードファイル
//=============================================================================
#include "controller.h"
#include "input.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CInputController::CInputController()
{
	m_pDInput = NULL;
	m_pDIJoypad = NULL;
	memset(m_Controller, 0, sizeof(m_Controller));
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputController::~CInputController()
{
}

//=============================================================================
//	ジョイスティック・デバイスを列挙（コールバック関数）
//=============================================================================
HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	HRESULT hr;
	CInputController* pController = (CInputController*)pvRef;
	hr = pController->CreateJoypad(lpddi);

	return DIENUM_CONTINUE;	// デバイスの列挙を中止
}

//=============================================================================
//	生成処理
//=============================================================================
HRESULT CInputController::CreateJoypad(LPCDIDEVICEINSTANCE lpddi)
{

	HRESULT hr = m_pInput->CreateDevice(lpddi->guidInstance, &m_Controller[0].pDIJoypad, NULL);

	return hr;
}

//=============================================================================
//	初期化処理
//=============================================================================
HRESULT CInputController::InitJoypad(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT	hr = NULL;
	CInput::Init(hInstance, hWnd);
	m_pDInput = CInput::GetInput();
	// コントローラーを探す
	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, this, DIEDFL_ATTACHEDONLY);

	if (!m_Controller[0].pDIJoypad)
	{
		return false;
	}

	for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
	{//ボタンの初期化
		m_Controller[0].Trigger.rgbButtons[nCntButton] = '\0';
	}

	// コントローラー用のデータ・フォーマットを設定
	hr = m_Controller[0].pDIJoypad->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		return false; // データフォーマットの設定に失敗
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	hr = m_Controller[0].pDIJoypad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return false; // モードの設定に失敗

	// 軸の値の範囲を設定
	// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
	// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
	// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドは、関係なし)

	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = RANGE_MIN;
	diprg.lMax = RANGE_MAX;

	//左スティック
	// X軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_X;
	m_Controller[0].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Y軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Y;
	m_Controller[0].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	//右スティック
	// Z軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Z;
	m_Controller[0].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z回転の範囲を設定
	diprg.diph.dwObj = DIJOFS_RZ;
	m_Controller[0].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	// 各軸ごとに、無効のゾーン値を設定する。
	// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
	// 指定する値は、10000に対する相対値(2000なら20パーセント)。
	DIPROPDWORD	dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = DEADZONE;

	//左スティック
	//X軸の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_X;
	m_Controller[0].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	//Y軸の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_Y;
	m_Controller[0].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	//右スティック
	//Z軸の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_Z;
	m_Controller[0].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	//Z回転の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_RZ;
	m_Controller[0].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	//コントローラー入力制御開始
	m_Controller[0].pDIJoypad->Acquire();

	return S_OK;
}

//=============================================================================
//	更新処理
//=============================================================================
void CInputController::UpdateJoypad(void)
{
	DIJOYSTATE2 ControllerState;
	HRESULT hr;
	int nCntButton;

	if (m_Controller[0].pDIJoypad != NULL)
	{
		hr = m_Controller[0].pDIJoypad->Poll();

		hr = m_Controller[0].pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState);

		//デバイスからデータを取得
		if (SUCCEEDED(hr = m_Controller[0].pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
		{
			//左スティックの情報
			//トリガー情報を保存
			//プレス情報
			m_Controller[0].State.lY = ControllerState.lY;
			m_Controller[0].State.lX = ControllerState.lX;
			m_Controller[0].State.lZ = ControllerState.lZ;
			m_Controller[0].State.lRz = ControllerState.lRz;

			//十字キーの情報
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				//十字キープレス情報を保存
				m_Controller[0].State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

			}

			//ボタンの情報
			for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
			{
				//キートリガー情報を保存
				m_Controller[0].Trigger.rgbButtons[nCntButton] = (m_Controller[0].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

				//キーリリース情報を保存
				m_Controller[0].Release.rgbButtons[nCntButton] = (m_Controller[0].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

				//キープレス情報を保存
				m_Controller[0].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
			}
		}
		else
		{
			//コントローラーのアクセス権を取得
			m_Controller[0].pDIJoypad->Acquire();
		}
	}
}

//=============================================================================
//	開放処理
//=============================================================================
void CInputController::ReleaseDInput(void)
{
	for (int nCntDevise = 0; nCntDevise < MAX_CONTROLLER; nCntDevise++)
	{
		//入力デバイスの開放
		if (m_Controller[nCntDevise].pDIJoypad != NULL)
		{
			//コントローラのアクセス権を開放
			m_Controller[nCntDevise].pDIJoypad->Unacquire();
			m_Controller[nCntDevise].pDIJoypad->Release();
			m_Controller[nCntDevise].pDIJoypad = NULL;
		}
	}
	CInput::Uninit();
}

//=============================================================================
//	押しているとき処理
//=============================================================================
bool CInputController::GetJoypadPress(int nButton)
{
	return(m_Controller[0].State.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//	押したときの処理
//=============================================================================
bool CInputController::GetJoypadTrigger(int nButton)
{
	return (m_Controller[0].Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//	離したとき処理
//=============================================================================
bool CInputController::GetJoypadRelease(int nButton)
{
	return(m_Controller[0].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//	コントローラーの情報取得関数
//=============================================================================
DIJOYSTATE2 CInputController::GetController(void)
{
	return m_Controller[0].State;
}
