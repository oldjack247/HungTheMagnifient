#ifndef _SCENE_H_
#define _SCENE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_SCENE 1280									//シーンの最大数

//=============================================================================
//クラス
//=============================================================================
class CScene
{//基本クラス
public:
	typedef enum
	{//描画優先順位
		PRIORITYTYPE_BG = 0,							//BG
		PRIORITYTYPE_EFFECT,							//エフェクト
		PRIORITYTYPE_ENEMY,								//敵
		PRIORITYTYPE_PLAYER,							//プレイヤー
		PRIORITYTYPE_BULLET,							//弾
		PRIORITYTYPE_EXPLOSION,							//爆発
		PRIORITYTYPE_UI,								//UI
		PRIORITYTYPE_WARNING,							//Warning
		PRIORITYTYPE_GAMECLEAR,
		PRIORITYTYPE_MINION,
		PRIORITYTYPE_MAX								//最大数
	}PRIORITYTYPE;

	typedef enum
	{
		OBJTYPE_NONE = 0,								//通常
		OBJTYPE_PLAYER,									//プレイヤー
		OBJTYPE_ENEMY,									//敵
		OBJTYPE_BULLET,									//弾				
		OBJTYPE_ITEM,									//アイテム
		OBJTYPE_BG,										//BG
		OBJTYPE_LIFE,
		OBJTYPE_MINION,
		OBJTYPE_MAX										//最大数
	}OBJTYPE;

	CScene(int nPriority = 0);							//コンストラクタ
	virtual ~CScene();									//デストラクタ
	virtual HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y) = 0;	//初期化処理
	virtual void Uninit(void) = 0;						//終了処理
	virtual void Update(void) = 0;						//更新処理
	virtual void Draw(void) = 0;						//描画処理
	static void UpdateAll(void);						//全部を更新
	static void DrawAll(void);							//全部を描画
	static void ReleaseAll(void);						//全部をリリース
	OBJTYPE GetObjType(void) { return m_nType; }		//オブジェクトタイプの取得関数
	void SetObjType(OBJTYPE type);						//オブジェクトタイプのセット関数
	static CScene* GetScene(int nPriority, int nNum);
	int GetPriority(void) { return m_nPriority; }
	void Release(void);									//オブジェクトをリリース

private:
	static CScene* m_apScene[PRIORITYTYPE_MAX][MAX_SCENE];	//シーンのポインタ
	static int m_nNumAll;								//全てのシーン
	int m_nID;											//情報を管理する
	int m_nPriority;									//優先順位
	OBJTYPE m_nType;									//オブジェクトの種類
};
#endif