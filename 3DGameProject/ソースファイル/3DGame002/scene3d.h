#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
// Scene3dクラス
//=============================================================================
class CScene3d :public CScene
{
public:
	CScene3d(int nPriority);//インクリメント
	~CScene3d();//デクリメント
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);		//描画処理

	//各種設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//位置
	void SetSize(D3DXVECTOR3 size) { m_size = size; }			//サイズ	
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);	//テクスチャ
	void BindMesh(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pBuffMat;
	DWORD					m_nNumMat;
	D3DXMATRIX			    m_mtxWorld;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	int m_nType;
};
#endif //_SCENE3D_H_