#pragma once
#include "DeviceResources.h"
#include "BaseClass/Collision.h"

class GoalObject
{
private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResources;
	//モデル
	std::unique_ptr<DirectX::Model> m_goalModel;
	std::unique_ptr<DirectX::Model> m_doorModel;
	//エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_state;

	//位置情報
	DirectX::SimpleMath::Vector3 m_position;

	//扉の判定
	bool m_key;

public:
	GoalObject(DX::DeviceResources* m_pDR);
	~GoalObject();

	//初期化
	void Initilize(DirectX::SimpleMath::Vector3 position);
	//更新
	void Update( DX::StepTimer& m_stepTimer,bool key);
	//描画
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//終了処理
	void Finalize();

};