//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"
#include "scene2d.h"

//=============================================================================
//静的メンバ変数
//=============================================================================
CScene* CScene::m_pTop[OBJTYPE_MAX] = {};
CScene* CScene::m_pCur[OBJTYPE_MAX] = {};
bool CScene::m_bUpdate[OBJTYPE_MAX] = {};
bool CScene::m_bDraw[OBJTYPE_MAX] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{
	m_pNext = NULL;
	m_pPrev = NULL;
	//トップがNULLの時
	if (m_pTop[nPriority] == NULL)
	{
		m_pTop[nPriority] = this;
	}
	// 現在のオブジェクト(m_pCur)が存在しない場合
	if (m_pCur[nPriority] == NULL)
	{
		m_pCur[nPriority] = this;
	}

	// 現在のオブジェクト(m_pCur)の次のオブジェクトを自分にする
	m_pCur[nPriority]->m_pNext = this;

	if (this == m_pCur[nPriority])
	{
		this->m_pPrev = NULL;
	}
	else
	{
		this->m_pPrev = m_pCur[nPriority];
	}

	// 現在のオブジェクト(m_pCur)を自分にする
	m_pCur[nPriority] = this;

	// 自分の次のオブジェクトをNULLにする
	this->m_pNext = NULL;

	m_objType = OBJTYPE_NONE;
	m_nObj = nPriority;
	m_bDeath = false;

}

//=============================================================================
//コンストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// オブジェクトタイプ設定
//=============================================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//=============================================================================
// リリース処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		CScene* pScene = m_pTop[nCount];

		while (pScene != NULL)
		{
			//終了処理
			CScene* pSave = pScene->m_pNext;
			pScene->Release();
			pScene = pSave;
		}
	}
}

//=============================================================================
// 指定したもの以外のリリース処理
//=============================================================================
void CScene::DesignationReleaseAll(OBJTYPE type)
{
	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		if (nCount != type)
		{
			CScene* pScene = m_pTop[nCount];

			while (pScene != NULL)
			{
				//終了処理
				CScene* pSave = pScene->m_pNext;
				pScene->Release();
				pScene = pSave;
			}
		}
	}
}

//=============================================================================
// 指定したオブジェクトのアップデートを止めるか動かすか
//=============================================================================
void CScene::SetbUpdate(bool bUpdate, OBJTYPE type)
{
	m_bUpdate[type] = bUpdate;
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene::UpdateAll(void)
{
	// OBJTYPEの数分回す
	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		// 更新するなら
		if (m_bUpdate[nCount] == false)
		{
			// Topの情報の保持
			CScene* pScene = m_pTop[nCount];
			while (pScene != NULL)
			{
				// Nextの情報の保持
				CScene* pSave = pScene->m_pNext;

				// 死亡フラグが立ってないとき
				if (pScene->m_bDeath == false)
				{
					//更新処理
					pScene->Update();
				}
				// Nextの情報をpSceneに入れる
				pScene = pSave;
			}
		}
	}

	// OBJTYPEの数分回す
	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		CScene* pScene = m_pTop[nCount];
		while (pScene != NULL)
		{
			CScene* pSave = pScene->m_pNext;

			// 死亡フラグが立ってたら
			if (pScene->m_bDeath == true)
			{
				// リストのつなぎなおしをして消す
				pScene->ConnectionList();
				//オブジェクトを破棄
				delete pScene;
			}
			pScene = pSave;
		}
	}
}

//=============================================================================
// オブジェクトクラスのリストのつなぎなおし
//=============================================================================
void CScene::ConnectionList(void)
{
	// 前の情報を持っているとき
	if (m_pPrev != NULL)
	{
		m_pPrev->m_pNext = m_pNext;
	}

	// 次の情報を持っているとき
	if (m_pNext != NULL)
	{
		m_pNext->m_pPrev = m_pPrev;
	}

	// 自分が先頭と現在のオブジェクトだったとき
	if (this == m_pTop[m_nObj] && this == m_pCur[m_nObj])
	{
		m_pTop[m_nObj] = NULL;
		m_pCur[m_nObj] = NULL;
	}

	// 自分が先頭のオブジェクトだったとき
	if (this == m_pTop[m_nObj])
	{
		m_pTop[m_nObj] = m_pNext;
	}

	// 自分が現在のオブジェクトだったとき
	if (this == m_pCur[m_nObj])
	{
		m_pCur[m_nObj] = m_pPrev;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCountpriority = 0; nCountpriority < OBJTYPE_MAX; nCountpriority++)
	{
		// 描画するなら
		if (m_bDraw[nCountpriority] == false)
		{
			// Topの情報の保持
			CScene* pScene = m_pTop[nCountpriority];

			while (pScene != NULL)
			{
				// Nextの情報の保持
				CScene* pSceneNext = pScene->m_pNext;

				if (pScene->m_bDeath != true)
				{
					pScene->Draw();
					pScene = pSceneNext;
				}
			}
		}
	}
}

//=============================================================================
// リリース処理
//=============================================================================
void CScene::Release(void)
{
	//死亡フラグを立てる
	m_bDeath = true;
}