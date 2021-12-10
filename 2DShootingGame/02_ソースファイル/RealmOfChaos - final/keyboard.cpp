//=============================================================================
//インクルードファイル
//=============================================================================
#include "keyboard.h"
#include "input.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
BYTE CInputKeyboard::m_aKeyState[NUM_KEY_MAX] = {};
BYTE CInputKeyboard::m_aKeyStateRelease[NUM_KEY_MAX] = {};
BYTE CInputKeyboard::m_aKeyStateTrigger[NUM_KEY_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイス(キーボード)の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマット設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定(フォアグラウンド&非排他モード)
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND || DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}
	//キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];//キーボードの入力情報
	int nCntKey;

	//デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//キーボード情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
			//キープレス保存
			/*m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];
			m_aKeyState[nCntKey] != aKeyState[nCntKey];*/
		}
	}
	else
	{
		//キーボードへのアクセス権を取得
		m_pDevice->Acquire();
	}
}

bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
