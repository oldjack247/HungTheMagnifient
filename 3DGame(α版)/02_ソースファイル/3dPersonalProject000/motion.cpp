//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "motion.h"
#include "loadmodeldata.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMotion::CMotion()
{
	// モデルパーツの初期化
	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		m_modelParent[nCount].m_nIndex = 0;
		m_modelParent[nCount].m_nParents = 0;
		m_modelParent[nCount].m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_modelParent[nCount].m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_modelParent[nCount].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// モーションの初期化
	for (int m_Type = 0; m_Type < TYPE_MAX; m_Type++)
	{
		for (int nCountMotion = 0; nCountMotion < MOTION_MAX; nCountMotion++)
		{
			m_motion[m_Type][nCountMotion].m_nLoop = false;
			m_motion[m_Type][nCountMotion].m_nNumKey = 0;

			for (int nCountKey = 0; nCountKey < MOTION_KEYSET_MAX; nCountKey++)
			{
				m_motion[m_Type][nCountMotion].m_KeyInfo[nCountKey].m_nFrame = 0;
				for (int nCountParts = 0; nCountParts < MAX_PARTS; nCountParts++)
				{
					m_motion[m_Type][nCountMotion].m_KeyInfo[nCountKey].m_pos[nCountParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_motion[m_Type][nCountMotion].m_KeyInfo[nCountKey].m_rot[nCountParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}

	// モーションの状態をMOTION_IDLEに
	m_nMotionState = 0;
	m_nMotionOldState = 0;

	// フレーム数とキー数の初期化
	m_nFreme = 0;
	m_nCntKey = 1;

	// 
	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		m_NumPos[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_NumRot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_NumRotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_AngleModifying = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// モーションクラスのインスタンス生成
//=============================================================================
CMotion* CMotion::Create(CCharacter* pCharacter, CharacterType Type)
{
	CMotion* pMotion;
	pMotion = new CMotion;
	pMotion->SetCharacter(pCharacter);
	pMotion->SetType(Type);

	return pMotion;
}

//=============================================================================
// モーションクラスの終了処理
//=============================================================================
void CMotion::Uninit(void)
{
	// モデルパーツの初期化
	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		m_modelParent[nCount].m_nIndex = 0;
		m_modelParent[nCount].m_nParents = 0;
		m_modelParent[nCount].m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_modelParent[nCount].m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_modelParent[nCount].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// モーションの初期化
	for (int m_Type = 0; m_Type < TYPE_MAX; m_Type++)
	{
		for (int nCountMotion = 0; nCountMotion < MOTION_MAX; nCountMotion++)
		{
			m_motion[m_Type][nCountMotion].m_nLoop = false;
			m_motion[m_Type][nCountMotion].m_nNumKey = 0;

			for (int nCountKey = 0; nCountKey < MOTION_KEYSET_MAX; nCountKey++)
			{
				m_motion[m_Type][nCountMotion].m_KeyInfo[nCountKey].m_nFrame = 0;
				for (int nCountParts = 0; nCountParts < MAX_PARTS; nCountParts++)
				{
					m_motion[m_Type][nCountMotion].m_KeyInfo[nCountKey].m_pos[nCountParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_motion[m_Type][nCountMotion].m_KeyInfo[nCountKey].m_rot[nCountParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}

	// モーションの状態をMOTION_IDLEに
	m_nMotionState = 0;
	m_nMotionOldState = 0;

	// フレーム数とキー数の初期化
	m_nFreme = 0;
	m_nCntKey = 1;

	// 
	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		m_NumPos[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_NumRot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_NumRotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// モーションクラスのモーションの読み込み
//=============================================================================
void CMotion::LoadMotion(const char* cText)
{
	// ファイルポイント
	FILE* pFile;

	// 変数宣言
	int  nCntLoad = 0;		// 読み込んだカウント
	int	 nCntKey = 0;		// キーの数のカウント
	int	 nCntMotion = 0;	// モーションの数のカウント
	char cReedText[MAX_TEXT];	// 文字として読み取り用
	char cHeadText[MAX_TEXT];	// 文字の判別用
	char cDie[MAX_TEXT];		// 使わない文字

	// ファイルを開く
	pFile = fopen(cText, "r");

	// 開けたたら
	if (pFile != NULL)
	{
		// SCRIPTの文字が見つかるまで
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// テキストからcReedText分文字を読み込む
			fgets(cReedText, sizeof(cReedText), pFile);
			// 読み込んだ文字ををcHeadTextに格納
			sscanf(cReedText, "%s", &cHeadText);
		}

		// cHeadTextがSCRIPTだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// END_SCRIPTの文字が見つかるまで
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				// テキストからcReedText分文字を読み込む
				fgets(cReedText, sizeof(cReedText), pFile);
				// 読み込んだ文字ををcHeadTextに格納
				sscanf(cReedText, "%s", &cHeadText);

				// cHeadTextがMOTIONSETだったら
				if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// END_MOTIONSETの文字が見つかるまで
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						// テキストからcReedText分文字を読み込む
						fgets(cReedText, sizeof(cReedText), pFile);
						// 読み込んだ文字ををcHeadTextに格納
						sscanf(cReedText, "%s", &cHeadText);

						// cHeadTextがMOTIONSETだったら
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							// m_nLoopにループをするか格納
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, (int*)&m_motion[m_Type][nCntMotion].m_nLoop);
						}

						// cHeadTextがNUM_KEYだったら
						if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							// m_nNumKeyにキーの数を格納
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_motion[m_Type][nCntMotion].m_nNumKey);
						}

						// cHeadTextがKEYSETだったら
						if (strcmp(cHeadText, "KEYSET") == 0)
						{
							// END_KEYSETの文字が見つかるまで
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								// テキストからcReedText分文字を読み込む
								fgets(cReedText, sizeof(cReedText), pFile);
								// 読み込んだ文字ををcHeadTextに格納
								sscanf(cReedText, "%s", &cHeadText);

								// cHeadTextがFRAMEだったら
								if (strcmp(cHeadText, "FRAME") == 0)
								{
									// m_nFrameにフレーム数を格納
									sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_motion[m_Type][nCntMotion].m_KeyInfo[nCntKey].m_nFrame);
								}
								// cHeadTextがKEYだったら
								if (strcmp(cHeadText, "KEY") == 0)
								{
									// END_KEYの文字が見つかるまで
									while (strcmp(cHeadText, "END_KEY") != 0)
									{
										// テキストからcReedText分文字を読み込む
										fgets(cReedText, sizeof(cReedText), pFile);
										// 読み込んだ文字ををcHeadTextに格納
										sscanf(cReedText, "%s", &cHeadText);

										// cHeadTextがPOSだったら
										if (strcmp(cHeadText, "POS") == 0)
										{
											// m_posに座標を格納
											sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_motion[m_Type][nCntMotion].m_KeyInfo[nCntKey].m_pos[nCntLoad].x,
												&m_motion[m_Type][nCntMotion].m_KeyInfo[nCntKey].m_pos[nCntLoad].y,
												&m_motion[m_Type][nCntMotion].m_KeyInfo[nCntKey].m_pos[nCntLoad].z);
										}
										// cHeadTextがROTだったら
										if (strcmp(cHeadText, "ROT") == 0)
										{
											// m_rotに回転を格納
											sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_motion[m_Type][nCntMotion].m_KeyInfo[nCntKey].m_rot[nCntLoad].x,
												&m_motion[m_Type][nCntMotion].m_KeyInfo[nCntKey].m_rot[nCntLoad].y,
												&m_motion[m_Type][nCntMotion].m_KeyInfo[nCntKey].m_rot[nCntLoad].z);
										}
									}
									// カウントの加算
									nCntLoad++;
								}
							}
							// 読み込んだカウントの初期化
							nCntLoad = 0;
							// カウントの加算
							nCntKey++;
						}
					}
					// キーの数の初期化
					nCntKey = 0;
					// カウントの加算
					nCntMotion++;
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
		// 開けなかったら
		else
		{
			printf("開けれませんでした\n");
		}
	}
}



//=============================================================================
// モーションクラスの更新処理
//=============================================================================
void CMotion::UpdateMotion(void)
{
	// モーションが1F前のモーションと違ったら
	if (m_nMotionOldState != m_nMotionState) {
		m_nFreme = 0;
		m_nCntKey = 0;
	}
	for (int nCntParts = 0; nCntParts < CLoadmodeldata::GetNumParts(m_Type); nCntParts++)
	{
		//フレーム開始時に、変更後の回転の値や位置の値などを設定
		if (m_nFreme == 0)
		{
			m_NumPos[nCntParts] = ((m_motion[m_Type][m_nMotionState].m_KeyInfo[m_nCntKey].m_pos[nCntParts] -
				(m_modelParent[nCntParts].m_pos - m_modelParent[nCntParts].m_posOrigin))
				/ float(m_motion[m_Type][m_nMotionState].m_KeyInfo[m_nCntKey].m_nFrame));

			m_NumRotDest[nCntParts] =
				m_motion[m_Type][m_nMotionState].m_KeyInfo[m_nCntKey].m_rot[nCntParts] - m_modelParent[nCntParts].m_rot;

			// 計算した回転の値がぶっ飛んでた時
			if (m_NumRotDest[nCntParts].x > D3DX_PI)
			{
				m_NumRotDest[nCntParts].x -= D3DX_PI * 2;
			}
			if (m_NumRotDest[nCntParts].x < -D3DX_PI)
			{
				m_NumRotDest[nCntParts].x += D3DX_PI * 2;
			}

			if (m_NumRotDest[nCntParts].y > D3DX_PI)
			{
				m_NumRotDest[nCntParts].y -= D3DX_PI * 2;
			}
			if (m_NumRotDest[nCntParts].y < -D3DX_PI)
			{
				m_NumRotDest[nCntParts].y += D3DX_PI * 2;
			}

			if (m_NumRotDest[nCntParts].z > D3DX_PI)
			{
				m_NumRotDest[nCntParts].z -= D3DX_PI * 2;
			}
			if (m_NumRotDest[nCntParts].z < -D3DX_PI)
			{
				m_NumRotDest[nCntParts].z += D3DX_PI * 2;
			}

			m_NumRot[nCntParts] = (m_NumRotDest[nCntParts] / float(m_motion[m_Type][m_nMotionState].m_KeyInfo[m_nCntKey].m_nFrame));
		}

		// 変更後の回転の値や位置の値を加算
		m_modelParent[nCntParts].m_pos += m_NumPos[nCntParts];
		m_modelParent[nCntParts].m_rot += m_NumRot[nCntParts];
	}

	// 現在のモーションの記録
	m_nMotionOldState = m_nMotionState;

	// ループするとき
	if (m_nFreme >= m_motion[m_Type][m_nMotionState].m_KeyInfo[m_nCntKey].m_nFrame && m_motion[m_Type][m_nMotionState].m_nLoop == 1)
	{
		m_nFreme = 0;
		m_nCntKey++;
		m_AngleModifying = false;

		// キーが記録されているキーより大きくなったら
		if (m_nCntKey >= m_motion[m_Type][m_nMotionState].m_nNumKey)
		{
			m_nCntKey = 0;
		}

	}
	// ループしないとき
	else if (m_nFreme >= m_motion[m_Type][m_nMotionState].m_KeyInfo[m_nCntKey].m_nFrame && m_motion[m_Type][m_nMotionState].m_nLoop == 0)
	{
		m_nFreme = 0;
		m_nCntKey++;
		m_AngleModifying = false;

		// キーが記録されているキーより大きくなったら
		if (m_nCntKey >= m_motion[m_Type][m_nMotionState].m_nNumKey)
		{
			for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
			{
				m_NumPos[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_NumRot[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			m_nCntKey = m_motion[m_Type][m_nMotionState].m_nNumKey;
			m_nMotionState = 0;
		}
	}
	else
	{
		m_nFreme++;
	}
}