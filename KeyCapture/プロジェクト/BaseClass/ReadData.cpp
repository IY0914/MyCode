#include "pch.h"
#include "BaseClass/ReadData.h"

#include <fstream>
#include <assert.h>

using namespace DirectX;

ReadData::ReadData()
{
	m_data = 0;	//データを初期化
}

ReadData::~ReadData()
{


}

ReadData ReadData::LoadData(const wchar_t* path)
{
	//移動用の変数
	ReadData readData;

	//ファイル読み込み用変数
	std::ifstream ifs;

	// ファイルオープン
	ifs.open(path, std::ios::in | std::ios::binary);

	// 読み込み失敗時、強制終了
	assert(ifs);

	// ファイルサイズを取得
	ifs.seekg(0, std::fstream::end);
	std::streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	std::streamoff begPos = ifs.tellg();
	readData.m_size = (unsigned int)(eofPos - begPos);

	// 読み込むためのメモリを確保
	readData.m_data.reset(new char[readData.m_size]);

	// ファイル先頭からバッファへコピー 
	ifs.read(readData.m_data.get(), readData.m_size);

	// ファイルクローズ
	ifs.close();

	return std::move(readData);
}

ReadData::ReadData(ReadData&& in)
{
	m_data = std::move(in.m_data);	//データの移動用
	m_size = in.m_size;				//サイズの移動(コピー)
}
