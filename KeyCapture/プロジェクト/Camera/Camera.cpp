#include "pch.h"
#include "Camera.h"
#include <winuser.h>	//マウスカーソル

using namespace DirectX;

std::unique_ptr<Camera> Camera::m_camera = nullptr;

Camera* const Camera::GetInstance()
{
	if (m_camera == nullptr)
	{
		// カメラクラスのインスタンスを生成する
		m_camera.reset(new Camera());
	}
	// カメラクラスのインスタンスを返す
	return m_camera.get();
}

Camera::Camera():
	m_deviceResources(nullptr),
	m_fov(DirectX::XMConvertToRadians(45.0f)),
	m_aspectRatio(0.0f),
	m_nearPlane(0.01f),
	m_farPlane(1000.0f),
	m_eye(0.0f,10.0f, 100.0f),
	m_target(0.1f,0.1f,0.1f),
	m_up(DirectX::SimpleMath::Vector3::UnitY),
	m_mousePos(0.0f,0.0f,0.0f),
	m_disRatio(0.0f,0.0f),
	m_saveAngle(0.0f,0.0f),
	m_keybBoard(nullptr),
	m_mosue(nullptr),
	m_mouseMode(Mouse::Mode::MODE_ABSOLUTE)
{

}

void Camera::CreateCameraSetting(DirectX::Keyboard* keyboard,DirectX::Mouse* mouse)
{
	//ポインター
	m_mosue = mouse;
	m_keybBoard = keyboard;

	//画面比率
	m_aspectRatio =
		static_cast<float>(m_deviceResources->GetOutputSize().right /
			m_deviceResources->GetOutputSize().bottom);
	//画面設定
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		m_fov,
		m_aspectRatio,
		m_nearPlane,
		m_farPlane
	);
	//描画行列
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
		m_eye,
		m_target,
		m_up
	);
	//マウス位置用画面比率
	m_disRatio.x = 1.0f / float(m_deviceResources->GetOutputSize().right);
	m_disRatio.y = 1.0f / float(m_deviceResources->GetOutputSize().bottom);
	//マウスの情報を取得
	auto mouseVel = mouse->Get().GetState();
	//マウスの位置を保存
	m_mousePos.x = mouseVel.x;
	m_mousePos.y = mouseVel.y;

	m_eye = SimpleMath::Vector3(m_target.x, m_target.y + 1.0f, m_target.z - 30.0f);
	m_target = SimpleMath::Vector3(m_target.x, m_target.y + 3.0f, m_target.z);
	m_up = SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
}

void Camera::Update()
{
	//マウスの情報を取得
	auto mouseVel = m_mosue->Get().GetState();
	//キーボードの情報を取得
	auto key = m_keybBoard->Get().GetState();
	//トラッカーのステートの変更
	m_mouseTracker.Update(mouseVel);
	//キーボードのステート変更
	m_stateTracker.Update(key);

	//マウスカーソルが画面外に出ないように
	LimitMouseCursol(mouseVel.x, mouseVel.y);

	if (key.R)
	{
		//マウスの位置を保存
		m_mousePos.x = mouseVel.x;
		m_mousePos.y = mouseVel.y;
		//マウスからの回転を保持
		m_saveAngle.x = m_mouseRot.x;
		m_saveAngle.y = m_mouseRot.y;
	}

	//比率からのマウス位置(前の座標 - 現在座標) * 画面比率
	float dx = (m_mousePos.x - mouseVel.x);
	float dy = (m_mousePos.y - mouseVel.y);

	//マウス位置からの回転
	float yAngle = dx * DirectX::XM_PI * m_disRatio.x;
	float xAngle = dy * DirectX::XM_PI * m_disRatio.y;

	////縦
	//m_mouseRot.x = m_saveAngle.x + xAngle;
	////横
	//m_mouseRot.y = m_saveAngle.y + yAngle;

	//マウスの回転を行列に変換
	DirectX::SimpleMath::Matrix m_xRot = SimpleMath::Matrix::CreateRotationX(xAngle);
	DirectX::SimpleMath::Matrix m_yRot = SimpleMath::Matrix::CreateRotationY(yAngle);
	//回転の合成
	DirectX::SimpleMath::Matrix m_rot = m_yRot * m_xRot;
	//返す用の変数
	m_MathRotate = m_rot.Invert();

	//SimpleMath::Vector3 eye(0.0f, 1.0f, -10.0f/*m_target.x, m_target.y + 7.0f, m_target.z - 10.0f*/);
	//SimpleMath::Vector3 target = SimpleMath::Vector3(m_target.x, m_target.y + 3.0f, m_target.z);
	//SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	//カメラの視点設定用のローカル変数
	SimpleMath::Vector3 eye(m_target.x, m_target.y + 20.0f, m_target.z - 10.0f);
	SimpleMath::Vector3 target = SimpleMath::Vector3(m_target.x, m_target.y, m_target.z);
	SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);
	
	//SimpleMath::Vector3 eye = SimpleMath::Vector3::Transform(m_target, m_rot);//(m_target.x, m_target.y + 1.0f, m_target.z);
	//SimpleMath::Vector3 target = SimpleMath::Vector3(m_target.x + 1.0f, m_target.y, m_target.z);
	//SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	//カメラの位置に回転を適用
	//eye = 
	//回転から上の基準を変更
	//up = SimpleMath::Vector3::Transform(up, m_rot);

	//回転込みのビュー行列
	m_eye = eye;
	m_up = up;
	m_target = target;

	//カメラの変更を適用
	m_view = SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

void Camera::LimitMouseCursol(float mousePosX, float mousePosY)
{
	mousePosX;
	mousePosY;
}
