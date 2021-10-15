#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define ENEMY_SIZE 100.0f					//通常の敵のサイズ
#define ENEMY_GHOST_SIZE 300.0f				
#define MAX_ENEMY 256						//敵の最大数
#define ENEMY_2FGHOST_AVATAR_SIZE 100.0f	//お化けの分身
#define ENEMY_1FBOSS_SIZE 400.0f			//1フロア目のボスのサイズ
#define ENEMY_2FBOSS_SIZE 300.0f			//2フロア目のボスのサイズ
#define ENEMY_3FBOSS_SIZE 400.0f			//3フロア目のボスのサイズ
#define ENEMY_BOSS_FOG   300.0f
#define BOSS_AVATAR_SIZE 120.0f
#define ENEMY_NOMAL_LIFE 2					//通常の敵のライフ
#define ENEMY_3F_LIFE 5						//通常の敵のライフ
#define ENEMY_1FBOSS_LIFE 80				//1フロア目のボスのライフ
#define ENEMY_2FBOSS_LIFE 20000				//覚醒する前の敵のライフ
#define ENEMY_2FBOSS_AWAKENING_LIFE 300		//覚醒したあとのライフ
#define ENEMY_3FBOSS_LIFE 500				//1フロア目のボスのライフ
#define COUNT_BOSS_AVATAR 20				//覚醒する数
#define NOMAL_ENEMY_SCORE 300				//普通の敵のスコア
#define FIRST_BOSS_SCORE 2000				//1Fのボスのスコア
#define SECOND_BOSS_SCORE 3000				//2Fのボスのスコア
#define THIRD_BOSS_SCORE 4000				//3Fのボスのスコア

//=============================================================================
//前方宣言
//=============================================================================
class CSound;
class CGameClear;

//=============================================================================
//クラス
//=============================================================================
class CEnemy : public CScene2d
{
public:
	CEnemy(int nPriority = PRIORITYTYPE_ENEMY);
	~CEnemy();
	typedef enum
	{//敵の種類
		TYPE_ENEMY_HARPY,					//敵
		TYPE_ENEMY_BOSS_FOG,				//
		TYPE_ENEMY_GHOST,					//おばけ
		TYPE_ENEMY_1FBOSS,					//ボス
		TYPE_ENEMY_2FGHOST,					//2Fのおばけ
		TYPE_ENEMY_2FBAT,					//蝙蝠
		TYPE_ENEMY_2FGHOST_AVATAR,			//おなけを倒したらそこから出てくるやつ
		TYPE_ENEMY_2FBOSS,					//ボス
		TYPE_ENEMY_2FBOSS_AVATAR,			//ボスからでてくる小さいボス
		TYPE_ENEMY_3FBOSS,
		TYPE_ENEMY_3FGHOST,
		TYPE_ENEMY_3FDEMON,
		TYPE_ENEMY_MAX
	}TYPEENEMY;

	typedef enum
	{//行動の種類
		ACTION_MOVE_NONE = 0,				//通常状態
		ACTION_MOVE_LEFT,					//左に移動しているとき
		ACTION_MOVE_RIGHT,					//右に移動しているとき
		ACTION_MOVE_BACK,					//戻る敵
		ACTION_ENEMY2_DROP_ITEM,			//おばけが爆発するとき
		ACTION_CIRCLE_RIGHT,				//右回転
		ACTION_CIRCLE_LEFT,					//左回転
		ACTION_2FBOSS_AWAKENING,			//2Fボスの覚醒したとき
		ACTION_MAX
	}ENEMY_ACTION_PATTERN;

	static HRESULT Load(void);										//テクスチャの読み込み処理
	static void Unload(void);										//テクスチャの破棄
	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size_x, float size_y, int nLife, TYPEENEMY Type, ENEMY_ACTION_PATTERN MoveType);				//生成処理			
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size_x, float size_y, int nLife, TYPEENEMY Type, ENEMY_ACTION_PATTERN MoveType);				//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理
	void SetLife(int nLife);										//ダメージ処理
	int GetLife(void);												//体力取得
	int GetRandNum(int min, int max);								//ランダム処理
	bool HitCcollision(D3DXVECTOR3 Pos, float size_x, float size_y);//当たり判定
	static void Set2FBossAwakening(bool b2FBossAwakening);			//セット			
private:
	D3DXVECTOR3 m_move;												//移動量
	D3DXVECTOR3 m_pos;												//位置
	static D3DXVECTOR3 m_2FBossPos;									//2フロア目の位置
	D3DXCOLOR m_col;												//色
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_ENEMY_MAX];			//テクスチャ
	int m_nLife;													//体力
	int m_nCntEnemy;												//カウンタ
	int m_nCnt2FBoss;												//ボスが弾を撃つタイミング
	int nCnt2FBossChange;											//覚醒エフェクトを出す時間
	int m_nCounterAnim;												//カウンター
	int m_nPatternAnim;												//パターンNO.
	static int nCnt2FBossAvatar;									//分身を倒した数
	float m_fAngle;													//角度
	TYPEENEMY m_type;												//敵の種類
	ENEMY_ACTION_PATTERN m_movetype;								//動きの種類
	bool m_b1FBossInvisible;										//透明かどうか											
	bool m_b2FBossBullet;											//ボスが分身を出すかどうか
	bool m_b2FBossAttack;											//覚醒したあとの攻撃
	static bool m_b2FBossAwakening;									//覚醒したかどうか
	static CGameClear* m_pGameClear;									//GameClear
	CSound* m_pSound;												//サウンドのポインタ
};
#endif