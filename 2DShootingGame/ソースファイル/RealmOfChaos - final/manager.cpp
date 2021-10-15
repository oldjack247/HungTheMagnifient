//=============================================================================
//インクルードファイル
//=============================================================================
#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include "manager.h"
#include <time.h>
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "input.h"
#include "keyboard.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "controller.h"
#include "score.h"
#include "life.h"
#include "item.h"
#include "effect.h"
#include "sound.h"
#include "tutorial.h"
#include "pause.h"
#include "game_clear.h"
#include "game_over.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CSound* CManager::m_pSound = NULL;
CGame* CManager::m_pGame = NULL;
CTitle* CManager::m_pTitle = NULL;
CTutorial* CManager::m_pTutorial = NULL;
CResult* CManager::m_pResult = NULL;
CFade* CManager::m_pFade = NULL;
CGameClear* CManager::m_pGameClear = NULL;
CGameOver* CManager::m_pGameOver = NULL;
CInputController* CManager::m_pInputController = NULL;
D3DXVECTOR3 CManager::m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;

//=============================================================================
//コンストラクタ
//=============================================================================
CManager::CManager()
{
}

CManager::~CManager()
{
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	WSADATA wsaData;
	//winsock初期化処理
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (nErr != 0)
	{
		//何らかの原因で初期化に設定した
		printf("WSAStartup faild\n");
	}
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		// 初期化処理
		if (m_pRenderer->Init(hWnd, bWindow))
		{
			return -1;
		}
	}

	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;
		m_pInputKeyboard->Init(hInstance, hWnd);
	}
	if (m_pInputController == NULL)
	{
		m_pInputController = new CInputController;
		m_pInputController->InitJoypad(hInstance, hWnd);
	}
	if (m_pFade == NULL)
	{
		m_pFade = CFade::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CManager::MODE_TITLE);
	}
	if (m_pSound == NULL)
	{
		// サウンドのメモリ確保
		m_pSound = new CSound;
	}
	// サウンドの初期化処理
	if (FAILED(m_pSound->InitSound(hWnd)))
	{
		return -1;
	}
	CTitle::Load();
	CTutorial::Load();
	CGameClear::Load();
	CGameOver::Load();
	CResult::Load();
	CPause::Load();
	SetMode(MODE_TITLE);
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//winsock終了処理
	WSACleanup();
	if (m_pRenderer != NULL)
	{
		// 終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	if (m_pInputController != NULL)
	{
		m_pInputController->Uninit();
		delete m_pInputController;
		m_pInputController = NULL;
	}
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// m_pSoundがNULLじゃないとき
	if (m_pSound != NULL)
	{
		// サウンドの終了処理
		m_pSound->UninitSound();

		// メモリの開放(サウンドのクラス)
		delete m_pSound;
		m_pSound = NULL;
	}
	CTitle::Unload();
	CTutorial::Unload();
	CGameClear::Unload();
	CGameOver::Unload();
	CResult::Unload();
	CPause::Unload();
	CScene::ReleaseAll();
}

//=============================================================================
//更新処理
//=============================================================================
void CManager::Update(void)
{
	m_pRenderer->Update();

	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}
	if (m_pInputController != NULL)
	{
		m_pInputController->UpdateJoypad();
	}
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}
	switch (m_mode)
	{
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;

	case CManager::MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Update();
		}
		break;
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Update();
		}
		break;

	case CManager::MODE_GAMECLEAR:
		if (m_pGameClear != NULL)
		{
			m_pGameClear->Update();
		}
		break;

	case CManager::MODE_GAMEOVER:
		if (m_pGameOver != NULL)
		{
			m_pGameOver->Update();
		}
		break;

	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;
	default:
		break;
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();								// 描画処理
	if (m_pFade != NULL)
	{
		m_pFade->Draw();
	}
	switch (m_mode)
	{
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Draw();
		}
		break;

	case CManager::MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Draw();
		}
		break;

	case CManager::MODE_GAME:
		break;

	case CManager::MODE_GAMECLEAR:
		if (m_pGameClear != NULL)
		{
			m_pGameClear->Draw();
		}
		break;

	case CManager::MODE_GAMEOVER:
		if (m_pGameOver != NULL)
		{
			m_pGameOver->Draw();
		}
		break;

	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Draw();
		}
		break;
	default:
		break;
	}
}

//=============================================================================
//レンダラーの情報を取得処理
//=============================================================================
CRenderer* CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
//キーボードの情報を取得処理
//=============================================================================
CInputKeyboard* CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case CManager::MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			m_pTutorial = NULL;
		}
		break;

	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

	case CManager::MODE_GAMECLEAR:
		if (m_pGameClear != NULL)
		{
			m_pGameClear->Uninit();
			m_pGameClear = NULL;
		}
		break;

	case CManager::MODE_GAMEOVER:
		if (m_pGameOver != NULL)
		{
			m_pGameOver->Uninit();
			m_pGameOver = NULL;
		}
		break;

	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
	default:
		break;
	}

	CScene::ReleaseAll();

	m_mode = mode;

	switch (m_mode)
	{
	case CManager::MODE_TITLE:
		if (m_pTitle == NULL)
		{
			m_pTitle = new CTitle;
			if (m_pTitle != NULL)
			{
				m_pTitle->Init(m_pos, BG_SIZE_X, BG_SIZE_Y);
			}
		}
		break;

	case CManager::MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{
			m_pTutorial = new CTutorial;
			if (m_pTutorial != NULL)
			{
				m_pTutorial->Init(m_pos, BG_SIZE_X, BG_SIZE_Y);
			}
		}
		break;

	case CManager::MODE_GAME:
		if (m_pGame == NULL)
		{
			m_pGame = new CGame;
			if (m_pGame != NULL)
			{
				m_pGame->Init(m_pos, 0.0f, 0.0f);
			}
		}
		break;

	case CManager::MODE_GAMECLEAR:
		if (m_pGameClear == NULL)
		{
			m_pGameClear = new CGameClear;
			if (m_pGameClear != NULL)
			{
				m_pGameClear->Init(m_pos, BG_SIZE_X / 2, BG_SIZE_Y / 2);
			}
		}
		break;

	case CManager::MODE_GAMEOVER:
		if (m_pGameOver == NULL)
		{
			m_pGameOver = new CGameOver;
			if (m_pGameOver != NULL)
			{
				m_pGameOver->Init(m_pos, BG_SIZE_X / 2, BG_SIZE_Y / 2);
			}
		}
		break;

	case CManager::MODE_RESULT:
		if (m_pResult == NULL)
		{
			m_pResult = new CResult;
			if (m_pResult != NULL)
			{
				m_pResult->Init(m_pos, BG_SIZE_X, BG_SIZE_Y);
			}
		}
		break;
	default:
		break;
	}
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}
	if (m_pInputController != NULL)
	{
		m_pInputController->UpdateJoypad();
	}
}

CInputController* CManager::GetController(void)
{
	return m_pInputController;
}

CSound* CManager::GetSound(void)
{
	// m_pSoundを返す
	return m_pSound;
}


