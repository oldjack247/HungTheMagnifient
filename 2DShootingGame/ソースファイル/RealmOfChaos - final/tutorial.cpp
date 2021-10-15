//=============================================================================
//インクルードファイル
//=============================================================================
#include "tutorial.h"
#include "keyboard.h"
#include "game.h"
#include "scene.h"
#include "scene2d.h"
#include "renderer.h"
#include "controller.h"
#include "sound.h"
#include "manager.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial(int nPriority) :CScene(nPriority)
{
	m_apScene2D = NULL;
	m_pSound = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CTutorial::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/BG/tutorial000.png",//ファイルの名前
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void CTutorial::Unload(void)
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
CTutorial* CTutorial::Create(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CTutorial* pTutorial = NULL;
	pTutorial = new CTutorial;
	pTutorial->Init(pos, size_x * 2, size_y * 2);
	return pTutorial;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CTutorial::Load();
	if (m_apScene2D == NULL)
	{
		m_apScene2D = new CScene2d;
		m_apScene2D->Init(pos, size_x, size_y);
		m_apScene2D->BindTexture(m_pTexture);
	}
	//サウンドを止める
	m_pSound->StopSound((m_pSound = CManager::GetSound())->SOUND_LABEL_TITLE_BGM);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
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
			CFade::SetFade(CManager::MODE_GAME);
			CFade::SetFadeSwitch(bFadeSwitch);
		}
	}
	//if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) ||
	//	pInputController->GetJoypadTrigger(CInputController::BUTTON_START))
	//{
	//	CManager::SetMode(CManager::MODE_GAME);
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}
