//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "loadmodeldata.h"

//=============================================================================
// キャラクタークラスのコンストラクタ
//=============================================================================
CCharacter::CCharacter(int nPriority) : CScene(nPriority)
{
	// モデルパーツの初期化
	memset(m_apFileName, 0, sizeof(m_apFileName));
	memset(m_apTextureFileName, 0, sizeof(m_apTextureFileName));
	memset(m_apModel, 0, sizeof(m_apModel));
	memset(m_nldxModelParent, 0, sizeof(m_nldxModelParent));
	memset(m_Beginpos, 0, sizeof(m_Beginpos));
	memset(m_Beginrot, 0, sizeof(m_Beginrot));

	m_nPartsNum = 0;
	m_bInvincible;
	m_pMotion = NULL;
	m_nStateCounter = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Type = CHARACTERTYPE_PLAYER;
}

//=============================================================================
// キャラクタークラスのデストラクタ
//=============================================================================
CCharacter::~CCharacter()
{

}

//=============================================================================
// キャラクタークラスの初期化処理
//=============================================================================
HRESULT CCharacter::Init(void)
{
	for (int nCount = 0; nCount < CLoadmodeldata::GetNumParts(m_Type); nCount++)
	{
		// モデルの生成
		m_apModel[nCount] = CModel::Create();

		if (nCount != 0)
		{
			m_apModel[nCount]->SetParent(m_apModel[m_nldxModelParent[nCount]]);
		}

		// モデルのバインド
		m_apModel[nCount]->BindModel(CLoadmodeldata::GetMesh(m_Type, nCount), CLoadmodeldata::GetBuffMat(m_Type, nCount), CLoadmodeldata::GetNumMat(m_Type, nCount));

		for (int nCntMat = 0; nCntMat < (int)CLoadmodeldata::GetNumMat(m_Type, nCount); nCntMat++)
		{
			// テクスチャのバインド
			m_apModel[nCount]->BindTexture(CLoadmodeldata::GetTextureData(m_Type, nCount), nCntMat);
		}

		m_apModel[nCount]->Setpos(m_Beginpos[nCount]);
		m_apModel[nCount]->Setrot(m_Beginrot[nCount]);

		m_apModel[nCount]->Init();
	}
	return E_NOTIMPL;
}

//=============================================================================
// キャラクタークラスの終了処理
//=============================================================================
void CCharacter::Uninit(void)
{
	// モデルパーツの初期化
	memset(m_apFileName, 0, sizeof(m_apFileName));
	memset(m_apTextureFileName, 0, sizeof(m_apTextureFileName));
	memset(m_apModel, 0, sizeof(m_apModel));

	m_pMotion = NULL;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// キャラクタークラスのモーションの読み込み
//=============================================================================
HRESULT CCharacter::LoadInitialPosition(char* cFileName)
{
	// ファイルポイント
	FILE* pFile;

	// 変数宣言
	int  nCntLoad = 0;	 // 読み込んだカウント
	char cReedText[MAX_TEXT]; // 文字として読み取り用
	char cHeadText[MAX_TEXT]; // 文字の判別用
	char cDie[MAX_TEXT];		 // 使わない文字
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ファイルを開く
	pFile = fopen(cFileName, "r");

	// 開けたら
	if (pFile != NULL)
	{
		// CHARACTERSETの文字が見つかるまで
		while (strcmp(cHeadText, "CHARACTERSET") != 0)
		{
			// テキストからcReedText分文字を受け取る
			fgets(cReedText, sizeof(cReedText), pFile);
			// cReedTextをcHeadTextに格納
			sscanf(cReedText, "%s", &cHeadText);
		}

		// cHeadTextがCHARACTERSETの時
		if (strcmp(cHeadText, "CHARACTERSET") == 0)
		{
			// cHeadTextがEND_CHARACTERSETになるまで
			while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
			{
				// テキストからcReedText分文字を受け取る
				fgets(cReedText, sizeof(cReedText), pFile);
				// cReedTextをcHeadTextに格納
				sscanf(cReedText, "%s", &cHeadText);

				// cHeadTextが改行の時
				if (strcmp(cHeadText, "\n") == 0)
				{
				}
				// cHeadTextがPARTSSETの時
				else if (strcmp(cHeadText, "PARTSSET") == 0)
				{
					// cHeadTextがEND_PARTSSETになるまで
					while (strcmp(cHeadText, "END_PARTSSET") != 0)
					{
						// テキストからcReedText分文字を受け取る
						fgets(cReedText, sizeof(cReedText), pFile);
						// cReedTextをcHeadTextに格納
						sscanf(cReedText, "%s", &cHeadText);

						// cHeadTextがINDEXになるまで
						if (strcmp(cHeadText, "INDEX") == 0)
						{
							// テキストのINDEXの値をm_nIndexに格納
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_nldxModelNum[nCntLoad]);
						}

						// cHeadTextがPARENTになるまで
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							// テキストのPARENTの値をm_nParentsに格納
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_nldxModelParent[nCntLoad]);
						}

						// cHeadTextがPOSになるまで
						if (strcmp(cHeadText, "POS") == 0)
						{
							// テキストのposの値をm_Beginposに格納
							sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
								&pos.x,
								&pos.y,
								&pos.z);

							// posの値をm_Beginposに代入
							m_Beginpos[nCntLoad] = pos;
						}

						// cHeadTextがROTになるまで
						if (strcmp(cHeadText, "ROT") == 0)
						{
							// テキストのrotの値をm_Beginrotに格納
							sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
								&rot.x,
								&rot.y,
								&rot.z);

							// rotの値をm_Beginrotに代入
							m_Beginrot[nCntLoad] = rot;
						}
					}
					// カウント加算
					nCntLoad++;
				}
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

	LoadMotion(cFileName);

	return E_NOTIMPL;
}

//=============================================================================
// キャラクタークラスのモーションの読み込み処理
//=============================================================================
void CCharacter::LoadMotion(char* FileName)
{
	m_pMotion = CMotion::Create(this, m_Type);

	if (m_pMotion != NULL)
	{
		m_pMotion->LoadMotion(FileName);
		for (int nCount = 0; nCount < CLoadmodeldata::GetNumParts(m_Type); nCount++)
		{
			m_pMotion->SetPos(nCount, m_Beginpos[nCount]);
			m_pMotion->SetRot(nCount, m_Beginrot[nCount]);
		}
	}
}


//=============================================================================
// キャラクタークラスの更新処理
//=============================================================================
void CCharacter::Update(void)
{
	if (m_pMotion != NULL)
	{
		m_pMotion->UpdateMotion();
	}

	// 無敵時間のとき
	if (m_bInvincible == true)
	{
		// カウンターを進める
		m_nStateCounter++;
	}
}

//=============================================================================
// キャラクタークラスの描画処理
//=============================================================================
void CCharacter::Draw(void)
{
	for (int nCount = 0; nCount < CLoadmodeldata::GetNumParts(m_Type); nCount++)
	{
		if (m_pMotion != NULL)
		{
			if (nCount != 0)
			{
				m_apModel[nCount]->SetModel(m_Beginpos[nCount], m_pMotion->GetRot(nCount), m_size);
			}
			else
			{
				m_apModel[nCount]->SetModel(m_pos + m_pMotion->GetPos(nCount), m_rot + m_pMotion->GetRot(nCount), m_size);
			}
		}
		else
		{
			m_apModel[nCount]->SetModel(m_pos, m_rot, m_size);
		}

		// モデルクラスの描画処理
		m_apModel[nCount]->Draw();
	}
}

//=============================================================================
// 無敵フラグ設定
//=============================================================================
void CCharacter::SetInvincible(bool bInvincible)
{
	m_bInvincible = bInvincible;
}

//=============================================================================
// 状態カウンターの設定
//=============================================================================
void CCharacter::SetStateCounter(int nStateCounter)
{
	m_nStateCounter = nStateCounter;
}