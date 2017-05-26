cbuffer cSkeletonBuffer : register(b0)
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

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;
};

GS_OUT VS_main(VS_IN input)
{
	GS_OUT output = (GS_OUT)0;

	float4 transformedPos = float4(0, 0, 0, 1);
	float4 transformedNorm = float4(0, 0, 0, 1);
	for (int connectedJ = 0; input.jointIndices[connectedJ] != -1 && connectedJ < 8; connectedJ++) {
		transformedPos += mul(float4(input.Pos, 1), boneTrans[input.jointIndices[connectedJ]]) * input.jointWeights[connectedJ];
		transformedNorm += mul(float4(input.Norm, 1), boneTrans[input.jointIndices[connectedJ]]) * input.jointWeights[connectedJ];
	}

	output.Pos = transformedPos;
	output.uv = input.uv;
	output.Norm = transformedNorm.xyz;

	return output;
}
