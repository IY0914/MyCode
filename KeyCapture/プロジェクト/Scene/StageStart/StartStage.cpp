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

	//�G�t�F�N�g
	m_effect = std::make_unique<DirectX::EffectFactory>(Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice());
	//���̍쐬
	m_effect->SetDirectory(L"Resources/cmo");
	m_planeModel = DirectX::Model::CreateFromCMO(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		L"Resources/cmo/Plane.cmo",
		*m_effect);
	//�X�J�C�h�[�����f���̍쐬
	m_skyDoomModel = Model::CreateFromCMO(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		L"Resources/cmo/SkyDoom.cmo",
		*m_effect
	);
	//�X�e�[�g�I�u�W�F�N�g
	m_states = std::make_unique<DirectX::CommonStates>(Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice());
	// �O���t�B�b�N�X�C���X�^���X���擾����
	m_graphics = Graphics::GetInstance();
	//�v���W�F�N�V������ݒ�
	Graphics::GetInstance()->SetProjectionMatrix(Camera::GetInstance()->GetProjection());
	//���̓g���b�J�[�̍쐬
	m_tracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	//�摜���[�h
	DirectX::CreateWICTextureFromFile(Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		L"Resources/Texture/TimeLimit.png", nullptr, m_timeLimitTex.ReleaseAndGetAddressOf());
	//�X�v���C�g�o�b�`�쐬
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext());

	//UI�쐬
	m_playerUI = std::make_unique<PlayerUI>(Graphics::GetInstance()->GetDeviceResources(),
		m_keyBoard, m_mouse);

	//�G��UI�쐬
	m_EnemyHpGauge = std::make_unique<EnemyHpGauge>(Graphics::GetInstance()->GetDeviceResources(), m_keyBoard, m_mouse);
}

PlayScene::~PlayScene()
{
	m_player.reset();
}

void PlayScene::Initilize()
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// ���\�[�X�f�B���N�g����ݒ肷��
	m_graphics->GetFX()->SetDirectory(L"Resources/cmo");

	// CMO�`���̃v���[���[���f�������[�h����
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

	//�X�J�C�h�[���̃��C�g��؂�
	m_skyDoomModel->UpdateEffects([](DirectX::IEffect* effect) 
		{
			auto lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			//���C�g�����Ă�����
			if(lights)
			{
				//1����3�ԃ��C�g�̐ݒ�
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
				//���C�g�̔����F��ύX����
				lights->SetAmbientLightColor(Colors::Black);
			}
			//���Ȕ���
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				//���Ȕ����F��ݒ�
				basicEffect->SetEmissiveColor(Colors::White);
			}
		});

	//�V�[���J�ڏ�����
	m_sceneChanger = false;

	//�v���C���[�̃C���X�^���X�𐶐�����
	m_player = std::make_unique<Player>(m_keyBoard, m_mouse, this, 70.0f, -90.0f, 1.0f, DirectX::SimpleMath::Vector3::Zero);
	//�v���C���[������������
	m_player->Initilize();
	//�G��������
	m_enemy.resize(1);
	m_enemy[0] = std::make_unique<Enemy>(m_keyBoard, m_mouse, this, 70.0f, -90.0f, 0.02f, DirectX::SimpleMath::Vector3::Zero);
	m_enemy[0]->Initilize();

	//�X�e�[�W�I�u�W�F�N�g�̍쐬
	m_stageObject = std::make_unique<StageObject>(
		Graphics::GetInstance()->GetDeviceResources(),
		m_keyBoard,
		m_mouse
	);
	//������
	m_stageObject->Initilize();

	//�e�̏�����
	InitilizeShadow(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice(),
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext()
	);

	//���Ԃ�ݒ�
	m_timeLimit = 60.0f;

	//UI�̏�����
	m_playerUI->Initialize();
	//�G��Hp�̏�����
	m_EnemyHpGauge->Initialize();

	//���f������ݒ�
	Collision::GetInstance()->SetPlayerBoudingBox(*m_playerModel.get());
	Collision::GetInstance()->SetBoudingBox(*m_enemyModel.get());
}

void PlayScene::Update(DX::StepTimer& m_stepTimer)
{
	// �o�ߎ��Ԃ��擾����
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());

	// �v���C���[�̃|�C���^���擾����
	auto player = dynamic_cast<Player*>(m_player.get());
	//�G�l�~�[�̃|�C���^���擾����
	auto enemy = dynamic_cast<Enemy*>(m_enemy[0].get());

	//��ԕω��X�V
	PlayerStateUpdate(m_stepTimer, player);
	EnemyStateUpdate(m_stepTimer, player, enemy);

	//�X�e�[�W�I�u�W�F�N�g�̍X�V
	m_stageObject->Update(m_stepTimer);
	// �G�l�~�[���X�V����
	m_enemy[0]->Update(elapsedTime);
	// �v���C���[���X�V����
	m_player->Update(elapsedTime);

	//�R���W�������X�V
	Collision::GetInstance()->Update(m_player->GetPosition(), m_enemy[0]->GetPosition());

	//�J�����̃^�[�Q�b�g�ɐݒ�
	Camera::GetInstance()->SetTargetPosition(m_player->GetPosition());

	//�O���t�B�b�N�X�ɑ΂��ăJ�����̕`��ݒ��n��
	Graphics::GetInstance()->SetViewMatrix(Camera::GetInstance()->GetView());
	Graphics::GetInstance()->SetProjectionMatrix(Camera::GetInstance()->GetProjection());
	// ���ʂ�����������
	DirectX::SimpleMath::Plane plane(0.0f, 1.0f, 0.0f, 0.0f);

	//�v���C���[��HP�Q�[�W��ݒ�
	m_playerUI->SetHPGauge(player->GetPlayerHP() / 1.0f);
	//UI�̍X�V
	m_playerUI->Update();

	//�G��Hp�Q�[�W�̍X�V
	m_EnemyHpGauge->Update(m_enemy[0]->GetPosition());
}


void PlayScene::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	//�`��s��
	DirectX::SimpleMath::Matrix planeScale;
	planeScale = DirectX::SimpleMath::Matrix::CreateScale(5.0f);	//�傫��10�{
	DirectX::SimpleMath::Matrix m_matrix;
	//�v���C���[�ɂ��Ă���
	m_matrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_player->GetPosition());

	//�X�J�C�h�[���̕`��d
	m_skyDoomModel->Draw(
		context,
		*m_states,
		DirectX::SimpleMath::Matrix::Identity,
		Camera::GetInstance()->GetView(),
		Camera::GetInstance()->GetProjection());


	DirectX::SimpleMath::Matrix planeMatrix = planeScale;
	//���̕`��
	m_planeModel->Draw(context,*m_states,planeMatrix,
		Camera::GetInstance()->GetView(),Camera::GetInstance()->GetProjection(),
		false, [&]() {
		//�[�x�X�e���V���̐ݒ�
		context->OMSetDepthStencilState(m_depthStencil_Floor.Get(), 0);

		//�e�N�X�`���T���v���̐ݒ�
		ID3D11SamplerState* sampler[] = { m_states->PointWrap()/*���_�ŕ⋭ ? */ };
		//�s�N�Z���V�F�[�_�[�̃T���v���ɃZ�b�g
		context->PSSetSamplers(0, 1, sampler);
	});

	//�L�����̕`��
	m_player->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());
	//�G�L�����̕`��
	m_enemy[0]->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());
	//�I�u�W�F�N�g�̕`��
	m_stageObject->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());

	//�e�̕`��
	DrawShadow(
		Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext(),
		m_states.get(),
		m_player->GetPosition()
	);

	//UI�̕`��
	m_playerUI->Render(Graphics::GetInstance()->GetViewMatrix(), Graphics::GetInstance()->GetProjectionMatrix());
	//�G��HP�̕`��
	m_EnemyHpGauge->Render(Graphics::GetInstance()->GetViewMatrix(), Graphics::GetInstance()->GetProjectionMatrix());
}

void PlayScene::Finalize()
{
	//�L�����̏I������
	m_player->Finalize();
}

void PlayScene::InitilizeShadow(ID3D11Device* device, ID3D11DeviceContext* context)
{
	//�x�[�V�b�N�G�t�F�N�g�̍쐬
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	//���C�e�B���O(OFF)
	m_basicEffect->SetLightingEnabled(false);
	//���_�J���[(OFF)
	m_basicEffect->SetVertexColorEnabled(false);
	//�e�N�X�`��(ON)
	m_basicEffect->SetTextureEnabled(true);

	//���̓��C�A�E�g
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	//�v���~�e�B�u�o�b�`
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<VertexPositionTexture>>(context);

	//�e�̃e�N�X�`���ǂݍ���
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
	//���X�y���V�h�o�b�t�@���g���Έ�a���̂Ȃ��e���ł���(���咲��)

	//�G�t�F�N�g�̐ݒ�
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(Graphics::GetInstance()->GetViewMatrix());
	m_basicEffect->SetProjection(Graphics::GetInstance()->GetProjectionMatrix());
	m_basicEffect->SetTexture(m_shadowTexture.Get());
	m_basicEffect->Apply(context);

	//���̓��C�A�E�g
	context->IASetInputLayout(m_inputLayout.Get());

	//�e�N�X�`���T���v���̐ݒ�
	ID3D11SamplerState* sampler[] = { states->LinearClamp() };
	context->PSSetSamplers(0, 1, sampler);

	//�A���t�@�u�����h�̐ݒ�
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);

	//�[�x�o�b�t�@�̐ݒ�
	//context->OMSetDepthStencilState(states->DepthRead(), 0);
		//�[�x�X�e���V���̐ݒ�
	context->OMSetDepthStencilState(m_depthStencil_Model.Get(), 1);


	//�e�̒��_���
	VertexPositionTexture vertexes[] =
	{
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(0.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3::Zero, SimpleMath::Vector2(1.0f, 1.0f))
	};
	//�e�̃C���f�b�N�X���
	uint16_t indexes[] = { 0,1,2,1,3,2 };

	//�e�̕\���ʒu�̐ݒ�
	vertexes[0].position = SimpleMath::Vector3(-radius, 0.01f, -radius) + position;
	vertexes[1].position = SimpleMath::Vector3(radius, 0.01f, -radius) + position;
	vertexes[2].position = SimpleMath::Vector3(-radius, 0.01f, radius) + position;
	vertexes[3].position = SimpleMath::Vector3(radius, 0.01f, radius) + position;

	//�e�̕`��
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
	// �o�ߎ��Ԃ��擾����
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());
	//���Ԃ����炷
	m_timeLimit -= elapsedTime;
	// �L�[�{�[�h�̏�Ԃ��擾����
	auto state = m_keyBoard->Get().GetState();
	// �L�[�{�[�h�̏�Ԃ��X�V����
	m_tracker->Update(state);

	// �}�E�X�̏�Ԃ��擾����
	auto mouseState = m_mouse->Get().GetState();
	// �}�E�X�g���b�J�[���X�V����
	m_buttonTracker->Update(mouseState);

	// [WASD]�L�[��
	if (state.W || state.S || state.A || state.D &&
		player->GetCurrentState() != player->GetAttackBladeState())
	{
		// �u���s�v��ԂɑJ�ڂ���
		player->ChangeState(player->GetWalkingState());
	}
	// [��]�N���b�N��
	else if (m_buttonTracker->leftButton)
	{
		// �u���U���v��ԂɑJ�ڂ���
		player->ChangeState(player->GetAttackBladeState());
	}
}

void PlayScene::EnemyStateUpdate(DX::StepTimer& m_stepTimer,Player* player,Enemy* enemy)
{
	//�G���v���C���[��ǐՂ���
	if (player->GetSurvive())
	{
		enemy->ChangeState(enemy->GetSearch());
		enemy->GetSearch()->SetTargetPosition(m_player->GetPosition());
	}
}

void PlayScene::SetStecilBuffer()
{
	//���̐[�x�X�e���V���̐ݒ�
	D3D11_DEPTH_STENCIL_DESC desc = {};

	//�[�x�e�X�g
	desc.DepthEnable = TRUE;
	//�[�x������������
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//���������C�R�[���Ȃ�e�X�g���p�X����
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	//�X�e���V���e�X�g(ON)
	desc.StencilEnable = TRUE;
	//0xff�Ń}�X�N����
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	//�\�ʂ̏ꍇ
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;	//=��������
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	//+1	�X�e���V���e�X�g(OK)
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//�������Ȃ�	�X�e���V���e�X�g(NG)
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//�������Ȃ�		�[�x�e�X�g(NG)

	//�\�ʂ������ꍇ
	desc.BackFace = desc.FrontFace;

	Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice()->CreateDepthStencilState(
		&desc, m_depthStencil_Floor.ReleaseAndGetAddressOf());

	//�[�x�����������܂Ȃ�
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace = desc.FrontFace;
	Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice()->CreateDepthStencilState(
		&desc, m_depthStencil_Model.ReleaseAndGetAddressOf());
}
