cbuffer constantBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float size : SIZE;
    float4 color : COLOR;
    int whatTriangle : WHATTRIANGLE;
};

struct GS_OUT
{
    float4 Pos : SV_POSITION;
    float4 colorPS : COLOR;
};

static const float4 triangleCorners[48] = //12
{
    //TETRAHED V1
    //VÄNSTER
    float4(-0.02f, 0.0f, 0.02f, 0.0f),
    float4(0.0f, 0.02f, 0.0f, 0.0f),
    float4(0.0f, 0.0f, -0.02f, 0.0f),

    //BAKOM
    float4(-0.02f, 0.0f, 0.02f, 0.0f),
    float4(0.0f, 0.02f, 0.0f, 0.0f),
    float4(0.02f, 0.0f, 0.02f, 0.0f),

    //HÖGER
    float4(0.0f, 0.0f, -0.02f, 0.0f),
    float4(0.0f, 0.02f, 0.0f, 0.0f),
    float4(0.02f, 0.0f, 0.02f, 0.0f),

    //UNDER
    float4(-0.02f, 0.0f, 0.02f, 0.0f),
    float4(0.02f, 0.0f, 0.02f, 0.0f),
    float4(0.0f, 0.0f, -0.02f, 0.0f),



    //TETRAHED V2
     float4(-0.02f, 0.0f, 0.02f, 0.0f),
    float4(0.0f, -0.02f, 0.0f, 0.0f),
    float4(0.0f, 0.0f, -0.02f, 0.0f),

    //BAKOM
    float4(-0.02f, 0.0f, 0.02f, 0.0f),
    float4(0.0f, -0.02f, 0.0f, 0.0f),
    float4(0.02f, 0.0f, 0.02f, 0.0f),

    //HÖGER
    float4(0.0f, 0.0f, -0.02f, 0.0f),
    float4(0.0f, -0.02f, 0.0f, 0.0f),
    float4(0.02f, 0.0f, 0.02f, 0.0f),

    //UNDER
    float4(-0.02f, 0.0f, 0.02f, 0.0f),
    float4(0.02f, 0.0f, 0.02f, 0.0f),
    float4(0.0f, 0.0f, -0.02f, 0.0f),



    //TETRAHED V3
    float4(-0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.0f, 0.0f, 0.02f, 0.0f),

    //BAKOM
    float4(0.01f, -0.02f, 0.0f, 0.0f),
    float4(0.0f, 0.0f, 0.02f, 0.0f),
    float4(0.02f, 0.01f, 0.0f, 0.0f),

    //HÖGER
    float4(-0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.0f, 0.0f, 0.02f, 0.0f),
    float4(0.01f, -0.02f, 0.0f, 0.0f),

    //UNDER
    float4(-0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.01f, -0.02f, 0.0f, 0.0f),



    //TETRAHED V4
    float4(-0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.0f, 0.0f, 0.02f, 0.0f),

    //BAKOM
    float4(-0.01f, -0.02f, 0.0f, 0.0f),
    float4(0.0f, 0.0f, 0.02f, 0.0f),
    float4(0.02f, 0.01f, 0.0f, 0.0f),

    //HÖGER
    float4(-0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.0f, 0.0f, 0.02f, 0.0f),
    float4(-0.01f, -0.02f, 0.0f, 0.0f),

    //UNDER
    float4(-0.02f, 0.01f, 0.0f, 0.0f),
    float4(0.02f, 0.01f, 0.0f, 0.0f),
    float4(-0.01f, -0.02f, 0.0f, 0.0f),
};

[maxvertexcount(48)]
void main(point VS_OUT input[1], inout TriangleStream<GS_OUT> OutputStream)
{
    GS_OUT output = (GS_OUT) 0;

    float4 pos = mul(input[0].position, world);

    for (int i = input[0].whatTriangle; i < input[0].whatTriangle + 12; i++)
    {
        output.Pos = pos + ((triangleCorners[i] * input[0].size));
        output.colorPS = input[0].color;
        
        output.Pos = mul(output.Pos, view);
        output.Pos = mul(output.Pos, projection);
        OutputStream.Append(output);
    }
}






//Graveyard
//float PI = 3.141592653589793238;

//float4 pos = mul(input[0].position, world);
//    pos = mul(pos, view);
//    pos = mul(pos, projection);

//float angle = input[0].angleOut.x * (PI / 180); // upp
//float x = cos(angle);
//float y = sin(angle);
//float4 directionVector = { x, y, 1, 1 };

//float angle2 = input[0].angleOut.y * (PI / 180); //vänster
//float x2 = cos(angle2);
//float y2 = sin(angle2);
//float4 directionVector2 = { x2, y2, 1, 1 };

//float angle3 = input[0].angleOut.z * (PI / 180); //höger
//float x3 = cos(angle3);
//float y3 = sin(angle3);
//float4 directionVector3 = { x3, y3, 1, 1 };