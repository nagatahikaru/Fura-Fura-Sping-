///////////////////////////////////////
// 構造体。
///////////////////////////////////////

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       //座標。
    float3 normal : NORMAL;         //法線。
    float3 tangent  : TANGENT;      //接ベクトル。
    float3 biNormal : BINORMAL;     //従ベクトル。
    float2 uv : TEXCOORD0;          //UV座標。
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0;         // アルベド
    float4 normal : SV_Target1;         // 法線
    float4 metaricShadowSmooth : SV_Target2;  // メタリック、影パラメータ、スムース。rにメタリック、gに影パラメータ、aにスムース。
};

///////////////////////////////////////
// 頂点シェーダーの共通処理をインクルードする。
///////////////////////////////////////

#include "../ModelVSCommon.h"


///////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);      //アルベドマップ
Texture2D<float4> g_normal : register(t1);      //法線マップ
Texture2D<float4> g_spacular : register(t2);    //スペキュラマップ

///////////////////////////////////////
// サンプラーステート
///////////////////////////////////////
sampler g_sampler : register(s0);

///////////////////////////////////////
// 関数
///////////////////////////////////////

// 法線マップから法線を取得。
float3 GetNormalFromNormalMap(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normal.SampleLevel (g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
    
    return newNormal;
}



// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer)
{
    SPSIn psIn;
    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal, isUsePreComputedVertexBuffer);
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換

    // ワールド空間の法線、接ベクトル、従ベクトルを計算する。
    CalcVertexNormalTangentBiNormalInWorldSpace(
		psIn.normal,
		psIn.tangent,
		psIn.biNormal,
		mWorldLocal,
		vsIn.normal,
		vsIn.tangent,
		vsIn.biNormal,
		isUsePreComputedVertexBuffer
	);

    psIn.uv = vsIn.uv;
    
    return psIn;
}
SPSOut PSMainCore( SPSIn psIn, int isShadowReciever)
{
    // G-Bufferに出力
    SPSOut psOut;
    // アルベドカラーと深度値を出力
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
    
    clip(psOut.albedo.a - 0.2f);    // ピクセルキル

    psOut.albedo.w = psIn.pos.z;
    // 法線を出力
    psOut.normal.xyz = GetNormalFromNormalMap( 
        psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv ) ;
    psOut.normal.w = 1.0f;
    // メタリックスムースを出力。
    psOut.metaricShadowSmooth = g_spacular.Sample(g_sampler, psIn.uv);
    // 影パラメータ。
    psOut.metaricShadowSmooth.g = 255.0f * isShadowReciever;
    
    return psOut;
}
// モデル用のピクセルシェーダーのエントリーポイント
SPSOut PSMain(SPSIn psIn)
{
    return PSMainCore(psIn, 0);
}
SPSOut PSMainShadowReciever(SPSIn psIn)
{
    return PSMainCore(psIn, 1);
}
// 背景(芝・観客席など)専用: 白いボールを目立たせるため、明るさと彩度を落として描画する。
SPSOut PSMainBackgroundTint(SPSIn psIn)
{
    SPSOut psOut = PSMainCore(psIn, 1); // 背景は影を受け取る。
    // 彩度を下げると緑が白っぽいグレーに寄ってボールとの区別がつきにくくなるため、
    // 彩度はむしろ少し上げて色味を保ったまま、明るさだけを落とす。
    float luminance = dot(psOut.albedo.rgb, float3(0.299f, 0.587f, 0.114f));
    static const float saturation = 1.6f; // 1.0で元の彩度、1.0超で色を濃くする。
    static const float brightness = 0.4f; // 明るさの倍率。
    // saturationを1.0超にすると成分が0~1の範囲を外れることがある。
    // 先にsaturateすると輝度の高いテクセルだけ0/1で急にクランプされてジャギーになるため、
    // 明るさを掛けて値を縮めてから最後にクランプする。
    float3 tinted = saturate(lerp(luminance.xxx, psOut.albedo.rgb, saturation) * brightness);
    // ファウルラインなど元々白い部分まで暗くすると、周囲の土/芝との差が急に大きくなって
    // 輪郭のジャギーが目立つため、明るいテクセルほど元の色に近づけて滑らかにつなぐ。
    float keepOriginalRate = smoothstep(0.6f, 0.9f, luminance);
    psOut.albedo.rgb = lerp(tinted, psOut.albedo.rgb, keepOriginalRate);
    return psOut;
}
