#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "particle.h"
#include "Fire.h"
#include "magiccircle.h"
#include "swordeffect.h"
#include "enemy.h"
#include "collision.h"
#include "debugcollision.h"
#include "enemydeatheffect.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture[TEX_TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle(int nPriority) : CBillboard(nPriority)
{
	m_fAngle = 0.0f;
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CParticle::Load(void)
{
	// レンダラー取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/slash_effect_1.png", &m_pTexture[TEX_TYPE_1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/slash_effect_2.png", &m_pTexture[TEX_TYPE_2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect.png", &m_pTexture[TEX_TYPE_SWORD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spark_effect.png", &m_pTexture[TEX_TYPE_PLAYERATTACK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spesialattackeffect02.png", &m_pTexture[TEX_TYPE_SPESIALATTACK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/fire.png", &m_pTexture[TEX_TYPE_FIRE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/magiccircle.png", &m_pTexture[TEX_TYPE_MAGICCIRCLE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemycreateeffect.png", &m_pTexture[TEX_TYPE_ENEMYCREATE_MAGICCIRCLE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemydeatheffect.png", &m_pTexture[TEX_TYPE_ENEMYDEATH]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemyattackeffect.png", &m_pTexture[TEX_TYPE_ENEMYATTACK]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CParticle::Unload(void)
{
	// テクスチャ種類分繰り返す
	for (int nCnt = 0; nCnt < TEX_TYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			// テクスチャRelease
			m_pTexture[nCnt]->Release();

			// m_pTextureをNULLに
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(TEX_TYPE TexType)
{
	// テクスチャタイプ
	m_TexType = TexType;

	// 初期化
	CBillboard::Init();

	// テクスチャ受け渡し
	BindTexture(m_pTexture[TexType]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	// 受け取り
	D3DXVECTOR3 pos = GetPos();
	float fAngle = GetAngle();
	D3DXVECTOR3 rot = GetRot();
	D3DXCOLOR col = GetColor();
	D3DXVECTOR3 size = GetSize();

	switch (m_TexType)
	{
	case TEX_TYPE_1:
		// 更新
		CBillboard::Update();
		break;

	case TEX_TYPE_2:
		// 更新
		CBillboard::Update();
		break;

	case TEX_TYPE_SWORD:
		// 更新
		CBillboard::Update();

		col.a -= SWORD_EFFECT_ALPHA_SUBTRACT;
		pos += m_Move * SWORD_EFFECT_SPEED;

		SetColor(col);
		SetPos(pos);
		break;

	case TEX_TYPE_PLAYERATTACK:
		// 更新
		CBillboard::Update();
		break;

	case TEX_TYPE_SPESIALATTACK:
		// 更新
		CBillboard::Update();
		break;

	case TEX_TYPE_FIRE:
		// 更新
		CBillboard::Update(m_fAngle);

		// 色や透明度を減らす
		col.a -= FIRE_SUBTRACT_COLOR_ALPHA;
		col.r -= FIRE_SUBTRACT_COLOR_RED_GREEN;
		col.g -= FIRE_SUBTRACT_COLOR_RED_GREEN;

		// テクスチャを回す角度の加算
		fAngle += D3DXToRadian(4.0f);
		pos += m_Move;

		size.x -= 0.01f;
		size.y -= 0.01f;

		// yだけ上げる
		m_Move.y += FIRE_SPEED_ADD_Y;

		// 移動量を減らしていく
		if (m_Move.z <= 0.0f)
		{
			m_Move.z += FIRE_SPEED_ADD_SUBTRACT_Z;
		}
		else if (m_Move.z >= 0.0f)
		{
			m_Move.z -= FIRE_SPEED_ADD_SUBTRACT_Z;
		}

		// 座標と回転と色のセット
		SetPos(pos);
		SetAngle(fAngle);
		SetColor(col);
		break;

	case TEX_TYPE_MAGICCIRCLE:
		// 更新
		CBillboard::Update();

		if (rot.y < D3DX_PI)
		{
			rot.y += D3DXToRadian(2.0f);
			size += D3DXVECTOR3(5.0f, 5.0f, 0.0f);
		}
		else
		{
			// 炎のpos
			D3DXVECTOR3 firepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 炎の生成する角度
			float fFireAngle = D3DXToRadian((rand() % FIRE_CREATE_ANGLE_MAX));

			// 移動量と距離
			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, cosf(fFireAngle) * FIRE_MOVE_Y, sinf(fFireAngle) * FIRE_MOVE_Z);
			D3DXVECTOR3 Distance = D3DXVECTOR3(size.x, 0.0f, size.y) / 2;

			// 炎を出す数分の角度
			float fFireCreateAngle = D3DXToRadian((float)FIRE_CREATE_ANGLE_MAX / (float)MAGICCERCLE_FLAMECREATE_NUM);

			// 中心にでかい炎を出す
			CFire::Create(pos, FIR_SIZE * 2, move * 2, FIRE_COLOR, FIRE_LIFE);

			// 生成する分炎を出す
			for (int nCount = 0; nCount < MAGICCERCLE_FLAMECREATE_NUM; nCount++)
			{
				firepos.x = pos.x - Distance.x * sinf(rot.y + fFireCreateAngle * (float)nCount);
				firepos.z = pos.z - Distance.z * cosf(rot.y + fFireCreateAngle * (float)nCount);
				CFire::Create(firepos, FIR_SIZE, move, FIRE_COLOR, FIRE_LIFE);
			}

			// 魔法陣の内側にいる敵の判定
			MagiccircleCollision();

			CScene::SetbUpdate(false, CScene::OBJTYPE_ENEMY);

#ifdef _DEBUG
			// 当たり判定の可視化
			CDebugCollision::Create(pos, D3DXVECTOR3(GetSize().x, GetSize().z, GetSize().y), CDebugCollision::TYPE_SPHERE);
#endif
		}

		SetRot(rot);
		SetSize(size);
		break;


	case TEX_TYPE_ENEMYCREATE_MAGICCIRCLE:
		// 更新
		CBillboard::Update();

		if (GetLife() == MAGICCERCLE_ENEMY_CREATE_TIME)
		{
			// 敵の生成
			CEnemy::Create(D3DXVECTOR3(pos.x, 35.0f, pos.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMY_SIZE, CEnemy::ENEMYSTATE_NOMAL);
		}
		break;

	case TEX_TYPE_ENEMYDEATH:
		// 更新
		CBillboard::Update();
		col.a -= ENEMYDEATHEFFEC_SUBTRACT_COLOR_ALPHA;
		pos += m_Move;

		// 座標と色のセット
		SetPos(pos);
		SetColor(col);
		break;

	case TEX_TYPE_ENEMYATTACK:
		// 更新
		CBillboard::Update();
		break;

	default:
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticle::Draw(void)
{
	// 描画
	CBillboard::Draw();
}

//=============================================================================
// 魔法陣の内側にいる敵の判定
//=============================================================================
void CParticle::MagiccircleCollision(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	CScene* pScene = GetSceneTop(OBJTYPE_ENEMY);
	do
	{
		if (pScene != NULL)
		{
			OBJTYPE objType = pScene->GetObjType();
			if (objType == OBJTYPE_ENEMY)
			{
				// 座標の受け取り
				D3DXVECTOR3 Getpos = ((CEnemy*)pScene)->GetPos();

				// 当たり判定
				if (CCollision::SphereCollisionInside(pos, size.x / 2, Getpos) == true)
				{
					// 敵へのダメージ
					((CEnemy*)pScene)->HitDamage(5, CEnemy::DAMAGESTATE_MAGICCIRCLE);
				}
			}
			// 次のポインタの受け取り
			pScene = pScene->GetSceneNext();
		}
	} while (pScene != NULL);
}