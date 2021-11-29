#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include <xaudio2.h>

//=============================================================================
//クラス定義
//=============================================================================
class CSound
{
public:
	CSound();
	~CSound();
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,		// タイトル
		SOUND_LABEL_BGM_GAME,			// ゲーム
		SOUND_LABEL_BGM_RESULT,			// リザルト
		SOUND_LABEL_SE_DECISION,		// 決定
		SOUND_LABEL_SE_PLAYER_DAMAGE,	// ダメージ(プレイヤー)
		SOUND_LABEL_SE_ZOMBIE_DAMAGE,	// ダメージ(ゾンビ)
		SOUND_LABEL_SE_ZOMBIE_DEATH,	// 死(ゾンビ)
		SOUND_LABEL_SE_ENEMY_DAMAGE,	// 敵を切った時の音
		SOUND_LABEL_SE_MAGIC_FIRE,		// 炎の音
		SOUND_LABEL_SE_MAGIC,			// 魔法の音
		SOUND_LABEL_SE_MAGICCIRCLE,		// 魔法陣の音
		SOUND_LABEL_SE_SLASH,			// 斬る音
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
private:
	typedef struct
	{
		char* pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2* m_pXAudio2;								    // XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;			    // マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

};

#endif
