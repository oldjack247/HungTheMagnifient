#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "character.h"5

//=============================================================================
// マクロ定義
//=============================================================================
#define MOTION_PLAYER_TEXT ("data/MODEL/PLAYER/MOTION/motion.txt")	// モーションのテキスト
#define PLAYER_LIFE (50)											// プレイヤーのライフ
#define PLAYER_ROT_SPEED (0.1f)										// キャラクターの回転する速度
#define PLAYER_SP_MAX (20)											// プレイヤーのSP
#define PLAYER_DEFAULT_POS (D3DXVECTOR3(0.0f, 32.0f, 0.0f))			// 座標
#define PLAYER_DEFAULT_ROT (D3DXVECTOR3(0.0f, 1.5f, 0.0f))			// 回転
#define PLAYER_DEFAULT_SIZE (D3DXVECTOR3(150.0f, 150.0f, 150.0f))	// 大きさ
#define PLAYER_MOVE_SPEED (4.0f)									// 歩く速度
#define PLAYER_ATTACK_MOVE_SPEED (0.5f)								// 攻撃した時の速度
#define PLAYER_DAMAGE_TIME (120.0f)									// プレイヤーがダメージを受ける時間
#define PLAYER_MOVE_MAP_LIMIT (3000.0f)								// プレイヤーの移動できる範囲
#define SWORD_COLISION_SIZE (D3DXVECTOR3(5.0f, 5.0f, 5.0f))		// 剣の当たり判定の大きさ
#define MOTION_SLASH_FRAME (75.0f)									// 斬るモーションのフレーム
#define MOTION_SLASH_FRAME_COLLISION_START (8.0f)					// 斬るモーションのエフェクトが出るまでのフレーム
#define MOTION_MAGICCIRCLE_FLAME (380.0f)							// 魔法陣を出すモーションのフレーム
#define MOTION_MAGICCIRCLE_CREATE_START (70.0f)						// 魔法陣を出すまでのフレーム
#define SUBTRACT_FLAME (1.0f)										// 減らすフレームの量
#define PLAYER_INVINCIBLE_TIME (150)								// 無敵時間

// 攻撃モーションの情報
#define PLAYER_STAND_ATTACK_KEY_000		(2)					// 1段目の攻撃が有効なキー
#define PLAYER_NEXT_ATTACK_KEY_000		(3)					// 次の攻撃が可能なキー
#define PLAYER_STAND_ATTACK_KEY_001		(2)					// 2段目の攻撃が有効なキー
#define PLAYER_NEXT_ATTACK_KEY_001		(2)					// 次の攻撃が可能なキー
#define PLAYER_STAND_ATTACK_KEY_002		(2)					// 3段目の攻撃が可能なキー
#define PLAYER_NEXT_ATTACK_KEY_002		(3)					// 次の攻撃の派生キー

// 回避モーションの情報
#define DODGE_RANGE						(5.0f)			// プレイヤーの回避量
#define MOTION_DODGE_FRAME				(40.0f)			// 回避時間
#define MOTION_DODGE_END_FRAME			(8.0f)			// 回避の解除時間

//=============================================================================
// 前方宣言
//=============================================================================
class CMotion;
class CModel;
class CReticle;
class CEnemy;

//=============================================================================
// プレイヤークラス
//=============================================================================
class CPlayer :public CCharacter
{
public:

	typedef enum
	{
		PLAYERMOTION_IDLE = 0,
		PLAYERMOTION_RUN,
		PLAYERMOTION_ATTACK000,
		PLAYERMOTION_DODGE,
	}PLAYERMOTION;

	typedef enum
	{
		PLAYERSTATE_NOMAL = 0,
		PLAYERSTATE_ATTACK,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_DODGE
	}PLAYERSTATE;

	CPlayer(int nPriority = CScene::OBJTYPE_PLAYER);//コンストラクタ
	~CPlayer();//デストラクタ

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);// 生成処理

	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);	// 各種設定

	void GamePad(void);								// ゲームパッド処理
	void Keyboard(void);							// マウスとキーボード処理
	void UpdateRot(void);							// 角度の更新処理
	void Movement(void);							// 移動の処理
	void Attack(D3DXVECTOR3 Distance);				// 攻撃の処理
	void Attack2(D3DXVECTOR3 Distance);				// 攻撃2の処理
	void Attack3(D3DXVECTOR3 Distance);				// 攻撃32の処理
	void Dodge(void);								// 回避の処理
	void Swordeffect(D3DXVECTOR3 Distance);			// 剣のエフェクト
	D3DXVECTOR3 EnemyCollision(D3DXVECTOR3 pos);	// 敵との判定
	bool HitDamage(int nDamage);					// ダメージ処理
private:
	D3DXVECTOR3 m_move;					// 移動量
	D3DXVECTOR3 m_rotDest;				// 回転(目標値)
	float m_fMotionTime;				// 操作できない時間
	float m_fDamageTime;				// ダメージを受ける時間
	int m_nSwordEffectTime;				// 剣のエフェクトの出す時間
	bool m_bDodge;						// 回避のフラグ
	int m_nDodgeCounter;				// 回避のカウンター
	PLAYERSTATE m_Playerstate;			// プレイヤーの状態
};

#endif