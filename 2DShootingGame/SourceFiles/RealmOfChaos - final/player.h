#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define PLAYER_SIZE 80.0f
#define MAX_LIFE 6
#define MINION_USE_LIMITE 3

//=============================================================================
//前方宣言
//=============================================================================
class CSound;
class CGameClear;

//=============================================================================
//クラス
//=============================================================================
class CPlayer : public CScene2d
{
public:
	typedef enum
	{//プレイヤーの状態
		PLAYERSTATE_NORMAL = 0,								//通常状態
		PLAYERSTATE_DAMAGE,									//ダメージを受けたとき
		PLAYERSTATE_DEATH,									//やられたとき
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	CPlayer(int nPriority = PRIORITYTYPE_PLAYER);			//コンストラクタ
	~CPlayer();												//デストラクタ
	static HRESULT Load(void);								//テクスチャの読み込み処理
	static void Unload(void);								//テクスチャの破棄
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXCOLOR col, float size_x, float size_y);		//生成処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float size_x, float size_y);	//初期化処理
	void Uninit(void);										//終了処理
	void Update(void);										//更新処理
	void Draw(void);										//描画処理
	void SetLife(int nLife);								//体力のセット関数
	static int GetLife(void) { return m_nLife; };			//体力の情報取得関数
	static D3DXVECTOR3 GetPos(void) { return m_pos; };		//位置の取得関数
	static void SetBulletChange(bool bBulletChange);
	static bool GetBulletChange(void) { return m_bBulletChange; };							//テキストファイルに書き込む関数
	static void SetMinion(bool bUseMinion);
	static bool GetMinion(void) { return m_bUseMinion; };
private:
	static D3DXVECTOR3 m_pos;								//位置
	D3DXVECTOR3 m_move;										//移動量
	D3DXCOLOR m_col;										//色
	static LPDIRECT3DTEXTURE9 m_pTexture;
	static int m_nLife;										//体力
	static bool m_bBulletChange;
	static bool m_bUseMinion;
	static CGameClear* m_pGameClear;
	int m_nCounterAnim;										//カウンター
	int m_nLimitMinion;
	int m_nPatternAnim;										//パターンNO.
	int m_nInvincible;										//無敵時間										
	PLAYERSTATE m_State;									//プレイヤーの状態取得
	CSound* m_pSound;										//サウンドのポインタ
};
#endif