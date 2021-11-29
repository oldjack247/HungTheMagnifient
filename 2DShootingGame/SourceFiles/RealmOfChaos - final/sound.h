#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "manager.h"

//=============================================================================
// サウンドクラス
//=============================================================================
class CSound
{
public:
	CSound();
	~CSound();

	typedef enum
	{
		SOUND_LABEL_TITLE_BGM = 0,
		SOUND_LABEL_GAME_BGM,
		SOUND_LABEL_2FBOSS_BATTLE_BGM,
		SOUND_LABEL_PLAYER_BULLET,
		SOUND_LABEL_BULLET_HIT,
		SOUND_LABEL_1FBOSS_BULLET,
		SOUND_LABEL_2FBOSS_CHARGE,
		SOUND_LABEL_2FBOSS_BULLET_FIRE,
		SOUND_LABEL_2FBOSS_FIRE_LOOP,
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	HRESULT InitSound(HWND hWnd);							//初期化処理
	void UninitSound(void);									//終了処理
	HRESULT PlaySound(SOUND_LABEL label);					//サウンド再生処理
	void StopSound(SOUND_LABEL label);						//サウンド停止処理
	void StopSound(void);									//サウンド停止処理

private:
	typedef struct
	{
		char* pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

															// 各音素材のパラメータ
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};

#endif
