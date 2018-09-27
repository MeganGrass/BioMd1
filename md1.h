#pragma once

#pragma pack(push, 1)

typedef struct tagTm2Header {
	ULONG pTexture;		// 0x00	// Absolute pointer to UV mapping data chunk
	ULONG padd;			// 0x04	// Zero (0)
	ULONG nObject;		// 0x08	// Object Count (relative; absolute = nObject / 2)
} Tm2Header;
typedef struct tagTm2Index {

	ULONG pVerticeT;	// 0x00	// Vertice Pointer (relative)
	ULONG nVerticeT;	// 0x04	// Vertice Count
	ULONG pNormalT;		// 0x08	// Normal Pointer (relative)
	ULONG nNormalT;		// 0x0C	// Normal Count
	ULONG pPrimitiveT;	// 0x10	// Primitive Pointer (relative)
	ULONG nPrimitiveT;	// 0x14	// Primitive Count
	ULONG pTextureT;	// 0x18	// Texture Pointer (relative)

	ULONG pVerticeQ;	// 0x1C	// Vertice Pointer (relative)
	ULONG nVerticeQ;	// 0x20	// Vertice Count
	ULONG pNormalQ;		// 0x24	// Normal Pointer (relative)
	ULONG nNormalQ;		// 0x28	// Normal Count
	ULONG pPrimitiveQ;	// 0x2C	// Primitive Pointer (relative)
	ULONG nPrimitiveQ;	// 0x30	// Primitive Count
	ULONG pTextureQ;	// 0x34	// Texture Pointer (relative)

} Tm2Index;

typedef struct tagTm2Vector {
	SHORT x;
	SHORT y;
	SHORT z;
	USHORT padd;
} Tm2Vector;

typedef struct tagTm2PrimitiveT {
	SHORT n0;
	SHORT v0;
	SHORT n1;
	SHORT v1;
	SHORT n2;
	SHORT v2;
} Tm2PrimitiveT;
typedef struct tagTm2TextureT {
	UCHAR u0;
	UCHAR v0;
	USHORT Clut;
	UCHAR u1;
	UCHAR v1;
	USHORT Page;
	UCHAR u2;
	UCHAR v2;
	USHORT padd0;
} Tm2TextureT;

typedef struct tagTm2PrimitiveQ {
	SHORT n0;
	SHORT v0;
	SHORT n1;
	SHORT v1;
	SHORT n2;
	SHORT v2;
	SHORT n3;
	SHORT v3;
} Tm2PrimitiveQ;
typedef struct tagTm2TextureQ {
	UCHAR u0;
	UCHAR v0;
	USHORT Clut;
	UCHAR u1;
	UCHAR v1;
	USHORT Page;
	UCHAR u2;
	UCHAR v2;
	USHORT padd0;
	UCHAR u3;
	UCHAR v3;
	USHORT padd1;
} Tm2TextureQ;

typedef struct tagTm2Triangle {
	std::vector <Tm2PrimitiveT> p;	// Primitive
	std::vector <Tm2TextureT> t;	// Texture
} Tm2Triangle;
typedef struct tagTm2Quadrangle {
	std::vector <Tm2PrimitiveQ> p;	// Primitive
	std::vector <Tm2TextureQ> t;	// Texture
} Tm2Quadrangle;
typedef struct tagTm2Object {
	std::vector <Tm2Vector> v;		// Vertice
	std::vector <Tm2Vector> n;		// Normal
	std::vector <Tm2Triangle> t;	// Triangle
	std::vector <Tm2Quadrangle> q;	// Quadrangle
} Tm2Object;

typedef struct tagRESIDENT_EVIL_2_MODEL {
	std::vector <Tm2Object> obj;	// Object
	ULONG nVertice;					// Vertice Total Count
	ULONG nNormal;					// Normal Total Count
	ULONG nTriangle;				// Triangle Total Count
	ULONG nQuadrangle;				// Quadrangle Total Count
} RESIDENT_EVIL_2_MODEL;

#pragma pack(pop)

class Resident_Evil_2_Model {
private:
public:

	System_File * File;

	Resident_Evil_2_Model(VOID) {}
	virtual ~Resident_Evil_2_Model(VOID) {}

	RESIDENT_EVIL_2_MODEL Dummy(VOID);

	RESIDENT_EVIL_2_MODEL Read(_iobuf * FILE, ULONG _Offset);
	BOOL Write(RESIDENT_EVIL_2_MODEL Model, _iobuf * FILE, ULONG _Offset);

	RESIDENT_EVIL_2_MODEL Open(CONST CHAR * _FileName, ...);
	BOOL SaveAs(RESIDENT_EVIL_2_MODEL Model, CONST CHAR * _FileName, ...);

	BOOL SaveObjAs(RESIDENT_EVIL_2_MODEL Model, size_t Id, CONST CHAR * _FileName, ...);
	BOOL SaveAllObjs(RESIDENT_EVIL_2_MODEL Model, CONST CHAR * NameBase, CONST CHAR * _Directory, ...);


};
