#pragma once

class Enemy
{
public:
	// 衝突中フラグを取得する
	bool GetInCollision() { return m_inCollision; }
	// 衝突中フラグをセット/リセットする
	void SetInCollision(bool inCollision) { m_inCollision = inCollision; }

public:
	//コンストラクタ
	Enemy(DX::DeviceResources* pDR, DirectX::Model* model, DirectX::Keyboard* key, DirectX::Mouse* mouse, DirectX::SimpleMath::Vector3 pos);
	//デストラクタ
	~Enemy();

	void Initilize(DirectX::SimpleMath::Vector3 pos);
	void Update(const float& m_stepTimer);
	void SeekNextPoint(int radius);
	//移動の関数
	DirectX::SimpleMath::Vector3 ReturnVelocity(DirectX::SimpleMath::Vector3 range);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

public:
	//目標位置を設定
	void SetTargetPos(DirectX::SimpleMath::Vector3 pos) { m_targetPos = pos; }

	//位置情報を取得
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	//床の状態を取得
	void SetFloorPosition(std::vector<DirectX::SimpleMath::Vector3> pos) { m_floorPosition = pos; }

private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResource;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//モデル
	DirectX::Model* m_model;
	//位置座標
	DirectX::SimpleMath::Vector3 m_position;
	//目標座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	//次の移位置
	DirectX::SimpleMath::Vector3 m_nextPos;
	//加速度
	DirectX::SimpleMath::Vector3 m_velocity;
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//移動可能な床の情報
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;

	// 衝突中
	bool m_inCollision;

	//敵に衝撃
	float m_inpact;
};