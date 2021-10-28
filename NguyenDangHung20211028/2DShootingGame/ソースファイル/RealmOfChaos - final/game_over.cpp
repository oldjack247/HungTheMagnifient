//=============================================================================
//インクルードファイル
//=============================================================================
#include "game_over.h"
#include "renderer.h"
#include "sound.h"
#include "input.h"
#include "keyboard.h"
#include "controller.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CGameOver::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGameOver::CGameOver(int nPriority) : CScene(nPriority)
{
	m_apScene2D = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGameOver::~CGameOver()
{

}

//=============================================================================
// テクスチャの描画処理
//=============================================================================
HRESULT CGameOver::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/UI/gameover000.png",//ファイルの名前
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CGameOver::Unload(void)
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
CGameOver* CGameOver::Create(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CGameOver* pGameOver = NULL;
	pGameOver == new CGameOver;
	pGameOver->Init(pos, size_x * 2, size_y * 2);
	return pGameOver;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGameOver::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CGameOver::Load();

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
void CGameOver::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGameOver::Update(void)
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
void CGameOver::Draw(void)
{

}
