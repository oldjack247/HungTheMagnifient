#ifndef _BG_H_
#define _BG_H_

//=============================================================================
//マクロ定義
//=============================================================================
#define BG_SIZE_X	1280		//X座標のサイズ
#define BG_SIZE_Y	720			//Y座標のサイズ
#define SUN_SIZE	500
#define MAX_BG 4

//=============================================================================
//前方宣言
//=============================================================================
class CManager;

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2d.h"

//=============================================================================
//クラス
//=============================================================================
class CBg :public CScene
{
public:
	// Bgの種類
	typedef enum
	{
		BG_SPACE_1 = 0,
		BG_SPACE_2,
		BG_SPACE_3,
		BG_SUN,
		BG_SPACE_MAX,
	}BG_TYPE;

	// 列挙型定義
	typedef enum
	{
		BG_MOVE = 0,
		BG_STOP,
		BG_MAX
	}BG_MOVETYPE;

	CBg(int nPriority = PRIORITYTYPE_BG);
	~CBg();
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);					//初期化処理
	static HRESULT Load(void);
	static void Unload(void);

	static CBg* Create(D3DXVECTOR3 pos, int nPriority, BG_MOVETYPE MoveType);	//生成処理
	void Uninit(void);															//終了処理
	void Update(void);															//更新処理
	void Draw(void);															//描画処理
	void SetBgType(BG_MOVETYPE MoveType) { m_BgType = MoveType; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_BG];
	CScene2d* m_apScene2D[MAX_BG];
	BG_MOVETYPE m_BgType;
	D3DXVECTOR3 m_pos;
	int m_nCounterAnim;
	int m_nPatternAnim;
};
#endif
