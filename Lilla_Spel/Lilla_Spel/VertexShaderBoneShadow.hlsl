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
cbuffer cSkeletonBuffer : register(b2)
{
	matrix boneTrans[64];
}
struct VS_IN
{
	float3 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;

	int jointIndices[8] : INT;
	float jointWeights[8] : FLOAT;
};


float4 VS_main(VS_IN input) : SV_POSITION
{
	float4 output;

	float4 transformedPos = float4(0, 0, 0, 1);
	for (int connectedJ = 0; input.jointIndices[connectedJ] != -1 && connectedJ < 8; connectedJ++) {
		transformedPos += mul(float4(input.Pos, 1), boneTrans[input.jointIndices[connectedJ]]) * input.jointWeights[connectedJ];
	}

	output = transformedPos;

	float4x4 mtx = mul(worldM, mul(viewM, projM));

	output = mul(output, mtx);


	return output;
}
