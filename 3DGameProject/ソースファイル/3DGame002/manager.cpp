//=============================================================================
// インクルードファイル
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "joystick.h"
#include "scene.h"
#include "scene2d.h"
#include "scene3d.h"
#include "camera.h"
#include "ui.h"
#include "model.h"
#include "light.h"
#include "player.h"
#include "enemy.h"
#include "tutorial.h"
#include "result.h"
#include "title.h"
#include "game.h"
#include "fade.h"
#include "map.h"
#include "particle.h"
#include "skybox.h"
#include "sound.h"
#include "gameover.h"
#include "loadmodeldata.h"
#include "debugcollision.h"
#include "number.h"

//=============================================================================
// スタティック変数初期化
//=============================================================================
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CInputJoystick* CManager::m_pInputJoystick = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CSound* CManager::m_pSound = NULL;
CFade* CManager::m_pFade = NULL;
CMode* CManager::m_pMode = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_NONE;

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindouw)
{
	//クラス生成と初期化	
	//レンダラー
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, TRUE);
	//キーボード
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);
	//ゲームパッド
	m_pInputJoystick = new CInputJoystick;
	m_pInputJoystick->Init(hInstance, hWnd);

	//サウンド
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		// 初期化処理
		if (FAILED(m_pSound->Init(hWnd)))
		{
			return -1;
		}
	}

	//テクスチャの読み込み
	LoadAll();

	//モードの設定
	SetMode(MODE_TITLE);

	// フェードのクリエイト
	CManager::CreateFade();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// すべての終了
	CScene::ReleaseAll();

	//テクスチャの破棄
	UnloadAll();

	//ライトの終了
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	//カメラの終了
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//ゲームパッドの終了
	if (m_pInputJoystick != NULL)
	{
		m_pInputJoystick->Uninit();
		delete m_pInputJoystick;
		m_pInputJoystick = NULL;
	}

	//キーボードの終了
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pMode != NULL)
	{
		//その時のモードの終了処理
		m_pMode->Uninit();
	}

	if (m_pSound != NULL)
	{
		//サウンド終了
		m_pSound->CSound::StopSound();
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	//レンダラーの終了
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	if (m_pInputKeyboard != NULL)
	{
		//キーボードの更新処理
		m_pInputKeyboard->Update();
	}

	if (m_pInputJoystick != NULL)
	{
		//ジョイスティックの更新処理
		m_pInputJoystick->Update();
	}

	if (m_pRenderer != NULL)
	{
#ifdef _DEBUG
		// FPSのセット
		m_pRenderer->SetNumFPS(m_nCountFPS);
#endif
		//レンダラーの更新処理
		m_pRenderer->Update();
	}

	if (m_pCamera != NULL)
	{
		//カメラのの更新処理
		m_pCamera->Update();
	}

	if (m_pMode != NULL)
	{
		//その時のモードの更新処理
		m_pMode->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		//描画処理
		m_pRenderer->Draw();
	}

	if (m_pMode != NULL)
	{
		//その時のモードの描画処理
		m_pMode->Draw();
	}
}

//=============================================================================
// テクスチャの読み込みまとめ
//=============================================================================
void CManager::LoadAll(void)
{
	CParticle::Load();
	CUi::Load();
	CMap::Load();
	CSkyBox::Load();
	CGameover::Load();
	CLoadmodeldata::LoadModelData("data/MODELNAME/player_model_name.txt", CHARACTERTYPE_PLAYER);
	CDebugCollision::Load();
	CNumber::Load();
}

//=============================================================================
// テクスチャの破棄まとめ
//=============================================================================
void CManager::UnloadAll(void)
{
	CMap::Unload();
	CUi::Unload();
	CParticle::Unload();
	CSkyBox::Unload();
	CGameover::Unload();
	CDebugCollision::Unload();
	CNumber::Unload();
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	if (m_pMode != NULL)
	{
		//その時のモードの終了処理
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = NULL;
	}

	if (m_pMode == NULL)
	{
		m_mode = mode;

		switch (m_mode)
		{
		case MODE_TITLE:

			//タイトルの生成
			m_pMode = new CTitle;

			//タイトルの初期化処理
			m_pMode->Init();
			break;

		case MODE_GAME:
			if (m_pCamera != NULL)
			{
				m_pCamera->Uninit();
				delete m_pCamera;
				m_pCamera = NULL;
			}
			//ゲームの生成
			m_pMode = new CGame;

			//ゲームの初期化処理
			m_pMode->Init();
			break;

		case MODE_TUTORIAL:

			//チュートリアルの生成
			m_pMode = new CTutorial;

			//チュートリアルの初期化処理
			m_pMode->Init();
			break;

		case MODE_RESULT:

			//クリア画面の生成
			m_pMode = new CResult;

			//クリア画面の初期化処理
			m_pMode->Init();

			break;
		case MODE_GAMEOVER:

			//ゲームオーバー画面の生成
			m_pMode = new CGameover;

			//ゲームオーバー画面の初期化処理
			m_pMode->Init();
			break;
		}
	}
}

//=============================================================================
// カメラの生成
//=============================================================================
void CManager::CreateCamera(void)
{
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;
		if (m_pCamera != NULL)
		{
			m_pCamera->Init();
		}
	}
}

//=============================================================================
// ライトの生成
//=============================================================================
void CManager::CreateLight(void)
{
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;
		if (m_pLight != NULL)
		{
			m_pLight->Init();
		}
	}
}

//=============================================================================
// フェードの生成
//=============================================================================
void CManager::CreateFade(void)
{
	if (m_pFade == NULL)
	{
		m_pFade = CFade::Create();
	}
}