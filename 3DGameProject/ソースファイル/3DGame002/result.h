#ifndef _RESULT_H_
#define _RESULT_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "mode.h"

//=============================================================================
// リザルトクラス
//=============================================================================
class CResult :public CMode
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_GAMEOVER,
		TYPE_CLEAR,
		TYPE_MAX
	}TYPE;
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	TYPE m_Type;
};

#endif