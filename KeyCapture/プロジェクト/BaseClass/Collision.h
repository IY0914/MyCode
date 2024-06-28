#pragma once
#include <vector>
#include <map>
#include <string>
#include "Camera/Camera.h"

class Collision
{
public:
	//シングルトン
	static Collision* const GetInstance();

private:
	//コリジョンのポインター
	static	std::unique_ptr<Collision> m_collision;

private:
	/*ポインター*/
	//デバイスリソース
	DX::DeviceResources* m_deviResources;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_state;
	//モデル
	DirectX::Model* m_model;
	//エフェクト
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//マウス
	DirectX::Mouse* m_mouse;
	//マウストラッカー
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonState;
	//キーボード
	DirectX::Keyboard* m_keyboard;
	//キーボードステートトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyState;

private:
	/*当たり判定*/
	//プレイヤーの当たり判定
	bool m_playerCol;
	//エネミーの当たり判定
	bool m_enemyCol;
	//アイテムの当たり判定
	bool m_ItemCol;
	//壁との当たり判定
	bool m_wallCol;

private:
	//敵の当たり判定
	std::vector<DirectX::BoundingBox> m_enemyBoudingBox;
	//プレイヤーの当たり判定
	DirectX::BoundingBox m_playerBoundingBox;
	//プレイヤーの攻撃当たり判定
	DirectX::BoundingBox m_attackBoudingBox;
	//動かないオブジェクトの当たり判定
	std::vector<DirectX::BoundingBox> m_objectBox;
	//壁にオブジェクトの当たり判定
	std::vector<DirectX::BoundingBox> m_wallObjectBox;
	//ゴール地点の当たり判定
	DirectX::BoundingBox m_goalBox;

private:
	//可視化用当たり判定
	std::vector<DirectX::GeometricPrimitive> m_geo;

public:
	//当たり判定を設定
	void SetBoudingBox(const DirectX::Model& model);
	void SetAttackBoudingBox(DirectX::SimpleMath::Vector3 pos,DirectX::SimpleMath::Vector3 scale);
	void SetPlayerBoudingBox(const DirectX::Model& player, DirectX::SimpleMath::Vector3 pos);
	void SetPlayerBoudingBox(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& size);

	//敵と攻撃の当たり判定の取得
	bool GetEnemyCollder() { return m_enemyCol; }

	//壁との当たり判定
	bool GetWallCollider() { return m_wallCol; }

public:
	// コンストラクタ
	Collision();

	// 代入は許容しない
	void operator=(const Collision& object) = delete;
	// コピーコンストラクタは許容しない
	Collision(const Collision& object) = delete;
	// デストラクタ
	~Collision();

	//初期化
	void Initialize();

	//更新
	 void Update(DirectX::SimpleMath::Vector3 playerpos,DirectX::SimpleMath::Vector3 
		 ,DirectX::SimpleMath::Vector3 weaponpos);

	 //描画
	 void Render();

	//終了処理
	 void Finalize();

	 //静的なオブジェクトの追加
	 void SetWallObjectModel(DirectX::Model* model, DirectX::SimpleMath::Matrix worldMatrix,DirectX::SimpleMath::Vector3 scale)
	 {
		 //バウンディングボックス
		 DirectX::BoundingBox m_boundingBox;

		 for (int i = 0; i < model->meshes.size(); i++)
		 {
			 m_boundingBox = model->meshes[i]->boundingBox;
			 m_boundingBox.Center = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Zero, worldMatrix);
			 m_boundingBox.Extents = scale;
		 }

		 m_wallObjectBox.push_back(m_boundingBox);
	 };

	 //ゴール地点の判定
	 void SetGoalBoundingBox(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 scale)
	 {
		 //バウンディングボックス
		 DirectX::BoundingBox boudingBox;

		 //当たり判定を入れる
		 boudingBox.Center = position;
		 boudingBox.Extents = scale;

		 m_goalBox = boudingBox;
	 }

	 //ジオメトリオブジェクトの追加
	 void SetObjectModel(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scale)
	 {
		 //バウンディングボックス
		 DirectX::BoundingBox m_boundingBox;

		 //バウンディングボックスに情報を追加する
		 m_boundingBox.Center = pos;
		 m_boundingBox.Extents = scale;


		 m_objectBox.push_back(m_boundingBox);
	 };

	 //オブジェクトの追加

	 //---------------------当たり判定--------------------------//

	 bool GetNextWallBound(DirectX::SimpleMath::Vector3 nextPos)
	 {
		 //判定用コピー
		 DirectX::BoundingBox playerBox = m_playerBoundingBox;
		 //プレイヤーの当たり判定を移動
		 playerBox.Center.x += nextPos.x;
		 playerBox.Center.y += nextPos.y;
		 playerBox.Center.z += nextPos.z;

		 //壁と接触しているかの判定
		 for (int i = 0; i < m_wallObjectBox.size(); i++)
		 {
			 if (m_wallObjectBox[i].Intersects(playerBox))
			 {
				 return true;
			 }
		 }

		 //移動できるので挿入
		 m_playerBoundingBox = playerBox;

		 return false;
	 }

	 //プレイヤーとアイテムの当たり判定の取得
	 bool GetItemCollider(int number) 
	 { 
		 
		 if (m_objectBox[number].Intersects(m_playerBoundingBox))
		 {
			 //要素の削除
			 m_objectBox.erase(m_objectBox.begin() + number);

			return true;
		 }
		 

		 return false;

	 }

	 //当たり判定の取得
	 bool GetDamageCollider(DirectX::SimpleMath::Vector3 pos)
	 {
		 for (int i = 0; i < m_enemyBoudingBox.size(); i++)
		 {
			 m_enemyBoudingBox[i].Center = pos;

			 if (m_enemyBoudingBox[i].Intersects(m_playerBoundingBox))
			 {
				 return true;
			 }
		 }

		 return false;
	 }

	 //ゴールの当たり判定の取得
	 bool GetGoalCollider()
	 {
		 if (m_playerBoundingBox.Intersects(m_goalBox))
		 {
			 return true;
		 }

		 return false;
	 }
};