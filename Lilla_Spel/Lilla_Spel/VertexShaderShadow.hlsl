cbuffer LBUFFER : register(b0)
{
	float4x4 worldM;
	float4x4 viewM;
	float4x4 projM;
	float3   lPos;
}
cbuffer CBUFFER : register(b1)
{
	float4x4 CworldM;
	float4x4 CviewM;
	float4x4 CprojM;
	float3	 lightPos;
}
struct VS_IN
{
	float3 Pos : SV_POSITION;
};


float4 VS_main(VS_IN input) : SV_POSITION
{
	float4 output;

	output = float4(input.Pos, 1.0f);

	float4x4 mtx = mul(worldM, mul(viewM, projM));

	output = mul(output, mtx);


	return output;
}