#ifndef _MAIN_H_
#define _MAIN_H_
//=============================================================================
//
// メイン処理 [main.h]
// Author : 
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)//警告対処
#include "dinput.h"//dをつけることでinput.cppのグロバール変数を定義化できる
#include <xaudio2.h>

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")//システム時刻取得用に必要
#pragma comment(lib, "dinput8.lib")//入力処理に必要
#pragma comment(lib,"ws2_32.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//頂点フォーマット(頂点座標[2D]/頂点カラー/テクスチャ座標)
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)
#define NUM_VERTEX (4)//頂点数
#define NUM_POLYGON (2)//ポリゴン数
#define POLYGON_SIZE 70.0f
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;//テクスチャ座標
}VERTEX_2D;


#endif