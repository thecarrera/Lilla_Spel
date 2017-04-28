Texture2D txDiffuse : register(t0);
SamplerState sampAni;
cbuffer CBUFFER : register(b0)
{
	float3 Kd;
	float3 Ka;
	float3 Ks;
}

struct FS_IN
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;
	float4 wPos : POSITION;
};

float4 FS_main(FS_IN input) : SV_Target
{
float3 lightPos = { 0.0f, 50.0f, -30.0f };
float3 lightDir = normalize(lightPos - input.wPos.xyz);
float3 r = reflect(lightDir, input.Norm.xyz);
float cos = dot(lightDir, input.Norm.xyz);

float spec = dot(r, -input.wPos.xyz);

float2 uv = input.uv;

uv.y = 1 - uv.y;

float3 s = /*txDiffuse.Sample(sampAni, uv).xyz*/ float3(1.0f, 0.0f, 1.0f) * cos /*+ pow(spec,2.f) * 0.2f*/;

clamp(s, 0, 1);

return float4(s, 1.f);
}