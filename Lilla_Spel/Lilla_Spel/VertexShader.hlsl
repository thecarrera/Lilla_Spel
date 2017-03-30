struct VS_IN
{
	float3 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

GS_OUT VS_main(VS_IN input)
{
	GS_OUT output = (GS_OUT)0;

	output.Pos = float4(input.Pos, 1.0f);
	output.Norm = float4(input.Norm, 0.0f);
	output.Tex = input.Tex;

	return output;
}