#pragma once

class IState
{
public:
	virtual ~IState() = default;
	// 状態名を取得する
	virtual const wchar_t* Name() = 0;
	// 初期化する
	virtual void Initialize() = 0;
	// 事前更新する
	virtual void PreUpdate() = 0;
	// 更新する
	virtual void Update(const float& elapsedTime) = 0;
	// 事後更新する
	virtual void PostUpdate() = 0;
	// 描画する
	virtual void Render() = 0;
};