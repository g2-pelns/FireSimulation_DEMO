//Globals
cbuffer MatrixBuffer
{
	matrix	worldMatrix;
	matrix	viewMatrix;
	matrix	projectionMatrix;
};

cbuffer NoiseBuffer
{
	float frameTime;
	float3 scrollSpeeds;
	float3 scales;
	float padding;
};

//TypeDefines
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoords1 : TEXCOORD1;
	float2 texCoords2 : TEXCOORD2;
	float2 texCoords3 : TEXCOORD3;
};

//Vertex Shader
PixelInputType FireVertexShader(VertexInputType input)
{
	PixelInputType output;

	//Change the position vector to be 4 units for proper matrix caculations
	input.position.w = 1.0f;

	//Calculate the position of the vertex against the world, view and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Store thetexture coordinates for the pixel shader
	output.tex = input.tex;

	//Makes the noise textures move upwards at different speeds to create a fire effect
	output.texCoords1 = (input.tex * scales.x);
	output.texCoords1.y = output.texCoords1.y + (frameTime * scrollSpeeds.x);

	output.texCoords2 = (input.tex * scales.x);
	output.texCoords2.y = output.texCoords2.y + (frameTime * scrollSpeeds.x);

	output.texCoords3 = (input.tex * scales.x);
	output.texCoords3.y = output.texCoords3.y + (frameTime * scrollSpeeds.x);

	return output;
}