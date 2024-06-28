#pragma once
#include "Camera/Camera.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "BaseClass/Object.h"
#include "ArcBall.h"
#include "T_TanakaLib/Graphics.h"
#include "DeviceResources.h"
#include "StageObject/StageObject.h"
#include "UserInterFace/PlayerUI.h"
#include "Enemy/EnemyHpGauge.h"
#include "BaseClass/Collision.h"

class Object;
class Player;
class Enemy;
class ArcBall;

class PlayScene
{
private:
	//モデル
	std::unique_ptr<DirectX::Model> m_planeModel;
	// プレーヤーモデル
	std::unique_ptr<DirectX::Model> m_playerModel;
	//敵のモデル
	std::unique_ptr<DirectX::Model> m_enemyModel;
	

	DX::AnimationSDKMESH m_playersdk;

	//プレイヤークラス
	std::unique_ptr<Object> m_player;

	//エネミークラス
	std::vector<std::unique_ptr<Object>> m_enemy;
	//エネミーのHpクラス
	std::unique_ptr<EnemyHpGauge> m_EnemyHpGauge;

	//ステージのオブジェクトクラス
	std::unique_ptr<StageObject> m_stageObject;
	//プレイヤーUI
	std::unique_ptr<PlayerUI> m_playerUI;
	//エフェクト
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//フォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//--------------------------------------------------------//
	//影画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shadowTexture;
	//ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//深度ステンシルステート（床用）
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencil_Floor;
	//深度ステンシルステート（モデル用）
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencil_Model;
	//--------------------------------------------------------//
	//グラフィックス
	Graphics* m_graphics;
	//キーボードとマウス
	DirectX::Mouse* m_mouse;
	DirectX::Keyboard* m_keyBoard;
	//キーボードトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_tracker;
	//マウストラッカー
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonTracker;
	//タイムリミット画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timeLimitTex;


	//スカイドーム
	std::unique_ptr<DirectX::Model> m_skyDoomModel;

	//シーン遷移
	bool m_sceneChanger;
	//タイムリミット
	float m_timeLimit;

public:
	//モデルの取得
	DirectX::Model* GetPlayerModel() { return m_playerModel.get(); }
	DirectX::Model* GetEnemyModel() { return m_enemyModel.get(); }
	//シーン遷移取得
	bool GetSceneChanger() { return m_sceneChanger; }

public:
	PlayScene(DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~PlayScene();

	//初期化
	void Initilize();
	//更新
	void Update( DX::StepTimer& m_stepTimer);
	//描画
	void Render();
	//終了処理
	void Finalize();

	//影の初期化
	void InitilizeShadow(ID3D11Device* device, ID3D11DeviceContext* context);
	//影の描画
	void DrawShadow(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Vector3 position, float radius = 0.5f);

	//プレイヤーの状態変化更新
	void PlayerStateUpdate(DX::StepTimer& m_stepTimer,Player* player);
	//敵の状態変化更新
	void EnemyStateUpdate(DX::StepTimer& m_stepTimer,Player* player,Enemy* enemy);
	//グラフィッククラスのステンシルバッファの設定
	void SetStecilBuffer();

};