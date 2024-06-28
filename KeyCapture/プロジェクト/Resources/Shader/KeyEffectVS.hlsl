#include "KeyEffect.hlsli"

//PS_INPUT型で返す(引数はVS_INPUT)
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;		//０で初期化?
	//頂点位置を取得
	output.pos = float4(input.pos, 1);

	//画像の色を取得
	output.color = input.color;
	//画像情報を取得
	output.tex = input.tex;

	return output;	//出力
}