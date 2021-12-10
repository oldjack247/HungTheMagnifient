//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene3d.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3d::CScene3d(int nPriority) :CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pTexture = NULL;
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_nNumMat = 0;
	D3DXMatrixIdentity(&m_mtxWorld);

}

//=============================================================================
// デストラクタ
//=============================================================================
CScene3d::~CScene3d()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3d::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene3d::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene3d::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScene3d::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL* pMat = nullptr;

	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxSize, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxSize);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pBuffMat != NULL)
	{
		//マテリアルデータへのポインタを取得する
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得する
	pDevice->GetMaterial(&matDef);

	pDevice->SetTexture(0, m_pTexture);

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_pMesh != NULL)
		{
			//モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);
		}
	}

	//保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
	pDevice->SetTexture(0, NULL);
}

//================================================================================
// 各種設定
//================================================================================

//=============================================================================
// テクスチャ
//=============================================================================
void CScene3d::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

void CScene3d::BindMesh(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;
}
