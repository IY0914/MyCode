#include "pch.h"
#include "KeyObject.h"
#include <random>

using namespace DirectX;

KeyObject::KeyObject(DX::DeviceResources* m_pDR)
	:m_deviceResources(m_pDR)	//デバイスリソース
	, m_key(false)
	, m_keyNum(0)
	, m_keyPiece(0)
{
	
}

KeyObject::~KeyObject()
{

}

void KeyObject::Initilize(int num)
{
	//デバイス
	auto device = m_deviceResources->GetD3DDevice();
	//コンテキスト
	auto context = m_deviceResources->GetD3DDeviceContext();
	
	//出現させる鍵の数
	m_keyNum = num;
	m_keyPiece = num;

	//エフェクト
	m_effect = std::make_unique<DirectX::EffectFactory>(device);

	//コモンステートオブジェクトの作成
	m_state = std::make_unique<DirectX::CommonStates>(device);

	//エフェクトのパスを設定
	m_effect->SetDirectory(L"Resources/cmo");

	//鍵のモデル
	m_keyModel = DirectX::Model::CreateFromCMO(device,L"Resources/cmo/key.cmo",*m_effect);

	//オブジェクトの作成
	m_object.resize(m_keyNum);		//要素数の変更
	//鍵の位置配列のサイズ変更
	m_keyPosition.resize(m_keyNum);
	//鍵オブジェクトの設定
	for (int i = 0; i < m_object.size(); i++)
	{
		m_object[i] = std::make_unique<Object>(m_deviceResources, m_keyModel.get(),L"Resources/Texture/Luseter.png");
		
	}
	
	//行列の初期化
	m_wallWorld = DirectX::SimpleMath::Matrix::Identity;
	
	//出現する鍵の数を回す
	for (int i = 0; i < m_keyPosition.size(); i++)
	{
		////ここから下３行は構文
		//std::random_device              seedGenerator;								//非決定的に数字を出す(本物の乱数)：デメリットは再現性がない
		//std::mt19937                    randomNumberGenerator{ seedGenerator() };	//乱数エンジン：引数の数字次第で再現性が変わる
		//std::uniform_int_distribution<UINT64> distribution{ 0, m_floorPosition.size()};	//実引数に型名指定できる<>,変数名{下限 , 上限}が設定できる

		//11の余りが0の所に置く 例:11,22,33
		m_keyPosition[i] = m_floorPosition[i];

		m_object[i]->Initilize(m_keyPosition[i]);
		m_object[i]->SetObjectNumber(i);

		//当たり判定を追加
		Collision::GetInstance()->SetObjectModel(m_keyPosition[i], SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
	}

}

void KeyObject::Update(DX::StepTimer& m_stepTimer)
{
	for (int i = 0; i < m_keyNum; i++)
	{

		m_object[i]->SetObjectNumber(i);

		m_object[i]->Update(m_stepTimer);

		if (!m_object[i]->GetPopItem())
		{
			//要素数の減少
			m_keyNum--;

			//要素の削除
			m_object.erase(m_object.begin() + i);
		}
	}

	if (m_keyNum == 0)
	{
		m_key = true;
	}
}


void KeyObject::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto device =m_deviceResources->GetD3DDevice();
	auto context =m_deviceResources->GetD3DDeviceContext();
	
	//鍵の描画
	for (int i = 0; i < m_keyNum; i++)
	{
		m_object[i]->Render(view, proj);
	}
}

void KeyObject::Finalize()
{
	
}