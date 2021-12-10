#ifndef _CAMERA_H_
#define _CAMERA_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CAMERA_VIEW_DISTANCE (200.0f)				// カメラの距離
#define CAMERA_MOVE_LEFT_RIGHT (D3DXToRadian(2.0f))	// カメラの左右の移動量
#define CAMERA_MOVE_UP_DOWN (D3DXToRadian(1.5f))	// カメラの上下の移動量
#define CAMERA_LIMIT_UP (0.5f)						// カメラの上の上限
#define CAMERA_LIMIT_DOWN (1.5f)					// カメラの下の上限

//=============================================================================
// カメラクラス
//=============================================================================
class CCamera
{
public:
	CCamera();
	~CCamera();				//デストラクタ
	void Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理

	void GamePad(void);		// ゲームパッド処理
	void CameraMovement(void); // カメラの移動処理
	D3DXVECTOR3 GetPosR(void) { return m_posR; }		// 角度の受け取り処理
	D3DXVECTOR3 GetPosV(void) { return m_posV; }		// 座標の受け取り処理
	float GetPhi(void) { return m_fPhi; }
	float GetAngle(void); // カメラの角度を受け取る処理
private:
	D3DXVECTOR3 m_posV;		//位置
	D3DXVECTOR3 m_posR;		//角度
	D3DXVECTOR3 m_vecU;
	D3DXVECTOR3 m_posVDest;		//位置
	D3DXVECTOR3 m_posRDest;		//角度

	D3DXMATRIX	mtxProjection;
	D3DXMATRIX	mtxView;
	float m_fDistance;		// 距離
	float m_fTheta;			// カメラの横
	float m_fPhi;			// カメラの縦
};

#endif