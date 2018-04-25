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
    matrix viewProjection : MATRIXA;
    matrix viewProjectionInverse : MATRIXB;
    matrix prevViewProjection : MATRIXC;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MotionBlurPixelShader(PixelInputType input) : SV_TARGET
{
    float2 texCoord = input.tex;
    int numSamples = 12;

    // Get the depth buffer value at this pixel.
    float zOverW = shaderTexture[1].Sample(SampleType, texCoord).r;
    
    // H is the viewport position at this pixel in the range -1 to 1.
    float4 H = float4(texCoord.x * 2 - 1, (1 - texCoord.y) * 2 - 1, zOverW, 1);
    
    // Transform by the view-projection inverse.
    float4 D = mul(H, input.viewProjectionInverse);
    
    // Divide by w to get the world position.
    float4 worldPos = D / D.w;

    // Current viewport position
    float4 currentPos = H;
    
    // Use the world position, and transform by the previous view-projection matrix.
    float4 previousPos = mul(worldPos, input.prevViewProjection);

    // Convert to nonhomogeneous points [-1,1] by dividing by w.
    previousPos /= previousPos.w;

    // Use this frame's position and last frame's to compute the pixel velocity.
    float2 velocity = (currentPos - previousPos) / 2.f;

    velocity = velocity * 5;

    // Get the initial color at this pixel.
    float4 color = shaderTexture[0].Sample(SampleType, texCoord);
    texCoord += velocity;

    for (int i = 0; i < numSamples; i++, texCoord += velocity)
    {
    
        // Sample the color buffer along the velocity vector.
        float4 currentColor = shaderTexture[0].Sample(SampleType, texCoord);
        // Add the current color to our color sum.
        color += currentColor;
    }

    // Average all of the samples to get the final blur color.
    float4 finalColor = color / numSamples;

    return finalColor;
    //return shaderTexture[1].Sample(SampleType, texCoord);
}