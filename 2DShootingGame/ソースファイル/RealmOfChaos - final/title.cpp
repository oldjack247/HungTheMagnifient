//=============================================================================
//インクルードファイル
//=============================================================================
#include "title.h"
#include "keyboard.h"
#include "game.h"
#include "scene.h"
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h" 
#include "controller.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture[TITLETYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle(int nPriority) : CScene(nPriority)
{
	for (int nCount = 0; nCount < TITLETYPE_MAX; nCount++)
	{
		m_apScene2D[nCount] = NULL;
	}
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_titletype = TITLETYPE_START;
	m_pSound = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// テクスチャの描画処理
//=============================================================================
HRESULT CTitle::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/BG/titlebg003.png",//ファイルの名前
		&m_pTexture[TITLETYPE_BG]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/BG/titlename001.png",//ファイルの名前
		&m_pTexture[TITLETYPE_NAME]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/BG/start001.png",//ファイルの名前
		&m_pTexture[TITLETYPE_START]);
	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTitle::Unload(void)
{
	for (int nCount = 0; nCount < TITLETYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CTitle* CTitle::Create(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CTitle* pTitle = NULL;
	pTitle = new CTitle;
	pTitle->Init(pos, size_x * 2, size_y * 2);
	return pTitle;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	for (int nCount = 0; nCount < TITLETYPE_MAX; nCount++)
	{
		if (m_apScene2D[nCount] == NULL)
		{
			m_apScene2D[nCount] = new CScene2d;
			if (m_apScene2D[nCount] != NULL)
			{
				m_apScene2D[nCount]->Init(pos, size_x, size_y);

				m_apScene2D[nCount]->BindTexture(m_pTexture[nCount]);
			}
		}
	}

	//サウンドのセット
	m_pSound->PlaySound((m_pSound = CManager::GetSound())->SOUND_LABEL_TITLE_BGM);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	VERTEX_2D* pVtx;

	bool bFadeSwitch = false;
	CInputKeyboard* pInputKeyboard = NULL;
	pInputKeyboard = CManager::GetInputKeyboard();
	CInputController* pInputController = NULL;
	pInputController = CManager::GetController();

	if (m_titletype == TITLETYPE_START)
	{
		if (m_col.a >= 0.0f)
		{
			m_col.a -= 0.01f;
		}
		else if (m_col.a <= 0.0f)
		{
			m_col.a = 1.0f;
		}
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN)
		|| pInputController->GetJoypadTrigger(CInputController::BUTTON_A))
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			//モード移行
			bFadeSwitch = true;
			CFade::SetFade(CManager::MODE_TUTORIAL);
			CFade::SetFadeSwitch(bFadeSwitch);
		}
	}
	else if (m_titletype == TITLETYPE_START)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_apScene2D[2]->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
		//頂点カラーの設定(0～255の数値で設定)
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		m_apScene2D[2]->m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
}
