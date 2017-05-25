//#pragma once
//
//#include "InputFileReader.hpp"
//#include "OBJLoader.hpp"
//
//using namespace InputFileReader;
//using namespace OBJLoader;
//
//
//inline Vertex** CreateTriangleData(ID3D11Device* gDevice, ID3D11Buffer**& gVertexBufferArray, 
//	int *&vertexCountOBJ, int &gVertexBuffer2_size, float *objCoords)
//{
//	Vertex** vtx;
//	int objCount = OL_getOBJCount();
//
//	string r = IFR_read();
//	if (r.find("bth") != string::npos) {
//		objCount = 1;
//		vtx = new Vertex*[1];
//		vtx[0] = new Vertex[6];
//		vertexCountOBJ = new int[objCount];
//		vertexCountOBJ[0] = 6;
//		gVertexBuffer2_size = 1;
//		vtx[0][0] = Vertex{ -1, 1, 0, 0, 0 };
//		vtx[0][1] = Vertex{ 1, -1, 0, 1, 1 };
//		vtx[0][2] = Vertex{ -1, -1, 0, 0, 1 };
//		vtx[0][3] = Vertex{ -1, 1, 0, 0, 0 };
//		vtx[0][4] = Vertex{ 1, 1, 0, 1, 0 };
//		vtx[0][5] = Vertex{ 1, -1, 0, 1, 1 };
//		//TSC_GetTangentSpace(vtx, variables->vertexCountOBJ, objCount);
//	}
//	else if (r.find("obj") != string::npos) {
//		vtx = new Vertex*[objCount];
//		vertexCountOBJ = new int[objCount];
//		gVertexBuffer2_size = objCount;
//		OL_loadVertex(vtx, vertexCountOBJ, gVertexBuffer2_size, objCoords); // Fetch information from OBJ files.
//		//TSC_GetTangentSpace(vtx, variables->vertexCountOBJ, objCount);
//	}
//
//	gVertexBufferArray = new ID3D11Buffer*[objCount];
//
//	for (int i = 0; i < objCount; i++) {
//		D3D11_BUFFER_DESC bufferDesc;
//		memset(&bufferDesc, 0, sizeof(bufferDesc));
//		bufferDesc.ByteWidth = sizeof(Vertex) * vertexCountOBJ[i];
//		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//
//		D3D11_SUBRESOURCE_DATA data;
//		data.pSysMem = &vtx[i][0];
//		gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferArray[i]);
//	}
//
//	return vtx;
//}
