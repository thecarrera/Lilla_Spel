Texture2D txDiffuse;
SamplerState sampAni;

cbuffer CBUFFER : register(b0)
{
	float4x4 worldM;
	float4x4 viewM;
	float4x4 projM;
	float3   lightPos;
}

struct FS_IN
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;
	float4 wPos : POSITION;
	float4 lPos : TEXCOORD0;
};

float4 FS_main(FS_IN input) : SV_Target
{
	float3 lightPos = { 20.0f, 50.0f, -0.0f };
	float3 lightDir = normalize(lightPos - input.wPos.xyz);
	float3 r = reflect(lightDir, input.Norm.xyz);
	float cos = dot(lightDir, input.Norm.xyz);

	float spec = dot(r, -input.wPos.xyz);

	float2 uv = input.uv;

	uv.x = 1 - uv.x;
	uv.y = 1 - uv.y;

	float3 s = txDiffuse.Sample(sampAni, uv).xyz + float3(0.1f, 0.1f, 0.15f) * cos /*+ pow(spec,2.f) * 0.1f*/;

	clamp(s, 0, 1);

	return float4(s, 1.f);
}