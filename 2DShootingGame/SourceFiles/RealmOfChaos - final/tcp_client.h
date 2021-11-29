#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include <winsock2.h>

//=============================================================================
//マクロ定義
//=============================================================================
#define SERVER_PORT_NUM 12345							//ポート番号
#define BUFFER_NUM 1024									//バッファの最大数

//=============================================================================
// tcp_clientクラス
//=============================================================================
class CTcpClient
{
public:
	CTcpClient();										//コンストラクタ
	~CTcpClient();										//デストラクタ
	static CTcpClient* Create(const char* pHostName, int nPortNum);	//生成処理
	void Release(void);									//終了処理
	bool Init(const char* pHostName, int mPortNum);		//IPアドレス,ポート番号
	int Send(char* pSendData, int nSendDataSize);		//send関数
	int Recv(char* pRecvBuf, int nRecvBufSize);			//rec関数
	void Close();										//クローズ関数
private:
	SOCKET m_sock;										//ソケット
};
#endif
