#include "fbx.h"
#include "SceneContext.h"
#include "SetCamera.h"

const int DEFAULT_WINDOW_WIDTH = 720;
const int DEFAULT_WINDOW_HEIGHT = 486;

class MyMemoryAllocator
{
public:
    static void* MyMalloc(size_t pSize)
    {
        char *p = (char*)malloc(pSize + FBXSDK_MEMORY_ALIGNMENT);
        memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
        return p + FBXSDK_MEMORY_ALIGNMENT;
    }

    static void* MyCalloc(size_t pCount, size_t pSize)
    {
        char *p = (char*)calloc(pCount, pSize + FBXSDK_MEMORY_ALIGNMENT);
        memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
        return p + FBXSDK_MEMORY_ALIGNMENT;
    }

    static void* MyRealloc(void* pData, size_t pSize)
    {
        if (pData)
        {
            FBX_ASSERT(*((char*)pData-1)=='#');
            if (*((char*)pData-1)=='#')
            {
                char *p = (char*)realloc((char*)pData - FBXSDK_MEMORY_ALIGNMENT, pSize + FBXSDK_MEMORY_ALIGNMENT);
                memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
                return p + FBXSDK_MEMORY_ALIGNMENT;
            }
            else
            {   // Mismatch
                char *p = (char*)realloc((char*)pData, pSize + FBXSDK_MEMORY_ALIGNMENT);
                memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
                return p + FBXSDK_MEMORY_ALIGNMENT;
            }
        }
        else
        {
            char *p = (char*)realloc(NULL, pSize + FBXSDK_MEMORY_ALIGNMENT);
            memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
            return p + FBXSDK_MEMORY_ALIGNMENT;
        }
    }

    static void MyFree(void* pData)
    {
        if (pData==NULL)
            return;
        FBX_ASSERT(*((char*)pData-1)=='#');
        if (*((char*)pData-1)=='#')
        {
            free((char*)pData - FBXSDK_MEMORY_ALIGNMENT);
        }
        else
        {   // Mismatch
            free(pData);
        }
    }
};

Fbx::Fbx(const char *modelName, const char *textureName)
{
    // Use a custom memory allocator
    FbxSetMallocHandler(MyMemoryAllocator::MyMalloc);
    FbxSetReallocHandler(MyMemoryAllocator::MyRealloc);
    FbxSetFreeHandler(MyMemoryAllocator::MyFree);
    FbxSetCallocHandler(MyMemoryAllocator::MyCalloc);

    // Initialize OpenGL.
    const bool lSupportVBO = InitializeOpenGL();

    m_scene = new SceneContext(modelName, textureName, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, lSupportVBO);
}

Fbx::~Fbx()
{
    if ( m_scene ) {
        delete m_scene;
        m_scene = nullptr;
    }
}

Fbx::Status Fbx::GetStatus () const
{
    return static_cast<Fbx::Status>(m_scene->GetStatus());
}

void Fbx::SetStatus(Status status)
{
    m_scene->SetStatus(static_cast<SceneContext::Status>(status));
}

bool Fbx::LoadFile ()
{
    bool bRet = m_scene->LoadFile();
    if ( bRet ) {
        /**< 适配显示模型 */
        FbxCamera* lCamera = GetCurrentCamera(const_cast<FbxScene *>(m_scene->GetScene()));
        if (lCamera) {
            SetCameraPerspective(CAMERA_FRONT);
            lCamera->FieldOfView.Set(11);
        }
    }
    return m_scene->LoadFile();
}

unsigned int Fbx::GetFrameMillTime () const
{
    return static_cast<unsigned int>(m_scene->GetFrameTime().GetMilliSeconds());
}

bool Fbx::OnDisplay()
{
    return m_scene->OnDisplay();
}

void Fbx::OnReshape(int pWidth, int pHeight)
{
    m_scene->OnReshape(pWidth, pHeight);
}

void Fbx::OnKeyboard(unsigned char pKey)
{
    m_scene->OnKeyboard(pKey);
}

void Fbx::OnMouse(Button pButton, State pState, int pX, int pY, bool isMouseMove/* = false*/)
{
    m_scene->OnMouse(static_cast<int>(pButton), static_cast<int>(pState), pX, pY, isMouseMove);
}

void Fbx::onMouseMove(int pX, int pY)
{
    m_scene->OnMouseMove(pX,pY);
}

void Fbx::OnMouseMotion(int pX, int pY)
{
    m_scene->OnMouseMotion(pX, pY);
}

void Fbx::OnTimerClick() const
{
    m_scene->OnTimerClick();
}

void Fbx::OnMouseWheel(WheelDir dir, int speed /*= 5*/)
{
    m_scene->OnMouseWheel(static_cast<int>(dir), speed);
}

void Fbx::SetAnimStackSelection ()
{
    int lCurrentAnimStackIndex = 0;
    // Add the animation stack names.
    const FbxArray<FbxString *> & lAnimStackNameArray = m_scene->GetAnimStackNameArray();
    for (int lPoseIndex = 0; lPoseIndex < lAnimStackNameArray.GetCount(); ++lPoseIndex) {
        // Track the current animation stack index.
        if (lAnimStackNameArray[lPoseIndex]->Compare(m_scene->GetScene()->ActiveAnimStackName.Get()) == 0) {
            lCurrentAnimStackIndex = lPoseIndex;
        }
    }
    m_scene->SetCurrentAnimStack(lCurrentAnimStackIndex);
}

void Fbx::SetCameraPerspective (CameraPerspective dir)
{
    FbxCamera* lCamera = GetCurrentCamera(const_cast<FbxScene *>(m_scene->GetScene()));
    if (!lCamera) return;
    /**< 这里都是采用特殊值进行设置。 */
    switch (dir) {
    case CAMERA_FRONT:
        lCamera->Position.Set(FbxVector4(2.1147970912866794,
                                         68.55186332403271,
                                         312.31488173644294,
                                         1));
        lCamera->InterestPosition.Set(FbxVector4(3.010149390931567,
                                                 27.769535010169356,
                                                 -3.576580205063635,
                                                 1));
        lCamera->Roll.Set(0);
        m_scene->SetStatus(SceneContext::MUST_BE_REFRESHED);
        break;
    case CAMERA_BACK:
        lCamera->Position.Set(FbxVector4(6.392042039003001,
                                         41.68944226685277,
                                         -316.6588447415284,
                                         1));
        lCamera->InterestPosition.Set(FbxVector4(-0.9876165737196263,
                                                 26.770668799919125,
                                                 -3.7184730727380586,
                                                 1));
        lCamera->Roll.Set(0);
        m_scene->SetStatus(SceneContext::MUST_BE_REFRESHED);
        break;
    case CAMERA_TOP:
        lCamera->Position.Set(FbxVector4(0.8187683307637696,
                                         334.2319148065412,
                                         -18.144925439529388,
                                         1));
        lCamera->InterestPosition.Set(FbxVector4(0.9562694946150492,
                                                 27.807397835795825,
                                                 -1.6323210201959308,
                                                 1));
        lCamera->Roll.Set(0);
        m_scene->SetStatus(SceneContext::MUST_BE_REFRESHED);
        break;
    case CAMERA_BOTTOM:
        lCamera->Position.Set(FbxVector4(-1.3565065435814563,
                                         -296.2328343369815,
                                         -21.495290842280472,
                                         1));
        lCamera->InterestPosition.Set(FbxVector4(-1.2532139460541347,
                                                 25.334137186939053,
                                                 1.4465413769287385,
                                                 1));
        lCamera->Roll.Set(0);
        m_scene->SetStatus(SceneContext::MUST_BE_REFRESHED);
        break;
    case CAMERA_LEFT:
        lCamera->Position.Set(FbxVector4(316.78314596111244,
                                         19.093815007487457,
                                         -4.870954736420524,
                                         1));
        lCamera->InterestPosition.Set(FbxVector4(-1.2532139460541345,
                                                 25.334137186939053,
                                                 1.4465413769287385,
                                                 1));
        lCamera->Roll.Set(0);
        m_scene->SetStatus(SceneContext::MUST_BE_REFRESHED);
        break;
    case CAMERA_RIGHT:
        lCamera->Position.Set(FbxVector4(-316.78314596111244,
                                         19.093815007487457,
                                         0.20302412280502224,
                                         1));
        lCamera->InterestPosition.Set(FbxVector4(-1.2532139460541345,
                                                 25.334137186939053,
                                                 -0.5558276348792219,
                                                 1));
        lCamera->Roll.Set(0);
        m_scene->SetStatus(SceneContext::MUST_BE_REFRESHED);
        break;
    case CAMERA_RIGHT_45:
        lCamera->Position.Set(FbxVector4(105.22168974233124,
                                         320.50971477440896,
                                         67.07258435538259,
                                         1));
        lCamera->InterestPosition.Set(FbxVector4(2.3467241285925278,
                                                 26.550666408547386,
                                                 0.3175469440251646,
                                                 1));
        lCamera->Roll.Set(0);
        m_scene->SetStatus(SceneContext::MUST_BE_REFRESHED);
    default:
        break;
    }
}

bool Fbx::existNode (int screenX, int screenY)
{
    return m_scene->queryNode(screenX, screenY) ? true : false;
}

bool Fbx::setServoAngle (const char *name, double targetAngle, AxisType dir, bool bSelectNode /*= true*/)
{
    return m_scene->setServoAngle(name, targetAngle, FbxString(getAxisName(dir).c_str()), bSelectNode);
}

double Fbx::getServoAngle (const char* name, AxisType dir)
{
    return m_scene->getServoAngle(name, FbxString(getAxisName(dir).c_str()));
}

std::string Fbx::getAxisName (AxisType dir)
{
    FbxString axisName = "";
    switch (dir) {
    case AXIS_X:
        axisName = ROTATE_X_NODE_NAME;
        break;
    case AXIS_Y:
        axisName = ROTATE_Y_NODE_NAME;
        break;
    case AXIS_Z:
        axisName = ROTATE_Z_NODE_NAME;
        break;
    }
    char* axis = axisName.Buffer();
    return std::string(axis);
}

void Fbx::setServoAngleChangeCallBack (std::function<void(const char*, double)> CallBack)
{
    m_scene->setServoAngleChangeCallBack(CallBack);
}

void Fbx::getServoAxisNameCallBack (std::function<std::string(const char*)> CallBack)
{
    m_scene->getServoAxisNameCallBack(CallBack);
}

void Fbx::setMouseMoveAxisNameCallBack(std::function<std::string (const char *)> CallBack)
{
    m_scene->setMouseMoveGetAxisCallBack(CallBack);
}

void Fbx::getServoRotationRangeCallBack (std::function<bool(const char*, double&, double&)> CallBack)
{
    m_scene->getServoRotationRangeCallBack(CallBack);
}

void Fbx::setSelectedNode(const char *nodeName)
{
    m_scene->SetSelectedNode(nodeName);
}

const char* Fbx::getSelectorFbxNode()
{
    return m_scene->getSelectFbxNodeName();

}
