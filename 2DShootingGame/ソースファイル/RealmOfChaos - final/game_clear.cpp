//=============================================================================
//インクルードファイル
//=============================================================================
#include "game_clear.h"
//#include "game.h"
#include "renderer.h"
#include "sound.h"
#include "input.h"
#include "keyboard.h"
#include "controller.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CGameClear::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGameClear::CGameClear(int nPriority) : CScene(nPriority)
{
	m_apScene2D = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGameClear::~CGameClear()
{

}

//=============================================================================
// テクスチャの描画処理
//=============================================================================
HRESULT CGameClear::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/UI/gameclear000.png",//ファイルの名前
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CGameClear::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CGameClear* CGameClear::Create(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CGameClear* pGameClear = NULL;

	pGameClear == new CGameClear;
	pGameClear->Init(pos, size_x * 2, size_y * 2);
	return pGameClear;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGameClear::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CGameClear::Load();

	if (m_apScene2D == NULL)
	{
		m_apScene2D = new CScene2d;
		m_apScene2D->Init(pos, size_x, size_y);
		m_apScene2D->BindTexture(m_pTexture);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGameClear::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGameClear::Update(void)
{
	VERTEX_2D* pVtx;
	bool bFadeSwitch = false;
	CInputKeyboard* pInputKeyboard = NULL;
	pInputKeyboard = CManager::GetInputKeyboard();
	CInputController* pInputController = NULL;
	pInputController = CManager::GetController();

	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN)
		|| pInputController->GetJoypadTrigger(CInputController::BUTTON_A))
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			//モード移行
			bFadeSwitch = true;
			CFade::SetFade(CManager::MODE_RESULT);
			CFade::SetFadeSwitch(bFadeSwitch);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGameClear::Draw(void)
{

}
