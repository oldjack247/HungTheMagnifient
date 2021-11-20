//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "joystick.h"
#include "enemy.h"
#include "motion.h"
#include "model.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "sound.h"
#include "collision.h"
#include "debugcollision.h"
#include "enemydeatheffect.h"
#include "hiteffect.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority) :CCharacter(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDamageTime = 0.0f;
	m_fMotionTime = 0.0f;
	m_fAttackTime = 0.0f;
	m_bAttack = false;
	m_EnemyState = ENEMYSTATE_NOMAL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, ENEMYSTATE EnemyState)
{
	CEnemy* pEnemy;
	pEnemy = new CEnemy;
	pEnemy->SetEnemy(pos, rot, size, EnemyState);
	pEnemy->Init();
	return pEnemy;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	SetObjType(OBJTYPE_ENEMY);
	SetType(CHARACTERTYPE_ENEMY);
	LoadInitialPosition(MOTION_ENEMY_TEXT);
	CCharacter::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	CCharacter::Uninit();

	// リリース
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	CCharacter::Update();

	if (CGame::GetPlayer != NULL)
	{
		//プレイヤーの場所の取得
		D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();

		// プレイヤーとの距離
		float fDistance = sqrtf(
			powf(PlayerPos.x - pos.x, 2.0f) +
			powf(PlayerPos.z - pos.z, 2.0f));

		// 死んでいないとき
		if (m_EnemyState != ENEMYSTATE_DEATH)
		{
			//方向の計算
			float angle = (float)atan2(PlayerPos.x - pos.x, PlayerPos.z - pos.z);
			rot.y = angle - D3DXToRadian(180.0f);
		}

		// 敵の状態
		switch (m_EnemyState)
		{
		case ENEMYSTATE_NOMAL:
			// プレイヤーと敵の距離が近かったら
			if (fDistance <= ENEMY_DISTANCE_PLAYER)
			{
				Attack();
			}
			else
			{
				//向いている方向にrot
				m_move.x = -sinf(rot.y) * ENEMY_MOVE_SPEED;
				m_move.z = -cosf(rot.y) * ENEMY_MOVE_SPEED;

				// チェイス中は走るモーション
				GetMotion()->SetMotion(ENEMYMOTION_RUN);
			}
			break;

		case ENEMYSTATE_DAMAGE:
			// チェイス中は走るモーション
			GetMotion()->SetMotion(ENEMYMOTION_DAMAGE);

			//向いている方向にrot
			m_move.x = sinf(rot.y) * ENEMY_DAMAGE_MOVE_SPEED;
			m_move.z = cosf(rot.y) * ENEMY_DAMAGE_MOVE_SPEED;

			// モーションの時間の減算
			m_fMotionTime--;
			if (m_fMotionTime <= 0.0f)
			{
				m_EnemyState = ENEMYSTATE_NOMAL;
			}
			break;

		case ENEMYSTATE_DEATH:
			// モーションの時間の減算
			m_fMotionTime--;
			if (m_fMotionTime <= 50.0f)
			{
				float fAngle = D3DXToRadian(360.0f / ENEMYDEATHEFFEC_CREATE_NUM);
				for (int nCount = 0; nCount < ENEMYDEATHEFFEC_CREATE_NUM; nCount++)
				{
					CEnemydeatheffect::Create(pos, ENEMYDEATHEFFEC_DEFAULT_SIZE,
						D3DXVECTOR3(sinf(fAngle * nCount), 0.0f, cosf(fAngle * nCount)) * ENEMYDEATHEFFEC_DEFAULT_SPEED,
						ENEMYDEATHEFFEC_DEFAULT_COLOR,
						ENEMYDEATHEFFEC_COUNTANIM * ENEMYDEATHEFFEC_PATTERNANIM * ENEMYDEATHEFFEC_TIMEANIM);
				}

				for (int nCount = 0; nCount < ENEMYDEATHEFFEC_CREATE_NUM; nCount++)
				{
					CEnemydeatheffect::Create(pos, ENEMYDEATHEFFEC_DEFAULT_SIZE,
						D3DXVECTOR3(sinf(fAngle * nCount), 1.0f, cosf(fAngle * nCount)) * (ENEMYDEATHEFFEC_DEFAULT_SPEED / 2.0f),
						ENEMYDEATHEFFEC_DEFAULT_COLOR,
						ENEMYDEATHEFFEC_COUNTANIM * ENEMYDEATHEFFEC_PATTERNANIM * ENEMYDEATHEFFEC_TIMEANIM);
				}

				CEnemydeatheffect::Create(pos, ENEMYDEATHEFFEC_DEFAULT_SIZE,
					D3DXVECTOR3(0.0f, 1.0f, 0.0f) * ENEMYDEATHEFFEC_DEFAULT_SPEED,
					ENEMYDEATHEFFEC_DEFAULT_COLOR,
					ENEMYDEATHEFFEC_COUNTANIM * ENEMYDEATHEFFEC_PATTERNANIM * ENEMYDEATHEFFEC_TIMEANIM);

				// 終了処理
				Uninit();
				return;
			}
			if (m_fMotionTime >= 131.0f)
			{
				//向いている方向にrot
				m_move.x = -sinf(rot.y) * ENEMY_DAMAGE_MOVE_SPEED;
				m_move.z = -cosf(rot.y) * ENEMY_DAMAGE_MOVE_SPEED;
			}
			else
			{
				m_move.x = 0.0f;
				m_move.z = 0.0f;
			}

			break;

		default:
			break;
		}

		// 敵同士の判定
		EnemyCollision();

		// 移動量の加算
		pos += m_move;

		// プレイヤーとの判定
		pos = PlayerCollision(pos);

		SetPos(pos);
		SetRot(rot);

	}
	// ダメージを受ける時間を減らす
	m_fDamageTime--;

#ifdef _DEBUG
	// 当たり判定の可視化
	CDebugCollision::Create(GetPos() + GetMotion()->GetPos(0), GetSize(), CDebugCollision::TYPE_SQUARE);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	CCharacter::Draw();
}

//=============================================================================
// ヒット処理
//=============================================================================
bool CEnemy::HitDamage(int nDamage, DAMAGESTATE DamageState)
{
	bool bHit = false;
	if (m_EnemyState == ENEMYSTATE_NOMAL || m_EnemyState == ENEMYSTATE_DAMAGE)
	{
		// ダメージを受ける時間が0以下だったら
		if (m_fDamageTime <= 0.0f)
		{
			// ダメージの種類によってダメージを受ける時間を変える
			switch (DamageState)
			{
			case DAMAGESTATE_SLASH:
				m_fDamageTime = ENEMY_DAMAGE_SRASH_TIME;
				break;

			//case DAMAGESTATE_MAGICCIRCLE:
			//	m_fDamageTime = ENEMY_DAMAGE_MAGICCIRCLE_TIME;
			//	break;

			default:
				break;
			}

			// ライフを減らす
			int nLife = GetLife();
			nLife -= nDamage;
			SetLife(nLife);

			// 状態をダメージを受けた状態にする
			m_EnemyState = ENEMYSTATE_DAMAGE;
			m_fMotionTime = ENEMY_DAMAGE_MOTION_TIME;

			//サウンドの再生
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DAMAGE);

			if (nLife <= 0)
			{
				//サウンドの再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ZOMBIE_DEATH);
				CGame::SubtractEnemy();

				// 死んだときのモーションと時間、状態
				GetMotion()->SetMotion(ENEMYMOTION_DEATH);
				m_fMotionTime = ENEMY_DEATH_MOTION_TIME;
				m_EnemyState = ENEMYSTATE_DEATH;
			}
			else
			{
				//サウンドの再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ZOMBIE_DAMAGE);
			}

			// 当たったことを返す
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
// 敵同士の当たり判定の処理
//=============================================================================
void CEnemy::EnemyCollision(void)
{
	// 死んでいないときとダメージを受けてないとき
	if (m_EnemyState != ENEMYSTATE_DEATH || m_EnemyState != ENEMYSTATE_DAMAGE)
	{
		CScene* pScene = GetSceneTop(OBJTYPE_ENEMY);
		do
		{
			if (pScene != NULL)
			{
				OBJTYPE objType = pScene->GetObjType();
				if (objType == OBJTYPE_ENEMY)
				{
					if (GetPos() != ((CEnemy*)pScene)->GetPos())
					{
						if (((CEnemy*)pScene)->GetEnemyState() != ENEMYSTATE_DEATH)
						{
							// 当たり判定
							if (CCollision::SphereCollision(GetPos(), GetSize().x / 2.0f, ((CEnemy*)pScene)->GetPos(), ((CEnemy*)pScene)->GetSize().x / 2.0f) == true)
							{
								float frot = atan2f(GetPos().x - ((CEnemy*)pScene)->GetPos().x, GetPos().z - ((CEnemy*)pScene)->GetPos().z);
								m_move.x -= sinf(frot + D3DX_PI);
								m_move.z -= cosf(frot + D3DX_PI);
							}
						}
					}
				}
				// 次のポインタの受け取り
				pScene = pScene->GetSceneNext();
			}
		} while (pScene != NULL);
	}
}

//=============================================================================
// プレイヤーとの当たり判定の処理
//=============================================================================
D3DXVECTOR3 CEnemy::PlayerCollision(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Getpos = pos;

	// 死んでいないとき
	if (m_EnemyState != ENEMYSTATE_DEATH)
	{
		CScene* pScene = GetSceneTop(OBJTYPE_PLAYER);
		do
		{
			if (pScene != NULL)
			{
				OBJTYPE objType = pScene->GetObjType();
				if (objType == OBJTYPE_PLAYER)
				{
					// 当たり判定
					if (CCollision::SphereCollision(GetPos(), GetSize().x / 2.0f, ((CPlayer*)pScene)->GetPos(), ((CPlayer*)pScene)->GetSize().x / 2.0f) == true)
					{
						if (m_EnemyState == ENEMYSTATE_NOMAL)
						{
							Getpos = GetPos();
						}
					}
				}
				// 次のポインタの受け取り
				pScene = pScene->GetSceneNext();
			}
		} while (pScene != NULL);
	}
	return Getpos;
}

//=============================================================================
// 攻撃の処理
//=============================================================================
void CEnemy::Attack(void)
{
	// 攻撃していないとき
	if (m_bAttack == false)
	{
		// 攻撃する時間の加算
		m_fAttackTime++;

		// 攻撃する時間が超えたら
		if (m_fAttackTime >= ENEMY_ATTACK_UNTIL_TIME)
		{
			// 攻撃のフラグ立て
			m_bAttack = true;

			// 攻撃モーション
			GetMotion()->SetMotion(ENEMYMOTION_ATTACK);
			m_fMotionTime = ENEMY_ATTACK_MOTION_TIME;
		}
	}
	else
	{
#ifdef _DEBUG
		// 当たり判定の可視化
		CDebugCollision::Create(D3DXVECTOR3(GetModelParts(15)->GetMtxWorld()._41,
			GetModelParts(15)->GetMtxWorld()._42,
			GetModelParts(15)->GetMtxWorld()._43),
			D3DXVECTOR3(50.0f, 50.0f, 50.0f), CDebugCollision::TYPE_SQUARE);
#endif
		// プレイヤーへの当たり判定
		CScene* pScene = CScene::GetSceneTop(CScene::OBJTYPE_PLAYER);
		do
		{
			if (pScene != NULL)
			{
				OBJTYPE objType = pScene->GetObjType();
				if (objType == OBJTYPE_PLAYER)
				{
					// 当たったかどうかと当たった場所
					bool bHit = false;
					D3DXVECTOR3 Hitpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					// 当たり判定
					if (CCollision::RectangleCollision3D(D3DXVECTOR3(GetModelParts(15)->GetMtxWorld()._41,
						GetModelParts(15)->GetMtxWorld()._42,
						GetModelParts(15)->GetMtxWorld()._43),
						SWORD_COLISION_SIZE, ((CPlayer*)pScene)->GetPos(), ((CPlayer*)pScene)->GetSize()) == true)
					{
						// 敵へのダメージ(敵側で当たったか返す)
						bHit = ((CPlayer*)pScene)->HitDamage(5);
					}

					// 敵に当たったなら
					if (bHit == true)
					{
						// 火花を出す
						CHiteffect::Create(D3DXVECTOR3(GetModelParts(15)->GetMtxWorld()._41,
							GetModelParts(15)->GetMtxWorld()._42,
							GetModelParts(15)->GetMtxWorld()._43) + Hitpos, HITEFFECT_ENEMY_SIZE, HITEFFECT_ENEMY_COLOR,
							HITEFFECT_ENEMY_COUNTANIM * HITEFFECT_ENEMY_PATTERNANIM * HITEFFECT_ENEMY_TIMEANIM, CHiteffect::HITEFFECTTYPE_ENEMY);
					}

				}
				pScene = pScene->GetSceneNext();
			}
		} while (pScene != NULL);

		// モーションの時間の減算
		m_fMotionTime--;
		if (m_fMotionTime <= 0.0f)
		{
			m_bAttack = false;
			m_fAttackTime = 0.0f;
			m_fMotionTime = 0.0f;
		}
	}
}

//=============================================================================
// 各種設定
//=============================================================================
void CEnemy::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, ENEMYSTATE EnemyState)
{
	SetObjType(OBJTYPE_ENEMY); //オブジェクトタイプの設定
	SetPos(pos);
	SetRot(rot);
	SetSize(size);
	m_EnemyState = EnemyState;
	SetLife(ENEMY_LIFE);
}