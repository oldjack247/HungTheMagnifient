#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//マクロ定義
//=============================================================================
#define COUNT_WAIT_FINISH_GAME (30)

//=============================================================================
//インクルードファイル
//=============================================================================
#include "manager.h"
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CPlayer;
class CEnemy;
class CBg;
class CScore;
class CLife;
class CEffect;
class CSound;
class CItem;
class CWarning;
class CBulletEffect;
class CMinion;

//=============================================================================
//クラス
//=============================================================================
class CGame : public CScene
{
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,										//何もしていない状態
		GAMESTATE_NORMAL,										//通常状態
		GAMESTATE_END,											//ゲーム終了状態
		GAMESTATE_MAX
	}GAMESTATE;

	CGame();													//コンストラクタ
	~CGame();													//デストラクタ
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理
	void SetGameState(GAMESTATE state);							//セット
	GAMESTATE GetGameState(void) { return m_gamestate; };		//ゲーム状態の情報取得
	static CEnemy* GetEnemy(void);								//敵の情報取得用
	static void SetScore(int nScore);							//スコアのセット
	static CScore* GetScore(void) { return m_pScore; }			//スコア取得用
	static void SetPlayerLife(int nLife);						//ライフのセット
	static void Set1FBossSpawn(bool bSpawn);					//スポーンしたか
	static void Set2FEnemySpawn(bool b2Spawn);					//スポーンしたか
	static void Set3FEnemySpawn(bool b2Spawn);					//スポーンしたか
private:
	D3DXVECTOR3 m_pos;											//位置
	GAMESTATE m_gamestate;										//ゲーム状態 
	int m_nCounterGameState;									//ゲーム状態カウント
	int n1FEnemySpawnTime;										//1Fの敵を生成する時間
	int n2FEnemySpawnTime;										//2Fの敵を生成する時間
	int n3FEnemySpawnTime;										//3Fの敵を生成する時間
	static CPlayer* m_pPlayer;									//プレイヤーのポインタ
	static CEnemy* m_pEnemy;									//敵のポインタ
	static CBg* m_pBg;											//BGのポインタ											
	static CScore* m_pScore;									//スコアのポインタ
	static CLife* m_pLife;										//ライフバーのポインタ
	static CEffect* m_pEffect;									//エフェクトのポインタ
	static CItem* m_pItem;										//アイテムのポインタ
	static CWarning* m_pWarning;								//Warning					
	static CMinion* m_pMinion;
	CSound* m_pSound;											//サウンドのポインタ
	static bool m_b1FBossSpawn;									//1Fのボスがスポーンするかどうか
	static bool m_b2FEnemySpawn;								//2Fのボスがスポーンするかどうか
	bool b2FBossAwakening;										//2Fのボスが覚醒したかどうか	
	static bool m_b3FEnemySpawn;								//3Fのボスがスポーンするかどうか										
};
#endif