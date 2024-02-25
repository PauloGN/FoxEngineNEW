cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

cbuffer BoneTransformBuffer : register(b1)
{
    matrix boneTransforms[256];
}

static matrix Identity =
{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
    if (length(weights) <= 0)
    {
        return Identity;
    }
    
    matrix transform = boneTransforms[indices[0]] * weights[0];
    transform += boneTransforms[indices[1]] * weights[1];
    transform += boneTransforms[indices[2]] * weights[2];
    transform += boneTransforms[indices[3]] * weights[3];
    return transform;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 texCoord : TEXCOORD;
    int4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 lightNDCPosition : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
    matrix toNDC = mul(boneTransform, wvp);
    
    output.position = mul(float4(input.position, 1.0f), toNDC);
    output.lightNDCPosition = mul(float4(input.position, 1.0f), toNDC);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float depth = 1.0f - input.lightNDCPosition.z / input.lightNDCPosition.w;
    return float4(depth, 1.0f, 1.0f, 1.0f);
}