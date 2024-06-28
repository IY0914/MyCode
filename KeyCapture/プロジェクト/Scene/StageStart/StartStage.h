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
	//���f��
	std::unique_ptr<DirectX::Model> m_planeModel;
	// �v���[���[���f��
	std::unique_ptr<DirectX::Model> m_playerModel;
	//�G�̃��f��
	std::unique_ptr<DirectX::Model> m_enemyModel;
	

	DX::AnimationSDKMESH m_playersdk;

	//�v���C���[�N���X
	std::unique_ptr<Object> m_player;

	//�G�l�~�[�N���X
	std::vector<std::unique_ptr<Object>> m_enemy;
	//�G�l�~�[��Hp�N���X
	std::unique_ptr<EnemyHpGauge> m_EnemyHpGauge;

	//�X�e�[�W�̃I�u�W�F�N�g�N���X
	std::unique_ptr<StageObject> m_stageObject;
	//�v���C���[UI
	std::unique_ptr<PlayerUI> m_playerUI;
	//�G�t�F�N�g
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//--------------------------------------------------------//
	//�e�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shadowTexture;
	//�x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//�[�x�X�e���V���X�e�[�g�i���p�j
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencil_Floor;
	//�[�x�X�e���V���X�e�[�g�i���f���p�j
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencil_Model;
	//--------------------------------------------------------//
	//�O���t�B�b�N�X
	Graphics* m_graphics;
	//�L�[�{�[�h�ƃ}�E�X
	DirectX::Mouse* m_mouse;
	DirectX::Keyboard* m_keyBoard;
	//�L�[�{�[�h�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_tracker;
	//�}�E�X�g���b�J�[
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonTracker;
	//�^�C�����~�b�g�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timeLimitTex;


	//�X�J�C�h�[��
	std::unique_ptr<DirectX::Model> m_skyDoomModel;

	//�V�[���J��
	bool m_sceneChanger;
	//�^�C�����~�b�g
	float m_timeLimit;

public:
	//���f���̎擾
	DirectX::Model* GetPlayerModel() { return m_playerModel.get(); }
	DirectX::Model* GetEnemyModel() { return m_enemyModel.get(); }
	//�V�[���J�ڎ擾
	bool GetSceneChanger() { return m_sceneChanger; }

public:
	PlayScene(DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~PlayScene();

	//������
	void Initilize();
	//�X�V
	void Update( DX::StepTimer& m_stepTimer);
	//�`��
	void Render();
	//�I������
	void Finalize();

	//�e�̏�����
	void InitilizeShadow(ID3D11Device* device, ID3D11DeviceContext* context);
	//�e�̕`��
	void DrawShadow(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Vector3 position, float radius = 0.5f);

	//�v���C���[�̏�ԕω��X�V
	void PlayerStateUpdate(DX::StepTimer& m_stepTimer,Player* player);
	//�G�̏�ԕω��X�V
	void EnemyStateUpdate(DX::StepTimer& m_stepTimer,Player* player,Enemy* enemy);
	//�O���t�B�b�N�N���X�̃X�e���V���o�b�t�@�̐ݒ�
	void SetStecilBuffer();

};