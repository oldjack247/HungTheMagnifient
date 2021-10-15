#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "manager.h"
#include "scene2d.h"
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CSound;

//=============================================================================
//クラス
//=============================================================================
class CTitle : public CScene
{
public:
	typedef enum
	{//タイトルテクスチャの種類
		TITLETYPE_BG = 0,										//背景
		TITLETYPE_NAME,											//ゲームの名前
		TITLETYPE_START,										//スタートボタン
		TITLETYPE_MAX
	}TITLETYPE;

	CTitle(int nPriority = PRIORITYTYPE_BG);					//コンストラクタ
	~CTitle();													//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み処理
	static void Unload(void);									//テクスチャの破棄
	static CTitle* Create(D3DXVECTOR3 pos, float size_x, float size_y);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TITLETYPE_MAX];		//テクスチャ
	TITLETYPE m_titletype;										//タイトル
	CScene2d* m_apScene2D[TITLETYPE_MAX];						//Scene2Dのポインタ										
	D3DXVECTOR3 m_pos;											//位置
	D3DXCOLOR m_col;											//色
	CSound* m_pSound;											//音のポインタ
};
#endif
