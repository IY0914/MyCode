#pragma once
#include "PlayScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "ResultScene.h"
#include "Camera/Camera.h"

class Scene
{
private:

	enum  SceneState
	{
		Title = 0,
		Play,
		Select,
		Setting,
		Quit,
		Result
	};

private:
	//�Q�[���V�[���̃|�C���^
	std::unique_ptr<PlayScene> m_playScene;
	//�^�C�g���V�[���̃|�C���^
	std::unique_ptr<TitleScene> m_titleScene;
	//�Z���N�g�V�[���̃|�C���^
	std::unique_ptr<SelectScene> m_selectScene;
	//���U���g�V�[���̃|�C���^
	std::unique_ptr<ResultScene> m_resultScene;
	//�f�o�C�X
	ID3D11Device* m_device;
	//�R���e�L�X�g
	ID3D11DeviceContext* m_context;

	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResource;

	//�V�[���X�V�p�񋓌^�ϐ�
	SceneState m_sceneState;

	//�L�[�{�[�h�ƃ}�E�X
	DirectX::Mouse* m_mouse;
	DirectX::Keyboard* m_keyBoard;	

	//�L�[�X�e�[�g
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyTracker;
private:
	//�V�[���J�ڗp�ϐ�
	bool m_sceneChanger;
	
public:
	Scene(DX::DeviceResources* pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~Scene();

	void Initialize();
	void Update(DX::StepTimer timer);
	void Render();
	void Finalize();

	void EndGame();

	//�X�e�[�W�I��p�̊֐�
	void StageSelectFunc();

public:
	//�V�[���̏�Ԃ�ݒ�
	void SetSceneState(SceneState& sceneState) { m_sceneState = sceneState; }
	//�V�[���̏�Ԃ��擾
	SceneState GetSceneState() { return m_sceneState; }
};
