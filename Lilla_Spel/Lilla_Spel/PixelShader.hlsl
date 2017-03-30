struct FS_IN
{
	float4 Pos : SV_POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float4 wPos : POSITION;
};

float4 FS_main(FS_IN input) : SV_Target
{
float3 lightPos = { 1.0f, 0.0f, -10.0f };
float3 lightDir = normalize(lightPos - input.wPos.xyz);
float3 r = reflect(lightDir, input.Norm.xyz);
float cos = dot(lightDir, input.Norm.xyz);

float spec = dot(r, -input.wPos.xyz);

float2 uv = input.Tex;

uv.y = 1 - uv.y;

float3 s = (0.5f, 0.5f, 0.5f) * cos + pow((0.3f,0.3f,0.3f),2.0f) * 1.f;

clamp(s, 0, 1);

return float4(float3(0.0f,0.0f,0.0f), 1.f);
}