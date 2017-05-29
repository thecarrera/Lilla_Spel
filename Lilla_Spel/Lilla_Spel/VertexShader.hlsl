struct VS_IN
{
	float3 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;
};

GS_OUT VS_main(VS_IN input)
{
	GS_OUT output = (GS_OUT)0;

	output.Pos = float4(input.Pos, 1.0f);
	output.uv = input.uv;
	output.Norm = input.Norm;

	return output;
}