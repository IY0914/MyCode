#include "pch.h"
#include "StageObject.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace DirectX;
#define MOVE_HEIGHT_TITLENAME (10)
#define KEY_NUM (3)

//int StageObject::m_Map[WIDTH][HEIGHT] =
//{
//	 {5, 5, 5, 5, 5, 5, 5, 5, 5},
//	 {3, 0, 0, 0, 0, 0, 0, 0, 5},
//	 {5, 0, 5, 5, 0, 5, 5, 0, 5},
//	 {5, 0, 5, 0, 0, 0, 5, 0, 5},
//	 {5, 0, 0, 0, 5, 0, 0, 0, 5},
//	 {5, 0, 5, 0, 0, 0, 5, 0, 5},
//	 {5, 0, 5, 5, 0, 5, 5, 0, 5},
//	 {5, 0, 0, 0, 0, 0, 0, 0, 1},
//	 {5, 5, 5, 5, 5, 5, 5, 5, 5}
//};

StageObject::StageObject(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse)
	:m_deviceResources(m_pDR)								//デバイスリソース
	, m_keyboard(key)										//キーボード
	, m_mouse(mouse)											//マウス
	, num(0)
	, m_goalOpen(false)

{
	//キーボードとマウスのトラッカー
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	//初期化
	m_objectinfo.clear();
}

StageObject::~StageObject()
{

}

void StageObject::Initilize(int StageNumber)
{
	//デバイス
	auto device = m_deviceResources->GetD3DDevice();
	//コンテキスト
	auto context = m_deviceResources->GetD3DDeviceContext();

	//マップのデータ作成
	LoadMap(pathList[StageNumber]);

	//エフェクト
	m_effect = std::make_unique<DirectX::EffectFactory>(device);

	//コモンステートオブジェクトの作成
	m_state = std::make_unique<DirectX::CommonStates>(device);

	//キーオブジェクトの作成
	m_keyObject = std::make_unique<KeyObject>(m_deviceResources);

	//ゴールオブジェクトの作成
	m_goalObject = std::make_unique<GoalObject>(m_deviceResources);

	//エフェクトのパスを設定
	m_effect->SetDirectory(L"Resources/cmo");

	//壁モデルのロード
	m_wallModel = DirectX::Model::CreateFromCMO(
		device,
		L"Resources/cmo/Wall.cmo",
		*m_effect
	);

	//鍵のモデル(仮)
	m_keyModel = DirectX::GeometricPrimitive::CreateBox(context, SimpleMath::Vector3(1.0f, 0.5f, 1.0f));

	//行列の初期化
	m_wallWorld = DirectX::SimpleMath::Matrix::Identity;

	//鍵の数
	num = KEY_NUM;

	for (int w = 0; w < WIDTH; w++)
	{
		for (int h = 0; h < HEIGHT; h++)
		{
			//床の位置を渡す
			if (m_Map[h][w] == 0)
			{
				m_floorPosition.push_back(SimpleMath::Vector3(w * 2.0f, 1.0f, h * 2.0f));
			}

			//鍵の位置を渡す
			if (m_Map[h][w] == 4)
			{
				m_keyObject->PushPosition(SimpleMath::Vector3(w * 2.0f, 1.0f, h * 2.0f));
			}

			//初期の敵の位置を渡す
			if (m_Map[h][w] == 2)
			{
				m_enemyPosition = SimpleMath::Vector3(w * 2.0f, 1.0f, h * 2.0f);
			}

			//スタート地点を設定する
			if (m_Map[h][w] == 1)
			{
				m_startPosition = SimpleMath::Vector3(w * 2.0f, 0.0f, h * 2.0f);
			}

			//ゴール地点を設定する
			if (m_Map[h][w] == 3)
			{
				m_goalObject->Initilize(DirectX::SimpleMath::Vector3(w * 2.0f, 0.0f, h * 2.0f));
			}

			//壁の地点を渡す
			if (m_Map[h][w] == 5)
			{
				m_wallWorld = DirectX::SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(w * 2.0f, 0.0f, h * 2.0f));

				//当たり判定の設定
				Collision::GetInstance()->SetWallObjectModel(m_wallModel.get(), m_wallWorld, SimpleMath::Vector3::One);
			}
		}
	}



	//キーの初期化
	m_keyObject->Initilize(3);
}

void StageObject::Update(DX::StepTimer& m_stepTimer)
{
	// 経過時間を取得する
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());
	//キーボードの状態を取得
	auto keystate = m_keyboard->Get().GetState();
	//キーボードステートを更新
	m_keyState->Update(keystate);
	//マウスの状態を取得
	auto mouse = m_mouse->Get().GetState();
	//マウスクリックを更新
	m_buttonTracker->Update(mouse);

	//キーオブジェクトを更新する
	m_keyObject->Update(m_stepTimer);

	//ゴールオブジェクト更新する
	m_goalObject->Update(m_stepTimer,m_keyObject->Getkeyget());

	//鍵の数の更新
	m_keyNum = m_keyObject->GetkeyNum();

	//ゴール条件(解放条件)
	if (m_keyObject->Getkeyget())
	{
		m_goalOpen = true;
	}


}


void StageObject::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto device =m_deviceResources->GetD3DDevice();
	auto context =m_deviceResources->GetD3DDeviceContext();
	
	//ステージの描画
	for (int w = 0; w < WIDTH; w++)
	{
		for (int h = 0; h < HEIGHT; h++)
		{
			//壁の配置
			if (m_Map[h][w] == 5)
			{
				//平行移動
				DirectX::SimpleMath::Matrix trans =
					DirectX::SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(w * 2.0f, 0.0f, h * 2.0f));

				m_wallWorld = trans;

				//壁の描画
				m_wallModel->Draw(
					context,
					*m_state,
					m_wallWorld,
					view,
					proj
				);
			}
		}
	}

	//鍵の描画
	m_keyObject->Render(view, proj);

	//ゴールの描画
	m_goalObject->Render(view, proj);

}

void StageObject::Finalize()
{
	
}

/// <summary>
/// マップ読み込み
/// 0 = 床
/// 1 = スタート
/// 2 = 敵
/// 3 = ゴール
/// 4 = 鍵
/// 5 = 壁
/// </summary>
/// <param name="path"></param>
void StageObject::LoadMap(const wchar_t* path)
{
	//インポートファイル用変数
	std::ifstream ifs;

	//変数に外部ファイルを開く
	ifs.open(path);

	//読み込み失敗した場合
	if (ifs.fail())
		return;

	//文字列取得
	std::string buf1;

	//行の取得
	for (int i = 0; i < WIDTH; i++)
	{
		//一行分
		std::getline(ifs, buf1);

		//いったん格納
		std::stringstream ss(buf1);

		//文字変数
		std::string buf2;

		//列の取得
		for (int j = 0; j < HEIGHT; j++)
		{
			std::getline(ss, buf2, ',');

			m_Map[i][j] = atoi(buf2.c_str());
		}
	}

	ifs.close();
}
