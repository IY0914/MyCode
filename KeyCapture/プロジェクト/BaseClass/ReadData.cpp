#include "pch.h"
#include "BaseClass/ReadData.h"

#include <fstream>
#include <assert.h>

using namespace DirectX;

ReadData::ReadData()
{
	m_data = 0;	//�f�[�^��������
}

ReadData::~ReadData()
{


}

ReadData ReadData::LoadData(const wchar_t* path)
{
	//�ړ��p�̕ϐ�
	ReadData readData;

	//�t�@�C���ǂݍ��ݗp�ϐ�
	std::ifstream ifs;

	// �t�@�C���I�[�v��
	ifs.open(path, std::ios::in | std::ios::binary);

	// �ǂݍ��ݎ��s���A�����I��
	assert(ifs);

	// �t�@�C���T�C�Y���擾
	ifs.seekg(0, std::fstream::end);
	std::streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	std::streamoff begPos = ifs.tellg();
	readData.m_size = (unsigned int)(eofPos - begPos);

	// �ǂݍ��ނ��߂̃��������m��
	readData.m_data.reset(new char[readData.m_size]);

	// �t�@�C���擪����o�b�t�@�փR�s�[ 
	ifs.read(readData.m_data.get(), readData.m_size);

	// �t�@�C���N���[�Y
	ifs.close();

	return std::move(readData);
}

ReadData::ReadData(ReadData&& in)
{
	m_data = std::move(in.m_data);	//�f�[�^�̈ړ��p
	m_size = in.m_size;				//�T�C�Y�̈ړ�(�R�s�[)
}
