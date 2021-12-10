#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
// ビルボードクラス
//=============================================================================
class CBillboard : public CScene
{
public:
	CBillboard(int nPriority);
	~CBillboard();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Update(float fangle);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void SetLife(int nLife) { m_nLife = nLife; }
	void SetAnimation(int nCountAnim, int nPatternAnim, int nTime) {
		m_nCountAnim = nCountAnim; m_nPatternAnim = nPatternAnim;
		m_nTime = nTime; m_Animation = true;
	}
	void SetAdditiveSynthesis(void) { m_bAdditiveSynthesis = true; }
	void SetZbuffer(void) { m_bUseZbuf = true; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXCOLOR GetColor(void) { return m_col; }
	int GetLife() { return m_nLife; }

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pBuffMat;		// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_rot;
	D3DXCOLOR m_col;
	int m_nLife;

	float                              m_fTexX1;      //テクスチャX座標
	float                              m_fTexX2;      //テクスチャX2座標
	float                              m_fTexY1;      //テクスチャY座標
	float                              m_fTexY2;      //テクスチャY座標
	D3DXMATRIX m_mtxWorld;					// マトリックス

	bool m_Animation;	// アニメーションするか
	int m_nCountAnim;	// 横の分割
	int m_nPatternAnim;	// 縦の分割
	int m_nTime;		// アニメーションにかける時間

	int m_nCountAnimChange;		// 横の分割の計算
	int m_nPatternAnimChange;	// 縦の分割の計算
	int m_nTimeChange;			// アニメーションにかける時間の計算

	bool m_bAdditiveSynthesis;	// 加算合成するか
	bool m_bUseZbuf;			// Zバッファを使うか
};
#endif