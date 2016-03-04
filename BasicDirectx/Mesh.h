#ifndef __MESHH__
#define __MESHH__

class CMesh
{
public:
	CMesh();
	virtual ~CMesh();

private:
	ID3D11Buffer* mBoxVB;
};

#endif // __MESHH__