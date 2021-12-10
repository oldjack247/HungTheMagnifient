#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
// CScene2Dクラス
//=============================================================================
class CScene2D :public CScene
{
public:
	CScene2D(int nPriority = OBJTYPE_NONE);
	~CScene2D();

	static CScene2D* Create(float nPosX, float nPosY);

	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);				// テクスチャのバインド

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }			// 位置のセット
	void SetSize(D3DXVECTOR3 size) { m_size = size; }			// サイズのセット
	void SetColor(D3DXCOLOR color) { m_color = color; }			// カラーのセット
	void SetLifebar(float fMaxHp, float fCur, D3DXCOLOR col);	// HPバーの設定のセット
	void SetTextureSelect(int nPattern, int nMaxPattern);		// HPバーの設定のセット

	D3DXVECTOR3 GetPos(void) { return m_pos; }		// 位置のゲット
	D3DXVECTOR3 GetSize(void) { return m_size; }	// サイズのゲット
	D3DXCOLOR GetColor(void) { return m_color; }	// カラーのゲット

private:
	LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;		// ポリゴンの位置
	D3DXVECTOR3				m_size;		// サイズ
	D3DXCOLOR				m_color;	// 色
};

#endif // !_SCENE2D_H_
