////////////////////////////////////////////////////////////////////////////////
// Filename: motionblur.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer PrevMatrixBuffer
{
	matrix prevWorldMatrix;
	matrix prevViewMatrix;
	matrix prevProjectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float2 velocity : VELOCITY;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType MotionBlurVertexShader(VertexInputType input)
{
    PixelInputType output;
	float4 prevWSP;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    prevWSP = mul(input.position, prevViewMatrix);
	prevWSP = mul(prevWSP, prevProjectionMatrix);

    output.velocity = (output.position - prevWSP) / 2.f;
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	return output;
}