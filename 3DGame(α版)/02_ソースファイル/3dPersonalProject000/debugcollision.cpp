	//=============================================================================
// インクルードファイル
//=============================================================================
#include "debugcollision.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH	CDebugCollision::m_pMesh[TYPE_MAX] = {};
LPD3DXBUFFER CDebugCollision::m_pBuffMat[TYPE_MAX] = {};
DWORD CDebugCollision::m_nNumMat[TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CDebugCollision::m_pTexture[TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CDebugCollision::CDebugCollision() : CScene3d(OBJTYPE_DEBUG)
{
	m_nLife = 2;
	m_type = TYPE_SPHERE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CDebugCollision::~CDebugCollision()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDebugCollision::Init()
{
	// 初期化処理
	CScene3d::Init();

	BindMesh(m_pMesh[m_type], m_pBuffMat[m_type], m_nNumMat[m_type]);
	BindTexture(m_pTexture[m_type]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CDebugCollision::Uninit(void)
{
	// 終了処理
	CScene3d::Uninit();

	m_nLife = 0;
	m_type = TYPE_SPHERE;
}

//=============================================================================
// 更新処理
//=============================================================================
void CDebugCollision::Update(void)
{
	// 更新処理
	CScene3d::Update();

	// ライフの減算
	m_nLife--;

	// ライフが0以下になったら
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CDebugCollision::Draw(void)
{
	// 描画処理
	//CScene3d::Draw();
}

//=============================================================================
// モデルデータ読み込み処理
//=============================================================================
HRESULT CDebugCollision::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX(LPCSTR("data/MODEL/TESTCOLLISION/HitTest_sphere.x"), D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &m_pBuffMat[TYPE_SPHERE], NULL, &m_nNumMat[TYPE_SPHERE], &m_pMesh[TYPE_SPHERE]);

	D3DXLoadMeshFromX(LPCSTR("data/MODEL/TESTCOLLISION/HitTest_square.x"), D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &m_pBuffMat[TYPE_SQUARE], NULL, &m_nNumMat[TYPE_SQUARE], &m_pMesh[TYPE_SQUARE]);

	return S_OK;
}

//=============================================================================
// モデルデータ破棄処理
//=============================================================================
void CDebugCollision::Unload(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_pMesh[nCount] != NULL)
		{
			m_pMesh[nCount]->Release();
			m_pMesh[nCount] = NULL;
		}
		if (m_pBuffMat[nCount] != NULL)
		{
			m_pBuffMat[nCount]->Release();
			m_pBuffMat[nCount] = NULL;
		}
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
CDebugCollision* CDebugCollision::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	CDebugCollision* pDebugCollision = NULL;
	pDebugCollision = new CDebugCollision;
	pDebugCollision->m_type = type;
	pDebugCollision->SetPos(pos);
	pDebugCollision->SetSize(size);
	pDebugCollision->Init();

	return pDebugCollision;
}