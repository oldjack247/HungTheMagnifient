#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
//#include "number.h"
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define	SCORE_POS_X			SCREEN_WIDTH - (35.0f * 8)		// スコアの表示基準位置(Ｘ座標)
#define	SCORE_POS_Y			20.0f							// スコアの表示基準位置(Ｙ座標)
#define	SCORE_WIDTH			35.0f							// スコアの数字の幅
#define	SCORE_HEIGHT		70.0f							// スコアの数字の高さ
#define	SCORE_INTERVAL_X	35.0f - 3.0f					// スコアの数字の表示間隔
#define	NUM_PLACE			8								// スコアの桁数

//=============================================================================
//クラス
//=============================================================================
class CScore : public CScene
{
public:
	CScore(int nPriority = PRIORITYTYPE_UI);						//コンストラクタ
	~CScore();														//デストラクタ
	static HRESULT Load(void);										//テクスチャの読み込み処理
	static void Unload(void);										//テクスチャの破棄
	static CScore* Create(D3DXVECTOR3 pos, float size_x, float size_y);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理
	void AddScore(int nValue);										//スコアの加算処理関数
	int GetScore(void) { return m_nScore; }							//スコアの情報取得関数
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;							//テクスチャ
	CScene2d* m_apScene2D[NUM_PLACE];								//Scene2Dのポインタ
	int m_nScore;													//スコア

};
#endif