#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define EXPLOSION_SIZE 50.0f
#define PLAYER_EXPLOSION_SIZE 100.0f
#define MAX_EXPLOSION 68								//最大数

//=============================================================================
//クラス
//=============================================================================
class CExplosion : public CScene2d
{
public:
	CExplosion(int nPriority = PRIORITYTYPE_EXPLOSION);	//コンストラクタ
	~CExplosion();										//デストラクタ
	typedef enum
	{//爆発の種類
		TYPE_SIMPLE = 0,								//普通の爆発				
		TYPE_ENEMY2_DEATH,								//アイテムをドロップするおばけの爆発
		TYPE_PLAYER_DEATH,
		TYPE_MAX
	}EXPLOSIONTYPE;

	static HRESULT Load(void);							//テクスチャの読み込み処理
	static void Unload(void);							//テクスチャの破棄
	static CExplosion* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size_x, float size_y, EXPLOSIONTYPE type); //生成処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size_x, float size_y, EXPLOSIONTYPE type); //初期化処理
	void Uninit(void);									//終了処理
	void Update(void);									//更新処理
	void Draw(void);									//描画処理
	void SetAnimation(void);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		//テクスチャ
	int m_nCounterAnim;									//カウンター
	int m_nPatternAnim;									//パターンNO.
	EXPLOSIONTYPE m_type;								//爆発の種類
	D3DXVECTOR3 m_pos;									//位置
	D3DXVECTOR3 m_move;									//移動量
	D3DXCOLOR m_col;									//色
	float m_size_x;										//X座標のサイズ
	float m_size_y;										//Y座標のサイズ
};
#endif 