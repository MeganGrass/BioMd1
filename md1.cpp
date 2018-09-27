
#include "stdafx.h"
#include FRAMEWORK_H

#include "md1.h"

// 
RESIDENT_EVIL_2_MODEL Resident_Evil_2_Model::Dummy(VOID) {

	RESIDENT_EVIL_2_MODEL Model;
	Model.nVertice = NULL;
	Model.nNormal = NULL;
	Model.nTriangle = NULL;
	Model.nQuadrangle = NULL;
	if (!Model.obj.empty()) {
		Model.obj.clear();
	}

	return Model;
}
// 
RESIDENT_EVIL_2_MODEL Resident_Evil_2_Model::Read(_iobuf * FILE, ULONG _Offset) {

	// Error
	if (!File->IsOpen(FILE)) { return Dummy(); }

	RESIDENT_EVIL_2_MODEL Model = Dummy();
	Tm2Header Header = { NULL };
	Tm2Index Index = { NULL };
	Tm2Vector Vector = { NULL };
	Tm2PrimitiveT PrimitiveT = { NULL };
	Tm2TextureT TextureT = { NULL };
	Tm2PrimitiveQ PrimitiveQ = { NULL };
	Tm2TextureQ TextureQ = { NULL };
	Tm2Triangle Triangle;
	Tm2Quadrangle Quadrangle;
	Tm2Object Object;
	ULONG pIndex = _Offset + sizeof(Tm2Header);
	ULONG pVertice = NULL;
	ULONG pNormal = NULL;
	ULONG pPrimitive = NULL;
	ULONG pTexture = NULL;

	// Read
	File->Read(FILE, _Offset, &Header, sizeof(Tm2Header));
	for (ULONG i = 0; i < (Header.nObject / 2); i++, pIndex += sizeof(Tm2Index)) {

		// Index
		File->Read(FILE, pIndex, &Index, sizeof(Tm2Index));
		Model.obj.push_back(Object);

		// Vertice
		Model.nVertice += Index.nVerticeT;
		pVertice = Index.pVerticeT + (_Offset + sizeof(Tm2Header));
		for (ULONG x = 0; x < Index.nVerticeT; x++, pVertice += sizeof(Tm2Vector)) {
			File->Read(FILE, pVertice, &Vector, sizeof(Tm2Vector));
			Model.obj[i].v.push_back(Vector);
		}

		// Normal
		Model.nNormal += Index.nNormalT;
		pNormal = Index.pNormalT + (_Offset + sizeof(Tm2Header));
		for (ULONG x = 0; x < Index.nNormalT; x++, pNormal += sizeof(Tm2Vector)) {
			File->Read(FILE, pNormal, &Vector, sizeof(Tm2Vector));
			Model.obj[i].n.push_back(Vector);
		}

		// Triangle
		Model.nTriangle += Index.nPrimitiveT;
		pPrimitive = Index.pPrimitiveT + (_Offset + sizeof(Tm2Header));
		pTexture = Index.pTextureT + (_Offset + sizeof(Tm2Header));
		for (ULONG x = 0; x < Index.nPrimitiveT; x++, pPrimitive += sizeof(Tm2PrimitiveT), pTexture += sizeof(Tm2TextureT)) {

			Model.obj[i].t.push_back(Triangle);

			// Primitive
			File->Read(FILE, pPrimitive, &PrimitiveT, sizeof(Tm2PrimitiveT));
			Model.obj[i].t[x].p.push_back(PrimitiveT);

			// Texture
			File->Read(FILE, pTexture, &TextureT, sizeof(Tm2TextureT));
			Model.obj[i].t[x].t.push_back(TextureT);

		}

		// Quadrangle
		Model.nQuadrangle += Index.nPrimitiveQ;
		pPrimitive = Index.pPrimitiveQ + (_Offset + sizeof(Tm2Header));
		pTexture = Index.pTextureQ + (_Offset + sizeof(Tm2Header));
		for (ULONG x = 0; x < Index.nPrimitiveQ; x++, pPrimitive += sizeof(Tm2PrimitiveQ), pTexture += sizeof(Tm2TextureQ)) {

			Model.obj[i].q.push_back(Quadrangle);

			// Primitive
			File->Read(FILE, pPrimitive, &PrimitiveQ, sizeof(Tm2PrimitiveQ));
			Model.obj[i].q[x].p.push_back(PrimitiveQ);

			// Texture
			File->Read(FILE, pTexture, &TextureQ, sizeof(Tm2TextureQ));
			Model.obj[i].q[x].t.push_back(TextureQ);

		}

	}

	// Terminate
	return Model;
}
BOOL Resident_Evil_2_Model::Write(RESIDENT_EVIL_2_MODEL Model, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!File->IsOpen(FILE)) { return FAIL; }

	// Buffer
	ULONG pIndex = _Offset + sizeof(Tm2Header);
	ULONG pVertice = pIndex + (sizeof(Tm2Index) * Model.obj.size());
	ULONG pNormal = pVertice + (sizeof(Tm2Vector) * Model.nVertice);
	ULONG pPrimitive = pNormal + (sizeof(Tm2Vector) * Model.nNormal);
	ULONG pTexture = pPrimitive + (sizeof(Tm2PrimitiveT) * Model.nTriangle) + (sizeof(Tm2PrimitiveQ) * Model.nQuadrangle);

	// Header
	Tm2Header Header = { NULL };
	Header.pTexture = (pTexture - _Offset);
	Header.padd = NULL;
	Header.nObject = (Model.obj.size() * 2);
	Tm2Index Index = { NULL };

	// Write
	File->Write(FILE, _Offset, &Header, sizeof(Tm2Header));
	for (size_t i = 0; i < Model.obj.size(); i++, pIndex += sizeof(Tm2Index)) {

		// Vertice
		Index.pVerticeT = Index.pVerticeQ = ((pVertice - sizeof(Tm2Header)) - _Offset);
		Index.nVerticeT = Index.nVerticeQ = Model.obj[i].v.size();
		File->Write(FILE, pVertice, &Model.obj[i].v[0], (Model.obj[i].v.size() * sizeof(Tm2Vector)));
		pVertice += (Model.obj[i].v.size() * sizeof(Tm2Vector));

		// Normal
		Index.pNormalT = Index.pNormalQ = ((pNormal - sizeof(Tm2Header)) - _Offset);
		Index.nNormalT = Index.nNormalQ = Model.obj[i].n.size();
		File->Write(FILE, pNormal, &Model.obj[i].n[0], (Model.obj[i].n.size() * sizeof(Tm2Vector)));
		pNormal += (Model.obj[i].n.size() * sizeof(Tm2Vector));

		// Triangle
		Index.pPrimitiveT = ((pPrimitive - sizeof(Tm2Header)) - _Offset);
		Index.pTextureT = ((pTexture - sizeof(Tm2Header)) - _Offset);
		Index.nPrimitiveT = Model.obj[i].t.size();
		for (size_t x = 0; x < Model.obj[i].t.size(); x++, pPrimitive += sizeof(Tm2PrimitiveT), pTexture += sizeof(Tm2TextureT)) {
			File->Write(FILE, pPrimitive, &Model.obj[i].t[x].p[0], sizeof(Tm2PrimitiveT));
			File->Write(FILE, pTexture, &Model.obj[i].t[x].t[0], sizeof(Tm2TextureT));
		}

		// Quadrangle
		Index.pPrimitiveQ = ((pPrimitive - sizeof(Tm2Header)) - _Offset);
		Index.pTextureQ = ((pTexture - sizeof(Tm2Header)) - _Offset);
		Index.nPrimitiveQ = Model.obj[i].q.size();
		for (size_t x = 0; x < Model.obj[i].q.size(); x++, pPrimitive += sizeof(Tm2PrimitiveQ), pTexture += sizeof(Tm2TextureQ)) {
			File->Write(FILE, pPrimitive, &Model.obj[i].q[x].p[0], sizeof(Tm2PrimitiveQ));
			File->Write(FILE, pTexture, &Model.obj[i].q[x].t[0], sizeof(Tm2TextureQ));
		}

		// Index
		File->Write(FILE, pIndex, &Index, sizeof(Tm2Index));

	}

	// Terminate
	return SUCCESS;
}
// 
RESIDENT_EVIL_2_MODEL Resident_Evil_2_Model::Open(CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	size_t _StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!File->Exists(FileName)) { return Dummy(); }

	// Open
	_iobuf * _File = File->Open(READ_FILE, FileName);
	if (!_File) { return Dummy(); }

	// Read
	RESIDENT_EVIL_2_MODEL Model = Read(_File, 0x00);

	// Close
	fclose(_File);

	// Terminate
	return Model;
}
BOOL Resident_Evil_2_Model::SaveAs(RESIDENT_EVIL_2_MODEL Model, CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	size_t _StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Open
	_iobuf * _File = File->Open(CREATE_FILE, FileName);
	if (!_File) { return FAIL; }

	// Write
	BOOL bRet = Write(Model, _File, NULL);

	// Close
	fclose(_File);

	// Terminate
	return bRet;
}
BOOL Resident_Evil_2_Model::SaveObjAs(RESIDENT_EVIL_2_MODEL Model, size_t Id, CONST CHAR * _FileName, ...) {

	// Error
	if ((Id + 1) > Model.obj.size()) { return FAIL; }

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	size_t _StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Open
	_iobuf * _File = File->Open(CREATE_FILE, FileName);
	if (!_File) { return FAIL; }

	// Index
	Tm2Index Index = { NULL };
	Index.nVerticeT = Index.nVerticeQ = Model.obj[Id].v.size();
	Index.nNormalT = Index.nNormalQ = Model.obj[Id].n.size();
	Index.nPrimitiveT = Model.obj[Id].t.size();
	Index.nPrimitiveQ = Model.obj[Id].q.size();
	Index.pVerticeT = Index.pVerticeQ = sizeof(Tm2Index);
	Index.pNormalT = Index.pNormalQ = (Index.pVerticeT + (sizeof(Tm2Vector) * Model.obj[Id].v.size()));
	Index.pPrimitiveT = Index.pNormalT + (sizeof(Tm2Vector) * Model.obj[Id].n.size());
	Index.pPrimitiveQ = Index.pPrimitiveT + (sizeof(Tm2PrimitiveT) * Model.obj[Id].t.size());
	Index.pTextureT = Index.pPrimitiveT + (sizeof(Tm2PrimitiveT) * Model.obj[Id].t.size()) + (sizeof(Tm2PrimitiveQ) * Model.obj[Id].q.size());
	Index.pTextureQ = Index.pTextureT + (sizeof(Tm2TextureT) * Model.obj[Id].t.size());
	File->Write(_File, sizeof(Tm2Header), &Index, sizeof(Tm2Index));

	// Header
	Tm2Header Header = { NULL };
	Header.pTexture = Index.pTextureT + sizeof(Tm2Header);
	Header.padd = NULL;
	Header.nObject = 2;
	File->Write(_File, NULL, &Header, sizeof(Tm2Header));

	// Vertice
	File->Write(_File, Index.pVerticeT + sizeof(Tm2Header), &Model.obj[Id].v[0], (Model.obj[Id].v.size() * sizeof(Tm2Vector)));

	// Normal
	File->Write(_File, Index.pNormalT + sizeof(Tm2Header), &Model.obj[Id].n[0], (Model.obj[Id].n.size() * sizeof(Tm2Vector)));

	// Primitive
	for (size_t x = 0; x < Model.obj[Id].t.size(); x++, Index.pPrimitiveT += sizeof(Tm2PrimitiveT)) {
		File->Write(_File, Index.pPrimitiveT + sizeof(Tm2Header), &Model.obj[Id].t[x].p[0], sizeof(Tm2PrimitiveT));
	}
	for (size_t x = 0; x < Model.obj[Id].q.size(); x++, Index.pPrimitiveQ += sizeof(Tm2PrimitiveQ)) {
		File->Write(_File, Index.pPrimitiveQ + sizeof(Tm2Header), &Model.obj[Id].q[x].p[0], sizeof(Tm2PrimitiveQ));
	}

	// Texture
	for (size_t x = 0; x < Model.obj[Id].t.size(); x++, Index.pTextureT += sizeof(Tm2TextureT)) {
		File->Write(_File, Index.pTextureT + sizeof(Tm2Header), &Model.obj[Id].t[x].t[0], sizeof(Tm2TextureT));
	}
	for (size_t x = 0; x < Model.obj[Id].q.size(); x++, Index.pTextureQ += sizeof(Tm2TextureQ)) {
		File->Write(_File, Index.pTextureQ + sizeof(Tm2Header), &Model.obj[Id].q[x].t[0], sizeof(Tm2TextureQ));
	}

	// Close
	fclose(_File);

	// Terminate
	return SUCCESS;
}
BOOL Resident_Evil_2_Model::SaveAllObjs(RESIDENT_EVIL_2_MODEL Model, CONST CHAR * NameBase, CONST CHAR * _Directory, ...) {

	// _Directory
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _Directory);
	size_t _StrLen = (_vscprintf(_Directory, _ArgList) + 2);
	CHAR * Directory = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(Directory, (_StrLen * 2));
	vsprintf_s(Directory, _StrLen, _Directory, _ArgList);
	__crt_va_end(_ArgList);

	File->CreateDir(Directory);

	if (!strlen(NameBase)) { NameBase = "obj"; }

	// Extract
	BOOL bRet = NULL;
	for (size_t Id = 0; Id < Model.obj.size(); Id++) {
		bRet = SaveObjAs(Model, Id, "%s\\%s%02d.md1", Directory, NameBase, Id);
	}

	// Terminate
	return bRet;
}