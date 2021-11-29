#ifndef _ITEM_H_
#define _ITEM_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ITEM_SIZE 90.0f
#define MAX_ITEM 56
#define ITEM_LIFE 1

//=============================================================================
//クラス
//=============================================================================
class CItem : public CScene2d
{
public:

	typedef enum
	{
		TYPE_ITEM_HP = 0,
		TYPE_ITEM_BULLET,
		TYPE_ITEM_MINION,
		TYPE_MAX
	}ITEMTYPE;

	CItem();
	~CItem();
	static HRESULT Load(void);											//テクスチャの読み込み処理
	static void Unload(void);											//テクスチャの破棄
	static CItem* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size_x, float size_y, ITEMTYPE Type);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size_x, float size_y, ITEMTYPE Type);	//初期化処理
	void Uninit(void);													//終了処理
	void Update(void);													//更新処理
	void Draw(void);													//描画処理
	void SetLife(int nLife);											//体力のセット
	bool HitCcollision(D3DXVECTOR3 Pos, float size_x, float size_y);	//当たり判定
private:
	D3DXVECTOR3 m_pos;													//位置
	D3DXVECTOR3 m_move;													//移動量
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	int m_nLife;														//体力
	ITEMTYPE m_type;
	int m_nCounterAnim;													//カウンター
	int m_nPatternAnim;													//パターンNO.
	bool m_bHit;														//当たったかどうか
};
#endif
