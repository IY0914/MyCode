#pragma once
#include "BaseClass/Object.h"
#include "BaseClass/IState.h"
#include "BaseClass/Collision.h"
#include "../Camera/Camera.h"

class Player
{
private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResource;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//エフェクト
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//モデル
	DirectX::Model* m_model;
	//四角形のモデル
	std::unique_ptr<DirectX::GeometricPrimitive> m_geometricModel;
	//キーボード
	DirectX::Keyboard* m_keyboard;
	//キーボードステート
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_stateTracker;
	//マウス
	DirectX::Mouse* m_mouse;
	//位置座標
	DirectX::SimpleMath::Vector3 m_position;
	//加速度
	DirectX::SimpleMath::Vector3 m_velocity;

	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_rot_Y;
	DirectX::SimpleMath::Matrix m_trans;

	//生き残っているか
	bool m_survive;
	
	//アイテム取得数
	int m_itemNum;

private:
	/*固定値*/

	//プレイヤーの速度
	const static float RUN_SPEED;
	//回避時間
	const static float AVOID_TIME;

public:
	/*------------ゲッター--------------*/
	//キーボードを取得
	DirectX::Keyboard* GetKeyBoard() { return m_keyboard; }
	//プレイヤーの生き残り判定の取得
	bool GetSurvive() { return m_survive; }
	//プレイヤーの位置を取得する
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	//プレイヤーの速度を返す
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }

	//セッター
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

public:
	//コンストラクタ
	Player(DX::DeviceResources* pDR,DirectX::Model* model, DirectX::Keyboard* key, DirectX::Mouse* mouse, DirectX::SimpleMath::Vector3 spownPos);
	//デストラクタ
	~Player();

	void Initilize();
	void Update(const float& m_stepTimer);
	void MoveUpdate(const float frameTimer);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();
};