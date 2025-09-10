//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    float4x4 matWVP;    // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matNormal; 
    float4x4 matW;      // ワールド行列
    float4 diffuseColor; // ディフューズカラー（マテリアルの色）
    //float4 speculaColor;
    bool isTexture; // テクスチャ貼ってあるかどうか
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv.xy;
    
    normal = mul(normal, matNormal);
    normal = normalize(normal);
    
    // 楽しい平行光源
    float4 light = float4(0, 0, -1, 0); // 高原の位置 // 那須光源
    light = normalize(light);
    outData.color = clamp(dot(normal, light), 0, 1); // 光線と頂点のベクトルとの内積の結果を突っ込んでいる
    
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT PsInData) : SV_Target
{
    // テクスチャから色を取得している
    //float4 specular = 
    
    const float ambientPower = 0.3;
    
    float4 diffuse;
    float4 ambient;
    if (isTexture == true)
    {
        diffuse = g_texture.Sample(g_sampler, PsInData.uv) * PsInData.color;
        ambient = g_texture.Sample(g_sampler, PsInData.uv) * float4(ambientPower, ambientPower, ambientPower, 1);
    }
    else
    {
        diffuse = diffuseColor;
        ambient = diffuseColor * float4(ambientPower, ambientPower, ambientPower, 1);
    }

    return diffuse + ambient;
}
