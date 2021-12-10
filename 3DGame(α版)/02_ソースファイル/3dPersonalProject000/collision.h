#ifndef _COLLISION_H_
#define _COLLISION_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// コリジョンクラス
//=============================================================================
class CCollision
{
public:
	static bool RectangleCollision3D(D3DXVECTOR3 pPos1, D3DXVECTOR3 size1, D3DXVECTOR3 pPos2, D3DXVECTOR3 size2);
	static bool SphereCollision(D3DXVECTOR3 pos1, float size1, D3DXVECTOR3 pos2, float size2);
	static bool SphereCollisionInside(D3DXVECTOR3 pos1, float size1, D3DXVECTOR3 pos2);
private:

};
#endif
