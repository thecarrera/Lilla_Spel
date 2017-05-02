cbuffer CBUFFER : register(b0)
{
	float4x4 worldM;
	float4x4 viewM;
	float4x4 projM;
}

struct GS_IN
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;
};

struct FS_OUT
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 wPos : POSITION;
};

[maxvertexcount(6)]
void GS_main(triangle GS_IN input[3], inout TriangleStream <FS_OUT> OutputStream)
{
	float3 n = normalize(mul(cross(input[1].Pos - input[0].Pos, input[2].Pos - input[0].Pos), worldM));

	FS_OUT output = (FS_OUT)0;

	for (int i = 0; i < 3; i++)
	{
		output.Pos = mul(mul(mul(input[i].Pos, worldM), viewM), projM);
		output.wPos = mul(input[i].Pos, worldM);
		output.uv = input[i].uv;
		output.normal = input[i].Norm;

		OutputStream.Append(output);
	}
}