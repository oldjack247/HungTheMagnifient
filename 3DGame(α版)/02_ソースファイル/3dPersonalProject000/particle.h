#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "billboard.h"

//=============================================================================
// パーティクルクラス
//=============================================================================
class CParticle : public CBillboard
{
public:
	// テクスチャタイプ
	typedef enum
	{
		TEX_TYPE_NONE = 0,
		TEX_TYPE_1,
		TEX_TYPE_2,
		TEX_TYPE_SWORD,
		TEX_TYPE_PLAYERATTACK,
		TEX_TYPE_FIRE,
		TEX_TYPE_MAGICCIRCLE,
		TEX_TYPE_ENEMYCREATE_MAGICCIRCLE,
		TEX_TYPE_ENEMYDEATH,
		TEX_TYPE_ENEMYATTACK,
		TEX_TYPE_MAX,
	}TEX_TYPE;

	CParticle(int nPriority = OBJTYPE_PARTICLE);
	~CParticle();

	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(TEX_TYPE TexType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MagiccircleCollision(void);

	void SetAngle(float fangle) { m_fAngle = fangle; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }

	float GetAngle(void) { return m_fAngle; }
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_TYPE_MAX];	// texture
	TEX_TYPE m_TexType;									// テクスチャタイプ
	float m_fAngle;										// 角度
	D3DXVECTOR3 m_Move;									// 角度
};
#endif