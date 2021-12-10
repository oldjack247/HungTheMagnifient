#ifndef _UI_H_
#define _UI_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_TITLE				("data/TEXTURE/title.png")				// タイトルのテクスチャ
#define TEXTURE_TUTORIAL			("data/TEXTURE/tutorial.png")			// チュートリアルのテクスチャ 
#define TEXTURE_GAMEOVER			("data/TEXTURE/gameover.png")			// ゲームオーバーのテクスチャ
#define TEXTURE_RESULT				("data/TEXTURE/result.png")				// リザルトのテクスチャ
#define TEXTURE_HP					("data/TEXTURE/hp.png")					// HPのテクスチャ
#define TEXTURE_HPFLAME				("data/TEXTURE/hpflame.png")			// HPの枠のテクスチャ
#define TEXTURE_HPSPBACK			("data/TEXTURE/hpspback.png")			// HPとSPの後ろのテクスチャ
#define TEXTURE_SP					("data/TEXTURE/sp.png")					// SPのテクスチャ
#define TEXTURE_SPFLAME				("data/TEXTURE/spflame.png")			// SPのフ枠のテクスチャ
#define TEXTURE_ENEMYREMAINDER		("data/TEXTURE/enemyrimainder.png")		// 敵の倒す数のテクスチャ
#define TEXTURE_PRESSSTART			("data/TEXTURE/pressstart.png")			// PRESSSTARTのテクスチャ

#define PLAYER_HP_COLOR				(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))			// HPの色
#define PLAYER_SP_COLOR				(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))			// SPの色
//#define PLAYER_ST_COLOR				(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))		// STの色

#define PLAYER_HP_GAUGE				(D3DXVECTOR3(250.0f, 30.0f, 0.0f))		// HPのゲージの座標
#define PLAYER_HP_GAUGE_SIZE		(D3DXVECTOR3(450.0f, 20.0f, 0.0f))		// HPのゲージのサイズ

#define PLAYER_SP_GAUGE				(D3DXVECTOR3(250.0f, 50.0f, 0.0f))		// SPのゲージの座標
#define PLAYER_SP_GAUGE_SIZE		(D3DXVECTOR3(450.0f, 20.0f, 0.0f))		// SPのゲージのサイズ

#define PLAYER_ST_GAUGE				(D3DXVECTOR3(250.0f, 70.0f, 0.0f))		// STのゲージの座標
#define PLAYER_ST_GAUGE_SIZE		(D3DXVECTOR3(450.0f, 20.f, 0.0f))		// STのゲージのサイズ

#define PRESSSTART_CHANGECOLOR_QUANTITY (0.01f)								// PRESSSTARTの色を変化させる量
#define PRESSSTART_POS (D3DXVECTOR3(SCREEN_CENTER_X, 550.0f, 0.0f))			// PRESSSTARTの座標
#define PRESSSTART_SIZE (D3DXVECTOR3(1000.0f, 140.0f, 0.0f))				// PRESSSTARTのサイズ

//=============================================================================
// UIクラス
//=============================================================================
class CUi :public CScene2D
{
public:
	CUi(int nPriority = OBJTYPE_UI);
	~CUi();

	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_TITLE,
		TYPE_TUTORIAL,
		TYPE_GAMEOVER,
		TYPE_RESULT,
		TYPE_PLAYER_LIFE,
		TYPE_HP,
		TYPE_HPFRAME,
		TYPE_HPSPBACK,
		TYPE_PLAYER_SP,
		TYPE_SP,
		TYPE_SPFRAME,
		TYPE_PRESSSTART,
		TYPE_MAX
	}TYPE;

	static CUi* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type); // クリエイト

	static HRESULT Load(void);	// テクスチャの読み込み
	static void Unload(void);	// テクスチャの破棄

	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];	// テクスチャの情報
	D3DXVECTOR3	m_pos;								// 座標
	D3DXVECTOR3	m_size;								// サイズ
	TYPE		m_type;								// タイプ
	bool		m_bPressenterflash;					// 暗くするか明るくするか
};
#endif // !_UI_H_
