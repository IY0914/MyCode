#pragma once

//カメラのシングルトン化
class Camera
{
public:
	static Camera* const GetInstance();


private:
	// コンストラクタ
	Camera();

	// 代入は許容しない
	void operator=(const Camera& object) = delete;
	// コピーコンストラクタは許容しない
	Camera(const Camera& object) = delete;

private:
	// Cameraクラスのインスタンスへのポインタ
	static std::unique_ptr<Camera> m_camera;
	//デバイスリソース
	DX::DeviceResources* m_deviceResources;
	//カメラの設定
	DirectX::SimpleMath::Matrix m_view, m_proj;
	//カメラ関係
	DirectX::SimpleMath::Vector3 m_eye,m_target,m_up;
	//マウストラッカー
	DirectX::Mouse::ButtonStateTracker m_mouseTracker;
	//キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_stateTracker;
	//マウスとキーボード
	DirectX::Keyboard* m_keybBoard;
	DirectX::Mouse* m_mosue;
	DirectX::Mouse::Mode m_mouseMode;
	//視野角
	float m_fov;
	//縦横比
	float m_aspectRatio;
	//最大描画距離
	float m_farPlane;
	//最低描画距離
	float m_nearPlane;

private:
	//マウスの座標位置
	DirectX::SimpleMath::Vector3 m_mousePos;
	//マウスの回転
	DirectX::SimpleMath::Vector2 m_mouseRot;
	//マウス回転保存用
	DirectX::SimpleMath::Vector2 m_saveAngle;
	//画面に対しての比率
	DirectX::SimpleMath::Vector2 m_disRatio;

	//合成後の回転
	DirectX::SimpleMath::Matrix m_MathRotate;

public:

	void CreateCameraSetting(DirectX::Keyboard* key, DirectX::Mouse* mouse);
	void PreUpdate();
	void Update();
	void LimitMouseCursol(float mousePosX,float mousePosy);

public:
	//セッター
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& target) { m_target = target; }
	//デバイスリソースの設定
	void SetDeviceResource(DX::DeviceResources* pDR) { m_deviceResources = pDR; }

	//回転量を返す関数
	DirectX::SimpleMath::Matrix GetRotateAngle() { return m_MathRotate; }

	//回転量を小数で返す
	float GetMouseRotate() { return m_mouseRot.y; }

	//ゲッター
	DirectX::SimpleMath::Vector3 GetEyePosition() { return m_eye; }
	DirectX::SimpleMath::Vector3 GetTargetPositon() { return m_target; }
	DirectX::SimpleMath::Vector3 GetUpRotate() { return m_up; }
	DirectX::SimpleMath::Matrix GetView() { return m_view; }
	DirectX::SimpleMath::Matrix GetProjection() { return m_proj; }
	DirectX::SimpleMath::Matrix GetRotate() { return m_MathRotate; }
};
