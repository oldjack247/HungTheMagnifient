#ifndef _DEBUGCOLLISION_H_
#define _DEBUGCOLLISION_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene3d.h"

//=============================================================================
// デバックコリジョンクラス
//=============================================================================
class CDebugCollision : public CScene3d
{
public:

	// デバックの種類
	typedef enum
	{
		TYPE_SPHERE = 0,	    // 球体
		TYPE_SQUARE,	        // 四角
		TYPE_MAX			    // 種類の最大数
	}TYPE;

	CDebugCollision();
	~CDebugCollision();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CDebugCollision* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[TYPE_MAX];
	static LPD3DXMESH				m_pMesh[TYPE_MAX];
	static LPD3DXBUFFER				m_pBuffMat[TYPE_MAX];
	static DWORD					m_nNumMat[TYPE_MAX];

	int m_nLife;    // 寿命
	TYPE m_type;    // 種類

};

#endif