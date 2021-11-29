#ifndef _HITEFFECT_H_
#define _HITEFFECT_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"particle.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define HITEFFECT_PLAYER_COUNTANIM (8)									// テクスチャの横の分割
#define HITEFFECT_PLAYER_PATTERNANIM (1)								// テクスチャの縦の分割
#define HITEFFECT_PLAYER_TIMEANIM (2)									// アニメーションの時間
#define HITEFFECT_PLAYER_SIZE (D3DXVECTOR3(200.0f, 200.0f, 0.0f))		// 大きさ
#define HITEFFECT_PLAYER_COLOR (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// 色

#define HITEFFECT_ENEMY_COUNTANIM (5)									// テクスチャの横の分割
#define HITEFFECT_ENEMY_PATTERNANIM (1)									// テクスチャの縦の分割
#define HITEFFECT_ENEMY_TIMEANIM (4)									// アニメーションの時間
#define HITEFFECT_ENEMY_SIZE (D3DXVECTOR3(100.0f, 100.0f, 0.0f))		// 大きさ
#define HITEFFECT_ENEMY_COLOR (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// 色

//=============================================================================
// スパーククラス
//=============================================================================
class CHiteffect : public CParticle
{
public:
	typedef enum
	{
		HITEFFECTTYPE_PLAYER = 0,
		HITEFFECTTYPE_ENEMY,
	}HITEFFECTTYPE;

	CHiteffect();
	~CHiteffect();

	static CHiteffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int Life, HITEFFECTTYPE type);
	HRESULT Init(HITEFFECTTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif