#ifndef _PARTICLE_SYSTEM_CLASS_H_
#define _PARTICLE_SYSTEM_CLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include <fstream>
#include <memory>
#include <vector>
#include <ctime>

#include "TextureClass.h"

using namespace std;
class ParticleSystemClass
{
public:
	ParticleSystemClass();
	ParticleSystemClass(const ParticleSystemClass&);
	~ParticleSystemClass();

	void Shutdown();
	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Render(ID3D11DeviceContext*, ID3D11Device*);

	bool Frame(float, ID3D11Device*, ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();

	int GetIndexCount();
	int GetVertexCount();
	int GetInstanceCount();

private:
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData, instanceData;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_instanceCount = 4;

	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_currentParticleCount;
	float m_accumulatedTime;
	int m_maxParticles;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 instancePosition;
	};

	struct ParticleType
	{
		float X, Y, Z;
		float red, green, blue;
		float velocity;
		bool active;
	};

	bool LoadModel(char*);
	void ReleaseModel();

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool InitParticleSystem();
	void ShutdownParticleSystem();

	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11Device*, ID3D11DeviceContext*);
	void RenderBuffers(ID3D11DeviceContext*);

	TextureClass* m_Texture;
	vector<unique_ptr<ParticleType>> particleModels;
	VertexType* instances;
	ID3D11Buffer* m_instanceBuffer;
	//ParticleType* m_particleList;
	//VertexType* m_vertices;
};

#endif