#ifndef _FIRESHADERCLASS_H_
#define _FIRESHADERCLASS_H_

//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;

class FireShaderClass
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	
	struct NoiseBufferType
	{
		float frameTime;
		DirectX::XMFLOAT3 scrollSpeeds;
		DirectX::XMFLOAT3 scales;
		float padding;
	};

	struct DistortionBufferType
	{
		DirectX::XMFLOAT2 distortion1;
		DirectX::XMFLOAT2 distortion2;
		DirectX::XMFLOAT2 distortion3;
		float distortionScale;
		float distortionBias;
	};

public:
	FireShaderClass();
	FireShaderClass(const FireShaderClass&);
	~FireShaderClass();

	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, float, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT2, DirectX::XMFLOAT2, DirectX::XMFLOAT2, float, float);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT2,
		DirectX::XMFLOAT2, DirectX::XMFLOAT2, float, float);
	void RenderShader(ID3D11DeviceContext*, int);
	
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11Buffer* m_noiseBuffer;
	ID3D11SamplerState* m_sampleState;

	ID3D11SamplerState* m_sampleState2;

	ID3D11Buffer* m_distortionBuffer;
};

#endif