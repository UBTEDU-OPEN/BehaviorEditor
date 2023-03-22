/****************************************************************************************

Copyright (C) 2015 Autodesk, Inc.
All rights reserved.

Use of this software is subject to the terms of the Autodesk license agreement
provided at the time of installation or download, or which otherwise accompanies
this software in either electronic or hard copy form.

****************************************************************************************/

#include "SceneContext.h"

#include "SceneCache.h"
#include "SetCamera.h"
#include "DrawScene.h"
#include "DrawText.h"
#include "targa.h"
#include "Common/Common.h"
#include <GetPosition.h>
#include <DrawScene.h>
#include <cmath>
#include<iostream>
#include<string>
using namespace std;

namespace
{
// Default file of ViewScene example
const char * SAMPLE_FILENAME = "humanoid.fbx";

// Button and action definition
const int LEFT_BUTTON = 0;
const int MIDDLE_BUTTON = 1;
const int RIGHT_BUTTON = 2;

const int BUTTON_DOWN = 0;
const int BUTTON_UP = 1;

// Find all the cameras under this node recursively.
void FillCameraArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pCameraArray)
{
    if (pNode)
    {
        if (pNode->GetNodeAttribute())
        {
            if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eCamera)
            {
                pCameraArray.Add(pNode);
            }
        }

        const int lCount = pNode->GetChildCount();
        for (int i = 0; i < lCount; i++)
        {
            FillCameraArrayRecursive(pNode->GetChild(i), pCameraArray);
        }
    }
}

// Find all the cameras in this scene.
void FillCameraArray(FbxScene* pScene, FbxArray<FbxNode*>& pCameraArray)
{
    pCameraArray.Clear();

    FillCameraArrayRecursive(pScene->GetRootNode(), pCameraArray);
}

// Find all poses in this scene.
void FillPoseArray(FbxScene* pScene, FbxArray<FbxPose*>& pPoseArray)
{
    const int lPoseCount = pScene->GetPoseCount();

    for (int i=0; i < lPoseCount; ++i)
    {
        pPoseArray.Add(pScene->GetPose(i));
    }
}

void PreparePointCacheData(FbxScene* pScene, FbxTime &pCache_Start, FbxTime &pCache_Stop)
{
    // This function show how to cycle through scene elements in a linear way.
    const int lNodeCount = pScene->GetSrcObjectCount<FbxNode>();
    FbxStatus lStatus;

    for (int lIndex=0; lIndex<lNodeCount; lIndex++)
    {
        FbxNode* lNode = pScene->GetSrcObject<FbxNode>(lIndex);

        if (lNode->GetGeometry())
        {
            int i, lVertexCacheDeformerCount = lNode->GetGeometry()->GetDeformerCount(FbxDeformer::eVertexCache);

            // There should be a maximum of 1 Vertex Cache Deformer for the moment
            lVertexCacheDeformerCount = lVertexCacheDeformerCount > 0 ? 1 : 0;

            for (i=0; i<lVertexCacheDeformerCount; ++i )
            {
                // Get the Point Cache object
                FbxVertexCacheDeformer* lDeformer = static_cast<FbxVertexCacheDeformer*>(lNode->GetGeometry()->GetDeformer(i, FbxDeformer::eVertexCache));
                if( !lDeformer ) continue;
                FbxCache* lCache = lDeformer->GetCache();
                if( !lCache ) continue;

                // Process the point cache data only if the constraint is active
                if (lDeformer->Active.Get())
                {
                    if (lCache->GetCacheFileFormat() == FbxCache::eMaxPointCacheV2)
                    {
                        // This code show how to convert from PC2 to MC point cache format
                        // turn it on if you need it.
#if 0 
                        if (!lCache->ConvertFromPC2ToMC(FbxCache::eMCOneFile,
                                                        FbxTime::GetFrameRate(pScene->GetGlobalTimeSettings().GetTimeMode())))
                        {
                            // Conversion failed, retrieve the error here
                            FbxString lTheErrorIs = lCache->GetStaus().GetErrorString();
                        }
#endif
                    }
                    else if (lCache->GetCacheFileFormat() == FbxCache::eMayaCache)
                    {
                        // This code show how to convert from MC to PC2 point cache format
                        // turn it on if you need it.
                        //#if 0
                        if (!lCache->ConvertFromMCToPC2(FbxTime::GetFrameRate(pScene->GetGlobalSettings().GetTimeMode()), 0, &lStatus))
                        {
                            // Conversion failed, retrieve the error here
                            FbxString lTheErrorIs = lStatus.GetErrorString();
                        }
                        //#endif
                    }


                    // Now open the cache file to read from it
                    if (!lCache->OpenFileForRead(&lStatus))
                    {
                        // Cannot open file
                        FbxString lTheErrorIs = lStatus.GetErrorString();

                        // Set the deformer inactive so we don't play it back
                        lDeformer->Active = false;
                    }
                    else
                    {
                        // get the start and stop time of the cache
                        FbxTime lChannel_Start;
                        FbxTime lChannel_Stop;
                        int lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.Get());
                        if(lCache->GetAnimationRange(lChannelIndex, lChannel_Start, lChannel_Stop))
                        {
                            // get the smallest start time
                            if(lChannel_Start < pCache_Start) pCache_Start = lChannel_Start;

                            // get the biggest stop time
                            if(lChannel_Stop  > pCache_Stop)  pCache_Stop  = lChannel_Stop;
                        }
                    }
                }
            }
        }
    }
}

// Load a texture file (TGA only now) into GPU and return the texture object name
bool LoadTextureFromFile(const FbxString & pFilePath, unsigned int & pTextureObject)
{
    if (pFilePath.Right(3).Upper() == "TGA")
    {
        tga_image lTGAImage;

        if (tga_read(&lTGAImage, pFilePath.Buffer()) == TGA_NOERR)
        {
            // Make sure the image is left to right
            if (tga_is_right_to_left(&lTGAImage))
                tga_flip_horiz(&lTGAImage);

            // Make sure the image is bottom to top
            if (tga_is_top_to_bottom(&lTGAImage))
                tga_flip_vert(&lTGAImage);

            // Make the image BGR 24
            tga_convert_depth(&lTGAImage, 24);

            // Transfer the texture date into GPU
            glGenTextures(1, &pTextureObject);
            glBindTexture(GL_TEXTURE_2D, pTextureObject);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, lTGAImage.width, lTGAImage.height, 0, GL_BGR,
                         GL_UNSIGNED_BYTE, lTGAImage.image_data);
            glBindTexture(GL_TEXTURE_2D, 0);

            tga_free_buffers(&lTGAImage);

            return true;
        }
    }

    return false;
}

// Bake node attributes and materials under this node recursively.
// Currently only mesh, light and material.
void LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer, bool pSupportVBO)
{
    // Bake material and hook as user data.
    const int lMaterialCount = pNode->GetMaterialCount();
    for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
    {
        FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
        if (lMaterial && !lMaterial->GetUserDataPtr())
        {
            FbxAutoPtr<MaterialCache> lMaterialCache(new MaterialCache);
            if (lMaterialCache->Initialize(lMaterial))
            {
                lMaterial->SetUserDataPtr(lMaterialCache.Release());
            }
        }
    }

    FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
    if (lNodeAttribute)
    {
        // Bake mesh as VBO(vertex buffer object) into GPU.
        if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh * lMesh = pNode->GetMesh();
            if (pSupportVBO && lMesh && !lMesh->GetUserDataPtr())
            {
                FbxAutoPtr<VBOMesh> lMeshCache(new VBOMesh);
                if (lMeshCache->Initialize(lMesh))
                {
                    lMesh->SetUserDataPtr(lMeshCache.Release());
                }
            }
        }
        // Bake light properties.
        else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
        {
            FbxLight * lLight = pNode->GetLight();
            if (lLight && !lLight->GetUserDataPtr())
            {
                FbxAutoPtr<LightCache> lLightCache(new LightCache);
                if (lLightCache->Initialize(lLight, pAnimLayer))
                {
                    lLight->SetUserDataPtr(lLightCache.Release());
                }
            }
        }
    }

    const int lChildCount = pNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
    {
        LoadCacheRecursive(pNode->GetChild(lChildIndex), pAnimLayer, pSupportVBO);
    }
}

// Unload the cache and release the memory under this node recursively.
void UnloadCacheRecursive(FbxNode * pNode)
{
    // Unload the material cache
    const int lMaterialCount = pNode->GetMaterialCount();
    for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
    {
        FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
        if (lMaterial && lMaterial->GetUserDataPtr())
        {
            MaterialCache * lMaterialCache = static_cast<MaterialCache *>(lMaterial->GetUserDataPtr());
            lMaterial->SetUserDataPtr(NULL);
            delete lMaterialCache;
        }
    }

    FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
    if (lNodeAttribute)
    {
        // Unload the mesh cache
        if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh * lMesh = pNode->GetMesh();
            if (lMesh && lMesh->GetUserDataPtr())
            {
                VBOMesh * lMeshCache = static_cast<VBOMesh *>(lMesh->GetUserDataPtr());
                lMesh->SetUserDataPtr(NULL);
                delete lMeshCache;
            }
        }
        // Unload the light cache
        else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
        {
            FbxLight * lLight = pNode->GetLight();
            if (lLight && lLight->GetUserDataPtr())
            {
                LightCache * lLightCache = static_cast<LightCache *>(lLight->GetUserDataPtr());
                lLight->SetUserDataPtr(NULL);
                delete lLightCache;
            }
        }
    }

    const int lChildCount = pNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
    {
        UnloadCacheRecursive(pNode->GetChild(lChildIndex));
    }
}

// Bake node attributes and materials for this scene and load the textures.
void LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const char * pFbxFileName, bool pSupportVBO)
{
    // Load the textures into GPU, only for file texture now
    const int lTextureCount = pScene->GetTextureCount();
    for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
    {
        FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
        FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
        if (lFileTexture && !lFileTexture->GetUserDataPtr())
        {
            // Try to load the texture from absolute path
            const FbxString lFileName = lFileTexture->GetFileName();

            // Only TGA textures are supported now.
            if (lFileName.Right(3).Upper() != "TGA")
            {
                FBXSDK_printf("Only TGA textures are supported now: %s\n", lFileName.Buffer());
                continue;
            }

            GLuint lTextureObject = 0;
            bool lStatus = LoadTextureFromFile(lFileName, lTextureObject);

            const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName);
            const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);
            if (!lStatus)
            {
                // Load texture from relative file name (relative to FBX file)
                const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
                lStatus = LoadTextureFromFile(lResolvedFileName, lTextureObject);
            }

            if (!lStatus)
            {
                // Load texture from file name only (relative to FBX file)
                const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
                const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
                lStatus = LoadTextureFromFile(lResolvedFileName, lTextureObject);
            }

            if (!lStatus)
            {
                FBXSDK_printf("Failed to load texture file: %s\n", lFileName.Buffer());
                continue;
            }

            if (lStatus)
            {
                GLuint * lTextureName = new GLuint(lTextureObject);
                lFileTexture->SetUserDataPtr(lTextureName);
            }
        }
    }

    LoadCacheRecursive(pScene->GetRootNode(), pAnimLayer, pSupportVBO);
}

// Unload the cache and release the memory fro this scene and release the textures in GPU
void UnloadCacheRecursive(FbxScene * pScene)
{
    const int lTextureCount = pScene->GetTextureCount();
    for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
    {
        FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
        FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
        if (lFileTexture && lFileTexture->GetUserDataPtr())
        {
            GLuint * lTextureName = static_cast<GLuint *>(lFileTexture->GetUserDataPtr());
            lFileTexture->SetUserDataPtr(NULL);
            glDeleteTextures(1, lTextureName);
            delete lTextureName;
        }
    }

    UnloadCacheRecursive(pScene->GetRootNode());
}
}

bool InitializeOpenGL()
{
    // Initialize GLEW.
    GLenum lError = glewInit();
    if (lError != GLEW_OK)
    {
        FBXSDK_printf("GLEW Error: %s\n", glewGetErrorString(lError));
        return false;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(static_cast<GLclampf>(37/255.0),
                 static_cast<GLclampf>(42/255.0),
                 static_cast<GLclampf>(53/255.0),
                 1.0f);

    // OpenGL 1.5 at least.
    if (!GLEW_VERSION_1_5)
    {
        FBXSDK_printf("The OpenGL version should be at least 1.5 to display shaded scene!\n");
        return false;
    }

    return true;
}

SceneContext::SceneContext(const char * pFileName, const char* textureName, int pWindowWidth, int pWindowHeight, bool pSupportVBO)
    : mFileName(pFileName), mStatus(UNLOADED), mServoAngleChangeCallBack(nullptr), mGetAxisCallBack(nullptr), mGetNodeRotationRangeCallBack(nullptr),
      mSdkManager(NULL), mScene(NULL), mImporter(NULL), mCurrentAnimLayer(NULL), mSelectedNode(NULL), mHoverNode(NULL),mAxisNode(NULL),
      mPoseIndex(-1), mCameraStatus(CAMERA_NOTHING), mPause(false), mShadingMode(SHADING_MODE_SHADED),
      mSupportVBO(pSupportVBO), mCameraZoomMode(ZOOM_FOCAL_LENGTH),
      mWindowWidth(pWindowWidth), mWindowHeight(pWindowHeight), mDrawText(new DrawText(textureName))
{
    if (mFileName == NULL)
        mFileName = SAMPLE_FILENAME;

    // initialize cache start and stop time
    mCache_Start = FBXSDK_TIME_INFINITE;
    mCache_Stop  = FBXSDK_TIME_MINUS_INFINITE;

    // Create the FBX SDK manager which is the object allocator for almost
    // all the classes in the SDK and create the scene.
    InitializeSdkObjects(mSdkManager, mScene);

    if (mSdkManager)
    {
        // Create the importer.
        int lFileFormat = -1;
        mImporter = FbxImporter::Create(mSdkManager,"");
        if (!mSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(mFileName, lFileFormat) )
        {
            // Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
            lFileFormat = mSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
        }

        // Initialize the importer by providing a filename.
        if(mImporter->Initialize(mFileName, lFileFormat) == true)
        {
            // The file is going to be imported at
            // the end of the first display callback.
            mWindowMessage = "Importing file ";
            mWindowMessage += mFileName;
            mWindowMessage += "\nPlease wait!";

            // Set scene status flag to ready to load.
            mStatus = MUST_BE_LOADED;
        }
        else
        {
            mWindowMessage = "Unable to open file ";
            mWindowMessage += mFileName;
            mWindowMessage += "\nError reported: ";
            mWindowMessage += mImporter->GetStatus().GetErrorString();
            mWindowMessage += "\nEsc to exit";
        }
    }
    else
    {
        mWindowMessage = "Unable to create the FBX SDK manager";
        mWindowMessage += "\nEsc to exit";
    }
}

SceneContext::~SceneContext()
{
    FbxArrayDelete(mAnimStackNameArray);

    delete mDrawText;

    // Unload the cache and free the memory
    if (mScene)
    {
        UnloadCacheRecursive(mScene);
    }

    // Delete the FBX SDK manager. All the objects that have been allocated
    // using the FBX SDK manager and that haven't been explicitly destroyed
    // are automatically destroyed at the same time.
    DestroySdkObjects(mSdkManager, true);
}

bool SceneContext::LoadFile()
{
    bool lResult = false;
    // Make sure that the scene is ready to load.
    if (mStatus == MUST_BE_LOADED)
    {
        if (mImporter->Import(mScene) == true)
        {
            // Check the scene integrity!
            FbxStatus status;
            FbxArray< FbxString*> details;
            FbxSceneCheckUtility sceneCheck(FbxCast<FbxScene>(mScene), &status, &details);
            lResult = sceneCheck.Validate(FbxSceneCheckUtility::eCkeckData);
            if (lResult == false)
            {
                if (details.GetCount())
                {
                    mStatus = UNLOADED;
                    FBXSDK_printf("Scene integrity verification failed with the following errors:\n");

                    for (int i = 0; i < details.GetCount(); i++)
                        FBXSDK_printf("   %s\n", details[i]->Buffer());

                    FbxArrayDelete<FbxString*>(details);
                }
            }

            if (lResult)
            {
                // Set the scene status flag to refresh
                // the scene in the first timer callback.
                mStatus = MUST_BE_REFRESHED;

                // Convert Axis System to what is used in this example, if needed
                FbxAxisSystem SceneAxisSystem = mScene->GetGlobalSettings().GetAxisSystem();
                FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
                if (SceneAxisSystem != OurAxisSystem)
                {
                    OurAxisSystem.ConvertScene(mScene);
                }

                // Convert Unit System to what is used in this example, if needed
                FbxSystemUnit SceneSystemUnit = mScene->GetGlobalSettings().GetSystemUnit();
                if (SceneSystemUnit.GetScaleFactor() != 1.0)
                {
                    //The unit in this example is centimeter.
                    FbxSystemUnit::cm.ConvertScene(mScene);
                }

                // Get the list of all the animation stack.
                mScene->FillAnimStackNameArray(mAnimStackNameArray);

                // Get the list of all the cameras in the scene.
                FillCameraArray(mScene, mCameraArray);

                // Convert mesh, NURBS and patch into triangle mesh
                FbxGeometryConverter lGeomConverter(mSdkManager);
                // pLegacy 默认为false, 加载速度慢，修改为true.
                lGeomConverter.Triangulate(mScene, true, true);

                // Bake the scene for one frame
                LoadCacheRecursive(mScene, mCurrentAnimLayer, mFileName, mSupportVBO);

                // Convert any .PC2 point cache data into the .MC format for
                // vertex cache deformer playback.
                PreparePointCacheData(mScene, mCache_Start, mCache_Stop);

                // Get the list of pose in the scene
                FillPoseArray(mScene, mPoseArray);

                // Initialize the window message.
                mWindowMessage = "File ";
                mWindowMessage += mFileName;
                mWindowMessage += "\nClick on the right mouse button to enter menu.";
                mWindowMessage += "\nEsc to exit.";

                // Initialize the frame period.
                mFrameTime.SetTime(0, 0, 0, 1, 0, mScene->GetGlobalSettings().GetTimeMode());

                // Print the keyboard shortcuts.
                FBXSDK_printf("Play/Pause Animation: Space Bar.\n");
                FBXSDK_printf("Camera Rotate: Left Mouse Button.\n");
                FBXSDK_printf("Camera Pan: Left Mouse Button + Middle Mouse Button.\n");
                FBXSDK_printf("Camera Zoom: Middle Mouse Button.\n");

                // 隐藏坐标轴节点
                setAxisNodeVisiable (false);
            }
        }
        else
        {
            // Import failed, set the scene status flag accordingly.
            mStatus = UNLOADED;

            mWindowMessage = "Unable to import file ";
            mWindowMessage += mFileName;
            mWindowMessage += "\nError reported: ";
            mWindowMessage += mImporter->GetStatus().GetErrorString();
        }

        // Destroy the importer to release the file.
        mImporter->Destroy();
        mImporter = NULL;
    }

    return lResult;
}

bool SceneContext::SetCurrentAnimStack(int pIndex)
{
    const int lAnimStackCount = mAnimStackNameArray.GetCount();
    if (!lAnimStackCount || pIndex >= lAnimStackCount)
    {
        return false;
    }

    // select the base layer from the animation stack
    FbxAnimStack * lCurrentAnimationStack = mScene->FindMember<FbxAnimStack>(mAnimStackNameArray[pIndex]->Buffer());
    if (lCurrentAnimationStack == NULL)
    {
        // this is a problem. The anim stack should be found in the scene!
        return false;
    }

    // we assume that the first animation layer connected to the animation stack is the base layer
    // (this is the assumption made in the FBXSDK)
    mCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
    mScene->SetCurrentAnimationStack(lCurrentAnimationStack);

    FbxTakeInfo* lCurrentTakeInfo = mScene->GetTakeInfo(*(mAnimStackNameArray[pIndex]));
    if (lCurrentTakeInfo)
    {
        mStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
        mStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
    }
    else
    {
        // Take the time line value
        FbxTimeSpan lTimeLineTimeSpan;
        mScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

        mStart = lTimeLineTimeSpan.GetStart();
        mStop  = lTimeLineTimeSpan.GetStop();
    }

    // check for smallest start with cache start
    if(mCache_Start < mStart)
        mStart = mCache_Start;

    // check for biggest stop with cache stop
    if(mCache_Stop  > mStop)
        mStop  = mCache_Stop;

    // move to beginning
    mCurrentTime = mStart;

    // Set the scene status flag to refresh
    // the scene in the next timer callback.
    mStatus = MUST_BE_REFRESHED;

    return true;
}

bool SceneContext::SetCurrentCamera(const char * pCameraName)
{
    if (!pCameraName)
    {
        return false;
    }

    FbxGlobalSettings& lGlobalCameraSettings = mScene->GetGlobalSettings();
    lGlobalCameraSettings.SetDefaultCamera(pCameraName);
    mStatus = MUST_BE_REFRESHED;
    return true;
}

bool SceneContext::SetCurrentPoseIndex(int pPoseIndex)
{
    mPoseIndex = pPoseIndex;
    mStatus = MUST_BE_REFRESHED;
    return true;
}

void SceneContext::OnTimerClick() const
{
    // Loop in the animation stack if not paused.
    if (mStop > mStart && !mPause)
    {
        // Set the scene status flag to refresh
        // the scene in the next timer callback.
        //mStatus = MUST_BE_REFRESHED;

        mCurrentTime += mFrameTime;

        if (mCurrentTime > mStop)
        {
            mCurrentTime = mStart;
        }
    }
    // Avoid displaying the same frame on
    // and on if the animation stack has no length.
    else
    {
        // Set the scene status flag to avoid refreshing
        // the scene in the next timer callback.
        //mStatus = REFRESHED;
    }

    mStatus = REFRESHED;
}

void SceneContext::OnMouseWheel(int dir, int speed /*= 10*/)
{
    CameraZoom(mScene, speed * dir, mCameraZoomMode);
    mStatus = MUST_BE_REFRESHED;
}

// Redraw the scene
bool SceneContext::OnDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Test if the scene has been loaded yet.
    if (mStatus != UNLOADED && mStatus != MUST_BE_LOADED)
    {
        glPushAttrib(GL_ENABLE_BIT);
        glPushAttrib(GL_LIGHTING_BIT);
        glEnable(GL_DEPTH_TEST);

        // Draw the front face only, except for the texts and lights.
        //glEnable(GL_CULL_FACE);

        // Set the view to the current camera settings.
        SetCamera(mScene, mCurrentTime, mCurrentAnimLayer, mCameraArray,
                  mWindowWidth, mWindowHeight);

        FbxPose * lPose = NULL;
        if (mPoseIndex != -1)
        {
            lPose = mScene->GetPose(mPoseIndex);
        }

        // If one node is selected, draw it and its children.
        FbxAMatrix lDummyGlobalPosition;

        InitializeLights(mScene, mCurrentTime, lPose);
        draw(mScene->GetRootNode(), mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose, mShadingMode);
        DisplayGrid(lDummyGlobalPosition);

        glPopAttrib();
        glPopAttrib();
    }

    DisplayWindowMessage();
    return true;
}

void SceneContext::OnReshape(int pWidth, int pHeight)
{
    glViewport(0, 0, (GLsizei)pWidth, (GLsizei)pHeight);
    mWindowWidth = pWidth;
    mWindowHeight = pHeight;
}

void SceneContext::OnKeyboard(unsigned char pKey)
{
    // Zoom In on '+' or '=' keypad keys
    if (pKey == 43 || pKey == 61)
    {
        FbxCamera* lCamera = GetCurrentCamera(mScene);
        if(lCamera)
        {
            //double lOriginalAperture = sqrt(lCamera->GetApertureWidth());
            CameraZoom(mScene, 10, mCameraZoomMode);
            mStatus = MUST_BE_REFRESHED;
        }
    }

    // Zoom Out on '-' or '_' keypad keys
    if (pKey == 45 || pKey == 95)
    {
        FbxCamera* lCamera = GetCurrentCamera(mScene);
        if(lCamera)
        {
            //double lOriginalAperture = sqrt(lCamera->GetApertureWidth());
            CameraZoom(mScene, 0 - 10, mCameraZoomMode);
            mStatus = MUST_BE_REFRESHED;
        }
    }

    // 'N' enable/disable normal display
    if (pKey == 'N' || pKey == 'n')
    {
        //gOGLScene->GetShadingManager()->SetDrawNormal(!gOGLScene->GetShadingManager()->IsDrawNormal());
    }

    // Pause and unpause when spacebar is pressed.
    if (pKey == ' ')
    {
        SetPause(!GetPause());
    }
}

void SceneContext::OnMouse(int pButton, int pState, int pX, int pY, bool isMouseMove)
{
    // Move the camera (orbit, zoom or pan) with the mouse.
    //cout << "pButton==" << pButton << ";pState===" << pState << ";pX==" << pX << ";pY==" << pY << ";isMouseMove==" << isMouseMove;
    FbxCamera* lCamera = GetCurrentCamera(mScene);
    if (lCamera)
    {
        mCamPosition = lCamera->Position.Get();
        mCamCenter = lCamera->InterestPosition.Get();
        mRoll = lCamera->Roll.Get();
    }

    mLastX = pX;
    mLastY = pY;

    if ( pButton == LEFT_BUTTON ) {
        if ( pState == BUTTON_DOWN ) {
            FbxNode* pNode = queryNode(pX, pY);
            if ( pNode ) {
                if ( isAxisNode(pNode) ) {
                    /**< 坐标轴节点 */
                    mCameraStatus = CAMERA_NOTHING;
                    mAxisNode = pNode;
                } else {
                    /**< 机器人节点 */
                    mCameraStatus = CAMERA_PAN;
                    mSelectedNode = pNode;
                    setAxisNodeVisiable(true);
                }
            } else {
                mAxisNode = nullptr;
                mSelectedNode = nullptr;
                mCameraStatus = CAMERA_PAN;
                setAxisNodeVisiable(false);
            }
        } else if ( pState == BUTTON_UP ) {
            mCameraStatus = CAMERA_NOTHING;
            mAxisNode = nullptr;
            if ( mAxisNode ) {
                /**< 坐标轴节点 */
                //....
            } else if (mSelectedNode){
                /**< 机器人节点 */
                setAxisNodeVisiable(true);
                if ( isMouseMove ) {
                    /**< 鼠标移动 */
                    //....
//                    printf()
                } else {
                    /**< 把坐标轴节点变换至选中机器人节点的矩阵 */
                    if ( mGetAxisCallBack ) {
                        std::string axisName = mGetAxisCallBack(mSelectedNode->GetName());
                        FbxNode* axisNode = mScene->FindNodeByName(axisName.c_str());
                        if ( axisNode ) {
                            FbxVector4 pNodeBoxMin(0,0,0);
                            FbxVector4 pNodeBoxMax(0,0,0);
                            FbxVector4 pNodeBoxCenter(0,0,0);
                            mSelectedNode->EvaluateGlobalBoundingBoxMinMaxCenter(pNodeBoxMin, pNodeBoxMax, pNodeBoxCenter);
                            FbxVector4 rotation = mSelectedNode->GetParent()->EvaluateGlobalTransform().GetR();
                            axisNode->LclRotation.Set(rotation);
                            axisNode->LclTranslation.Set(pNodeBoxCenter);
                            mStatus = MUST_BE_REFRESHED;
                        }
                    }
                }
            } else {
                /**< 未选择节点 */
                //....
            }
        }
    }else if ( pButton == RIGHT_BUTTON ) {
        if ( pState == BUTTON_DOWN ) {
            mCameraStatus = CAMERA_ORBIT;
        } else if ( pState == BUTTON_UP ) {
            mCameraStatus = CAMERA_NOTHING;
        }
    }
}

void SceneContext::OnMouseMove(int pX, int pY)
{
    FbxNode* pNode = queryNode(pX, pY);
    if ( pNode ) {
        std::cout << "pNode==" << pNode->GetName() << endl;
        if ( isAxisNode(pNode) ) {
            /**< 坐标轴节点 */
            mCameraStatus = CAMERA_NOTHING;
            mAxisNode = pNode;
        } else {
            /**< 机器人节点 */
            mCameraStatus = CAMERA_PAN;
            mSelectedNode = pNode;
            setAxisNodeVisiable(true);
        }
    }else {
        mAxisNode = nullptr;
        mSelectedNode = nullptr;
        mCameraStatus = CAMERA_PAN;
        setAxisNodeVisiable(false);
    }

    mCameraStatus = CAMERA_NOTHING;
    mAxisNode = nullptr;
     if (mSelectedNode){
        /**< 机器人节点 */
        setAxisNodeVisiable(true);
        /**< 把坐标轴节点变换至选中机器人节点的矩阵 */
        if ( mMouseMoveGetAxisCallBack ) {

            std::string axisName = mMouseMoveGetAxisCallBack(mSelectedNode->GetName());
            FbxNode* axisNode = mScene->FindNodeByName(axisName.c_str());
            if ( axisNode ) {
                FbxVector4 pNodeBoxMin(0,0,0);
                FbxVector4 pNodeBoxMax(0,0,0);
                FbxVector4 pNodeBoxCenter(0,0,0);
                mSelectedNode->EvaluateGlobalBoundingBoxMinMaxCenter(pNodeBoxMin, pNodeBoxMax, pNodeBoxCenter);
                FbxVector4 rotation = mSelectedNode->GetParent()->EvaluateGlobalTransform().GetR();
                axisNode->LclRotation.Set(rotation);
                axisNode->LclTranslation.Set(pNodeBoxCenter);
                mStatus = MUST_BE_REFRESHED;
            }

        }
    }

}

void SceneContext::OnMouseMotion(int pX, int pY)
{
    /**< 选择坐标轴节点, 不进行相机操作, 坐标轴和机器人节点进行旋转 */
    if ( mAxisNode && mSelectedNode  ) {
        FbxVector4 pNodeBoxMin(0,0,0);
        FbxVector4 pNodeBoxMax(0,0,0);
        FbxVector4 pNodeBoxCenter(0,0,0);
        bool bRet = mAxisNode->EvaluateGlobalBoundingBoxMinMaxCenter(pNodeBoxMin, pNodeBoxMax, pNodeBoxCenter);
        if ( bRet ) {
            // 世界坐标转化屏幕坐标
            int winx = 0;
            int winy = 0;
            int winz = 0;
            bRet = globalToScreenTransform(pNodeBoxCenter[0], pNodeBoxCenter[1], pNodeBoxCenter[2], winx, winy, winz);
            if ( bRet ) {
                // 计算旋转角度
                Custom::LineF l1(winx, winy, mLastX, mLastY);
                Custom::LineF l2(winx, winy, pX, pY);
                /**< 角度在0 ~ 360 */
                double rotation = l1.angleTo(l2);
                /**< 旋转角度转化为-180 ~ 180 */
                range_angle_180(rotation);
                // 旋转坐标轴和节点
                const char* nodeName = mSelectedNode->GetName();
                FbxNode* parent = mSelectedNode->GetParent();
                FbxDouble3 lNodeChange = parent->LclRotation.Get();
                const char* name = mAxisNode->GetName();
                if ( strcmp(name, ROTATE_X_NODE_NAME) == 0 ) {
                    lNodeChange[0] += rotation;
                    range_angle_180(lNodeChange[0]);
                    limitRationAngle(nodeName, lNodeChange[0]);
                    if ( mServoAngleChangeCallBack ) {
                        mServoAngleChangeCallBack(nodeName, lNodeChange[0]);
                    }
                } else if (strcmp(name, ROTATE_Y_NODE_NAME) == 0) {
                    lNodeChange[1] += rotation;
                    range_angle_180(lNodeChange[1]);
                    limitRationAngle(nodeName, lNodeChange[1]);
                    if ( mServoAngleChangeCallBack ) {
                        mServoAngleChangeCallBack(nodeName, lNodeChange[1]);
                    }
                } else if (strcmp(name, ROTATE_Z_NODE_NAME) == 0) {
                    lNodeChange[2] += rotation;
                    range_angle_180(lNodeChange[2]);
                    limitRationAngle(nodeName, lNodeChange[2]);
                    if ( mServoAngleChangeCallBack ) {
                        mServoAngleChangeCallBack(nodeName, lNodeChange[2]);
                    }
                }

                parent->LclRotation.Set(lNodeChange);
                FbxVector4 lChange = parent->EvaluateGlobalTransform().GetR();
                mAxisNode->LclRotation.Set(lChange);

                mLastX = pX;
                mLastY = pY;
                mStatus = MUST_BE_REFRESHED;
            }
        }
    } else { // 选择机器人节点和未选择节点都需要进行相机操作，选择坐标轴节点时则不进行相机操作且没有滑过的节点高亮显示
        /**< 进行相机操作 */
        int motion;
        switch (mCameraStatus)
        {
        default:
            break;
        case CAMERA_ORBIT:
            CameraOrbit(mScene, mCamPosition, mRoll, pX-mLastX, mLastY-pY);
            break;

        case CAMERA_ZOOM:
            motion = mLastY-pY;
            CameraZoom(mScene, motion, mCameraZoomMode);
            mLastY = pY;
            break;

        case CAMERA_PAN:
            CameraPan(mScene, mCamPosition, mCamCenter, mRoll, pX-mLastX, mLastY - pY);

            FbxCamera* lCamera = GetCurrentCamera(mScene);
            if (lCamera)
            {
                mCamPosition = lCamera->Position.Get();
                mCamCenter = lCamera->InterestPosition.Get();
                mRoll = lCamera->Roll.Get();
            }

            mLastX = pX;
            mLastY = pY;
            break;
        }

        /**< 保存鼠标滑过的节点 */
        //mHoverNode = queryNode(pX, pY);
        mStatus = MUST_BE_REFRESHED;
    }
}

void SceneContext::SetSelectedNode(const char * nodeName)
{
    FbxNode* pNode = mScene->FindNodeByName(nodeName);
    mSelectedNode = pNode;
    mStatus = MUST_BE_REFRESHED;
}

void SceneContext::SetShadingMode(ShadingMode pMode)
{
    mShadingMode = pMode;
    mStatus = MUST_BE_REFRESHED;
}

void SceneContext::DisplayWindowMessage()
{
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, mWindowWidth, 0, mWindowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Display message in the left up corner of the window
    const float lX = 5;
    const float lY = static_cast<float>(mWindowHeight) - 20;
    glTranslatef(lX, lY, 0);

    mDrawText->SetPointSize(15.f);
    // shenbin 2020-04-16
    //mDrawText->Display(mWindowMessage.Buffer());

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void SceneContext::DisplayGrid(const FbxAMatrix & pTransform)
{
    glPushMatrix();
    glMultMatrixd(pTransform);

    // Draw a grid 500*500
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(1.0);
    const int hw = 20;
    const int step = 2;
    const int bigstep = 10;
    int       i;

    // Draw Grid
    for (i = -hw; i <= hw; i+=step) {

        if (i % bigstep == 0) {
            glLineWidth(2.0);
        } else {
            glLineWidth(1.0);
        }
        glBegin(GL_LINES);
        glVertex3i(i,0,-hw);
        glVertex3i(i,0,hw);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(-hw,0,i);
        glVertex3i(hw,0,i);
        glEnd();

    }

    // Write some grid info
    const GLfloat zoffset = -2.f;
    const GLfloat xoffset = 1.f;
    mDrawText->SetPointSize(4.f);
    for (i = -hw; i <= hw; i+=bigstep)
    {

        FbxString scoord;
        int lCount;

        // Don't display origin
        //if (i == 0) continue;
        if (i == 0) {
            scoord = "0";
            lCount = (int)scoord.GetLen();
            glPushMatrix();
            glVertex3f(i+xoffset,0,zoffset);
            glRotatef(-90,1,0,0);
            
            mDrawText->Display(scoord.Buffer());

            glPopMatrix();

            continue;
        }

        // X coordinates
        scoord = "X: ";
        scoord += i;
        lCount = (int)scoord.GetLen();

        glPushMatrix();
        glTranslatef(i+xoffset,0,zoffset);
        glRotatef(-90,1,0,0);
        mDrawText->Display(scoord.Buffer());
        glPopMatrix();

        // Z coordinates
        scoord = "Z: ";
        scoord += i;
        lCount = (int)scoord.GetLen();

        glPushMatrix();
        glTranslatef(xoffset,0,i+zoffset);
        glRotatef(-90,1,0,0);
        mDrawText->Display(scoord.Buffer());
        glPopMatrix();

    }

    glPopMatrix();
}


void SceneContext::SetZoomMode( CameraZoomMode pZoomMode)
{
    if( pZoomMode == ZOOM_POSITION)
    {
        mCameraZoomMode = ZOOM_POSITION;
    }
    else
    {
        mCameraZoomMode =  ZOOM_FOCAL_LENGTH;
    }
}

FbxNode* SceneContext::findNode (FbxNode* node, double x, double y,double z)
{
    if ( node->GetMesh() && node->GetVisibility() ) {
        FbxVector4 pBBoxMin(0.f, 0.f, 0.f);
        FbxVector4 pBBoxMax(0.f, 0.f, 0.f);
        FbxVector4 pBBoxCenter(0.f, 0.f, 0.f);
        bool bRet = node->EvaluateGlobalBoundingBoxMinMaxCenter (pBBoxMin, pBBoxMax, pBBoxCenter);
        if ( bRet ) {
            if ( (x > pBBoxMin[0] && x < pBBoxMax[0]) &&
                 (y > pBBoxMin[1] && y < pBBoxMax[1]) &&
                 (z > pBBoxMin[2] && z < pBBoxMax[2])) {
                return node;
            }
        }
    } else {
        for (int i = 0; i < node->GetChildCount(); i ++ ) {
            FbxNode* child = findNode(node->GetChild(i), x, y,z);
            if ( child ) {
                return child;
            }
        }
    }

    return nullptr;
}

FbxNode* SceneContext::queryNode (int winx, int winy)
{
    FbxNode* pChild = nullptr;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    bool bRet = screenToGlobalTransform(winx, winy, x, y, z);
    if ( bRet ) {
        FbxNode* pRoot = mScene->GetRootNode();
        for (int i = 0; i < pRoot->GetChildCount(); i ++ ) {
            pChild = findNode(pRoot->GetChild(i), x, y,z);
            if ( pChild ) {
                break;
            }
        }
    }
    return pChild;
}

void SceneContext::setAxisNodeVisiable(bool bVisiable)
{
    //TODO: 关闭显示了axis
    bVisiable = false;
    FbxNode* pAxisNode = mScene->FindNodeByName("Rotate");
    if ( pAxisNode ) {
        pAxisNode->SetVisibility(bVisiable);

        if ( bVisiable ) {
            if ( mSelectedNode ) {
                std::string axisName = mGetAxisCallBack(mSelectedNode->GetName());
                FbxNode* axisNode = mScene->FindNodeByName(axisName.c_str());
                int nCnt = pAxisNode->GetChildCount();
                for ( int i = 0; i < nCnt; i ++ ) {
                    FbxNode* pNode = pAxisNode->GetChild(i);
                    if ( pNode && axisNode == pNode ) {
                        pNode->SetVisibility(true);
                    } else {
                        pNode->SetVisibility(false);
                    }
                }
            }
        } else {
            int nCnt = pAxisNode->GetChildCount();
            for ( int i = 0; i < nCnt; i ++ ) {
                FbxNode* pNode = pAxisNode->GetChild(i);
                if ( pNode ) {
                    pNode->SetVisibility(bVisiable);
                }
            }
        }
    }
}

bool SceneContext::isAxisNode (FbxNode* pNode)
{
    if ( pNode ) {
        const char* name = pNode->GetName();
        if ( strcmp(name, ROTATE_X_NODE_NAME) == 0 ||
             strcmp(name, ROTATE_Y_NODE_NAME) == 0 ||
             strcmp(name, ROTATE_Z_NODE_NAME) == 0 ) {
            return true;
        }
    }
    return false;
}

void SceneContext::draw (FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,
                         FbxAMatrix& pParentGlobalPosition, FbxPose* pPose,
                         ShadingMode pShadingMode)
{
    FbxAMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);
    if (pNode->GetNodeAttribute())
    {
        // Geometry offset.
        // it is not inherited by the children.
        FbxAMatrix lGeometryOffset = GetGeometry(pNode);
        FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;
        ShadingMode color = pShadingMode;
        if ( mHoverNode == pNode ) {
            color = SHADING_MODE_HOVERCOLOR;
        } else if ( mAxisNode == pNode ) {
            color = SHADING_MODE_DEFAULTCOLOR;
        }
        DrawNode(pNode, pTime, pAnimLayer, pParentGlobalPosition, lGlobalOffPosition, pPose, color);
    }

    const int lChildCount = pNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
    {
        FbxNode* pChildNode = pNode->GetChild(lChildIndex);
        if ( !pChildNode->GetVisibility() ) {
            continue;
        }

        //const char* name = pChildNode->GetName();
        if ( mSelectedNode && pNode == mSelectedNode->GetParent() ) {
            DrawNodeRecursive(pChildNode, pTime, pAnimLayer, lGlobalPosition, pPose, SHADING_MODE_DEFAULTCOLOR);
        } else {
            draw(pChildNode, pTime, pAnimLayer, lGlobalPosition, pPose, pShadingMode);
        }
    }
}

bool SceneContext::screenToGlobalTransform (int winx, int winy, double& x, double& y, double& z)
{
    GLdouble modelview[16]{0};
    GLdouble projection[16]{0};
    GLint viewport[4]{0};

    /*Read the projection, modelview and viewport matrices using the glGet functions.*/
    glGetIntegerv( GL_VIEWPORT, viewport );
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );

    //Read the window z value from the z-buffer
    GLfloat winz = 0.0f;
    glReadPixels(winx, viewport[3] - winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
    if ( fabs(winz - 1.0) < 1e-6 || fabs(winz) < 1e-6) {
        return false;
    }

    //Use the gluUnProject to get the world co-ordinates of
    //the point the user clicked and save in objx, objy, objz.
    int nRet = gluUnProject(winx, viewport[3] - winy, winz, modelview, projection, viewport, &x, &y, &z);
    if ( nRet == GLU_FALSE ) {
        return false;
    }

    return true;
}

bool SceneContext::globalToScreenTransform (double x, double y, double z, int& winx, int& winy, int& winz)
{
    GLdouble modelview[16]{0};
    GLdouble projection[16]{0};
    GLint viewport[4]{0};

    /*Read the projection, modelview and viewport matrices using the glGet functions.*/
    glGetIntegerv( GL_VIEWPORT, viewport );
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );

    double dx = 0.0f;
    double dy = 0.0f;
    double dz = 0.0f;
    /**< 获取的坐标为左下角屏幕坐标 */
    int nRet = gluProject(x,
                          y,
                          z,
                          modelview,
                          projection,
                          viewport,
                          &dx,
                          &dy,
                          &dz);
    if ( nRet = GLU_FALSE ) {
        return false;
    }

    /**< 左下角屏幕坐标转化为左上角屏幕坐标 */
    winx = static_cast<int>(dx);
    winy = static_cast<int>(viewport[3] - dy);
    winz = static_cast<int>(dz);
    return true;
}

bool SceneContext::setServoAngle(const FbxString& name, double targetAngle, const FbxString& axisName, bool bSelectNode/* = true*/)
{
    FbxNode* pNode = mScene->FindNodeByName(name);
    if ( pNode == nullptr ) {
        return false;
    }

    FbxNode* parent = pNode->GetParent();
    if ( parent == nullptr ) {
        return false;
    }

    range_angle_180(targetAngle);
    FbxDouble3 lChange = parent->LclRotation.Get();
    if ( strcmp(axisName, ROTATE_X_NODE_NAME) == 0 ) {
        lChange[0] = targetAngle;
    } else if (strcmp(axisName, ROTATE_Y_NODE_NAME) == 0) {
        lChange[1] = targetAngle;
    } else if (strcmp(axisName, ROTATE_Z_NODE_NAME) == 0) {
        lChange[2] = targetAngle;
    }

    parent->LclRotation.Set(lChange);
    if ( bSelectNode ) {
        mSelectedNode = pNode;

        FbxVector4 pNodeBoxMin(0,0,0);
        FbxVector4 pNodeBoxMax(0,0,0);
        FbxVector4 pNodeBoxCenter(0,0,0);
        bool bRet = mSelectedNode->EvaluateGlobalBoundingBoxMinMaxCenter(pNodeBoxMin, pNodeBoxMax, pNodeBoxCenter);
        if ( bRet ) {
            // 世界坐标转化屏幕坐标
            int winx = 0;
            int winy = 0;
            int winz = 0;
            bRet = globalToScreenTransform(pNodeBoxCenter[0], pNodeBoxCenter[1], pNodeBoxCenter[2], winx, winy, winz);
        }

    }
    mStatus = MUST_BE_REFRESHED;



    return true;
}

bool SceneContext::setServoOffsetAngle(const FbxString& name, double offsetAngle, const FbxString& axisName)
{
    FbxNode* pNode = mScene->FindNodeByName(name);
    if ( pNode == nullptr ) {
        return false;
    }

    FbxNode* parent = pNode->GetParent();
    if ( parent == nullptr ) {
        return false;
    }

    FbxDouble3 lChange = parent->LclRotation.Get();
    if ( strcmp(axisName, ROTATE_X_NODE_NAME) == 0 ) {
        lChange[0] += offsetAngle;
        range_angle_180(lChange[0]);
    } else if (strcmp(axisName, ROTATE_Y_NODE_NAME) == 0) {
        lChange[1] += offsetAngle;
        range_angle_180(lChange[1]);
    } else if (strcmp(axisName, ROTATE_Z_NODE_NAME) == 0) {
        lChange[2] += offsetAngle;
        range_angle_180(lChange[2]);
    }
    parent->LclRotation.Set(lChange);

    FbxVector4 pNodeBoxMin(0,0,0);
    FbxVector4 pNodeBoxMax(0,0,0);
    FbxVector4 pNodeBoxCenter(0,0,0);
    bool bRet = pNode->EvaluateGlobalBoundingBoxMinMaxCenter(pNodeBoxMin, pNodeBoxMax, pNodeBoxCenter);
    if ( bRet ) {
        // 世界坐标转化屏幕坐标
        int winx = 0;
        int winy = 0;
        int winz = 0;
        bRet = globalToScreenTransform(pNodeBoxCenter[0], pNodeBoxCenter[1], pNodeBoxCenter[2], winx, winy, winz);
    }
    mStatus = MUST_BE_REFRESHED;
    return true;
}

double SceneContext::getServoAngle (const FbxString& name, const FbxString& axisName)
{
    FbxNode* pNode = mScene->FindNodeByName(name);
    if ( pNode == nullptr ) {
        return 0.f;
    }

    FbxNode* parent = pNode->GetParent();
    if ( parent == nullptr ) {
        return false;
    }

    double angle = 0.f;
    FbxDouble3 lChange = parent->LclRotation.Get();
    if ( strcmp(axisName, ROTATE_X_NODE_NAME) == 0 ) {
        angle = lChange[0];
    } else if (strcmp(axisName, ROTATE_Y_NODE_NAME) == 0) {
        angle = lChange[1];
    } else if (strcmp(axisName, ROTATE_Z_NODE_NAME) == 0) {
        angle = lChange[2];
    }

    range_angle_180(angle);
    return angle;
}

double SceneContext::range_angle_180 (double& angle)
{
    while (angle > 180)
        angle -= 360;
    while (angle < -180)
        angle += 360;
    return angle;
}

double SceneContext::limitRationAngle (const char* name, double& angle)
{
    if ( mGetNodeRotationRangeCallBack ) {
        double dMinAngle = 0.f;
        double dMaxAngle = 0.f;
        if ( !mGetNodeRotationRangeCallBack(name, dMinAngle, dMaxAngle) ) {
            return angle;
        }

        // 超出限定范围值处理方式
        if ( angle < dMinAngle || angle > dMaxAngle ) {
            // dMaxAngle ~ angle   角度差(0 ~ 180范围内)
            double dAngleToMax = fabs(angle - dMaxAngle);
            if ( dAngleToMax > 180 ) {
                dAngleToMax = 360 - dAngleToMax;
            }

            // dMinAngle ~ angle 角度差(0 ~ 180范围内)
            double dAngleToMin = fabs(angle - dMinAngle);
            if ( dAngleToMin > 180 ) {
                dAngleToMin = 360 - dAngleToMin;
            }

            if ( dAngleToMax > dAngleToMin ) {
                angle = dMinAngle;
            } else {
                angle = dMaxAngle;
            }
        }
    }

    return angle;
}

const char *SceneContext::getSelectFbxNodeName()
{
    if(mSelectedNode == NULL){
        return NULL;
    }else {
        return mSelectedNode->GetName();
    }
}

namespace Custom {
double LineF::angle() const
{
    const double theta = std::atan2(-dy(), dx()) * 360.0 / M_2PI;
    const double theta_normalized = theta < 0 ? theta + 360 : theta;
    if (mFuzzyCompare(theta_normalized, double(360)))
        return double(0);
    else
        return theta_normalized;
}

double LineF::angleTo (const LineF& l) const
{
    if (isNull() || l.isNull())
        return 0;

    const double a1 = angle();
    const double a2 = l.angle();

    const double delta = a2 - a1;
    const double delta_normalized = delta < 0 ? delta + 360 : delta;

    if (mFuzzyCompare(delta, double(360)))
        return 0;
    else
        return delta_normalized;
}

bool LineF::isNull() const
{
    return mFuzzyCompare(_x1, _x2) && mFuzzyCompare(_y1, _y2);
}

} // end custom
