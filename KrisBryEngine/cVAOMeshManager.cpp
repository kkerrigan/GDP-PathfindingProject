#include <gameMath.h>	

#include "Global.h"
#include "cVAOMeshManager.h"
#include "Ply/CPlyFile5nt.h"
#include "Ply/CStringHelper.h"

#include <fstream>
#include <string>
#include <iostream> 
#include <sstream>

cVAOMeshManager::cVAOMeshManager() {		// constructor

	this->m_fileLoader = cVAOMeshManager::PLY5N;
	return;
}

cVAOMeshManager::~cVAOMeshManager() {	// destructor

	// Will call ShutDown();
	return;
}

void cVAOMeshManager::SetBasePath(std::string basepath) {

	this->m_basePath = basepath;
	return;
}

cVAOMeshManager* cVAOMeshManager::getInstance() {
	static cVAOMeshManager instance;

	return &instance;
}

// Taken from https://stackoverflow.com/questions/3407012/c-rounding-up-to-the-nearest-multiple-of-a-number
unsigned int cVAOMeshManager::m_roundUp(unsigned int numToRound, unsigned int multiple) {

	if (multiple == 0) {
		return numToRound;
	}

	int remainder = numToRound % multiple;
	if (remainder == 0) {
		return numToRound;
	}

	return numToRound + multiple - remainder;
}

//
//void cVAOMeshManager::changeLoaderToOriginal(void)
//{
//	this->m_fileLoader = cVAOMeshManager::ORIGINAL;
//	return;
//}

void cVAOMeshManager::changeLoaderToPly5n(void) {

	this->m_fileLoader = cVAOMeshManager::PLY5N;
	return;
}

void cVAOMeshManager::changeLoaderToAssimp(void) {

	this->m_fileLoader = cVAOMeshManager::ASSIMP;
	return;
}


// Note: the shader program ID is needed to tie 
// the buffer to the vertex layout of the shader
bool cVAOMeshManager::LoadModelIntoVAO(sModelDrawInfo &drawInfo, unsigned int shaderProgramID) {

	// See if this model has already been loaded
	std::map< std::string /*model name*/, sModelDrawInfo >::iterator itModel = this->m_mapModel_to_VAOID.find(drawInfo.meshFileName);

	if (itModel != this->m_mapModel_to_VAOID.end()) {
		this->m_AppendTextToLastError("Model ", false);
		this->m_AppendTextToLastError(drawInfo.meshFileName, false);
		this->m_AppendTextToLastError(" was already loaded.");
		return false;
	}

	// Does the drawInfo object already have data in it? 
	// (like from assimp or whatever)
	if (drawInfo.numberOfVertices == 0) {

		// No, so we need to load it...
		// Load the model from the file
		std::cout << "About to load " << drawInfo.meshFileName << "..." << std::endl;
		std::cout.flush();
		if (!this->m_LoadModelFromFile(drawInfo)) {
			// Add some error code
			std::cout << "Didn't load " << drawInfo.meshFileName << " file." << std::endl;
			std::cout.flush();
			return false;
		}
	}//if ( drawInfo.numberOfVertices == 0 )


	// Load the model data into the GPU
	//LoadMeshIntoGPUBuffer
	if (!this->m_LoadMeshInfo_Into_VAO(drawInfo, shaderProgramID)) {
		// Add some error text
		return false;
	}

	// Store this mesh into the map
	this->m_mapModel_to_VAOID[drawInfo.meshFileName] = drawInfo;

	return true;
}

// Looks up draw info by name (meshFileName)			
bool cVAOMeshManager::FindDrawInfoByModelName(sModelDrawInfo &drawInfo) {
	std::map<std::string /*model name*/, sModelDrawInfo>::iterator itModel = this->m_mapModel_to_VAOID.find(drawInfo.meshFileName);

	if (itModel == this->m_mapModel_to_VAOID.end()) {
		// Didn't find model
		return false;
	}

	// Copy the draw info back to the caller.
	drawInfo = itModel->second;

	return true;
}

bool cVAOMeshManager::m_LoadModelFromFile_AssimpLoader(sModelDrawInfo &drawInfo) {

	assert(true);
	return false;
}


bool cVAOMeshManager::m_LoadModelFromFile(sModelDrawInfo &drawInfo) {
	switch (this->m_fileLoader) {
		//case cVAOMeshManager::ORIGINAL:
		//	return this->m_LoadModelFromFile_OriginalLoader( drawInfo );
		//	break;
	case cVAOMeshManager::PLY5N:
		return this->m_LoadModelFromFile_Ply5nLoader(drawInfo);
		break;
	case cVAOMeshManager::ASSIMP:
		return this->m_LoadModelFromFile_AssimpLoader(drawInfo);
		break;
	}

	return false;
}


bool cVAOMeshManager::m_LoadModelFromFile_Ply5nLoader(sModelDrawInfo &drawInfo) {

	std::string fileToLoadFullPath = drawInfo.meshFileName;
	if (this->m_basePath != "") {
		fileToLoadFullPath = this->m_basePath + "/" + drawInfo.meshFileName;
	}

	CPlyFile5nt plyLoader;
	std::wstring error;
	if (!plyLoader.OpenPLYFile2(CStringHelper::ASCIIToUnicodeQnD(fileToLoadFullPath), error)) {

		this->m_AppendTextToLastError("Didn't load the ", false);
		this->m_AppendTextToLastError(fileToLoadFullPath, false);
		this->m_AppendTextToLastError(" file.");
		this->m_AppendTextToLastError(CStringHelper::UnicodeToASCII_QnD(error));
		return false;
	}

	// Create an internal mesh object
	drawInfo.pMeshData = new cMesh();


	drawInfo.numberOfVertices = plyLoader.GetNumberOfVerticies();
	drawInfo.pMeshData->numberOfVertices = plyLoader.GetNumberOfVerticies();
	//	std::cout << "vertices: " << drawInfo.numberOfVertices << std::endl;

	drawInfo.numberOfTriangles = plyLoader.GetNumberOfElements();
	drawInfo.pMeshData->numberOfTriangles = plyLoader.GetNumberOfElements();

	drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
	drawInfo.pMeshData->numberOfIndices = drawInfo.numberOfIndices;

	//	std::cout << "triangles: " << drawInfo.numberOfTriangles << std::endl;


		// Allocate the arrays, and padd it with a little but (round up to 64)
	drawInfo.pMeshData->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[this->m_roundUp(drawInfo.numberOfVertices, 64)];
	drawInfo.pMeshData->pTriangles = new cTriangle[this->m_roundUp(drawInfo.numberOfTriangles, 64)];
	drawInfo.pMeshData->pIndices = new unsigned int[this->m_roundUp(drawInfo.numberOfIndices, 64)];


	//memset( drawInfo.pVerticesFromFile, 0, sizeof( sPlyVertex ) * drawInfo.numberOfVertices );

	// Read the vertex data into the array
	PlyVertex curVert;
	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++) {

		curVert = plyLoader.getVertex_at(index);

		drawInfo.pMeshData->pVertices[index].x = curVert.xyz.x;
		drawInfo.pMeshData->pVertices[index].y = curVert.xyz.y;
		drawInfo.pMeshData->pVertices[index].z = curVert.xyz.z;

		drawInfo.pMeshData->pVertices[index].nx = curVert.nx;
		drawInfo.pMeshData->pVertices[index].ny = curVert.ny;
		drawInfo.pMeshData->pVertices[index].nz = curVert.nz;

		//  Also load the UV values
		drawInfo.pMeshData->pVertices[index].u0 = curVert.tex0u;
		drawInfo.pMeshData->pVertices[index].v0 = curVert.tex0v;
		drawInfo.pMeshData->pVertices[index].u1 = 0.0f;
		drawInfo.pMeshData->pVertices[index].v1 = 0.0f;

		drawInfo.pMeshData->pVertices[index].r = curVert.red;
		drawInfo.pMeshData->pVertices[index].g = curVert.green;
		drawInfo.pMeshData->pVertices[index].b = curVert.blue;
		drawInfo.pMeshData->pVertices[index].a = curVert.alpha;

		drawInfo.pMeshData->pVertices[index].bx = 0.0f;
		drawInfo.pMeshData->pVertices[index].by = 0.0f;
		drawInfo.pMeshData->pVertices[index].bz = 0.0f;

		drawInfo.pMeshData->pVertices[index].tx = 0.0f;
		drawInfo.pMeshData->pVertices[index].ty = 0.0f;
		drawInfo.pMeshData->pVertices[index].tz = 0.0f;

		for (unsigned int boneIndex = 0; boneIndex != NUMBEROFBONES; boneIndex++) {

			drawInfo.pMeshData->pVertices[index].boneID[boneIndex] = 0;
			drawInfo.pMeshData->pVertices[index].boneWeights[boneIndex] = 0.0f;
		}

	}//for ( unsigned int index...

//	memset( drawInfo.pTriangles, 0, sizeof( sPlyTriangle ) * drawInfo.numberOfTriangles );

	PlyElement curElement;
	unsigned int index = 0;
	unsigned int triIndex = 0;
	for (; triIndex != drawInfo.numberOfTriangles; index += 3, triIndex++) {

		curElement = plyLoader.getElement_at(triIndex);

		drawInfo.pMeshData->pTriangles[triIndex].vertex_ID_0 = curElement.vertex_index_1;
		drawInfo.pMeshData->pTriangles[triIndex].vertex_ID_1 = curElement.vertex_index_2;
		drawInfo.pMeshData->pTriangles[triIndex].vertex_ID_2 = curElement.vertex_index_3;

		// Unpack the index information, too
		drawInfo.pMeshData->pIndices[index + 0] = curElement.vertex_index_1;
		drawInfo.pMeshData->pIndices[index + 1] = curElement.vertex_index_2;
		drawInfo.pMeshData->pIndices[index + 2] = curElement.vertex_index_3;

	}//for ( unsigned int index...

	plyLoader.calcualteExtents();

	// Assume the 1st one is the largest and smallest:
	drawInfo.minX = plyLoader.getMinX();		drawInfo.pMeshData->minXYZ.x = plyLoader.getMinX();
	drawInfo.minY = plyLoader.getMinY();		drawInfo.pMeshData->minXYZ.y = plyLoader.getMinY();
	drawInfo.minZ = plyLoader.getMinZ();		drawInfo.pMeshData->minXYZ.z = plyLoader.getMinZ();

	drawInfo.maxX = plyLoader.getMaxX();		drawInfo.pMeshData->maxXYZ.x = plyLoader.getMaxX();
	drawInfo.maxY = plyLoader.getMaxY();		drawInfo.pMeshData->maxXYZ.y = plyLoader.getMaxY();
	drawInfo.maxZ = plyLoader.getMaxZ();		drawInfo.pMeshData->maxXYZ.z = plyLoader.getMaxZ();

	drawInfo.extentX = drawInfo.maxX - drawInfo.minX;		drawInfo.pMeshData->maxExtentXYZ.x = drawInfo.extentX;
	drawInfo.extentY = drawInfo.maxY - drawInfo.minY;		drawInfo.pMeshData->maxExtentXYZ.y = drawInfo.extentY;
	drawInfo.extentZ = drawInfo.maxZ - drawInfo.minZ;		drawInfo.pMeshData->maxExtentXYZ.z = drawInfo.extentZ;

	drawInfo.maxExtent = plyLoader.getMaxExtent();		drawInfo.pMeshData->maxExtent = drawInfo.maxExtent;

	return true;
}

bool cVAOMeshManager::m_LoadMeshInfo_Into_VAO(sModelDrawInfo &drawInfo, unsigned int shaderProgramID) {

	// See if we've been passed a cMesh object...
	if (drawInfo.pMeshData == NULL) {

		this->m_AppendTextToLastError("cMesh object is missing (NULL). Can't load mesh into GPU buffers.");
		return false;
	}

	if (drawInfo.pMeshData->numberOfVertices == 0) {

		this->m_AppendTextToLastError("There are no vertices in the cMesh object. Can't load anything into GPU.");
		return false;
	}

	if (drawInfo.pMeshData->pVertices == NULL) {

		this->m_AppendTextToLastError("cMesh object is NULL. Nothing to load into GPU.");
		return false;
	}

	// Creates a VAO (Vertex Array Object)
	glGenVertexArrays(1, &(drawInfo.VAO_ID));
	glBindVertexArray(drawInfo.VAO_ID);

	// Just to confuse you, this is called VBO (Vertex Buffer Object)
	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &(drawInfo.vertexBufferID));			// vertex_buffer
	// "Vertex" buffer
	// - Sets the "type" of buffer
	// - Makes it the 'current' buffer
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	unsigned int vertexBufferSizeInBytes =
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * drawInfo.numberOfVertices;

	if (drawInfo.bVertexBufferIsDynamic) {

		// CAN update the vertex information (later)
		glBufferData(GL_ARRAY_BUFFER,
			vertexBufferSizeInBytes,		// sizeof(vertices), 
			drawInfo.pMeshData->pVertices,			// vertices, 
			GL_DYNAMIC_DRAW);
	}
	else {

		// DON'T INDEND to update the vertex buffer info (faster)
		glBufferData(GL_ARRAY_BUFFER,
			vertexBufferSizeInBytes,	// sizeof(vertices), 
			drawInfo.pMeshData->pVertices,			// vertices, 
			GL_STATIC_DRAW);	//*************
	}

	// Create the index buffer...
	glGenBuffers(1, &(drawInfo.indexBufferID));			// vertex_buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.indexBufferID);

	unsigned int indexBufferSizeInBytes = sizeof(unsigned int) * drawInfo.numberOfIndices;


	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferSizeInBytes,	// sizeof(vertices), 
		drawInfo.pMeshData->pIndices,			// vertices, 
		GL_STATIC_DRAW);

	// Set the vertex layout
	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");
	GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");
	GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUV_x2");
	
	// Add the remaining vertex attributes (binormal, tangent, bone ID and weight)
	GLint vpos_vTanXYZ_Location = glGetAttribLocation(shaderProgramID, "vTanXYZ");
	GLint vpos_vBiNormXYZ_Location = glGetAttribLocation(shaderProgramID, "vBiNormXYZ");
	GLint vpos_vBoneID_Location = glGetAttribLocation(shaderProgramID, "vBoneID");
	GLint vpos_vBoneWeight_Location = glGetAttribLocation(shaderProgramID, "vBoneWeight");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location,	// "vPosition"
		4,				//  vec3 (xyz)
		GL_FLOAT,
		GL_FALSE,		// DON'T "normalize"
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, x));

	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location,		// cColour
		4,					// vec3 (rgb)
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, r));

	// Now add the normal vertex attribute
	glEnableVertexAttribArray(vnorm_location);
	glVertexAttribPointer(vnorm_location,		// 
		4,					// vec3 (xyz)
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	//   9  size in bytes
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, nx));

	// Now add the normal vertex attribute
	glEnableVertexAttribArray(vUVx2_location);
	glVertexAttribPointer(vUVx2_location,		// 
		4,					// vec4 vUV_x2;
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	//   float u0, v0, u1, v1;
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, u0));

	// Add the other 4 things we now have in the Skinned Mesh vertex layout
	//glEnableVertexAttribArray(vpos_vTanXYZ_Location);
	//glVertexAttribPointer(vpos_vTanXYZ_Location  ,		// vTanXYZ
				//		   4,					// vec4 
				//		   GL_FLOAT, 
				//		   GL_FALSE,
	//                       sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
				//		   (void*) offsetof( sVertex_xyz_rgba_n_uv2_bt_4Bones, tx ) );

	//glEnableVertexAttribArray(vpos_vBiNormXYZ_Location );
	//glVertexAttribPointer(vpos_vBiNormXYZ_Location   ,		// vBiNormXYZ
				//		   4,								// vec4 
				//		   GL_FLOAT, 
				//		   GL_FALSE,
	//                       sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
				//		   (void*) offsetof( sVertex_xyz_rgba_n_uv2_bt_4Bones, bx ) );

	glEnableVertexAttribArray(vpos_vBoneID_Location);
	glVertexAttribPointer(vpos_vBoneID_Location,		// vBoneID
		4,								// vec4 
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, boneID[0]));

	glEnableVertexAttribArray(vpos_vBoneWeight_Location);
	glVertexAttribPointer(vpos_vBoneWeight_Location,		// vBoneID
		4,								// vec4 
		GL_FLOAT,
		GL_FALSE,
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones),	// size in bytes //sizeof(float) * 9,		// was 6 
		(void*)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, boneWeights[0]));


	// Get rid of everything we don't need.
	// Set the "current" VAO to nothing.
	glBindVertexArray(0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vnorm_location);
	glDisableVertexAttribArray(vUVx2_location);

	// The added skinned mesh elements of the vertex class
	glDisableVertexAttribArray(vpos_vTanXYZ_Location);
	glDisableVertexAttribArray(vpos_vBiNormXYZ_Location);
	glDisableVertexAttribArray(vpos_vBoneID_Location);
	glDisableVertexAttribArray(vpos_vBoneWeight_Location);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	return true;
}

void cVAOMeshManager::m_AppendTextToLastError(std::string text, bool addNewLineBefore /*=true*/) {

	std::stringstream ssError;
	ssError.str() = this->m_lastErrorString;

	if (addNewLineBefore) {
		ssError << std::endl;
	}
	ssError << text;

	this->m_lastErrorString = ssError.str();

	return;
}

std::string cVAOMeshManager::GetLastError(bool bAndClear /*=true*/) {

	std::string errorText = this->m_lastErrorString;
	if (bAndClear) {
		this->m_lastErrorString = "";
	}

	return errorText;
}

void cVAOMeshManager::ShutDown(void) {

	// TODO: Clear the map

	return;
}

void cVAOMeshManager::UpdateModelVertexInformation(sModelDrawInfo &drawInfo) {

	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	unsigned int vertexBufferSizeInBytes =
		sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * drawInfo.numberOfVertices;


	// CAN update the vertex information (later)
	glBufferData(GL_ARRAY_BUFFER,
		vertexBufferSizeInBytes,		// sizeof(vertices), 
		drawInfo.pMeshData->pVertices,			// vertices, 
		GL_DYNAMIC_DRAW);	//*************


	glBindBuffer(GL_ARRAY_BUFFER, 0);	// "un-bind" the buffer

	return;
}