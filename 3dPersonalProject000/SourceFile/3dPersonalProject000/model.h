#ifndef _MODEL_H_
#define _MODEL_H_

//=============================================================================
// モデルクラス
//=============================================================================
class CModel
{
public:
	CModel();
	~CModel();

	static CModel* Create();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat);
	void BindTexture(LPDIRECT3DTEXTURE9 Texture, int nCount);
	void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);

	void SetParent(CModel* cmodel);
	void SetWorldMatrix(D3DXMATRIX mtxWorld);
	void SetBoolShow(bool bShow);

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }
	void Setrot(D3DXVECTOR3 rot) { m_rot = rot; }
	void Setsize(D3DXVECTOR3 size) { m_size = size; }

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }
private:
	LPD3DXMESH m_pMesh;		 // メッシュ情報のポインタ
	LPD3DXBUFFER m_pBuffMat; // マテリアル情報のポインタ
	DWORD m_nNumMat;		 // マテリアル情報の数
	D3DXMATRIX m_mtxWorld;	 // 行列計算用
	LPDIRECT3DTEXTURE9 m_pTexture[50]; // テクスチャの情報

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_size;		// サイズ
	bool		m_bShow; // モデルを表示するか
	CModel* m_pParent;
};

#endif