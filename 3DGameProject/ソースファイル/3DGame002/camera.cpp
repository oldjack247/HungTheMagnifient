//=============================================================================
// インクルードファイル
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "keyboard.h"
#include "joystick.h"
#include "player.h"
#include "game.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	memset(mtxProjection, NULL, sizeof(mtxProjection));
	memset(mtxView, NULL, sizeof(mtxView));
	m_fDistance = 0;
	m_fTheta = 1.0f;
	m_fPhi = 1.7f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	memset(mtxProjection, NULL, sizeof(mtxProjection));
	memset(mtxView, NULL, sizeof(mtxView));
	m_fDistance = sqrtf(powf(m_posV.x - m_posR.x, 2) + powf(m_posV.z - m_posR.z, 2));
	m_fTheta = 1.0f;
	m_fPhi = 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	GamePad();

	//=============================================================================
	//カメラ描画
	//=============================================================================
	D3DXMatrixIdentity(&mtxView);
	D3DXMatrixLookAtLH(&mtxView, &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixIdentity(&mtxProjection);
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10, 10000);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//=============================================================================
// ゲームパッド処理
//=============================================================================
void CCamera::GamePad(void)
{
	//コントローラーの取得処理
	DIJOYSTATE pStick = {};
	CInputJoystick* pInputJoystick = CManager::GetInputJoystick();
	LPDIRECTINPUTDEVICE8 pJoystickDevice = CInputJoystick::GetDevice();
	if (pJoystickDevice != NULL)
	{
		pJoystickDevice->Poll();
		pJoystickDevice->GetDeviceState(sizeof(DIJOYSTATE), &pStick);
	}

	//--------------------------
	//移動
	//--------------------------		
	// 右スティックを左に倒す
	if (pStick.lZ < 500)
	{
		m_fPhi -= CAMERA_MOVE_LEFT_RIGHT;
	}
	// 右スティックを右に倒す
	if (pStick.lZ > -500)
	{
		m_fPhi += CAMERA_MOVE_LEFT_RIGHT;
	}
	// 右スティックを上に倒す
	if (pStick.lRz < 500)
	{
		m_fTheta -= CAMERA_MOVE_UP_DOWN;
	}
	// 右スティックを下に倒す
	if (pStick.lRz > -500)
	{
		m_fTheta += CAMERA_MOVE_UP_DOWN;
	}

	// カメラの移動処理
	CameraMovement();

	if (CGame::GetPlayer() != NULL)
	{
		//注視点
		m_posR = CGame::GetPlayer()->GetPos();
	}

	//視点	
	m_fDistance = CAMERA_VIEW_DISTANCE;	//距離
	m_posV.x = m_fDistance * (sinf(m_fTheta) * cosf(m_fPhi)) + m_posR.x;
	m_posV.y = m_fDistance * cosf(m_fTheta) + m_posR.y;
	m_posV.z = m_fDistance * (sinf(m_fTheta) * sinf(m_fPhi)) + m_posR.z;
}

//=============================================================================
// カメラの移動処理
//=============================================================================
void CCamera::CameraMovement(void)
{
	// 左右の範囲を超えないようにする
	if (m_fPhi >= D3DXToRadian(360.0f))
	{
		m_fPhi = 0.0f;
	}
	else if (m_fPhi <= D3DXToRadian(-360.0f))
	{
		m_fPhi = 0.0f;
	}

	// 上下の範囲の制限
	if (m_fTheta >= CAMERA_LIMIT_DOWN)
	{
		m_fTheta = CAMERA_LIMIT_DOWN;
	}
	else if (m_fTheta <= CAMERA_LIMIT_UP)
	{
		m_fTheta = CAMERA_LIMIT_UP;
	}

}

//=============================================================================
// カメラの角度を受け取る処理
//=============================================================================
float CCamera::GetAngle(void)
{
	// 返す角度
	float fReturnAngle = 0.0f;

	// 角度を決める(カメラの位置から、注視点を見た時の角度)
	fReturnAngle = atan2f((m_posR.x - m_posV.x), (m_posR.z - m_posV.z));

	return fReturnAngle;
}