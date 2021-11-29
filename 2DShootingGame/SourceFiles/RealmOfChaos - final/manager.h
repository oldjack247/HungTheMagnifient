#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include <windows.h>
#include "main.h"

//=============================================================================
//前方宣言
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CPlayer;
class CBullet;
class CSound;
class CBg;
class CEnemy;
class CGame;
class CTitle;
class CTutorial;
class CResult;
class CFade;
class CInputController;
class CGameClear;
class CGameOver;

//=============================================================================
//クラス
//=============================================================================
class CManager
{
public:
	typedef enum
	{//モードの種類
		MODE_TITLE = 0,												//タイトル
		MODE_TUTORIAL,												//チュートリアル
		MODE_GAME,													//ゲーム
		MODE_RESULT,												//リザルト
		MODE_GAMECLEAR,
		MODE_GAMEOVER,
		MODE_MAX
	}MODE;

	CManager();														//コンストラクタ
	~CManager();													//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理
	static CRenderer* GetRenderer(void);							//レンダラーの情報取得湯
	static CInputKeyboard* GetInputKeyboard(void);					//キーボードの情報取得湯
	static CSound* GetSound(void);									//サウンドの情報取得湯
	static void SetMode(MODE mode);									//
	static MODE GetMode(void) { return m_mode; };
	static CFade* GetFade(void) { return m_pFade; };				//フェードの情報取得
	static CInputController* GetController(void);					//コントローラーの情報取得湯
private:
	static MODE m_mode;												//モードの情報を持った変数
	static D3DXVECTOR3 m_pos;										//位置
	static CInputKeyboard* m_pInputKeyboard;						//キーボードのポインタ
	static CRenderer* m_pRenderer;									//レンダラーの7ポインタ
	static CSound* m_pSound;										//サウンドのポインタ
	static CBg* m_pBg;												//BGのポインタ
	static CGame* m_pGame;											//ゲームのポインタ
	static CTitle* m_pTitle;										//タイトルのポインタ
	static CTutorial* m_pTutorial;
	static CGameClear* m_pGameClear;
	static CGameOver* m_pGameOver;
	static CResult* m_pResult;										//リザルトのポインタ
	static CFade* m_pFade;											//フェードのポインタ
	static CInputController* m_pInputController;					//コントローラーのポインタ
};
#endif