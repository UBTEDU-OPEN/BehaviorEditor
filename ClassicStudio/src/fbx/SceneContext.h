/****************************************************************************************

Copyright (C) 2015 Autodesk, Inc.
All rights reserved.

Use of this software is subject to the terms of the Autodesk license agreement
provided at the time of installation or download, or which otherwise accompanies
this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _SCENE_CONTEXT_H
#define _SCENE_CONTEXT_H

#include "GlFunctions.h"
#include <functional>

class DrawText;

namespace Custom {
    class LineF;
}

// custom define shenbin 2020-05-07
#define M_2PI 6.28318530717958647692528676655900576
#define ROTATE_NODE_NAME    "Rotate"
#define ROTATE_X_NODE_NAME  "Rotate_X"
#define ROTATE_Y_NODE_NAME  "Rotate_Y"
#define ROTATE_Z_NODE_NAME  "Rotate_Z"

// This class is responsive for loading files and recording current status as
// a bridge between window system such as GLUT or Qt and a specific FBX scene.
class SceneContext
{
public:
    enum Status
    {
        UNLOADED,               // Unload file or load failure;
        MUST_BE_LOADED,         // Ready for loading file;
        MUST_BE_REFRESHED,      // Something changed and redraw needed;
        REFRESHED               // No redraw needed.
    };
    Status GetStatus() const { return mStatus; }
    void SetStatus(Status status) { mStatus = status; }

    // Initialize with a .FBX, .DAE or .OBJ file name and current window size.
    SceneContext(const char * pFileName, const char* textureName, int pWindowWidth, int pWindowHeight, bool pSupportVBO);
    ~SceneContext();

    // Return the FBX scene for more informations.
    const FbxScene * GetScene() const { return mScene; }
    // Load the FBX or COLLADA file into memory.
    bool LoadFile();

    // The time period for one frame.
    const FbxTime GetFrameTime() const { return mFrameTime; }

    // Call this method when redraw is needed.
    bool OnDisplay();
    // Call this method when window size is changed.
    void OnReshape(int pWidth, int pHeight);
    // Call this method when keyboard input occurs.
    void OnKeyboard(unsigned char pKey);
    // Call this method when mouse buttons are pushed or released.
    void OnMouse(int pButton, int pState, int pX, int pY, bool isMouseMove/* = false*/);
    void OnMouseMove(int pX,int pY);
    // Call this method when mouse is moved.
    void OnMouseMotion(int pX, int pY);
    // Call this method when timer is finished, for animation display.
    void OnTimerClick() const;

    // Methods for creating menus.
    // Get all the cameras in current scene, including producer cameras.
    const FbxArray<FbxNode *> & GetCameraArray() const { return mCameraArray; }
    // Get all the animation stack names in current scene.
    const FbxArray<FbxString *> & GetAnimStackNameArray() const { return mAnimStackNameArray; }
    // Get all the pose in current scene.
    const FbxArray<FbxPose *> & GetPoseArray() const { return mPoseArray; }

    // The input index is corresponding to the array returned from GetAnimStackNameArray.
    bool SetCurrentAnimStack(int pIndex);
    // Set the current camera with its name.
    bool SetCurrentCamera(const char * pCameraName);
    // The input index is corresponding to the array returned from GetPoseArray.
    bool SetCurrentPoseIndex(int pPoseIndex);
    // Set the currently selected node from external window system.
    void SetSelectedNode(const char * nodeName);
    // Set the shading mode, wire-frame or shaded.
    void SetShadingMode(ShadingMode pMode);

    // Pause the animation.
    void SetPause(bool pPause) { mPause = pPause; }
    // Check whether the animation is paused.
    bool GetPause() const { return mPause; }

    enum CameraZoomMode
    {
        ZOOM_FOCAL_LENGTH,
        ZOOM_POSITION
    };
    CameraZoomMode  GetZoomMode()        { return mCameraZoomMode; }
    void            SetZoomMode( CameraZoomMode pZoomMode);

    // add custom function
    /**
     * @brief draw   绘制3D模型
     */
    void draw (FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,
               FbxAMatrix& pParentGlobalPosition, FbxPose* pPose,
               ShadingMode pShadingMode);
    /**
     * @brief 给定父节点，递归查询包含三维点(x,y)的节点
     * @param 父节点
     * @param opengl三维坐标x
     * @param opengl三维坐标y
     * @return 返回符合条件的子节点
     */
    FbxNode* findNode (FbxNode *node, double x, double y,double z);

    /**
     * @brief 查找节点
     * @param 左上角屏幕坐标X
     * @param 左上角屏幕坐标Y
     * @return 返回找到的子节点
     */
    FbxNode* queryNode (int winx, int winy);

    /**
     * @brief setAxisNodeVisiable 设置坐标轴是否可见
     * @param bVisiable           true表示可见，否则不可见
     */
    void setAxisNodeVisiable(bool bVisiable);

    /**
     * @brief isAxisNode 判定节点是否属于坐标轴节点
     * @param pNode      待判定节点
     * @return  坐标轴节点为true,否则false
     */
    bool isAxisNode (FbxNode* pNode);

    /**
     * @brief 鼠标滚动事件
     * @param 鼠标滚轮方向
     * @param 鼠标滚轮速度
     */
    void OnMouseWheel(int dir, int speed = 10);              // shenbin 2020-4-7

    /**
     * @brief 左上角屏幕坐标转化opengl三维坐标
     * @param 左上角屏幕坐标X
     * @param 左上角屏幕坐标Y
     * @param opengl三维坐标x
     * @param opengl三维坐标y
     * @param opengl三维坐标z
     * @return true表示转换成功，false表示转换失败
     */
    bool screenToGlobalTransform (int winx, int winy, double& x, double& y, double& z);

    /**
     * @brief opengl三维坐标转化为左上角屏幕坐标
     * @param opengl三维坐标x
     * @param opengl三维坐标y
     * @param opengl三维坐标z
     * @param 左上角屏幕坐标X
     * @param 左上角屏幕坐标Y
     * @param 左上角屏幕坐标Z
     * @return true表示转换成功，false表示转换失败
     */
    bool globalToScreenTransform (double x, double y, double z, int& winx, int& winy, int& winz);

    /**
     * @brief setServoAngle  设置舵机角度
     * @param name           舵机名称
     * @param targetAngle    目标角度
     * @param axisName       旋转坐标轴名称
     * @param axisName       节点是否选中状态，默认选中
     * @return 设置是否成功
     */
    bool setServoAngle(const FbxString& name, double targetAngle, const FbxString& axisName, bool bSelectNode = true);

    /**
     * @brief setServoOffsetAngle  设置舵机角度
     * @param name           舵机名称
     * @param offsetAngle    偏移角度
     * @param axisName       旋转坐标轴名称
     * @return 设置是否成功
     */
    bool setServoOffsetAngle(const FbxString& name, double offsetAngle, const FbxString& axisName);

    /**
     * @brief getServoAngle  获得舵机角度
     * @param name           舵机名称
     * @param axisName       旋转坐标轴名称
     * @return 舵机角度
     */
    double getServoAngle (const FbxString& name, const FbxString& axisName);

    /**
     * @brief range_angle_180 角度换算到[-180,180]区间
     * @param angle  源角度
     * @return [-180, 180]区间角度
     */
    double range_angle_180 (double& angle);

    /**
     * @brief limitRationAngle 限制旋转角度值
     * @param name  节点名称
     * @param angle 节点目标旋转角度
     * @return  返回处理之后的旋转角度，方法如下：
     *          1、设置角度 = 目标角度 > 最大角度 ? 最大角度 : 目标角度;
     *          2、设置角度 = 目标角度 < 最小角度 ? 最小角度 : 目标角度
     */
    double limitRationAngle (const char* name, double& angle);
    const char * getSelectFbxNodeName();
    /**
     * @brief setServoAngleChangeCallBack 设置舵机角度改变的回调函数
     * @param CallBack 回调函数
     */
    void setServoAngleChangeCallBack (std::function<void(const char*, double)> CallBack) {
        mServoAngleChangeCallBack = CallBack;
    }

    /**
     * @brief getServoAxisNameCallBack 获取节点可旋转轴名称
     * @param CallBack 回调函数
     */
    void getServoAxisNameCallBack (std::function<std::string(const char*)> CallBack) {
        mGetAxisCallBack = CallBack;
    }

    void setMouseMoveGetAxisCallBack(std::function<std::string(const char*)> CallBack){
        mMouseMoveGetAxisCallBack = CallBack;
    }

    /**
     * @brief getServoRotationRangeCallBack 获取舵机可旋转范围
     * @param CallBack 回调函数
     */
    void getServoRotationRangeCallBack (std::function<bool(const char*, double&, double&)> CallBack) {
        mGetNodeRotationRangeCallBack = CallBack;
    }

private:
    // Display information about current status in the left-up corner of the window.
    void DisplayWindowMessage();
    // Display a X-Z grid.
    void DisplayGrid(const FbxAMatrix & pTransform);

    enum CameraStatus
    {
        CAMERA_NOTHING,
        CAMERA_ORBIT,
        CAMERA_ZOOM,
        CAMERA_PAN
    };

    const char * mFileName;
    mutable Status mStatus;
    mutable FbxString mWindowMessage;

    FbxManager * mSdkManager;
    FbxScene * mScene;
    FbxImporter * mImporter;
    FbxAnimLayer * mCurrentAnimLayer;
    FbxNode * mSelectedNode;    // 机器人选择的节点
    FbxNode * mHoverNode;       // 鼠标滑过的节点 shenbin 2020-04-24
    FbxNode * mAxisNode;        // 坐标轴选择的节点

    /**< 参数依次为舵机在3D模型中的名称，舵机变化之后的角度值 */
    std::function<void(const char*, double)> mServoAngleChangeCallBack;
    /**< 参数为舵机节点名称, 返回舵机转动坐标轴， */
    std::function<std::string(const char*)>  mGetAxisCallBack;

    /** 鼠标移动到机器人某个部位时，返回该部位的名称**/
    std::function<std::string(const char*)> mMouseMoveGetAxisCallBack;

    /**< 参数依次为舵机节点名称，最小值，最大值 */
    std::function<bool(const char*, double&, double&)> mGetNodeRotationRangeCallBack;

    int mPoseIndex;
    FbxArray<FbxString*> mAnimStackNameArray;
    FbxArray<FbxNode*> mCameraArray;
    FbxArray<FbxPose*> mPoseArray;

    mutable FbxTime mFrameTime, mStart, mStop, mCurrentTime;
    mutable FbxTime mCache_Start, mCache_Stop;

    // Data for camera manipulation
    mutable int mLastX, mLastY;
    mutable FbxVector4 mCamPosition, mCamCenter;
    mutable double mRoll;
    mutable CameraStatus mCameraStatus;

    bool mPause;
    ShadingMode mShadingMode;
    bool mSupportVBO;

    //camera zoom mode
    CameraZoomMode mCameraZoomMode;

    int mWindowWidth, mWindowHeight;
    // Utility class for draw text in OpenGL.
    DrawText * mDrawText;
};

// Initialize GLEW, must be called after the window is created.
bool InitializeOpenGL();

namespace Custom {

template <typename T>
inline T mAbs(const T &t)
{
    return t >= 0 ? t : -t;
}

template <typename T>
inline const T &mMin(const T &a, const T &b)
{
    return (a < b) ? a : b;
}

static inline bool mFuzzyCompare(double p1, double p2)
{
    return (mAbs(p1 - p2) * 1000000000000. <= mMin(mAbs(p1), mAbs(p2)));
}

// custom class Line
#ifndef M_2PI
#define M_2PI 6.28318530717958647692528676655900576
#endif

/**
 * @brief 重写Line类，目的是不依赖于Qt平台
 */
class LineF
{
public:
    LineF () : _x1(0.0f), _y1(0.0f), _x2(0.0f), _y2(0.0f) {}
    LineF (double x1, double y1, double x2, double y2) : _x1(x1), _y1(y1), _x2(x2), _y2(y2){}
    ~LineF () {}

public:
    double angle() const;
    double angleTo (const LineF& l) const;
    double dx() const { return _x2 - _x1; }
    double dy() const { return _y2 - _y1; }
    bool isNull() const;

private:
    double _x1{0.0f};
    double _y1{0.0f};
    double _x2{0.0f};
    double _y2{0.0f};
};
} // end Custom

#endif // _SCENE_CONTEXT_H

