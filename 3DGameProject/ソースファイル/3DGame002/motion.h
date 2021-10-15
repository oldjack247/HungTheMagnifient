#ifndef _MOTION_H_
#define _MOTION_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// ヘッダファイルのインクルード
//=============================================================================
#include "main.h"
#include "character.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS (15)			  // モデルパーツの最大数
#define MOTION_MAX (20)			  // キー最大数
#define MOTION_ANIMATION_NUM (60) // アニメーション数
#define MOTION_KEYSET_MAX (60)	  // キーセット最大数
#define MOTION_KEY_MAX (60)		  // キー最大数

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct {
	int m_nIndex;			 // モデルナンバー
	int m_nParents;			 // モデルの親子関係
	D3DXVECTOR3 m_pos;		 // 位置
	D3DXVECTOR3 m_posOrigin; // 始まりの位置
	D3DXVECTOR3 m_rot;		 // 角度
}MODELPARENT;

// アニメーションキーセット情報
typedef struct
{
	D3DXVECTOR3 m_pos[MAX_PARTS];
	D3DXVECTOR3 m_rot[MAX_PARTS];
	int m_nFrame; // 1モーションのキーフレーム数
}KEY_INFO;

// アニメーション情報
typedef struct
{
	bool m_nLoop;						   // ループするか
	int m_nNumKey;						   // モーションのキー数
	KEY_INFO m_KeyInfo[MOTION_KEYSET_MAX]; // アニメーションに含むキーセット情報
}ANIMATION;

//=============================================================================
// モーションクラス
//=============================================================================
class CMotion
{
public:
	typedef enum
	{
		TYPE_PLAYER,   // プレイヤー
		TYPE_ENEMY,	   // 敵
		TYPE_MAX	   // 最大値
	}MOTIONTYPE;

	CMotion();
	~CMotion();

	static CMotion* Create(CCharacter* pCharacter, CharacterType Type);

	void Uninit(void);
	void LoadMotion(const char* cText);
	void UpdateMotion(void);

	void SetMotion(int nMotionstate) { m_nMotionState = nMotionstate; }
	void SetType(CharacterType Type) { m_Type = Type; }
	void SetCharacter(CCharacter* pCharacter) { m_pCharacter = pCharacter; }

	void SetPos(int nCount, D3DXVECTOR3 pos) { m_modelParent[nCount].m_pos = pos; }
	void SetRot(int nCount, D3DXVECTOR3 rot) { m_modelParent[nCount].m_rot = rot; }

	int GetMotionState(void) { return m_nMotionState; }
	int GetIndex(int nCount) { return m_modelParent[nCount].m_nIndex; }
	int GetParents(int nCount) { return m_modelParent[nCount].m_nParents; }
	D3DXVECTOR3 GetPos(int nCount) { return m_modelParent[nCount].m_pos; }
	D3DXVECTOR3 GetRot(int nCount) { return m_modelParent[nCount].m_rot; }
private:
	MODELPARENT m_modelParent[MAX_PARTS]; // モデルのパーツごとの情報
	ANIMATION m_motion[TYPE_MAX][MOTION_MAX];		  // モーションごとの情報

	int m_nMotionState;	  // モーションの構造体
	int m_nMotionOldState; // 1F前のモーションの構造体

	CharacterType m_Type;

	CCharacter* m_pCharacter; // キャラクターのインスタンス

	int m_nFreme;  // フレーム数
	int m_nCntKey; // キー数

	D3DXVECTOR3 m_NumPos[MAX_PARTS];		// posの差分
	D3DXVECTOR3 m_NumRot[MAX_PARTS];		// rotの差分
	D3DXVECTOR3 m_NumRotDest[MAX_PARTS];	// rotの差分

	bool m_AngleModifying; // 角度の修正をしたか
};

#endif