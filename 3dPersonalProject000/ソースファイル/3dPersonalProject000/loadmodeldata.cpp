//=============================================================================
// インクルードファイル
//=============================================================================
#include "loadmodeldata.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//静的メンバ変数
//=============================================================================
LPD3DXMESH CLoadmodeldata::m_pMesh[MODELDATA_MAX][MAX_PARTS] = {};
LPD3DXBUFFER CLoadmodeldata::m_pBuffMat[MODELDATA_MAX][MAX_PARTS] = {};
DWORD CLoadmodeldata::m_nNumMat[MODELDATA_MAX][MAX_PARTS] = {};
char CLoadmodeldata::cModelFileName[MODELDATA_MAX][MAX_PARTS][MAX_TEXT] = {};
int CLoadmodeldata::m_nNumParts[MODELDATA_MAX] = {};
int CLoadmodeldata::m_nModelNum = 0;
LPDIRECT3DTEXTURE9 CLoadmodeldata::m_pTexture[MODELDATA_MAX][MAX_PARTS][MAX_MATERIAL] = {};

//=============================================================================
// [LoadModel] モデルの読み込み
// FileName : 読み込むファイルの名前
// type		: キャラクターのタイプ
//=============================================================================
HRESULT CLoadmodeldata::LoadModelData(char* cFileName, int nType)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ファイルポイント
	FILE* pFile;

	// 変数宣言
	int  nCntPart = 0;		// 読み込んだカウント
	char cReedText[MAX_TEXT];	// 文字として読み取り用
	char cHeadText[MAX_TEXT];	// 文字の判別用
	char cDie[MAX_TEXT];		// 使わない文字

	// ファイルを開く
	pFile = fopen(cFileName, "r");

	// 開けたら
	if (pFile != NULL)
	{
		// SCRIPTの文字が見つかるまで
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// テキストからcReedText分文字を受け取る
			fgets(cReedText, sizeof(cReedText), pFile);

			// cReedTextをcHeadTextに格納
			sscanf(cReedText, "%s", &cHeadText);
		}

		// cHeadTextがSCRIPTの時
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// cHeadTextがEND_SCRIPTになるまで
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReedText, sizeof(cReedText), pFile);
				sscanf(cReedText, "%s", &cHeadText);

				// cHeadTextがMODELNAMESETの時
				if (strcmp(cHeadText, "MODELNAMESET") == 0)
				{
					// cHeadTextがEND_MODELNAMESETになるまで
					while (strcmp(cHeadText, "END_MODELNAMESET") != 0)
					{
						fgets(cReedText, sizeof(cReedText), pFile);
						sscanf(cReedText, "%s", &cHeadText);

						// cHeadTextが改行の時
						if (strcmp(cHeadText, "\n") == 0)
						{
						}
						// cHeadTextがMODELNUMの時
						else if (strcmp(cHeadText, "MODELNUM") == 0)
						{
							// テキストのINDEXの値をm_nIndexに格納
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_nModelNum);
						}
						// cHeadTextがPARTNUMの時
						else if (strcmp(cHeadText, "PARTSNUM") == 0)
						{
							// テキストのINDEXの値をm_nNumPartsに格納
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_nNumParts[m_nModelNum]);
						}
						// cHeadTextがMODEL_FILENAMEの時
						else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
						{
							// テキストのINDEXの値をm_nIndexに格納
							sscanf(cReedText, "%s %s %s", &cDie, &cDie, &cModelFileName[m_nModelNum][nCntPart]);
							// カウント加算
							nCntPart++;
						}
					}
				}
				nCntPart = 0;
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

	for (int nType = 0; nType < MODELDATA_MAX; nType++)
	{
		// モデルの読み込み
		for (int nCount = 0; nCount < m_nNumParts[nType]; nCount++)
		{
			// Xファイルの読み込み
			D3DXLoadMeshFromX(cModelFileName[nType][nCount],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_pBuffMat[nType][nCount],
				NULL,
				&m_nNumMat[nType][nCount],
				&m_pMesh[nType][nCount]
			);
		}

		// テクスチャのロード
		LoadTexture(nType);
	}


	return S_OK;
}

//=============================================================================
// [LoadTexture] テクスチャの読み込み
// type		: キャラクターのタイプ
//=============================================================================
HRESULT CLoadmodeldata::LoadTexture(int nType)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCount = 0; nCount < m_nNumParts[nType]; nCount++)
	{
		// マテリアル情報を取り出す
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat[nType][nCount]->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (signed)m_nNumMat[nType][nCount]; nCntMat++)
		{
			// 使用しているテクスチャがあれば読み込む
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャ読み込み
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_pTexture[nType][nCount][nCntMat])))
				{
					return E_FAIL;
				}
			}
		}
	}
	return S_OK;
}