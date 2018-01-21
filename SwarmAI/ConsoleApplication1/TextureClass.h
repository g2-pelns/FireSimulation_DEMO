#ifndef _TEXTURE_CLASS_H_
#define _TEXTURE_CLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	bool LoadTarga(char*, int&, int&);
	unsigned char* m_targaData;
	ID3D11ShaderResourceView* m_textureView;
	ID3D11Texture2D* m_texture;
};

#endif

