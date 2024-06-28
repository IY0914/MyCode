#pragma once
#include "DeviceResources.h"
#include "BaseClass/Collision.h"
#include "BaseClass/Object.h"
#include <vector>

class KeyObject
{
private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResources;
	//収集モデル
	std::unique_ptr<DirectX::Model> m_keyModel;
	//エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_state;

	//オブジェクト
	std::vector<std::unique_ptr<Object>> m_object;
	
	
	//出現する鍵の数
	int m_keyNum;		//受け渡し用
	int m_keyPiece;		//判定用
	bool m_key;

	//床の位置配列
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;
	//鍵の位置配列
	std::vector<DirectX::SimpleMath::Vector3> m_keyPosition;

private:
	//壁描画行列
	DirectX::SimpleMath::Matrix m_wallWorld;

public:
	bool Getkeyget() { return m_key; }

	int GetkeyNum() { return m_keyNum; }

public:
	KeyObject(DX::DeviceResources* m_pDR);
	~KeyObject();

	//初期化
	void Initilize(int num);
	//更新
	void Update( DX::StepTimer& m_stepTimer);
	//描画
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//終了処理
	void Finalize();

	//鍵の位置を詰め込む
	void PushPosition(DirectX::SimpleMath::Vector3 pos) { m_floorPosition.push_back(pos); }

};