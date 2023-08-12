cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    float params[3];
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texCoord;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 0.0f;
    
    if (mode == 0)//None
    {
         finalColor = textureMap0.Sample(textureSampler, input.texcoord);
    }
    else if (mode == 1)//Monochrome
    {
        
    }
    else if (mode == 2)//Invert
    {
        
    }
    else if (mode == 3)//Mirror
    {
        
    }
    else if (mode == 4)//Blur
    {
        
    }
    else if (mode == 5)//Combine2
    {
        
    }
    else if (mode == 6)//MotionBlur
    {
        
    }
    
    return finalColor;
}