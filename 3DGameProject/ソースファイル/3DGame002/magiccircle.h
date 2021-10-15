#ifndef _MAGICCIRCLE_H_
#define _MAGICCIRCLE_H_

//=============================================================================
// マクロ定義
//=============================================================================
#define MAGICCERCLE_DEFAULT_SIZE (D3DXVECTOR3(200.0f, 200.0f, 0.0f))			// 魔法陣の初期サイズ
#define MAGICCERCLE_DEFAULT_ROT (D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f))	// 魔法陣の初期角度
#define MAGICCERCLE_DEFAULT_COLOR (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))			// 魔法陣の初期色
#define MAGICCERCLE_FLAMECREATE_NUM (6)											// 魔法陣の炎の生成する数
#define MAGICCERCLE_ENEMY_DEFAULT_SIZE (D3DXVECTOR3(200.0f, 200.0f, 0.0f))		// 魔法陣の初期サイズ

#define MAGICCERCLE_ENEMY_COUNTANIM (11)	// テクスチャの横の分割
#define MAGICCERCLE_ENEMY_PATTERNANIM (1)	// テクスチャの縦の分割
#define MAGICCERCLE_ENEMY_TIMEANIM (4)		// アニメーションの時間
#define MAGICCERCLE_ENEMY_CREATE_TIME (12)	// 敵を出す時間

//=============================================================================
// インクルードファイル
//=============================================================================
#include "billboard.h"
#include "particle.h"

//=============================================================================
// ファイアクラス
//=============================================================================
class CMagiccercle : public CParticle
{
public:
	typedef enum
	{
		MAGICCIRCLETYPE_PLAYER = 0,
		MAGICCIRCLETYPE_ENEMY,
	}MAGICCIRCLETYPE;

	CMagiccercle(int nPriority = OBJTYPE_PARTICLE);
	~CMagiccercle();

	static CMagiccercle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, int Life, MAGICCIRCLETYPE type);

	HRESULT Init(MAGICCIRCLETYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif