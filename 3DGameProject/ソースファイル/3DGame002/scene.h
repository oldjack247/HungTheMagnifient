#ifndef _SCENE_H_
#define _SCENE_H_
//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// オブジェクトクラス
//=============================================================================
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_DEBUG,
		OBJTYPE_SKYBOX,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_PARTICLE,
		OBJTYPE_UI,
		OBJTYPE_FADE,
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = OBJTYPE_NONE);
	virtual ~CScene();

	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void) { return m_objType; }

	//純粋仮想関数
	virtual HRESULT Init(void) = 0;	//初期化処理
	virtual void Uninit(void) = 0;	//終了処理
	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;		//描画処理

	static void UpdateAll(void);	//更新まとめ
	static void DrawAll(void);		//描画まとめ
	static void ReleaseAll(void);	//終了まとめ

	static void DesignationReleaseAll(OBJTYPE type);
	static void SetbUpdate(bool bUpdate, OBJTYPE type);

	void ConnectionList(void); // データのつなぎなおし

	static bool GetbUpdate(OBJTYPE type) { return m_bUpdate[type]; }
	static CScene* GetSceneTop(int nPriority) { return m_pTop[nPriority]; } // シーンのデータを返す
	CScene* GetSceneNext(void) { return m_pNext; } // シーンのネクストのデータを返す

protected:
	void Release(void);

private:
	OBJTYPE m_objType;					// オブジェクトの種類
	static CScene* m_pTop[OBJTYPE_MAX];	// 先頭のオブジェクトへのポインタ
	static CScene* m_pCur[OBJTYPE_MAX];	// 現在のオブジェクトへのポインタ
	CScene* m_pPrev;					// 前のオブジェクトへのポインタ
	CScene* m_pNext;					// 次のオブジェクトへのポインタ
	int m_nObj;							// オブジェクトのナンバー
	bool m_bDeath;						// 死亡フラグ
	static bool m_bUpdate[OBJTYPE_MAX]; // 更新するかどうか
	static bool m_bDraw[OBJTYPE_MAX];	// 描画するかどうか
};

#endif // !_RENDERER_H_