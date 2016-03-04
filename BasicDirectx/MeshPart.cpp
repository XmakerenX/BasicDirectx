#include "MeshPart.h"


CMeshPart::CMeshPart()
{
}


CMeshPart::~CMeshPart()
{
}

void CMeshPart::draw(ID3D11DeviceContext* pDeviceContext, ID3D11InputLayout* pInputLayout)
{

	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// maybe change it to get as class variable
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_IB, m_indexFormat, 0);

	// Draw the primitive.
	pDeviceContext->IASetPrimitiveTopology(m_primitiveType);

	pDeviceContext->DrawIndexed(m_indexCount, m_startIndex, m_vertexOffset);
}