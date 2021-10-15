//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"
#include "scene2d.h"
#include "renderer.h"
#include "main.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
int CScene::m_nNumAll = 0;
CScene* CScene::m_apScene[PRIORITYTYPE_MAX][MAX_SCENE] = {};

//=============================================================================
//値のクリア
//=============================================================================
CScene::CScene(int nPriority)
{
	m_nPriority = nPriority;
	for (int nCount = 0; nCount < MAX_SCENE; nCount++)
	{
		if (m_apScene[m_nPriority][nCount] == NULL)
		{
			m_apScene[m_nPriority][nCount] = this;
			m_nID = nCount;
			m_nNumAll++;
			break;
		}
	}
	m_nType = OBJTYPE_NONE;
}

//=============================================================================
//デストラクタ
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScene::Draw(void)
{
}

//=============================================================================
//全て更新
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nPriority = 0; nPriority < PRIORITYTYPE_MAX; nPriority++)
	{
		for (int nCount = 0; nCount < MAX_SCENE; nCount++)
		{
			if (m_apScene[nPriority][nCount] != NULL)
			{
				m_apScene[nPriority][nCount]->Update();
			}
		}
	}
}

//=============================================================================
//全て描画
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nPriority = 0; nPriority < PRIORITYTYPE_MAX; nPriority++)
	{
		for (int nCount = 0; nCount < MAX_SCENE; nCount++)
		{
			if (m_apScene[nPriority][nCount] != NULL)
			{
				m_apScene[nPriority][nCount]->Draw();
			}
		}
	}
}

//=============================================================================
//全て開放
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nPriority = 0; nPriority < PRIORITYTYPE_MAX; nPriority++)
	{
		for (int nCount = 0; nCount < MAX_SCENE; nCount++)
		{
			if (m_apScene[nPriority][nCount] != NULL)
			{
				m_apScene[nPriority][nCount]->Uninit();
				delete m_apScene[nPriority][nCount];
				m_apScene[nPriority][nCount] = NULL;
			}
		}
	}
}

//=============================================================================
//特定のオブジェクトのみ開放
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		int nID;
		int nPriority;
		nID = m_nID;
		nPriority = m_nPriority;
		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
		m_nNumAll--;
	}
}

//=============================================================================
// オブジェクトタイプのセット
//=============================================================================
void CScene::SetObjType(OBJTYPE type)
{
	m_nType = type;
}

//=============================================================================
// シーンの情報取得
//=============================================================================
CScene* CScene::GetScene(int nPriority, int nNum)
{
	return m_apScene[nPriority][nNum];
}
