#pragma once
#include "Camera/Camera.h"
#include "Player/Player.h"
#include "Player/UI.h"
#include "Enemy/Enemy.h"
#include "ArcBall.h"
#include "StageObject/StageObject.h"
#include "BaseClass/Collision.h"
#include "Camera/Camera.h"


class Player;
class Enemy;

class PlayScene
{
private:
	//モデル
	std::unique_ptr<DirectX::Model> m_planeModel;
	// プレーヤーモデル
	std::unique_ptr<DirectX::Model> m_playerModel;
	//敵のモデル
	std::unique_ptr<DirectX::Model> m_enemyModel;
	
	//デバイスリソース
	DX::DeviceResources* m_deviceResource;

	//プレイヤークラス
	std::unique_ptr<Player> m_player;
	//プレイヤーのUI
	std::unique_ptr<UI> m_UI;
	//エネミークラス
	std::vector<std::unique_ptr<Enemy>> m_enemy;

	//ステージのオブジェクトクラス
	std::unique_ptr<StageObject> m_stageObject;
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
	//キーボードとマウス
	DirectX::Mouse* m_mouse;
	DirectX::Mouse::Mode m_mouseMode;
	DirectX::Keyboard* m_keyBoard;
	//キーボードトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_tracker;
	//マウストラッカー
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonTracker;
	//タイムリミット画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timeLimitTex;
	//デバッグフォント
	std::unique_ptr<DirectX::SpriteFont> m_debugFont;

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
	PlayScene(DX::DeviceResources* pDR,DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~PlayScene();

	//初期化
	void Initilize(int StageNumber);
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
	
	//グラフィッククラスのステンシルバッファの設定
	void SetStecilBuffer();

	//デバッグ表示
	void Debug();

};