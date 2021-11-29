#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
//インクルードファイル
//=============================================================================
#include "player.h"
#include "scene2d.h"
#include "renderer.h"
#include "main.h"
#include "manager.h"
#include "keyboard.h"
#include "bullet.h"
#include "result.h"
#include "item.h"
#include "enemy.h"
#include <stdio.h>
#include "score.h"
#include "game.h"
#include "controller.h" 
#include "fade.h"
#include "sound.h"
#include "effect.h"
#include "explosion.h"
#include "game_clear.h"
#include "client_ranking.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
int CPlayer::m_nLife = NULL;
D3DXVECTOR3 CPlayer::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
bool CPlayer::m_bBulletChange = false;
bool CPlayer::m_bUseMinion = false;
CGameClear* CPlayer::m_pGameClear = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2d(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nInvincible = 0;
	m_State = PLAYERSTATE_NORMAL;
	m_bBulletChange = false;
	m_bUseMinion = false;
	m_pSound = NULL;
}

//=============================================================================
//デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
//テクスチャの読み込み処理
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/player/Spaceship001.png",
		&m_pTexture);
	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CPlayer::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//生成処理
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float size_x, float size_y)
{
	CPlayer* pPlayer = NULL;
	pPlayer = new CPlayer;
	pPlayer->Init(pos, col, size_x, size_y);
	m_nLife = MAX_LIFE;

	return pPlayer;
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float size_x, float size_y)
{
	VERTEX_2D* pVtx;
	CScene2d::Init(pos, size_x, size_y);
	m_col = col;
	BindTexture(m_pTexture);

	SetObjType(OBJTYPE_PLAYER);
	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
	//頂点座標の設定(右回り)
	pVtx[0].pos = D3DXVECTOR3(pos.x - (PLAYER_SIZE / 2), pos.y - (PLAYER_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (PLAYER_SIZE / 2), pos.y - (PLAYER_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - (PLAYER_SIZE / 2), pos.y + (PLAYER_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + (PLAYER_SIZE / 2), pos.y + (PLAYER_SIZE / 2), 0.0f);

	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	CScene2d::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CPlayer::Update(void)
{
	VERTEX_2D* pVtx;
	bool b2FBossAwakening = false;
	CInputKeyboard* pInputKeyboard = NULL;
	pInputKeyboard = CManager::GetInputKeyboard();
	CInputController* pInputController = NULL;
	pInputController = CManager::GetController();
	DIJOYSTATE2 Controller = pInputController->GetController();
	m_nCounterAnim++;
	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.01075f + 0.01075f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.01075f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.01075f + 0.01075f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.01075f, 1.0f);
	if (m_nCounterAnim >= 2)
	{//一定時間経過した
		m_nCounterAnim = 0;
		m_nPatternAnim++;
		if (m_nPatternAnim >= 93)
		{//パターン数を超えた
			m_nPatternAnim = 0;
		}
	}
	m_pVtxBuff->Unlock();

	m_pos = GetPosition();

	if (pInputKeyboard->GetKeyboardPress(DIK_D))
	{
		m_move += D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_A))
	{
		m_move += D3DXVECTOR3(-3.0f, 0.0f, 0.0f);
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_W))
	{
		m_move += D3DXVECTOR3(0.0f, -3.0f, 0.0f);
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_S))
	{
		m_move += D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	}
	if (Controller.lX != 0 || Controller.lY != 0)
	{//コントローラー
		float fAngle = (float)atan2(Controller.lX, Controller.lY * 1.0f);
		float fPlayerAngle = atan2f(Controller.lX * -1.0f, Controller.lY * 1.0f);
		m_move.x += sinf(fAngle) * 3.0f;
		m_move.y += cosf(fAngle) * 3.0f;
	}
	//SPACEキーかコントローラーのAボタンを押したら弾が発射される
	if (pInputKeyboard->GetKeyboardTrigger(DIK_SPACE) ||
		pInputController->GetJoypadTrigger(CInputController::BUTTON_A))
	{
		/*if (m_bUseMinion == true)
		{
			CMinion::Create(D3DXVECTOR3(m_pos.x, m_pos.y - (PLAYER_SIZE / 2), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE);
		}*/

		if (m_bBulletChange == true)
		{
			CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y - (PLAYER_SIZE / 2), 0.0f), D3DXVECTOR3(0.0f, -12.0f, 0.0f), PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, 50, CBullet::BULLETTYPE_PLAYER);
			CBullet::Create(D3DXVECTOR3(m_pos.x + (PLAYER_SIZE / 2), m_pos.y - (PLAYER_SIZE / 2), 0.0f), D3DXVECTOR3(0.0f, -12.0f, 0.0f), PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, 50, CBullet::BULLETTYPE_PLAYER_2);
			CBullet::Create(D3DXVECTOR3(m_pos.x - (PLAYER_SIZE / 2), m_pos.y - (PLAYER_SIZE / 2), 0.0f), D3DXVECTOR3(0.0f, -12.0f, 0.0f), PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, 50, CBullet::BULLETTYPE_PLAYER_2);
		}
		else
		{
			CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y - (PLAYER_SIZE / 2), 0.0f), D3DXVECTOR3(0.0f, -12.0f, 0.0f), PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, 50, CBullet::BULLETTYPE_PLAYER);
		}

		CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			0.0f, 0.0f,
			CEffect::TYPE_BULLET_SPAWN);
		//発射音
		m_pSound->PlaySound((m_pSound = CManager::GetSound())->SOUND_LABEL_PLAYER_BULLET);
	}

	//ダメージを受けたとき
	if (m_State == PLAYERSTATE_DAMAGE)
	{
		m_nInvincible--;
		if (m_nInvincible <= 80)
		{
			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
			//頂点カラーの設定(0～255の数値で設定)
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			m_pVtxBuff->Unlock();
		}

		if (m_nInvincible <= 0)
		{
			m_State = PLAYERSTATE_NORMAL;
		}
	}

	//画面外に出ないようにする
	if (m_pos.x < 100.0f - (PLAYER_SIZE / 2))
	{
		m_pos.x = 100.0f - (PLAYER_SIZE / 2);
	}
	if (m_pos.x > SCREEN_WIDTH - (PLAYER_SIZE / 2))
	{
		m_pos.x = SCREEN_WIDTH - (PLAYER_SIZE / 2);
	}
	if (m_pos.y < 100.0f - (PLAYER_SIZE / 2))
	{
		m_pos.y = 100.0f - (PLAYER_SIZE / 2);
	}
	if (m_pos.y > SCREEN_HEIGHT - (PLAYER_SIZE / 2))
	{
		m_pos.y = SCREEN_HEIGHT - (PLAYER_SIZE / 2);
	}

	//慣性
	m_move.x += ((0.0f) - m_move.x) * 0.5f;
	m_move.y += ((0.0f) - m_move.y) * 0.5f;
	m_move.x -= ((0.0f) - m_move.x) * 0.5f;
	m_move.y -= ((0.0f) - m_move.y) * 0.5f;

	//位置更新
	m_pos += m_move;
	SetPosition(m_pos);
	SetCol(m_col);
	bool bFadeSwitch = false;

	//体力がなくなったら
	if (m_nLife <= 0)
	{
		CScore* pScore = NULL;
		pScore = CGame::GetScore();
		int nScore = pScore->GetScore();
		m_bBulletChange = false;
		CExplosion::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			PLAYER_EXPLOSION_SIZE, PLAYER_EXPLOSION_SIZE,
			CExplosion::TYPE_PLAYER_DEATH);
		//CCrient_ranking::SetRanking(nScore);
		Uninit();

		//フェードをいれてリザルト画面にいく
		bFadeSwitch = true;
		CFade::SetFade(CManager::MODE_GAMEOVER);
		CFade::SetFadeSwitch(bFadeSwitch);
	}
	else
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (PLAYER_SIZE / 2), m_pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (PLAYER_SIZE / 2), m_pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (PLAYER_SIZE / 2), m_pos.y + (PLAYER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (PLAYER_SIZE / 2), m_pos.y + (PLAYER_SIZE / 2), 0.0f);
		//頂点カラーの設定(0～255の数値で設定)
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;
		//pVtx += 4;//頂点データのポインタを4つ分進める
		m_pVtxBuff->Unlock();
	}

}

//=============================================================================
//描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CScene2d::Draw();
}

//=============================================================================
//ダメージを受けたとき
//=============================================================================
void CPlayer::SetLife(int nLife)
{
	VERTEX_2D* pVtx;
	if (m_nInvincible <= 0)
	{
		m_nLife -= nLife;
		m_State = PLAYERSTATE_DAMAGE;
		m_nInvincible = 100;
	}
	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
	//頂点カラーの設定(0～255の数値で設定)
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	m_pVtxBuff->Unlock();

}

//=============================================================================
// 使っている弾を変える
//=============================================================================
void CPlayer::SetBulletChange(bool bBulletChange)
{
	m_bBulletChange = bBulletChange;
}

//=============================================================================
// 使っているMinionを変える
//=============================================================================
void CPlayer::SetMinion(bool bUseMinion)
{
	m_bUseMinion = bUseMinion;
}