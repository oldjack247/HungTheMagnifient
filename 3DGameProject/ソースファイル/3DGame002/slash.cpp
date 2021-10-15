#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "joystick.h"
#include "billboard.h"
#include "particle.h"
#include "slash.h"
#include "collision.h"
#include "enemy.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CSlash::CSlash(int nPriority) : CParticle(nPriority)
{
	m_SlashCount = NEXT_SLASH_COUNT;
	m_MoveAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CSlash::~CSlash()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CSlash* CSlash::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 moveangle, D3DXCOLOR col, int Life, TYPE type)
{
	// CSlashポインタ
	CSlash* pSlash = NULL;

	// メモリ確保
	pSlash = new CSlash;

	// 位置座標設定
	pSlash->SetPos(pos);

	// サイズ設定
	pSlash->SetSize(size);

	// カラー設定
	pSlash->SetColor(col);

	// カラー設定
	pSlash->SetLife(Life);

	pSlash->m_MoveAngle = moveangle;

	// 初期化
	pSlash->Init(type);

	// ポインタを返す
	return pSlash;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSlash::Init(TYPE type)
{
	switch (type)
	{
	case TYPE_SLASH_1:
		// 初期化
		CParticle::Init(TEX_TYPE_1);
		break;

	case TYPE_SLASH_2:
		// 初期化
		CParticle::Init(TEX_TYPE_2);
		break;

	default:
		break;
	}

	SetType(type);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CSlash::Uninit(void)
{
	// 終了
	CParticle::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CSlash::Update(void)
{
	// 更新
	CParticle::Update();

	D3DXVECTOR3 pos = GetPos();
	D3DXCOLOR col = GetColor();
	//switch (m_Type)
	//{
	//case TYPE_SLASH_1:
	//	m_SlashCount++;
	//	pos += m_MoveAngle * SLASH_SPEED;
	//
	//	if (m_SlashCount > NEXT_SLASH_COUNT)
	//	{
	//		CSlash::Create(GetPos(), GetSize(), -m_MoveAngle + D3DXVECTOR3(sinf(24.0f), 0.0f, cosf(24.0f)),
	//			GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
	//
	//		//CSlash::Create(GetPos(), GetSize(), D3DXVECTOR3((-GetMove().x) + sinf((24.0f)), 0.0f, (-GetMove().z) + cosf((24.0f))),
	//		//	GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
	//
	//		//CSlash::Create(GetPos(), GetSize(), D3DXVECTOR3((-GetMove().x) , 0.0f, (-GetMove().z)),
	//		//	GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
	//
	//		//CSlash::Create(GetPos(), GetSize(), D3DXVECTOR3((-GetMove().x) - sinf(-(24.0f)), 0.0f, (-GetMove().z) - cosf(-(24.0f))),
	//		//	GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
	//
	//		CSlash::Create(GetPos(), GetSize(), -m_MoveAngle - D3DXVECTOR3(sinf(24.0f), 0.0f, cosf(24.0f)),
	//			GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
	//
	//		m_SlashCount = 0;
	//	}
	//	break;
	//
	//case TYPE_SLASH_2:
	//	pos += m_MoveAngle * SLASH_SPEED;
	//	break;
	//default:
	//	break;
	//}

	//// 角度の計算をしていないとき
	//if (m_bCalculate == false)
	//{
	//	// m_fSlashFlameが19.0f以上だったら
	//	if (m_fSlashFlame > 19.0f)
	//	{
	//		// m_fSlashFlameと19.0fの差
	//		float fFlame = m_fSlashFlame - 19.0f;
	//		m_bCalculate = true;
	//
	//		//差の分だけ回して角度を減らす
	//		for (int nCount = 0; nCount < (int)fFlame; nCount++)
	//		{
	//			m_fSlashAngleLimit -= 6.0f;
	//		}
	//	}
	//	else if (m_fSlashFlame < 19.0f)
	//	{
	//		m_bCalculate = true;
	//
	//		//差の分だけ回して角度を増やす
	//		for (int nCount = 19; (int)m_fSlashFlame < nCount; nCount--)
	//		{
	//			m_fSlashAngleLimit -= 6.0f;
	//		}
	//	}
	//}

	switch (m_Type)
	{
	case TYPE_SLASH_1:
		m_SlashCount++;
		pos += m_MoveAngle * SLASH_SPEED;

		if (m_SlashCount > NEXT_SLASH_COUNT)
		{
			CSlash::Create(GetPos(), GetSize(),
				-m_MoveAngle,
				GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
		}


		//if (m_SlashCount > NEXT_SLASH_COUNT)
		//{
		//	for (int nCount = 0; nCount < NEXT_SLASH_NUM; nCount++)
		//	{
		//		fSlashAngle = m_fSlashAngleLimit - ((m_fSlashAngleLimit * 2) / (NEXT_SLASH_NUM - 1)) * nCount;
		//		if (fSlashAngle > 0.0f)
		//		{
		//			CSlash::Create(GetPos(), GetSize(),
		//				D3DXVECTOR3(sinf(D3DXToRadian(fSlashAngle + m_MoveAngle.y)), 0.0f, cosf(D3DXToRadian(fSlashAngle + m_MoveAngle.y))),
		//				GetColor(), NEXT_SLASH_LIFE, m_fSlashFlame, CSlash::TYPE_SLASH_2);
		//		}
		//		if (fSlashAngle == 0.0f)
		//		{
		//			CSlash::Create(GetPos(), GetSize(),
		//				-m_MoveAngle,
		//				GetColor(), NEXT_SLASH_LIFE, m_fSlashFlame, CSlash::TYPE_SLASH_2);
		//		}
		//		else if (fSlashAngle < 0.0f)
		//		{
		//			CSlash::Create(GetPos(), GetSize(),
		//				D3DXVECTOR3(sinf(D3DXToRadian(fSlashAngle + m_MoveAngle.y)), 0.0f, cosf(D3DXToRadian(fSlashAngle + m_MoveAngle.y))),
		//				GetColor(), NEXT_SLASH_LIFE, m_fSlashFlame, CSlash::TYPE_SLASH_2);
		//		}
		//	}
		//
		//	//CSlash::Create(GetPos(), GetSize(), D3DXVECTOR3((-GetMove().x) + sinf((24.0f)), 0.0f, (-GetMove().z) + cosf((24.0f))),
		//	//	GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
		//	//
		//	//CSlash::Create(GetPos(), GetSize(), D3DXVECTOR3((-GetMove().x) , 0.0f, (-GetMove().z)),
		//	//	GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
		//	//
		//	//CSlash::Create(GetPos(), GetSize(), D3DXVECTOR3((-GetMove().x) - sinf(-(24.0f)), 0.0f, (-GetMove().z) - cosf(-(24.0f))),
		//	//	GetColor(), NEXT_SLASH_LIFE, CSlash::TYPE_SLASH_2);
		//	//
		//	//CSlash::Create(GetPos() - D3DXVECTOR3(sinf(-24.0f) * fDistance, 0.0f, cosf(-24.0f) * fDistance), GetSize(), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//	//	GetColor(), NEXT_SLASH_LIFE, m_fSlashFlame, CSlash::TYPE_SLASH_2);
		//
		//	m_SlashCount = 0;
		//}
		break;

	case TYPE_SLASH_2:
		col.a -= 0.05f;
		SetColor(col);
		break;

	default:
		break;
	}

	// 敵への当たり判定
	CScene* pScene = CScene::GetSceneTop(CScene::OBJTYPE_ENEMY);
	do
	{
		if (pScene != NULL)
		{
			OBJTYPE objType = pScene->GetObjType();
			if (objType == OBJTYPE_ENEMY)
			{
				// 当たり判定
				if (CCollision::RectangleCollision3D(GetPos(), GetSize(),
					((CEnemy*)pScene)->GetPos(), ((CEnemy*)pScene)->GetSize()) == true)
				{
					// 弾を消す
					Uninit();
				}
			}
			pScene = pScene->GetSceneNext();
		}
	} while (pScene != NULL);

	SetPos(pos);
}
//=============================================================================
// 描画処理
//=============================================================================
void CSlash::Draw(void)
{
	// 描画
	CParticle::Draw();
}