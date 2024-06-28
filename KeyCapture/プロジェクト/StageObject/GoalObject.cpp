#include "pch.h"
#include "GoalObject.h"

using namespace DirectX;

GoalObject::GoalObject(DX::DeviceResources* m_pDR)
	: m_deviceResources(m_pDR)	//デバイスリソース
	, m_position(DirectX::SimpleMath::Vector3::Zero)
	, m_key(false)
{
	
}

GoalObject::~GoalObject()
{

}

void GoalObject::Initilize(DirectX::SimpleMath::Vector3 position)
{
	//デバイス
	auto device = m_deviceResources->GetD3DDevice();
	//コンテキスト
	auto context = m_deviceResources->GetD3DDeviceContext();

	//位置情報
	m_position = position;

	//エフェクトファクトリーの作成
	m_effect = std::make_unique<DirectX::EffectFactory>(device);
	m_effect->SetDirectory(L"Resources/cmo");

	//共通ステート
	m_state = std::make_unique<DirectX::CommonStates>(device);

	//モデルロード
	m_goalModel = DirectX::Model::CreateFromCMO(device, L"Resources/cmo/GoalFlag.cmo", *m_effect.get());
	m_doorModel = DirectX::Model::CreateFromCMO(device, L"Resources/cmo/Door.cmo", *m_effect.get());

	//当たり判定の取得
	Collision::GetInstance()->SetGoalBoundingBox(position, m_goalModel->meshes[0]->boundingBox.Extents);
}

void GoalObject::Update(DX::StepTimer& m_stepTimer, bool key)
{
	m_key = key;
}

void GoalObject::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto device =m_deviceResources->GetD3DDevice();
	auto context =m_deviceResources->GetD3DDeviceContext();
	
	//平行移動行列
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	//ゴールモデルの描画
	m_goalModel->Draw(context, *m_state.get(), world, view, proj);

	if (!m_key)
	{
		//ゴールの手前に出現
		world *= DirectX::SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, 0.0f, -1.0f));

		//ゴールが解放されていない場合(扉の描画)
		m_doorModel->Draw(context, *m_state.get(), world, view, proj);
	}
}

void GoalObject::Finalize()
{
	
}