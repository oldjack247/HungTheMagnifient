#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "character.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOTION_PLAYER_TEXT ("data/MODEL/PLAYER/MOTION/motion.txt")	// モーションのテキスト
#define PLAYER_LIFE (50)											// プレイヤーのライフ
#define PLAYER_SP_MAX (20)											// プレイヤーのSP
#define PLAYER_DEFAULT_POS (D3DXVECTOR3(0.0f, 43.0f, 0.0f))			// 座標
#define PLAYER_DEFAULT_ROT (D3DXVECTOR3(0.0f, 1.5f, 0.0f))			// 回転
#define PLAYER_DEFAULT_SIZE (D3DXVECTOR3(50.0f, 80.0f, 50.0f))		// 大きさ
#define PLAYER_MOVE_SPEED (2.0f)									// 歩く速度
#define PLAYER_ATTACK_MOVE_SPEED (1.0f)								// 攻撃した時の速度
#define PLAYER_DAMAGE_TIME (120.0f)									// プレイヤーがダメージを受ける時間
#define PLAYER_MOVE_MAP_LIMIT (800.0f)								// プレイヤーの移動できる範囲
#define SWORD_COLISION_SIZE (D3DXVECTOR3(10.0f, 10.0f, 10.0f))			// 剣の当たり判定の大きさ
#define MOTION_SLASH_FLAME (26.0f)									// 斬るモーションのフレーム
#define MOTION_SLASH_FLAME_COLLISION_START (8.0f)					// 斬るモーションのエフェクトが出るまでのフレーム
#define MOTION_MAGICCIRCLE_FLAME (380.0f)							// 魔法陣を出すモーションのフレーム
#define MOTION_MAGICCIRCLE_CREATE_START (70.0f)						// 魔法陣を出すまでのフレーム
#define SUBTRACT_FLAME (1.0f)			// 減らすフレームの量

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
		PLAYERMOTION_SLASH,
		PLAYERMOTION_MAGIC,
	}PLAYERMOTION;

	typedef enum
	{
		PLAYERSTATE_NOMAL = 0,
		PLAYERSTATE_ATTACK,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_MAGIC,
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
	void Attack(D3DXVECTOR3 Distance);				// 攻撃の処理
	void Swordeffect(D3DXVECTOR3 Distance);			// 剣のエフェクト
	void Magic(void);								// 魔法の処理
	D3DXVECTOR3 EnemyCollision(D3DXVECTOR3 pos);	// 敵との判定
	bool HitDamage(int nDamage);					// ダメージ処理

	float GetSpGauge(void) { return m_fSpGauge; }	// spゲージの量を返す
private:
	D3DXVECTOR3 m_move;					// 移動量
	float m_fMotionTime;				// 操作できない時間
	float m_fDamageTime;				// ダメージを受ける時間
	float m_fSpGauge;					// 必殺技を出すためのゲージの量
	int m_nSwordEffectTime;				// 剣のエフェクトの出す時間
	PLAYERSTATE m_Playerstate;			// プレイヤーの状態
};

#endif