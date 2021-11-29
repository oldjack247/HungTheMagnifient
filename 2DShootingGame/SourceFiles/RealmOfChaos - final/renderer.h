#ifndef _RENDERER_H_
#define _RENDERER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_RENDERER 3

//=============================================================================
//クラス
//=============================================================================
class CRenderer
{//基本クラス
public:
	CRenderer();								//コンストラクタ
	~CRenderer();								//デストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);		//初期化処理
	void Uninit(void);							//終了処理
	void Update(void);							//更新処理
	void Draw(void);							//描画処理
	LPDIRECT3DDEVICE9 GetDevice(void);			//取得処理
	static void ResumeGame(void) { m_bPause = false; }
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPD3DXFONT m_pFont;
	static bool	m_bPause;
};
#endif