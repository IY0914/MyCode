#include "pch.h"
#include "PlayScene.h"
#include <string>
#include <iostream>

using namespace DirectX;

PlayScene::PlayScene(DX::DeviceResources* pDR,DirectX::Keyboard* key, DirectX::Mouse* mouse):
	m_deviceResource(pDR),
	m_keyBoard(key),
	m_mouse(mouse),
	m_player(nullptr),
	m_timeLimit(0.0f)
{
	//�G�t�F�N�g�̍쐬
	m_effect = std::make_unique<DirectX::EffectFactory>(m_deviceResource->GetD3DDevice());
	//�f�o�b�O�t�H���g�̍쐬
	m_debugFont = std::make_unique<DirectX::SpriteFont>(m_deviceResource->GetD3DDevice(), L"Resources/Font/SegoeUI_18.spritefont");

	//���̍쐬
	m_effect->SetDirectory(L"Resources/cmo");
	m_planeModel = DirectX::Model::CreateFromCMO(
		m_deviceResource->GetD3DDevice(),
		L"Resources/cmo/Plane.cmo",
		*m_effect);
	//�X�J�C�h�[�����f���̍쐬
	m_skyDoomModel = Model::CreateFromCMO(
		m_deviceResource->GetD3DDevice(),
		L"Resources/cmo/SkyDoom.cmo",
		*m_effect
	);
	//�X�e�[�g�I�u�W�F�N�g
	m_states = std::make_unique<DirectX::CommonStates>(m_deviceResource->GetD3DDevice());
	//���̓g���b�J�[�̍쐬
	m_tracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	//�摜���[�h
	DirectX::CreateWICTextureFromFile(m_deviceResource->GetD3DDevice(),
		L"Resources/Texture/TimeLimit.png", nullptr, m_timeLimitTex.ReleaseAndGetAddressOf());
	//�X�v���C�g�o�b�`�쐬
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(
		m_deviceResource->GetD3DDeviceContext());
}

PlayScene::~PlayScene()
{
	m_player.reset();
}

void PlayScene::Initilize(int StageNumber)
{
	auto device = m_deviceResource->GetD3DDevice();

	//�����蔻��̏�����
	Collision::GetInstance()->Initialize();

	m_effect->SetDirectory(L"Resources/cmo");

	// CMO�`���̃v���[���[���f�������[�h����
	m_playerModel = DirectX::Model::CreateFromCMO(
		device,
		L"Resources/cmo/Player.cmo",
		*m_effect,
		DirectX::ModelLoader_IncludeBones | DirectX::ModelLoader_Clockwise);

	m_enemyModel = DirectX::Model::CreateFromCMO(
		m_deviceResource->GetD3DDevice(),
		L"Resources/cmo/Player.cmo",
		*m_effect
	);

	//�X�e�[�W�I�u�W�F�N�g�̍쐬
	m_stageObject = std::make_unique<StageObject>(
		m_deviceResource,
		m_keyBoard,
		m_mouse
	);

	//������
	m_stageObject->Initilize(StageNumber);

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
	m_player = std::make_unique<Player>(m_deviceResource,m_playerModel.get(), m_keyBoard, m_mouse, DirectX::SimpleMath::Vector3(0.0f,1.0f,0.0f));
	//�v���C���[������������
	m_player->Initilize();

	//UI�̍쐬
	m_UI = std::make_unique<UI>();
	m_UI->Initialize(m_deviceResource, m_deviceResource->GetOutputSize().right, m_deviceResource->GetOutputSize().bottom);

	//�G��������
	m_enemy.resize(1);
	m_enemy[0] = std::make_unique<Enemy>(m_deviceResource,m_enemyModel.get(), m_keyBoard, m_mouse, m_stageObject->GetEnemyPosition());
	m_enemy[0]->Initilize(m_stageObject->GetEnemyPosition());
	m_enemy[0]->SetFloorPosition(m_stageObject->GetFloorPosition());


	//�e�̏�����
	InitilizeShadow(
		m_deviceResource->GetD3DDevice(),
		m_deviceResource->GetD3DDeviceContext()
	);

	//���Ԃ�ݒ�
	m_timeLimit = 60.0f;

	//�J�n�n�_��ݒ�
	m_player->SetPosition(m_stageObject->GetStartPosition());

	//���f������ݒ�
	Collision::GetInstance()->SetPlayerBoudingBox(*m_playerModel.get(),m_player->GetPosition());
	Collision::GetInstance()->SetBoudingBox(*m_enemyModel.get());

}

void PlayScene::Update(DX::StepTimer& m_stepTimer)
{
	// �o�ߎ��Ԃ��擾����
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());

	//�������Ԃ�����
	m_timeLimit -= elapsedTime;

	//�X�e�[�W�I�u�W�F�N�g�̍X�V
	m_stageObject->Update(m_stepTimer);
	// �v���C���[���X�V����
	m_player->Update(elapsedTime);
	//�G�̒ǐՑΏۂɃv���C���[��ݒ�
	m_enemy[0]->SetTargetPos(m_player->GetPosition());
	
	

	//�R���W�������X�V
    Collision::GetInstance()->Update(m_player->GetPosition(), m_enemy[0]->GetPosition(),m_player->GetPosition());
	//UI�̍X�V
	m_UI->Update(m_stageObject->GetKeyNum());

	//�J�����̃^�[�Q�b�g�ɐݒ�
	Camera::GetInstance()->SetTargetPosition(m_player->GetPosition());
	

	// ���ʂ�����������
	DirectX::SimpleMath::Plane plane(0.0f, 1.0f, 0.0f, 0.0f);
	
	//����3����ăS�[���܂ōs������
	if (m_stageObject->GetGoalOpen() && Collision::GetInstance()->GetGoalCollider())
	{
		m_sceneChanger = true;
	}
	
}


void PlayScene::Render()
{
	auto context = m_deviceResource->GetD3DDeviceContext();
	//�`��s��
	DirectX::SimpleMath::Matrix planeScale;
	planeScale = DirectX::SimpleMath::Matrix::CreateScale(5.0f);
	DirectX::SimpleMath::Matrix m_matrix;

	//�v���C���[�ɂ��Ă���
	m_matrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_player->GetPosition());

	//�X�J�C�h�[���̕`��
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
	//m_enemy[0]->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());

	//�I�u�W�F�N�g�̕`��
	m_stageObject->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());

	//UI�̕`��
	m_UI->Render();

	Collision::GetInstance()->Render();

	//�e�̕`��
	DrawShadow(
		m_deviceResource->GetD3DDeviceContext(),
		m_states.get(),
		m_player->GetPosition()
	);

	//�f�o�b�O�\��
	Debug();
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
	m_basicEffect->SetView(Camera::GetInstance()->GetView());
	m_basicEffect->SetProjection(Camera::GetInstance()->GetProjection());
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

	m_deviceResource->GetD3DDevice()->CreateDepthStencilState(
		&desc, m_depthStencil_Floor.ReleaseAndGetAddressOf());

	//�[�x�����������܂Ȃ�
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace = desc.FrontFace;
	m_deviceResource->GetD3DDevice()->CreateDepthStencilState(
		&desc, m_depthStencil_Model.ReleaseAndGetAddressOf());
}

void PlayScene::Debug()
{
	m_spriteBatch->Begin();

	m_spriteBatch->End();

}
