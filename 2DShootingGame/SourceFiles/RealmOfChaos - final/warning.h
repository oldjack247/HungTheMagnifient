#ifndef _WARNING_H_
#define _WARNING_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "manager.h"
#include "scene2d.h"
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CSound;

//=============================================================================
//マクロ定義
//=============================================================================
#define BOSS_WARNING_SIZE_X 875
#define BOSS_WARNING_SIZE_Y 225

//=============================================================================
//クラス
//=============================================================================
class CWarning : public CScene2d
{
public:
	typedef enum
	{//タイトルテクスチャの種類
		WARNINGTYPE_NONE = 0,
		WARNINGTYPE_BOSS1,
		WARNINGTYPE_BOSS2,
		WARNINGTYPE_MAX
	}WARNINGTYPE;

	CWarning(int nPriority = PRIORITYTYPE_WARNING);					//コンストラクタ
	~CWarning();												//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み処理
	static void Unload(void);									//テクスチャの破棄
	static CWarning* Create(D3DXVECTOR3 pos, float size_x, float size_y, WARNINGTYPE m_type);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y, WARNINGTYPE m_type);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[WARNINGTYPE_MAX];		//テクスチャ
	WARNINGTYPE m_type;											//タイトル
	CScene2d* m_apScene2D[WARNINGTYPE_MAX];						//Scene2Dのポインタ										
	D3DXVECTOR3 m_pos;											//位置
	D3DXCOLOR m_col;											//色
	CSound* m_pSound;											//音のポインタ
	float m_size_x;												//x座標のサイズ
	float m_size_y;												//y座標のサイズ
	int nStopCount;
};
#endif
