cbuffer constantBuffer
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float4 normal : NORMAL;
};

[maxvertexcount(3)]
void GS_main(triangle VS_OUT input[3], inout TriangleStream<GS_OUT> OutputStream)
{
	GS_OUT output = (GS_OUT)0;

	float3 vector1 = (input[0].Pos - input[1].Pos);
	float3 vector2 = (input[0].Pos - input[2].Pos);

	float4 crossProduct = float4(cross(vector1, vector2), 0);

	crossProduct = normalize(crossProduct);

	output.normal = crossProduct;

	output.normal = mul(output.normal, world);

	output.normal = normalize(output.normal);

	for (uint j = 0; j < 3; j++)
	{
		output.Pos = input[j].Pos + crossProduct;

		output.Pos = (mul(output.Pos, world));
		output.Pos = (mul(output.Pos, view));
		output.Pos = (mul(output.Pos, projection));

		OutputStream.Append(output);
	}
	OutputStream.RestartStrip();
}