#ifndef _LIFE_H_
#define _LIFE_H_

//=============================================================================
//マクロ定義
//=============================================================================
#define HP_SIZE_X 10						//X軸の大きさ
#define HP_SIZE_Y 5							//Y軸の大きさ
#define	NUM_HP 50							//数

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"
#include "scene2d.h"
#include "player.h"

//=============================================================================
//クラス
//=============================================================================
class CLife : public CScene
{
public:
	CLife(int nPriority = PRIORITYTYPE_UI);								//コンストラクタ
	~CLife();															//デストラクタ
	static HRESULT Load(void);											//テクスチャの読み込み処理
	static void Unload(void);											//テクスチャの破棄
	static CLife* Create(D3DXVECTOR3 pos, float size_x, float size_y);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);			//初期化処理
	void Uninit(void);													//終了処理
	void Update(void);													//更新処理
	void Draw(void);													//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2d* m_apScene2D[MAX_LIFE];									//Scene2Dのポインタ
};
#endif
