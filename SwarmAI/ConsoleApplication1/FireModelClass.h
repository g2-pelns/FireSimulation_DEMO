#ifndef _FIRE_MODEL_CLASS_H_
#define _FIRE_MODEL_CLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include <fstream>
#include <memory>
#include <vector>
#include <ctime>

#include "FireShaderClass.h"

using namespace std;
using namespace DirectX;

class FireModelClass
{
public:
	FireModelClass();
	FireModelClass(const FireModelClass&);
	~FireModelClass();

	void ShutDown();
	bool Init(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*, HWND);
	void Render(ID3D11DeviceContext*, ID3D11Device*);

	int GetIndexCount();
	int GetVertexCount();
	int GetInstanceCount();

	ID3D11ShaderResourceView* GetTexture();

	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();

private:
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData, instanceData;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_instanceCount = 4;

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float yu, tv;
		float nx, ny, nz;
	};

	//struct InstanceType
	//{
	//	XMFLOAT3 position;
	//};

	void ShutDownBuffers();
	bool InitBuffers(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadModel(char*);
	void ReleaseModel();

	bool LoadTexture(ID3D11Device*,HWND, WCHAR*);
	void ReleaseTexture();

	void updateInstPositions(ID3D11Device*);

	vector<unique_ptr<ModelType>> m_model;
	VertexType* instances;
	//FireShaderClass* m_fireShader;
	//ModelType* m_model;
	ID3D11Buffer* m_instanceBuffer;
	FireShaderClass* m_Texture;
};
#endif