#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BOSS_EFFECT_SIZE 10.0f						//ボスのエフェクトのサイズ
#define ENEMY_EFFECT_SIZE 80.0f						//敵のエフェクトのサイズ
#define BULLET_SPAWN_SIZE 40.0f
#define BULLET_DEATH_SIZE 150.0f
#define BULLET_EFFECT_SIZE_X 15.0f
#define BULLET_EFFECT_SIZE_Y 20.0f
#define MAX_EFFECT 150
#define FADE_SIZE 1.5f
#define FADE_COL 0.02f
#define FADE_COUNT 5

//=============================================================================
//クラス
//=============================================================================
class CEffect : public CScene2d
{
public:
	CEffect(int nPriority = PRIORITYTYPE_EFFECT);
	~CEffect();
	typedef enum
	{
		TYPE_2FBOSS = 0,
		TYPE_ENEMY2_DEATH,
		TYPE_BULLET_SPAWN,
		TYPE_BULLET_DEATH,
		TYPE_BULLET_EFFECT,
		TYPE_MAX
	}EFFECTTYPE;

	static HRESULT Load(void);
	static void Unload(void);
	static CEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size_x, float size_y, EFFECTTYPE type);					//生成処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size_x, float size_y, EFFECTTYPE type);							//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	int m_nCounterAnim;								//カウンター
	int m_nPatternAnim;								//パターンNO.
	EFFECTTYPE m_type;								//種類
	D3DXVECTOR3 m_pos;								//位置
	D3DXVECTOR3 m_move;								//移動量
	D3DXCOLOR m_col;								//カラー
	float m_size_x;									//x座標のサイズ
	float m_size_y;									//y座標のサイズ
	int m_nEffectFade;								//消滅カウント
};
#endif 