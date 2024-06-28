#include "pch.h"
#include "Player.h"
#include "Scene/PlayScene.h"

using namespace DirectX;

const float	Player::RUN_SPEED      = 5.0f;	//プレイヤーの最高速度
const float	Player::AVOID_TIME	   = 1.0f;	//回避時間

Player::Player(DX::DeviceResources* pDR, DirectX::Model* model,
	DirectX::Keyboard* key,DirectX::Mouse* mouse, DirectX::SimpleMath::Vector3 pos):
	m_deviceResource(pDR),					//デバイスリソース			
	m_model(model),							//プレーヤーモデル	
	m_keyboard(key),						//キーボード
	m_mouse(mouse),							//マウス
	m_position(pos),						//位置
	m_survive(false),						//生き残り判定									
	m_itemNum(0)
{
	
}

Player::~Player()
{
	
}

void Player::Initilize()
{
	auto device = m_deviceResource->GetD3DDevice();
	auto context = m_deviceResource->GetD3DDeviceContext();

	//コモンステートの作成
	m_states = std::make_unique<DirectX::CommonStates>(device);
	//エフェクトの作成
	m_effect = std::make_unique<DirectX::EffectFactory>(device);
	//キーボードステートトラッカーを作成
	m_stateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//ジオメトリモデル
	m_geometricModel = DirectX::GeometricPrimitive::CreateBox(context, SimpleMath::Vector3(1.0f, 2.0f, 1.0f));

	//生き残り初期化
	m_survive = true;
}

void Player::Update(const float& m_stepTimer)
{
	//キー情報
	auto key = m_keyboard->Get().GetState();

	//プレイヤーの動き
	MoveUpdate(m_stepTimer);

	if (//次のプレイヤーの位置を渡す
		Collision::GetInstance()->GetNextWallBound(m_velocity))
	{
		m_velocity = SimpleMath::Vector3::Zero;
	}
	//ゴールが解放されていない場合通れない
	if (Collision::GetInstance()->GetGoalCollider())
	{
		m_velocity = SimpleMath::Vector3::Zero;
	}

	//位置に加速度を加算
	m_position += m_velocity;
}

void Player::MoveUpdate(const float frameTimer)
{
	//キー情報
	auto key = m_keyboard->GetState();
	//キーステート
	m_stateTracker->Update(key);
	//マウス情報
	auto mouse = m_mouse->GetState();

	//加速度を初期化
	m_velocity = { 0.0f };

	//前進
	if (key.W)			m_velocity.z += frameTimer * RUN_SPEED;
	//後進
	if (key.S)			m_velocity.z += frameTimer * -RUN_SPEED;
	//右移動
	if (key.D)			m_velocity.x += frameTimer * -RUN_SPEED;
	//左移動
	if (key.A)			m_velocity.x += frameTimer *  RUN_SPEED;
}


void Player::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//デバイスとコンテキスト
	auto device = m_deviceResource->GetD3DDevice();
	auto context = m_deviceResource->GetD3DDeviceContext();

	//行列変換(回転)
	m_rot_Y = SimpleMath::Matrix::CreateRotationY(Camera::GetInstance()->GetMouseRotate());
	//行列変換(移動)
	m_trans = SimpleMath::Matrix::CreateTranslation(m_position);

	//行列計算
	m_world = m_rot_Y * m_trans;

	//モデルの描画
	//m_model->Draw(context, *m_states, m_world, view, proj);
	m_geometricModel->Draw(m_world, view, proj,Colors::Red);
}

void Player::Finalize()
{
}