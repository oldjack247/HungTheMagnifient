#ifndef _RESULT_H_
#define _RESULT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "manager.h"
#include "scene2d.h"
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CRanking;
class CSound;
class CCrient_ranking;

//=============================================================================
//クラス
//=============================================================================
class CResult : public CScene
{
public:
	CResult(int nPriority = 0);												//コンストラクタ
	~CResult();																//デストラクタ
	static HRESULT Load(void);												//テクスチャの読み込み処理
	static void Unload(void);												//テクスチャの破棄
	static CResult* Create(D3DXVECTOR3 pos, float size_x, float size_y);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2d* m_apScene2D;													//Scene2Dのポインタ
	static CBg* m_pBg;														//BGのポインタ
	static CRanking* m_pRanking;											//ランキングのポインタ
	CSound* m_pSound;														//サウンドのポインタ
	static CCrient_ranking* m_pCrient_ranking;
};
#endif