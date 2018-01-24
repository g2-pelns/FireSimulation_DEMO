#include "ParticleSystemClass.h"

ParticleSystemClass::ParticleSystemClass()
{
	m_Texture = 0;
	//m_particleList = 0;
	//m_vertices = 0;


	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_instanceBuffer = 0;
}

ParticleSystemClass::ParticleSystemClass(const ParticleSystemClass& other)
{
}

ParticleSystemClass::~ParticleSystemClass()
{
}

void ParticleSystemClass::Shutdown()
{
	// Release the buffers.
	ShutdownBuffers();
	// Release the particle system.
	ShutdownParticleSystem();

	ReleaseModel();
	// Release the texture used for the particles.
	ReleaseTexture();
	return;
}

bool ParticleSystemClass::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that is used for the particles.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}
	
	//// Initialize the particle system.
	//result = InitParticleSystem();
	//if (!result)
	//{
	//	return false;
	//}

	// Create the buffers that will be used to render the particles with.
	result = InitBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ParticleSystemClass::Frame(float frameTime,ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;


	// Release old particles.
	//KillParticles();

	//// Emit new particles.
	//EmitParticles(frameTime);

	////// Update the position of the particles.
	//UpdateParticles(frameTime);

	////// Update the dynamic vertex buffer with the new position of each particle.
	//result = UpdateBuffers(device,deviceContext);
	//if (!result)
	//{
	//	return false;
	//}

	return true;
}

void ParticleSystemClass::Render(ID3D11DeviceContext* deviceContext, ID3D11Device* device)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	//updateInstPositions(device);
	return;
}

ID3D11ShaderResourceView* ParticleSystemClass::GetTexture()
{
	return m_Texture->GetTexture();
}

int ParticleSystemClass::GetIndexCount()
{
	return m_indexCount;
}

int ParticleSystemClass::GetVertexCount()
{
	return m_vertexCount;
}

int ParticleSystemClass::GetInstanceCount()
{
	return m_instanceCount;
}

bool ParticleSystemClass::LoadModel(char* fileName)
{
	ifstream fIn;
	char input;

	//Open the model file
	fIn.open(fileName);

	//if cound not open the file then exit
	if (fIn.fail())
	{
		return false;
	}

	fIn.get(input);
	//Read up to the value of vertex count
	//fin.get(input);
	while (input != ':')
	{
		fIn.get(input);
	}

	//Read in the vertex count
	fIn >> m_vertexCount;
	//Set the number of indices to be the same as the vertex count

	m_indexCount = m_vertexCount;

	particleModels.reserve(m_vertexCount);

	fIn.get(input);
	while (input != ':')
	{
		fIn.get(input);
	}
	fIn.get(input);
	fIn.get(input);

	//Create the model using the vertex count that was read in
	/*m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
	return false;
	}*/

	//Read up to the beginning of the data
	/*fin.get(input);
	while (input != ':')
	{
	fin.get(input);
	}*/
	//fin.get(input);
	//fin.get(input);

	//Read the vertex data
	for (auto i = 0; i < m_vertexCount; i++)
	{
		particleModels.push_back(make_unique<ParticleType>());
		particleModels.back()->Z += 30;
		fIn >> particleModels[i]->X >> particleModels[i]->Y >> particleModels[i]->Z;
	}

	//Close the model file
	fIn.close();
	return true;
}

bool ParticleSystemClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Init(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ParticleSystemClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ParticleSystemClass::InitParticleSystem()
{
	int i;
	// Set the random deviation of where the particles can be located when emitted.
	m_particleDeviationX = 0.5f;
	m_particleDeviationY = 0.1f;
	m_particleDeviationZ = 2.0f;

	// Set the speed and speed variation of particles.
	m_particleVelocity = 1.0f;
	m_particleVelocityVariation = 0.2f;

	// Set the physical size of the particles.
	m_particleSize = 0.2f;

	// Set the number of particles to emit per second.
	m_particlesPerSecond = 1.0f;

	// Set the maximum number of particles allowed in the particle system.
	m_maxParticles = particleModels.size();

	// Create the particle list.
	/*m_particle = new ParticleType[m_maxParticles];
	if (!m_particleList)
	{
		return false;
	}*/

	// Initialize the particle list.
	for (i = 0; i<m_maxParticles; i++)
	{
		particleModels[i]->active = false;
	}

	// Initialize the current particle count to zero since none are emitted yet.
	m_currentParticleCount = 0;

	// Clear the initial accumulated time for the particle per second emission rate.
	m_accumulatedTime = 0.0f;

	return true;
}

void ParticleSystemClass::ShutdownParticleSystem()
{
	// Release the particle list.
	particleModels.clear();
	return;
}

bool ParticleSystemClass::InitBuffers(ID3D11Device* device)
{
	int i = 0;
	HRESULT result;
	VertexType* vertices;
	//InstanceType* instances;
	unsigned long* indices;

	// Set the maximum number of vertices in the vertex array.
	m_vertexCount += 36;

	// Set the maximum number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array for the particles that will be rendered.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//// Initialize vertex array to zeros at first.
	//memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));
	std::srand(static_cast <unsigned> (time(0)));

	// Initialize the index array.
	for (i = 36; i < m_vertexCount; i++)
	{
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		vertices[i - 36].position = { particleModels[i - 36]->X, particleModels[i - 36]->Y, particleModels[i - 36]->Z };
		vertices[i - 36].color = { g, r, b, 1.0f };

		indices[i - 36] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	m_instanceCount = 10000;

	instances = new VertexType[m_instanceCount];

	if (!instances)
	{
		return false;
	}

	int x = -50, y = 0, z = 5;
	int iterator = 0;
	int ziter = 0;
	for (int i = 0; i < m_instanceCount; i++)
	{
		instances[i].position = DirectX::XMFLOAT3(x, y, z);

		x += 4;
		iterator++;
		if (iterator == 10)
		{
			iterator = 0;
			ziter++;
			y -= 4;
			x = -50;
			if (ziter == 10)
			{
				z += 4;
				y = 0;
				ziter = 0;
			}
		}
	}

	//Set up the description of the instance buffer
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(VertexType)* m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	//Give the subrespurce structure a pointer to the instace data
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	//Create the instance buffer
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	
	delete[] indices;
	indices = 0;

	return true;
}

void ParticleSystemClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ParticleSystemClass::EmitParticles(float frameTime)
{
	bool emitParticle, found;
	float positionX, positionY, positionZ, velocity, red, green, blue;
	int index, i, j;


	// Increment the frame time.
	m_accumulatedTime += 0.5f;//frameTime;

	// Set emit particle to false for now.
	emitParticle = false;

	// Check if it is time to emit a new particle or not.
	if (m_accumulatedTime > (1000.0f / m_particlesPerSecond))
	{
		m_accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// If there are particles to emit then emit one per frame.
	if ((emitParticle == true) && (m_currentParticleCount < (m_maxParticles - 1)))
	{
		m_currentParticleCount++;

		// Now generate the randomized particle properties.
		positionX = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationX;
		positionY = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationY;
		positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationZ;

		//velocity = m_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * m_particleVelocityVariation;

		red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

		// Now since the particles need to be rendered from back to front for blending we have to sort the particle array.
		// We will sort using Z depth so we need to find where in the list the particle should be inserted.
		index = 0;
		/*found = false;
		while (!found)
		{
			if ((m_particle[index]->active == false) || (m_instances[index].position.z < positionZ))
			{
				found = true;
			}
			else
			{
				index++;
			}
		}*/

		// Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particle.
		i = 0;
		j = i - 1;

		while (i != m_currentParticleCount)
		{
			//instances[i].position = DirectX::XMFLOAT3(particleModels[i]->X, particleModels[i]->Y, particleModels[i]->Z);;
			//m_instances[i].color = DirectX::XMFLOAT4(m_particle[i]->red, m_particle[i]->green, m_particle[i]->blue, 1.0f);
			
			//particleModels[i]->active = true;
			//m_particle[i]->velocity = m_particle[j]->velocity;
			//m_particle[i]->active = m_particle[j]->active;
			//i++;
			//j++;
		}

		// Now insert it into the particle array in the correct depth order.
		//m_instances[index].position = DirectX::XMFLOAT3(positionX, positionY, positionZ);
		//m_instances[index].color = DirectX::XMFLOAT4(red,green,blue, 1.0f);
		
		//m_particle[index]->velocity = velocity;
		//particleModels[index]->active = true;

		//m_vertices[index].position = DirectX::XMFLOAT3(positionX, positionY, positionZ);
		//m_vertices[index].color = DirectX::XMFLOAT4(red, green, blue, 0);
	}

	return;
}

void ParticleSystemClass::UpdateParticles(float frameTime)
{
	int i;


	// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
	for (i = 0; i<m_instanceCount; i++)
	{
		instances[i].position = DirectX::XMFLOAT3(instances[i].position.x, instances[i].position.y, instances[i].position.z - (0.5f * frameTime * 1.0f)); ;
		//m_vertices[i].position.z = m_vertices[i].position.z - (m_particleList[i].velocity * frameTime * 1.0f);
	}

	return;
}

void ParticleSystemClass::KillParticles()
{
	int i, j;


	//// Kill all the particles that have gone below a certain height range.
	//for (i = 0; i<m_maxParticles; i++)
	//{
	//	if ((m_particleList[i].active == true) && (m_particleList[i].posY < -30.0f))
	//	{
	//		m_particleList[i].active = false;
	//		m_currentParticleCount--;

	//		// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly.
	//		for (j = i; j<m_maxParticles - 1; j++)
	//		{
	//			m_particleList[j].posX = m_particleList[j + 1].posX;
	//			m_particleList[j].posY = m_particleList[j + 1].posY;
	//			m_particleList[j].posZ = m_particleList[j + 1].posZ;
	//			m_particleList[j].red = m_particleList[j + 1].red;
	//			m_particleList[j].green = m_particleList[j + 1].green;
	//			m_particleList[j].blue = m_particleList[j + 1].blue;
	//			m_particleList[j].velocity = m_particleList[j + 1].velocity;
	//			m_particleList[j].active = m_particleList[j + 1].active;
	//		}
	//	}
	//}

	return;
}

bool ParticleSystemClass::UpdateBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	int index, i;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Initialize vertex array to zeros at first.
	//memset(m_instances, 0, (sizeof(VertexType) * m_vertexCount));

	// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles.
	index = 0;

	for(i=0; i<m_currentParticleCount; i++)
	{
		// Bottom left.
		instances[i].position = DirectX::XMFLOAT3(particleModels[i]->X, particleModels[i]->Y, particleModels[i]->Z);
		//m_instances[i].texture = DirectX::XMFLOAT2(0.0f, 1.0f);
		//m_instances[i].color = DirectX::XMFLOAT4(m_particle[i]->red, m_particle[i]->green, m_particle[i]->blue, 1.0f);
		index++;
	}

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	// Lock the vertex buffer.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)instances, (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);
	
	
	
	return true;
}

void ParticleSystemClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set vertex buffer stride and offset.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(VertexType);

	//Set the buffer offsets
	offsets[0] = 0;
	offsets[1] = 0;

	//Set he array of ointers to the vertex and instance buffers
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void ParticleSystemClass::ReleaseModel()
{
	particleModels.clear();
	return;
}