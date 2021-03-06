//Globals
cbuffer MatrixBuffer
{
	matrix	worldMatrix;
	matrix	viewMatrix;
	matrix	projectionMatrix;
};

//TypeDefines
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float3 instancePosition : INSTANCE;
};


struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

//Vertex Shader
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	//Change the position vector to be 4 units for proper matrix caculations
	input.position.w = 1.0f;

	// Update the position of the vertices based on the data for this particular instance.
	input.position.x += input.instancePosition.x;
	input.position.y += input.instancePosition.y;
	input.position.z += input.instancePosition.z;


	//Calculate the position of the vertex against the world, view and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Store the input color for the pixel shader to use
	output.color = input.color;

	return output;
}