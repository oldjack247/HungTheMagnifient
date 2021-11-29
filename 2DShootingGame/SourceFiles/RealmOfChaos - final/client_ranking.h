#ifndef _CRIENT_RANKING_H_
#define _CRIENT_RANKING_H_

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_RANKING 5				// ランキングデータ数
#define SEND_BUF 256				//送るバッファの最大数
#define REC_BUF 128					//受け取るバッファの最大数
#define SEND_BYTE 5					//送るバイト数
#define MAX_IP_ADDRESS 128
#define IP_ADDRESS "127.0.0.1"		//IPアドレス
#define AZUR

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "manager.h"
#include "scene2d.h"
#include "scene.h"
#include "score.h"

//=============================================================================
//クラス
//=============================================================================
class CCrient_ranking : public CScene
{
public:
	typedef struct
	{
		float m_pos_y;											//Y軸の位置
		int m_nScore;											//スコア
		int m_nRank;											//順位
	} RankingData;

	typedef enum
	{
		COMMAND_TYPE_NONE = 0,
		COMMAND_TYPE_GET_RANKING,		// ランキング一覧を取得
		COMMAND_TYPE_SET_RANKING		// ランキングを設定
	} COMMAND_TYPE;

	CCrient_ranking();
	~CCrient_ranking();
	static CCrient_ranking* Create(D3DXVECTOR3 pos, float size_x, float size_y);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float size_x, float size_y);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理
	static void GetRanking(void);								//スコアとランキングの受信
	static int SetRanking(int nScore);							//スコアの送信
private:
	static RankingData m_RankingData[MAX_RANKING];				//ランキングデータ
	CScore* m_apScore[MAX_RANKING];								//スコアのポインタ							
};
#endif