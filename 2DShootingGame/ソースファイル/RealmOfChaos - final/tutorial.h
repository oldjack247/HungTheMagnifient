#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

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
class CTutorial : public CScene
{
public:
	CTutorial(int nPriority = 0);												//コンストラクタ
	~CTutorial();																//デストラクタ
	static HRESULT Load(void);												//テクスチャの読み込み処理
	static void Unload(void);												//テクスチャの破棄
	static CTutorial* Create(D3DXVECTOR3 pos, float size_x, float size_y);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2d* m_apScene2D;													//Scene2Dのポインタ
	CSound* m_pSound;														//サウンドのポインタ
};
#endif