#ifndef FBX_H
#define FBX_H
#include "global.h"
#include <functional>

class SceneContext;
class FBX_LIB Fbx
{
public:
    explicit Fbx(const char* modelName, const char* textureName);
    ~Fbx ();

    enum Status
    {
        UNLOADED,               // Unload file or load failure;
        MUST_BE_LOADED,         // Ready for loading file;
        MUST_BE_REFRESHED,      // Something changed and redraw needed;
        REFRESHED               // No redraw needed.
    };

    Status GetStatus () const;
    void SetStatus(Status status);

    enum Button
    {
        LEFT_BUTTON = 0,
        MIDDLE_BUTTON,
        RIGHT_BUTTON
    };

    enum State
    {
        BUTTON_DOWN = 0,
        BUTTON_UP = 1,
    };

    enum WheelDir
    {
        WHEEL_UP = -1,
        WHEEL_DOWN = 1
    };

    enum CameraPerspective
    {
        CAMERA_PERSPECT = 1,
        CAMERA_FRONT,
        CAMERA_BACK,
        CAMERA_TOP,
        CAMERA_BOTTOM,
        CAMERA_LEFT,
        CAMERA_RIGHT,
        CAMERA_RIGHT_45,
    };

    enum AxisType
    {
        AXIS_X = 1,
        AXIS_Y,
        AXIS_Z
    };


public:
    bool LoadFile ();
    // Call this method when redraw is needed.
    bool OnDisplay();
    unsigned int GetFrameMillTime () const;

    /**
     * @brief 事件相关接口
     * @return
     */
public:
    // Call this method when window size is changed.
    void OnReshape(int pWidth, int pHeight);
    // Call this method when keyboard input occurs.
    void OnKeyboard(unsigned char pKey);
    // Call this method when mouse buttons are pushed or released.
    void OnMouse(Button pButton, State pState, int pX, int pY, bool isMouseMove = false);

    void onMouseMove(int pX,int pY);

    // Call this method when mouse is moved.
    void OnMouseMotion(int pX, int pY);
    // Call this method when timer is finished, for animation display.
    void OnTimerClick() const;
    void OnMouseWheel(WheelDir dir, int speed = 10);

public:
    /**
     * @brief 设置运行动画，默认第一个
     * @return
     */
    void SetAnimStackSelection ();

    /**
     * @brief 设置摄像机视角
     * @param 视角参数，CameraPerspective
     * @return
     */
    void SetCameraPerspective (CameraPerspective dir);

    /**
     * @brief 屏幕坐标是否存在节点
     * @param screenX
     * @param screenY
     * @return
     */
    bool existNode (int screenX, int screenY);

    /**
     * @brief setServoAngle 设置舵机角度
     * @param name          舵机名称
     * @param targetAngle   目标角度
     * @param dir           旋转方向(绕着坐标轴旋转)
     * @param bSelectNode   节点是否设置选中状态，默认选中
     * @return
     */
    bool setServoAngle (const char* name, double targetAngle, AxisType dir, bool bSelectNode /*= true*/);

    /**
     * @brief getServoAngle 获取舵机角度
     * @param name          舵机名称
     * @return 舵机角度
     */
    double getServoAngle (const char* name, AxisType dir);

    std::string getAxisName(AxisType dir);

    /**
     * @brief setServoAngleChangeCallBack 设置舵机角度变化回调
     * @param CallBack 回调函数可调用对象
     */
    void  setServoAngleChangeCallBack (
            std::function<void(const char *, double)> CallBack
            );

    void  getServoAxisNameCallBack (
            std::function<std::string(const char *)> CallBack
            );
    void setMouseMoveAxisNameCallBack(std::function<std::string(const char *)> CallBack);
    void getServoRotationRangeCallBack (
            std::function<bool(const char *, double &, double &)> CallBack
            );
    void setSelectedNode(const char* nodeName);
    const char*  getSelectorFbxNode();
protected:
   SceneContext* m_scene{nullptr};
};

#endif // FBX_H
