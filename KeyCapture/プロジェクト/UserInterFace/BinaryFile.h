#pragma once
#include "DeviceResources.h"

class BinaryFile
{
protected:
	//�f�[�^�i�[�p
	std::unique_ptr<char[]> m_data;

	//�T�C�Y
	unsigned int m_size;

public:
	BinaryFile();
	~BinaryFile();
	
	static BinaryFile LoadFile(const wchar_t* filepath);

	//���[�u�R���X�g���N�^
	BinaryFile(BinaryFile&& filedata);

	//�A�N�Z�T
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }
};