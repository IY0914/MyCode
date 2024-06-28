#include "pch.h"
#include "PlayScene.h"
#include <string>
#include <iostream>

using namespace DirectX;

PlayScene::PlayScene(DirectX::Keyboard* key, DirectX::Mouse* mouse):
	m_keyBoard(key),
	m_mouse(mouse),
	m_player(nullptr),
	m_timeLimit(0.0f)
{

	//エフェクト
	m_effect = std::make_unique<DirectX::EffectFactory>(Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice());
	//床の作成
	m_effect->SetDirectory(L"Resources/cmo");
	m_planeModel = DirectX::Model::CreateFromCMO(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		L"Resources/cmo/Plane.cmo",
		*m_effect);
	//スカイドームモデルの作成
	m_skyDoomModel = Model::CreateFromCMO(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		L"Resources/cmo/SkyDoom.cmo",
		*m_effect
	);
	//ステートオブジェクト
	m_states = std::make_unique<DirectX::CommonStates>(Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice());
	// グラフィックスインスタンスを取得する
	m_graphics = Graphics::GetInstance();
	//プロジェクションを設定
	Graphics::GetInstance()->SetProjectionMatrix(Camera::GetInstance()->GetProjection());
	//入力トラッカーの作成
	m_tracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	//画像ロード
	DirectX::CreateWICTextureFromFile(Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		L"Resources/Texture/TimeLimit.png", nullptr, m_timeLimitTex.ReleaseAndGetAddressOf());
	//スプライトバッチ作成
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext());

	//UI作成
	m_playerUI = std::make_unique<PlayerUI>(Graphics::GetInstance()->GetDeviceResources(),
		m_keyBoard, m_mouse);

	//敵のUI作成
	m_EnemyHpGauge = std::make_unique<EnemyHpGauge>(Graphics::GetInstance()->GetDeviceResources(), m_keyBoard, m_mouse);
}

PlayScene::~PlayScene()
{
	m_player.reset();
}

void PlayScene::Initilize()
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// リソースディレクトリを設定する
	m_graphics->GetFX()->SetDirectory(L"Resources/cmo");

	// CMO形式のプレーヤーモデルをロードする
	m_playerModel = DirectX::Model::CreateFromCMO(
		device,
		L"Resources/cmo/PlayerPone.cmo",
		*m_graphics->GetFX(),
		DirectX::ModelLoader_IncludeBones | DirectX::ModelLoader_Clockwise);

	m_enemyModel = DirectX::Model::CreateFromCMO(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		L"Resources/cmo/Player.cmo",
		*m_graphics->GetFX()
	);

	//スカイドームのライトを切る
	m_skyDoomModel->UpdateEffects([](DirectX::IEffect* effect) 
		{
			auto lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			//ライトがついていたら
			if(lights)
			{
				//1から3番ライトの設定
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
				//ライトの発光色を変更する
				lights->SetAmbientLightColor(Colors::Black);
			}
			//自己発光
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				//自己発光色を設定
				basicEffect->SetEmissiveColor(Colors::White);
			}
		});

	//シーン遷移初期化
	m_sceneChanger = false;

	//プレイヤーのインスタンスを生成する
	m_player = std::make_unique<Player>(m_keyBoard, m_mouse, this, 70.0f, -90.0f, 1.0f, DirectX::SimpleMath::Vector3::Zero);
	//プレイヤーを初期化する
	m_player->Initilize();
	//敵を初期化
	m_enemy.resize(1);
	m_enemy[0] = std::make_unique<Enemy>(m_keyBoard, m_mouse, this, 70.0f, -90.0f, 0.02f, DirectX::SimpleMath::Vector3::Zero);
	m_enemy[0]->Initilize();

	//ステージオブジェクトの作成
	m_stageObject = std::make_unique<StageObject>(
		Graphics::GetInstance()->GetDeviceResources(),
		m_keyBoard,
		m_mouse
	);
	//初期化
	m_stageObject->Initilize();

	//影の初期化
	InitilizeShadow(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext()
	);

	//時間を設定
	m_timeLimit = 60.0f;

	//UIの初期化
	m_playerUI->Initialize();
	//敵のHpの初期化
	m_EnemyHpGauge->Initialize();

	//モデル情報を設定
	Collision::GetInstance()->SetPlayerBoudingBox(*m_playerModel.get());
	Collision::GetInstance()->SetBoudingBox(*m_enemyModel.get());
}

void PlayScene::Update(DX::StepTimer& m_stepTimer)
{
	// 経過時間を取得する
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());

	// プレイヤーのポインタを取得する
	auto player = dynamic_cast<Player*>(m_player.get());
	//エネミーのポインタを取得する
	auto enemy = dynamic_cast<Enemy*>(m_enemy[0].get());

	//状態変化更新
	PlayerStateUpdate(m_stepTimer, player);
	EnemyStateUpdate(m_stepTimer, player, enemy);

	//ステージオブジェクトの更新
	m_stageObject->Update(m_stepTimer);
	// エネミーを更新する
	m_enemy[0]->Update(elapsedTime);
	// プレイヤーを更新する
	m_player->Update(elapsedTime);

	//コリジョンを更新
	Collision::GetInstance()->Update(m_player->GetPosition(), m_enemy[0]->GetPosition());

	//カメラのターゲットに設定
	Camera::GetInstance()->SetTargetPosition(m_player->GetPosition());

	//グラフィックスに対してカメラの描画設定を渡す
	Graphics::GetInstance()->SetViewMatrix(Camera::GetInstance()->GetView());
	Graphics::GetInstance()->SetProjectionMatrix(Camera::GetInstance()->GetProjection());
	// 平面を初期化する
	DirectX::SimpleMath::Plane plane(0.0f, 1.0f, 0.0f, 0.0f);

	//プレイヤーのHPゲージを設定
	m_playerUI->SetHPGauge(player->GetPlayerHP() / 1.0f);
	//UIの更新
	m_playerUI->Update();

	//敵のHpゲージの更新
	m_EnemyHpGauge->Update(m_enemy[0]->GetPosition());
}


void PlayScene::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	//描画行列
	DirectX::SimpleMath::Matrix planeScale;
	planeScale = DirectX::SimpleMath::Matrix::CreateScale(5.0f);	//大きさ10倍
	DirectX::SimpleMath::Matrix m_matrix;
	//プレイヤーについてくる
	m_matrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_player->GetPosition());

	//スカイドームの描画d
	m_skyDoomModel->Draw(
		context,
		*m_states,
		DirectX::SimpleMath::Matrix::Identity,
		Camera::GetInstance()->GetView(),
		Camera::GetInstance()->GetProjection());


	DirectX::SimpleMath::Matrix planeMatrix = planeScale;
	//床の描画
	m_planeModel->Draw(context,*m_states,planeMatrix,
		Camera::GetInstance()->GetView(),Camera::GetInstance()->GetProjection(),
		false, [&]() {
		//深度ステンシルの設定
		context->OMSetDepthStencilState(m_depthStencil_Floor.Get(), 0);

		//テクスチャサンプラの設定
		ID3D11SamplerState* sampler[] = { m_states->PointWrap()/*頂点で補強 ? */ };
		//ピクセルシェーダーのサンプルにセット
		context->PSSetSamplers(0, 1, sampler);
	});

	//キャラの描画
	m_player->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());
	//敵キャラの描画
	m_enemy[0]->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());
	//オブジェクトの描画
	m_stageObject->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());

	//影の描画
	DrawShadow(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext(),
		m_states.get(),
		m_player->GetPosition()
	);

	//UIの描画
	m_playerUI->Render(Graphics::GetInstance()->GetViewMatrix(), Graphics::GetInstance()->GetProjectionMatrix());
	//敵のHPの描画
	m_EnemyHpGauge->Render(Graphics::GetInstance()->GetViewMatrix(), Graphics::GetInstance()->GetProjectionMatrix());
}

void PlayScene::Finalize()
{
	//キャラの終了処理
	m_player->Finalize();
}

void PlayScene::InitilizeShadow(ID3D11Device* device, ID3D11DeviceContext* context)
{
	//ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	//ライティング(OFF)
	m_basicEffect->SetLightingEnabled(false);
	//頂点カラー(OFF)
	m_basicEffect->SetVertexColorEnabled(false);
	//テクスチャ(ON)
	m_basicEffect->SetTextureEnabled(true);

	//入力レイアウト
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	//プリミティブバッチ
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<VertexPositionTexture>>(context);

	//影のテクスチャ読み込み
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(
			device,
			L"Resources/dds/Shadow.dds",
			nullptr,
			m_shadowTexture.ReleaseAndGetAddressOf()
		)
	);
}

void PlayScene::DrawShadow(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Vector3 position, float radius)
{
	//※スペンシドバッファを使えば違和感のない影ができる(自主調べ)

	//エフェクトの設定
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(Graphics::GetInstance()->GetViewMatrix());
	m_basicEffect->SetProjection(Graphics::GetInstance()->GetProjectionMatrix());
	m_basicEffect->SetTexture(m_shadowTexture.Get());
	m_basicEffect->Apply(context);

	//入力レイアウト
	context->IASetInputLayout(m_inputLayout.Get());

	//テクスチャサンプラの設定
	ID3D11SamplerState* sampler[] = { states->LinearClamp() };
	context->PSSetSamplers(0, 1, sampler);

	//アルファブレンドの設定
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);

	//深度バッファの設定
	//context->OMSetDepthStencilState(states->DepthRead(), 0);
		//深度ステンシルの設定
	context->OMSetDepthStencilState(m_depthStencil_Model.Get(), 1);


	//影の頂点情報
	VertexPositionTexture vertexes[] =
	{
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(0.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(1.0f, 1.0f))
	};
	//影のインデックス情報
	uint16_t indexes[] = { 0,1,2,1,3,2 };

	//影の表示位置の設定
	vertexes[0].position = SimpleMath::Vector3(-radius, 0.01f, -radius) + position;
	vertexes[1].position = SimpleMath::Vector3(radius, 0.01f, -radius) + position;
	vertexes[2].position = SimpleMath::Vector3(-radius, 0.01f, radius) + position;
	vertexes[3].position = SimpleMath::Vector3(radius, 0.01f, radius) + position;

	//影の描画
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indexes, _countof(indexes),
		vertexes, _countof(vertexes)
	);
	m_primitiveBatch->End();

}

void PlayScene::PlayerStateUpdate(DX::StepTimer& m_stepTimer,Player* player)
{
	// 経過時間を取得する
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());
	//時間を減らす
	m_timeLimit -= elapsedTime;
	// キーボードの状態を取得する
	auto state = m_keyBoard->Get().GetState();
	// キーボードの状態を更新する
	m_tracker->Update(state);

	// マウスの状態を取得する
	auto mouseState = m_mouse->Get().GetState();
	// マウストラッカーを更新する
	m_buttonTracker->Update(mouseState);

	// [WASD]キーで
	if (state.W || state.S || state.A || state.D &&
		player->GetCurrentState() != player->GetAttackBladeState())
	{
		// 「歩行」状態に遷移する
		player->ChangeState(player->GetWalkingState());
	}
	// [左]クリックで
	else if (m_buttonTracker->leftButton)
	{
		// 「剣攻撃」状態に遷移する
		player->ChangeState(player->GetAttackBladeState());
	}
}

void PlayScene::EnemyStateUpdate(DX::StepTimer& m_stepTimer,Player* player,Enemy* enemy)
{
	//敵がプレイヤーを追跡する
	if (player->GetSurvive())
	{
		enemy->ChangeState(enemy->GetSearch());
		enemy->GetSearch()->SetTargetPosition(m_player->GetPosition());
	}
}

void PlayScene::SetStecilBuffer()
{
	//床の深度ステンシルの設定
	D3D11_DEPTH_STENCIL_DESC desc = {};

	//深度テスト
	desc.DepthEnable = TRUE;
	//深度情報を書き込む
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//小さいかイコールならテストをパスする
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	//ステンシルテスト(ON)
	desc.StencilEnable = TRUE;
	//0xffでマスクする
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	//表面の場合
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;	//=だったら
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	//+1	ステンシルテスト(OK)
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//何もしない	ステンシルテスト(NG)
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//何もしない		深度テスト(NG)

	//表面も同じ場合
	desc.BackFace = desc.FrontFace;

	Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice()->CreateDepthStencilState(
		&desc, m_depthStencil_Floor.ReleaseAndGetAddressOf());

	//深度情報を書き込まない
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace = desc.FrontFace;
	Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice()->CreateDepthStencilState(
		&desc, m_depthStencil_Model.ReleaseAndGetAddressOf());
}
