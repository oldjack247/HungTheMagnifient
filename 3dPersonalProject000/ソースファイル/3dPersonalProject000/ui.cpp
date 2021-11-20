//=============================================================================
//インクルードファイル
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "ui.h"
#include "player.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CUi::m_pTexture[TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CUi::CUi(int nPriority) :CScene2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_NONE;
	m_bPressenterflash = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CUi* CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	CUi* pUi;
	pUi = new CUi(OBJTYPE_UI);
	pUi->SetPosition(pos);
	pUi->SetSize(size);
	pUi->m_type = type;
	pUi->Init();
	return pUi;
}

//=============================================================================
// 読み込み処理
//=============================================================================
HRESULT CUi::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE, &m_pTexture[TYPE_TITLE]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TUTORIAL, &m_pTexture[TYPE_TUTORIAL]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAMEOVER, &m_pTexture[TYPE_GAMEOVER]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT, &m_pTexture[TYPE_RESULT]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HP, &m_pTexture[TYPE_HP]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HPFLAME, &m_pTexture[TYPE_HPFLAME]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HPSPBACK, &m_pTexture[TYPE_HPSPBACK]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SP, &m_pTexture[TYPE_SP]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SPFLAME, &m_pTexture[TYPE_SPFLAME]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMYREMAINDER, &m_pTexture[TYPE_ENEMYREMAINDER]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PRESSSTART, &m_pTexture[TYPE_PRESSSTART]);
	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CUi::Unload(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUi::Init(void)
{
	CScene2D::Init();
	SetObjType(CScene::OBJTYPE_UI);
	BindTexture(m_pTexture[m_type]);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUi::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUi::Update(void)
{
	// 色のゲット
	D3DXCOLOR col = GetColor();

	switch (m_type)
	{
	case TYPE_PLAYER_LIFE:
		if ((float)CGame::GetPlayer()->GetLife() <= 0.0f)
		{
			Uninit();
			return;
		}

		CScene2D::SetLifebar((float)PLAYER_LIFE, (float)CGame::GetPlayer()->GetLife(), PLAYER_HP_COLOR);
		break;

	case TYPE_PRESSSTART:
		CScene2D::Update();
		// 色を変える
		if (m_bPressenterflash == false)
		{
			col.a -= PRESSSTART_CHANGECOLOR_QUANTITY;

			// 0.0fを下回ったらtrueに
			if (col.a <= 0.0f)
			{
				m_bPressenterflash = true;
			}
		}
		else if (m_bPressenterflash == true)
		{
			col.a += PRESSSTART_CHANGECOLOR_QUANTITY;

			// 1.0fを上回ったらfalseに
			if (col.a >= 1.0f)
			{
				m_bPressenterflash = false;
			}
		}

		// 色のセット
		SetColor(col);
		break;

	default:
		CScene2D::Update();
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CUi::Draw(void)
{
	CScene2D::Draw();
}