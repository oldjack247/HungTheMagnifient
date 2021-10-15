//=============================================================================
//インクルードファイル
//=============================================================================
#include "result.h"
#include "keyboard.h"
#include "game.h"
#include "scene.h"
#include "scene2d.h"
#include "renderer.h"
#include "player.h"
#include "enemy.h"
#include "manager.h"
#include "score.h"
#include "controller.h"
#include "sound.h"
#include "fade.h"
#include "client_ranking.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CResult::m_pTexture = NULL;
CCrient_ranking* CResult::m_pCrient_ranking = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult(int nPriority) : CScene(nPriority)
{
	m_apScene2D = NULL;
	m_pSound = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CResult::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/BG/rankingbg001.png",//ファイルの名前
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void CResult::Unload(void)
{
	CScore::Unload();
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CResult* CResult::Create(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CResult* pResult = NULL;
	pResult = new CResult;
	pResult->Init(pos, size_x * 2, size_y * 2);
	return pResult;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CScore::Load();
	if (m_apScene2D == NULL)
	{
		m_apScene2D = new CScene2d;
		m_apScene2D->Init(pos, size_x, size_y);
		m_apScene2D->BindTexture(m_pTexture);
	}
	//m_pCrient_ranking = CCrient_ranking::Create(D3DXVECTOR3(600.0f, 100.0f, 0.0f), SCORE_WIDTH, 85);
	//サウンドを止める
	m_pSound->StopSound((m_pSound = CManager::GetSound())->SOUND_LABEL_GAME_BGM);
	//サウンドを止める
	m_pSound->StopSound((m_pSound = CManager::GetSound())->SOUND_LABEL_2FBOSS_BATTLE_BGM);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	CScore::Unload();
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	bool bFadeSwitch = false;
	CInputKeyboard* pInputKeyboard = NULL;
	pInputKeyboard = CManager::GetInputKeyboard();
	CInputController* pInputController = NULL;
	pInputController = CManager::GetController();
	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) ||
		pInputController->GetJoypadTrigger(CInputController::BUTTON_A))
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			//モード移行
			bFadeSwitch = true;
			CFade::SetFade(CManager::MODE_TITLE);
			CFade::SetFadeSwitch(bFadeSwitch);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{

}

