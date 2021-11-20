#ifndef _FADE_H_
#define _FADE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "renderer.h"
#include "manager.h"
#include "mode.h"
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define FADE_RATE (0.05f) // フェードの速度

//=============================================================================
// フェードクラス
//=============================================================================
class CFade :public CScene2D
{
public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT
	}FADE;

	CFade(int nPriority = OBJTYPE_FADE);	//コンストラクタ
	~CFade();								//デストラクタ

	static CFade* Create(void);				// クリエイト

	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理	
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理

	void FadeIn(void);						// フェードイン
	void FadeOut(void);						// フェードアウト

	void SetFade(CManager::MODE mode);	// フェードの設定
	FADE GetFadeState(void) { return m_fade; }						// フェードの状態のゲット
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXCOLOR				m_color;		// 色
	CManager::MODE			m_modeNext;		// 次の画面(モード)
	FADE					m_fade;			// フェード状態
};
#endif // !_FADE_H_
