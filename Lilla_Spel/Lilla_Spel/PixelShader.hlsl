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
	float3 uv : UV;
	float3 Norm : NORMAL;
	float4 wPos : POSITION;
};

float4 FS_main(FS_IN input) : SV_Target
{
float3 lightPos = { 1.0f, 0.0f, -10.0f };
float3 lightDir = normalize(lightPos - input.wPos.xyz);
float3 r = reflect(lightDir, input.Norm.xyz);
float cos = dot(lightDir, input.Norm.xyz);

float spec = dot(r, -input.wPos.xyz);

float2 uv = input.uv;

uv.y = 1 - uv.y;

float3 s = txDiffuse.Sample(sampAni, uv).xyz * cos + pow(float3(0.3f,0.3f,0.3f),3.0f) * 1.f;

clamp(s, 0, 1);

return float4(s, 1.f);
}