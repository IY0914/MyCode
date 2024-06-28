#pragma once
#include <memory>

class ReadData
{
protected:

	// データ
	std::unique_ptr<char[]> m_data;

	// サイズ
	unsigned int m_size;

private:
	 
public:

	ReadData();

	~ReadData();
	//呼び出しをできるように標準化
	static ReadData LoadData(const wchar_t* path);

	//ムーブコンストラクタ
	ReadData(ReadData&& in);

	// アクセサ
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }

};