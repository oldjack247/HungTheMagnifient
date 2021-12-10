#ifndef _ENEMYDEATHEFFECT_H_
#define _ENEMYDEATHEFFECT_H_

//=============================================================================
// マクロ定義
//=============================================================================
#define ENEMYDEATHEFFEC_DEFAULT_SIZE (D3DXVECTOR3(80.0f, 80.0f, 0.0f))		// 敵が死んだエフェクトのの初期サイズ
#define ENEMYDEATHEFFEC_DEFAULT_COLOR (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 敵が死んだエフェクトのの初期カラー
#define ENEMYDEATHEFFEC_DEFAULT_SPEED (1.5f)								// 敵が死んだエフェクトのの初期スピード
#define ENEMYDEATHEFFEC_SUBTRACT_COLOR_ALPHA (0.01f)						// 透明度の下げる量
#define ENEMYDEATHEFFEC_COUNTANIM (13)	// テクスチャの横の分割
#define ENEMYDEATHEFFEC_PATTERNANIM (1) // テクスチャの縦の分割
#define ENEMYDEATHEFFEC_TIMEANIM (4)	// アニメーションの時間
#define ENEMYDEATHEFFEC_CREATE_NUM (6)	// 作る数

//=============================================================================
// インクルードファイル
//=============================================================================
#include "billboard.h"
#include "particle.h"

//=============================================================================
// ファイアクラス
//=============================================================================
class CEnemydeatheffect : public CParticle
{
public:
	CEnemydeatheffect(int nPriority = OBJTYPE_PARTICLE);
	~CEnemydeatheffect();

	static CEnemydeatheffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXVECTOR3 move, D3DXCOLOR col, int Life);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif