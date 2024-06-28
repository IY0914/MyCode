#pragma once
#include "DeviceResources.h"
#include "BaseClass/Collision.h"
#include "KeyObject.h"
#include "GoalObject.h"
#include <vector>

class StageObject
{
public:
	struct ObjectInfo
	{
		//位置情報
		DirectX::SimpleMath::Vector3 position;
		//回転
		float rotate;
		//バウンディングボックス
		DirectX::BoundingBox m_boudingBox;
	};

private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResources;
	//キーボード
	DirectX::Keyboard* m_keyboard;
	//キーボードステート
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyState;
	//マウス
	DirectX::Mouse* m_mouse;
	//マウスステートトラッカー
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonTracker;
	//モデル
	std::unique_ptr<DirectX::Model> m_wallModel;
	//収集モデル
	std::unique_ptr<DirectX::GeometricPrimitive> m_keyModel;
	//エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//オブジェクト情報
	std::vector<ObjectInfo> m_objectinfo;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_state;


	//鍵オブジェクト
	std::unique_ptr<KeyObject> m_keyObject;
	//鍵の数
	int num;

	//ゴールのオブジェクト
	std::unique_ptr<GoalObject> m_goalObject;
	//ゴールの開錠判定
	int m_keyNum;		//個数
	bool m_goalOpen;
	
	//マップ
	const static int WIDTH = 20;
	const static int HEIGHT = 20;
	int m_Map[WIDTH][HEIGHT];

	//マップの配列
	const wchar_t* pathList[3] =
	{ 
	  L"Resources/Data/MapData1.csv",
	  L"Resources/Data/MapData2.csv",
	  L"Resources/Data/MapData3.csv" 
	};

	//開始地点
	DirectX::SimpleMath::Vector3 m_startPosition;
	//ゴール地点
	DirectX::SimpleMath::Vector3 m_GoalPosition;
	//敵の地点
	DirectX::SimpleMath::Vector3 m_enemyPosition;
	//床の地点
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;

private:
	//壁描画行列
	DirectX::SimpleMath::Matrix m_wallWorld;

public:
	StageObject(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~StageObject();

	//初期化
	void Initilize(int StageNumber);
	//更新
	void Update( DX::StepTimer& m_stepTimer);
	//描画
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//終了処理
	void Finalize();
	//マップ読み込み
	void LoadMap(const wchar_t* path);

public:
	//位置の情報
	DirectX::SimpleMath::Vector3 GetStartPosition() { return m_startPosition; }
	DirectX::SimpleMath::Vector3 GetGoalPosition() { return m_GoalPosition; }
	DirectX::SimpleMath::Vector3 GetEnemyPosition() { return m_enemyPosition; }
	std::vector<DirectX::SimpleMath::Vector3> GetFloorPosition() { return m_floorPosition; }
	//ゴール条件の取得
	bool GetGoalOpen() { return m_goalOpen; }

	//鍵の個数を返す
	int GetKeyNum() { return m_keyNum; }
};