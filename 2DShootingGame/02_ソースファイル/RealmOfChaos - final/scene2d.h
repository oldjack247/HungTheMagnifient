#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
//クラス
//=============================================================================
class CScene2d : public CScene
{//派生クラス
public:
	//======================================================
	//列挙型定義
	//======================================================
	typedef enum
	{
		BG_MOVE = 0,
		BG_INPUT,
		DIRECT_MAX,
	}BGOPRETION;

	CScene2d(int nPriority = 0);									//コンストラクタ
	~CScene2d();													//デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理
	void SetPosition(D3DXVECTOR3 pos);								//位置のセット
	D3DXVECTOR3 GetPosition(void);									//位置の情報取得
	void SetCol(D3DXCOLOR col);										//色のセット
	D3DXCOLOR GetCol(void);											//色の情報取得
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					//テクスチャのセット
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								//バッファ
	void MoveBg(D3DXVECTOR2 move, BGOPRETION bg);
private:
	LPDIRECT3DTEXTURE9 m_pTexture;								//テクスチャ
	D3DXVECTOR2        m_MoveBg;								// 背景の移動量を保存
	D3DXVECTOR3		   m_pos;									//位置
	D3DXCOLOR		   m_col;									//色							
};
#endif