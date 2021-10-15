#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene.h"
#include "model.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS (15) // モデルのパーツの最大数
#define MAX_MATERIAL (50)

//=============================================================================
// 前方宣言
//=============================================================================
class CMotion;
class CReticle;

typedef enum
{
	CHARACTERTYPE_PLAYER = 0,
	CHARACTERTYPE_ENEMY,
	CHARACTERTYPE_MAX
}CharacterType;

//=============================================================================
// キャラクタークラス
//=============================================================================
class CCharacter :public CScene
{
public:
	CCharacter(int nPriority);//コンストラクタ
	~CCharacter();//デストラクタ

	virtual HRESULT Init(void);//初期化処理
	virtual void Uninit(void);//終了処理
	virtual void Update(void);//更新処理
	virtual void Draw(void);//描画処理

	HRESULT LoadInitialPosition(char* cFileName);
	void LoadMotion(char* FileName);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetModelPartPos(int nNumPart, D3DXVECTOR3 pos) { m_apModel[nNumPart]->Setpos(pos); }
	void SetModelPartRot(int nNumPart, D3DXVECTOR3 rot) { m_apModel[nNumPart]->Setrot(rot); }
	void SetModelPartSize(int nNumPart, D3DXVECTOR3 size) { m_apModel[nNumPart]->Setsize(size); }
	void SetModelPartsNum(int nPartNum) { m_nPartsNum = nPartNum; }
	void SetLife(int nLife) { m_nLife = nLife; }
	void SetType(CharacterType Type) { m_Type = Type; }

	//受け渡し処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }//場所
	D3DXVECTOR3 GetRot(void) { return m_rot; }//角度
	D3DXVECTOR3 GetSize(void) { return m_size / 2; }//角度
	D3DXVECTOR3 GetModelPartPos(int nNumPart) { return m_apModel[nNumPart]->GetPos(); }
	D3DXVECTOR3 GetModelPartRot(int nNumPart) { return m_apModel[nNumPart]->GetRot(); }
	D3DXVECTOR3 GetModelPartSize(int nNumPart) { return m_apModel[nNumPart]->GetSize(); }
	CharacterType GetType(void) { return m_Type; }
	CMotion* GetMotion(void) { return m_pMotion; }
	CModel* GetModelParts(int nNumPart) { return m_apModel[nNumPart]; }
	CCharacter* GetCharacter(void) { return this; }
	int GetLife(void) { return m_nLife; }
private:
	char* m_apFileName[MAX_PARTS];	  // ファイルの名前
	char* m_apTextureFileName[MAX_PARTS]; //テクスチャのファイルの名前

	int m_nldxModelNum[MAX_PARTS];			 // モデルナンバー
	int m_nldxModelParent[MAX_PARTS];			 // モデルの親子関係

	D3DXVECTOR3 m_Beginpos[MAX_PARTS];
	D3DXVECTOR3 m_Beginrot[MAX_PARTS];

	int m_nPartsNum;

	D3DXVECTOR3 m_pos;					// 場所
	D3DXVECTOR3 m_rot;					// 角度
	D3DXVECTOR3 m_size;
	CharacterType m_Type;
	CMotion* m_pMotion;					// モーションクラスのポインタ
	CModel* m_apModel[MAX_PARTS]; // モデルクラスのポインタ

	int m_nLife;
};

#endif