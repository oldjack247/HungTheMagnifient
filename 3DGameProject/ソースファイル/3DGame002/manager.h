#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoystick;
class CCamera;
class CLight;
class CFade;
class CPlayer;
class CModel;
class CMode;
class CSound;

//=============================================================================
// マネージャークラス
//=============================================================================
class CManager
{
public:
	//画面遷移の列挙型
	typedef enum
	{
		MODE_NONE = 0,
		MODE_TITLE,		//タイトル
		MODE_GAME,		//ゲーム
		MODE_TUTORIAL,	//チュートリアル
		MODE_RESULT,	//リザルト
		MODE_GAMEOVER, //ゲームオーバー
		MODE_MAX
	}MODE;

	CManager();													//インクリメント
	~CManager();												//デクリメント
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindouw);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理

	void LoadAll(void);											//テクスチャの読み込みまとめ
	void UnloadAll(void);										//テクスチャの破棄まとめ

	static void SetMode(MODE mode);								//モードの設定

	static void CreateCamera(void);
	static void CreateLight(void);
	static void CreateFade(void);

	void SetNumFPS(int nCountFPS) { m_nCountFPS = nCountFPS; }

	static CRenderer* GetRenderer(void) { return m_pRenderer; }					//レンダラー
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }	//キーボード
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; }			//マウス
	static CInputJoystick* GetInputJoystick(void) { return m_pInputJoystick; }	//ゲームパッド
	static CCamera* GetCamera(void) { return m_pCamera; }						//カメラ
	static CLight* GetLight(void) { return m_pLight; }							//ライト
	static CFade* GetFade(void) { return m_pFade; }								//フェード
	static MODE GetMode(void) { return m_mode; }								//モード
	static CSound* GetSound(void) { return m_pSound; }							//サウンド

private:
	static CRenderer* m_pRenderer;				//レンダラー
	static CInputKeyboard* m_pInputKeyboard;	//キーボード
	static CInputMouse* m_pInputMouse;			//マウス
	static CInputJoystick* m_pInputJoystick;	//ゲームパッド
	static CCamera* m_pCamera;					//カメラ
	static CLight* m_pLight;					//ライト
	static CFade* m_pFade;						//フェード
	static CSound* m_pSound;					//サウンド
	static CMode* m_pMode;						//モードのポインタ
	static MODE m_mode;							//現在のモード
	int m_nCountFPS;							// FPSの値
};

#endif // !_MANAGER_H_
