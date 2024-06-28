#pragma once
#include "DeviceResources.h"

class BinaryFile
{
protected:
	//データ格納用
	std::unique_ptr<char[]> m_data;

	//サイズ
	unsigned int m_size;

public:
	BinaryFile();
	~BinaryFile();
	
	static BinaryFile LoadFile(const wchar_t* filepath);

	//ムーブコンストラクタ
	BinaryFile(BinaryFile&& filedata);

	//アクセサ
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }
};