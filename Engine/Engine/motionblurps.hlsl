////////////////////////////////////////////////////////////////////////////////
// Filename: motionblur.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture[2];
SamplerState SampleType;


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float2 velocity : VELOCITY;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MotionBlurPixelShader(PixelInputType input) : SV_TARGET
{
    /*int numSamples = 3;
    float2 texCoord = input.tex;

	float4 color = shaderTexture[1].Sample(SampleType, texCoord);
    texCoord += input.velocity;

    for (int i = 0; i < numSamples; i++, texCoord += input.velocity)
    {
        float4 newColor = shaderTexture[1].Sample(SampleType, texCoord);
		color += newColor;
    }
	
    return color / numSamples;*/
    float2 texCoord = input.tex;
    return shaderTexture[1].Sample(SampleType, texCoord);

}