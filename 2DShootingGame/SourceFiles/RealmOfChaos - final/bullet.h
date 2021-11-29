#ifndef _BULLET_H_
#define _BULLET_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BULLET_SIZE 50.0f				//基本的な弾のサイズ
#define PLAYER_BULLET_SIZE 25.0f		//プレイヤーの弾のサイズ
#define PLAYER_BULLET_2_SIZE 20.0f
#define PLAYER_BULLET_3_SIZE 25.0f
#define BOSS2_BULLET_SIZE 150.0f		//2フロア目のボスのサイズ7
#define ENEMY_BULLET_SIZE 50.0f
#define MAX_BULLET 256					//弾の最大数
#define ANGLE 0.05f
#define ANGLE2 0.05f
#define RADIUS 100

//=============================================================================
//前方宣言
//=============================================================================
class CSound;

//=============================================================================
//クラス
//=============================================================================
class CBullet : public CScene2d
{
public:
	typedef enum
	{
		BULLETTYPE_PLAYER = 0,	//プレイヤーの弾
		BULLETTYPE_PLAYER_2,
		BULLETTYPE_PLAYER_3,
		BULLETTYPE_ENEMY,
		BULLETTYPE_1FBOSS,
		BULLETTYPE_2FBOSS,		//ボスの弾
		BULLETTYPE_2FBOSS2,		//ボスの弾が消えた後の全方向弾
		BULLETTYPE_2FBOSS3,
		BULLETTYPE_MAX
	}BULLETTYPE;
	CBullet(int nPriority = PRIORITYTYPE_BULLET);
	~CBullet();
	static HRESULT Load(void);
	static void Unload(void);
	static CBullet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size_x, float size_y, int nLife, BULLETTYPE type);				//生成処理
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size_x, float size_y, int nLife, BULLETTYPE type);						//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw(void);						//描画処理
	bool HitCcollision(D3DXVECTOR3 Pos, float size_x, float size_y);

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_move;
	static LPDIRECT3DTEXTURE9 m_pTexture[BULLETTYPE_MAX];
	int m_nLife;				//弾の体力
	int m_nCounterAnim;			//カウンター
	int m_nPatternAnim;			//パターンNO.
	BULLETTYPE m_type;			//弾の種類
	float m_r;					//半径
	float m_fAngle;				//角度
	float m_fAngle2;			//角度
	CSound* m_pSound;
};
#endif 
