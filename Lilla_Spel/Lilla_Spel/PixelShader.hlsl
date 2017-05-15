Texture2D txDiffuse : register(t0);
Texture2D txShadow : register(t1);
Texture2D txMask : register(t2);
Texture2D txGround : register(t3);
SamplerState sampAni : register(s0);
SamplerState samp : register(s1);
cbuffer LBUFFER : register(b0)
{
	float4x4 lWorld;
	float4x4 lView;
	float4x4 lProj;
	float4	 lPos;
}

struct FS_IN
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;
	float4 wPos : POSITION0;
	float4 lPos : TEXCOORD0;
};
SamplerComparisonState BlurSamp : register(s2) {
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	ComparisonFunc = LESS_EQUAL;
};
float2 texOffset(int u, int v)
{
	return float2(u * 1.0f / 800, v*1.0f / 600);
}

float4 FS_main(FS_IN input) : SV_Target
{
	float  shadowBias = .0028f;
	float3 ambient = { 0.1f, 0.1f, 0.1f };
	float4 lightPos = { 0.0f, 3.0f, 0.0f, 1.0f };

	float3 lightDir = normalize(lightPos.xyz - input.wPos.xyz);
	float3 r = reflect(lightDir, input.Norm.xyz);

	input.lPos.xy /= input.lPos.w;

	if (input.lPos.x < -1.0f || input.lPos.x > 1.0f ||
	input.lPos.y < -1.0f || input.lPos.y > 1.0f ||
	input.lPos.z < 0.0f	 || input.lPos.z > 1.0f) return float4(ambient,1.0f);



	//Convert LightPos to Texturespaces
	input.lPos.x = input.lPos.x / 2 + 0.5f;
	input.lPos.y = input.lPos.y / -2 + 0.5f;

	//Apply Shadow Bias
	input.lPos.z -= shadowBias;

	float shadowDepth = input.lPos.z / input.lPos.w;
	//float shadowDepth = txShadow.Sample(samp, input.lPos.xy).r;

	//float shadowFactor = input.lPos.z <= shadowDepth;


	//TODO Calculate sum with txMask
	float sum = 0;
	float4 mask = { 0,0,0,0 };
	//float4 ground = txGround.Gather(samp, input.lPos.xyz, 0);
	mask = txMask.Gather(samp,input.lPos.xy, 1);
	input.lPos.x %= 1.0f;
	input.lPos.y %= 1.0f;
	clamp(mask, 0, 1);
	//float x, y;
	//for (y = -1.5f; y <= 1.5f; y += 1.0f) {
	//	for (x = -1.5f; x <= 1.5f; x += 1.0f) {
	//
	//		sum += txShadow.SampleCmpLevelZero(BlurSamp, input.lPos.x + input.lPos.y + texOffset(x,y).x + texOffset(x, y).y, shadowDepth * mask.x);
	//	}
	//
	//}

	float cos = dot(lightDir, input.Norm.xyz);
	float shadowFactor = sum / 16.0f;
	clamp(shadowFactor, 0.8f, 1);

	float spec = dot(r, -input.wPos.xyz);

	float2 uv = input.uv;



	//	uv.y = 1 - uv.y;

	//return float4(input.uv,1.0f);
	float3 diff = saturate(dot(normalize(input.Norm), lightDir));
	//diff = diff* (ground * 0.4);

	float3 s = /*txDiffuse.Sample(sampAni, uv).xyz*/ ambient * cos; //+ pow(spec, 2.0f) * 1.0;

	clamp(diff, 0.2f, 0.6f);

	//return mask;
	//return input.lPos;
	//return float4(shadowFactor, shadowFactor, shadowFactor, 1);
	//return float4(input.uv, 1.0f);
	//return input.wPos;
	//return float4(input.Norm, 1.0);
	//return float4(diff, 1.0f);
	return float4(1, 1, 1, 1);
	//return float4(saturate(1 * diff + ambient), 1);

}