#pragma once

class IState
{
public:
	virtual ~IState() = default;
	// ��Ԗ����擾����
	virtual const wchar_t* Name() = 0;
	// ����������
	virtual void Initialize() = 0;
	// ���O�X�V����
	virtual void PreUpdate() = 0;
	// �X�V����
	virtual void Update(const float& elapsedTime) = 0;
	// ����X�V����
	virtual void PostUpdate() = 0;
	// �`�悷��
	virtual void Render() = 0;
};