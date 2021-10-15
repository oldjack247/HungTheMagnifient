#ifndef _FADE_H_
#define _FADE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "manager.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define FADE_RATE 0.05f					//時間

//=============================================================================
//クラス
//=============================================================================
class CFade
{
public:
	typedef enum
	{//フェードの情報
		FADE_NONE,										//通常
		FADE_IN,										//フェードイン
		FADE_OUT,										//フェードアウト
		FADE_MAX										//最大数

	}FADE;

	CFade();											//コンストラクタ
	~CFade();											//デストラクタ
	static CFade* Create(D3DXVECTOR3 pos, float size_x, float size_y, CManager::MODE modenext);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y, CManager::MODE modenext);	//初期化処理
	void Uninit(void);									//終了処理
	void Update(void);									//更新処理
	void Draw(void);									//描画処理
	static void SetFade(CManager::MODE modenext);		//フェードのセット
	static FADE GetFade(void) { return m_Fade; }
	static void SetFadeSwitch(bool bFadeSwitch);		//フェードを切り替え
private:
	static FADE m_Fade;									//フェードの情報を持った変数
	static CManager::MODE m_ModeNext;					//モードの情報を持った変数
	static D3DXCOLOR m_col;								//カラー
	static bool m_bFadeSwitch;							//フェードを行うかどうか
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					//バッファ
};
#endif