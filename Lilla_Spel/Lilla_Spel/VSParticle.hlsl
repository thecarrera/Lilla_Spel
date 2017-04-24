cbuffer MatrixBuffer
{
	matrix worldM;
	matrix viewM;
	matrix projM;
};

struct VS_IN
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_IN main(VS_IN input)
{
	VS_OUT output;

	//position vectoren ska vara 4 lång
	input.position.w = 1.0f;

	output.position = mul(mul(mul(input.position, worldM), viewM), projM);

	output.color = input.color;

	return output;
}