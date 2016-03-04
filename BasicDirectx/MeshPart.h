#ifndef __MESHPARTH__
#define __MESHPARTH__

#include <d3d11.h>
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3  Pos;
	DirectX::XMFLOAT4  Color;
};

class CMeshPart
{
public:
	CMeshPart();
	virtual ~CMeshPart();

	void draw(ID3D11DeviceContext* pDeviceContext, ID3D11InputLayout* pInputLayout);

private:
	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	ULONG m_indexCount;
	ULONG m_startIndex;
	ULONG m_vertexOffset;

	DXGI_FORMAT m_indexFormat;

	D3D_PRIMITIVE_TOPOLOGY m_primitiveType;

};

#endif // __MESHPARTH__