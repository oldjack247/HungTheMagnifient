#ifndef _FIRE_H_
#define _FIRE_H_

//=============================================================================
// マクロ定義
//=============================================================================
#define FIRE_SPEED_ADD_SUBTRACT_Z (0.06f)		// Zの移動量の加算と減算用
#define FIRE_SPEED_ADD_Y (0.05f)				// Yの加算用
#define FIRE_SUBTRACT_COLOR_RED_GREEN (0.03f)	// 赤と緑の下げる量
#define FIRE_SUBTRACT_COLOR_ALPHA (0.003f)		// 透明度の下げる量

#define FIR_SIZE (D3DXVECTOR3(50.0f, 50.0f, 0.0f))			// サイズ
#define FIRE_COLOR (D3DXCOLOR(0.6f, 0.9f, 1.0f, 1.0f))		// 色
#define FIRE_LIFE (60)										// ライフ
#define FIRE_MOVE_Y (1.5f)									// ライフ
#define FIRE_MOVE_Z (2.2f)									// ライフ
#define FIRE_CREATE_ANGLE_MAX (360)							// 生成する角度の最大値

//=============================================================================
// インクルードファイル
//=============================================================================
#include "billboard.h"
#include "particle.h"

//=============================================================================
// ファイアクラス
//=============================================================================
class CFire : public CParticle
{
public:
	CFire(int nPriority = OBJTYPE_PARTICLE);
	~CFire();

	static CFire* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
		D3DXVECTOR3 move, D3DXCOLOR col, int Life);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif