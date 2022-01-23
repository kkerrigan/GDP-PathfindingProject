#include <cVAOMeshManager.h>
#include <cShaderManager.h>
#include <iostream>

void loadModels(){
	cVAOMeshManager* pMeshManager = cVAOMeshManager::getInstance();
	pMeshManager->SetBasePath("./assets/models/");
	int shaderProgramId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "flat.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
		{
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
		{
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
		{
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "rect1_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
		{
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
		{
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "bun_res3_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
		{
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	// Instance drawing models
	//shaderProgramId = cShaderManager::getInstance()->getIDFromFriendlyName("InstanceShader");

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "cube_flat_shaded_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
		{
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	return;
}