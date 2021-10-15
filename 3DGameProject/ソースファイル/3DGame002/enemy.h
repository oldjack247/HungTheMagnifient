#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "character.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOTION_ENEMY_TEXT ("data/MODEL/ENEMY/MOTION/motion.txt")	// モーションのテキスト

#define ENEMY_SIZE (D3DXVECTOR3(30.0f,80.0f,30.0f))					// 当たり判定サイズ
#define ENEMY_LIFE (10)												// 体力
#define ENEMY_MOVE_SPEED (1.0f)										// 移動量
#define ENEMY_DAMAGE_MOVE_SPEED (0.5f)								// ダメージを受けた時の移動量
#define ENEMY_DISTANCE_PLAYER (45.0f)								// プレイヤーとの距離
#define ENEMY_CREATE_NUM (99)										// 敵を生成する数
#define ENEMY_CREATE_ANGLE (360)									// 敵を生成する角度
#define ENEMY_CREATE_DISTANCE (D3DXVECTOR3(1000.0f, 0.0f, 1000.0f))	// 敵を生成する距離
#define ENEMY_CREATE_TIME (20)										// 敵を生成する時間
#define ENEMY_DAMAGE_SRASH_TIME (10.0f)								// 斬撃でのダメージを受ける時間
#define ENEMY_DAMAGE_MAGICCIRCLE_TIME (60.0f)						// 魔法陣でのダメージを受ける時間
#define ENEMY_DAMAGE_MOTION_TIME (88.0f)							// ダメージモーションの時間
#define ENEMY_ATTACK_MOTION_TIME (103.0f)							// 攻撃するモーションの時間
#define ENEMY_ATTACK_UNTIL_TIME (120.0f)							// 攻撃するまでの時間
#define ENEMY_ATTACK_COLISION_SIZE (D3DXVECTOR3(5.0f, 5.0f, 5.0f))	// 剣の当たり判定の大きさ
#define ENEMY_DEATH_MOTION_TIME (217.0f)							// 剣の当たり判定の大きさ

//=============================================================================
// 前方宣言
//=============================================================================
class CMotion;
class CModel;

//=============================================================================
// エネミークラス
//=============================================================================
class CEnemy :public CCharacter
{
public:
	typedef enum
	{
		ENEMYSTATE_NOMAL = 0,
		ENEMYSTATE_DAMAGE,
		ENEMYSTATE_DEATH,
	}ENEMYSTATE;

	// モーションの状態
	typedef enum
	{
		ENEMYMOTION_IDLE = 0,
		ENEMYMOTION_RUN,
		ENEMYMOTION_DAMAGE,
		ENEMYMOTION_ATTACK,
		ENEMYMOTION_DEATH,
	}ENEMYMOTION;

	// 受けたダメージの種類
	typedef enum
	{
		DAMAGESTATE_SLASH = 0,
		DAMAGESTATE_MAGICCIRCLE,
	}DAMAGESTATE;

	CEnemy(int nPriority = CScene::OBJTYPE_ENEMY);	// コンストラクタ
	~CEnemy();										// デストラクタ

	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, ENEMYSTATE EnemyState);	// 生成処理

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	bool HitDamage(int nDamage, DAMAGESTATE DamageState);	// ダメージを受けた時の処理
	void EnemyCollision(void);								// 敵同士の当たり判定
	void Attack(void);										// 攻撃の処理
	D3DXVECTOR3 PlayerCollision(D3DXVECTOR3 pos);			// プレイヤーとの当たり判定

	void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, ENEMYSTATE EnemyState);	// セット

	ENEMYSTATE GetEnemyState(void) { return m_EnemyState; } // 敵の状態の取得
private:
	D3DXVECTOR3 m_move;			// 移動量
	float m_fDamageTime;		// ダメージを受ける時間
	float m_fMotionTime;		// モーションの時間
	float m_fAttackTime;		// 攻撃する時間
	bool m_bAttack;				// 攻撃したかどうか
	ENEMYSTATE m_EnemyState;	// 敵の状態
};

#endif