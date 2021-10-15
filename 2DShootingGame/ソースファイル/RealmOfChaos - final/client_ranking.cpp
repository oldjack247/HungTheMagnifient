//=============================================================================
//インクルードファイル
//=============================================================================
#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
#include "client_ranking.h"
#include "main.h"
#include "tcp_client.h"
#include "score.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CCrient_ranking::RankingData CCrient_ranking::m_RankingData[MAX_RANKING] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CCrient_ranking::CCrient_ranking()
{
	memset(m_RankingData, 0, sizeof(m_RankingData));
	memset(m_apScore, 0, sizeof(m_apScore));
}

//=============================================================================
//デストラクタ
//=============================================================================
CCrient_ranking::~CCrient_ranking()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CCrient_ranking* CCrient_ranking::Create(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CCrient_ranking* pCrient_ranking = NULL;
	if (pCrient_ranking == NULL)
	{
		pCrient_ranking = new CCrient_ranking;
	}
	pCrient_ranking->Init(pos, size_x, size_y);
	return pCrient_ranking;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCrient_ranking::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	GetRanking();
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		m_apScore[nCntRank] = CScore::Create(D3DXVECTOR3(600.0f, 70.0f + (nCntRank * 130), 0.0f), size_x, size_y);
		m_apScore[nCntRank]->AddScore(m_RankingData[nCntRank].m_nScore);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCrient_ranking::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCrient_ranking::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CCrient_ranking::Draw(void)
{
}

//=============================================================================
// スコアとランキングの受信
//=============================================================================
void CCrient_ranking::GetRanking(void)
{
	char aDestIPAddr[MAX_IP_ADDRESS] = IP_ADDRESS;

	int nIndex = 0;

	//pTcpClient生成、サーバーに接続
	CTcpClient* pTcpClient = CTcpClient::Create(aDestIPAddr, SERVER_PORT_NUM);
	//Sendを実行
	//送る
	char aSendBuf[SEND_BUF];

	aSendBuf[0] = COMMAND_TYPE_GET_RANKING;

	pTcpClient->Send(aSendBuf, 1);

	//ランキング一覧取得レスポンス送信
	char aRecvBuf[BUFFER_NUM];
	memset(aRecvBuf, 0, sizeof(aRecvBuf));

	//Recvを実行
	pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		int nScore = *(int*)&aRecvBuf[nIndex];
		nScore = ntohl(nScore);
		m_RankingData[nCntRank].m_nScore = nScore;
		nIndex += sizeof(int);
	}
}

//=============================================================================
// スコアの送信
//=============================================================================
int CCrient_ranking::SetRanking(int nScore)
{

	char aDestIPAddr[128] = IP_ADDRESS;

	CTcpClient* pTcpClient = CTcpClient::Create(aDestIPAddr, SERVER_PORT_NUM);
	int nIndex = 0;

	//TCP送受信クラスの設定
	if (pTcpClient == NULL)
	{
		return 0;
	}

	//ランキング設定リクエストの送信
	//1バイト目:COMMAND_TYPE_SET_RANKINGを設定
	char aSendBuf[SEND_BUF];
	aSendBuf[0] = COMMAND_TYPE_SET_RANKING;
	//2～5バイト目:スコアを設定
	nScore = htonl(nScore);
	memcpy(&aSendBuf[1], &nScore, sizeof(int));
	//送信
	//全部送る
	pTcpClient->Send(aSendBuf, SEND_BYTE);
	//ランキング設定のレスポンスを受信
	char aRecvBuf[REC_BUF];
	memset(&aRecvBuf[0], 0, sizeof(aRecvBuf));
	pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));
	pTcpClient->Release();
	return (int)aRecvBuf[0];
}
