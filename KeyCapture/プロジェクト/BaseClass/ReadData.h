#pragma once
#include <memory>

class ReadData
{
protected:

	// �f�[�^
	std::unique_ptr<char[]> m_data;

	// �T�C�Y
	unsigned int m_size;

private:
	 
public:

	ReadData();

	~ReadData();
	//�Ăяo�����ł���悤�ɕW����
	static ReadData LoadData(const wchar_t* path);

	//���[�u�R���X�g���N�^
	ReadData(ReadData&& in);

	// �A�N�Z�T
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }

};