#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#if defined(ANDROID_NDK) || defined(__BORLANDC__) || defined(__QNXNTO__)
#   include <ctype.h>
#   include <limits.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#	if defined(__PS3__)
#		include <stddef.h>
#	endif
#   include <map>
#   include <memory.h>
#else
#   include <cctype>
#   include <climits>
#   include <cstdio>
#   include <cstdlib>
#   include <cstring>
#   include <map>
#   include <memory>
#   include <set>
#endif

#ifdef _WIN32
#   ifdef BEHAVIOR_EXPORT
#       define BEHAVIOR_LIB __declspec(dllexport)
#   elif defined(BEHAVIOR_IMPORT)
#       define BEHAVIOR_LIB __declspec(dllimport)
#   else
#       define BEHAVIOR_LIB
#   endif
#elif __GNUC__ >= 4
#   define BEHAVIOR_LIB __attribute__((visibility("default")))
#else
#   define BEHAVIOR_LIB
#endif

namespace tinyxml2
{
class XMLElement;
class XMLDocument;
}

namespace BEH
{

#define DEFAULT_ANGLE 0               /**< 缺省舵机角度 */
#define DEFAULT_FRAME_RATE 20         /**< 缺省的帧率 */
#define DEFAULT_DIVISION 50           /**< 缺省的分度值 */

#define BEHCPP_DISABLE_COPY(...) \
  __VA_ARGS__(const __VA_ARGS__ &) = delete; \
  __VA_ARGS__ & operator=(const __VA_ARGS__ &) = delete;

enum class ServoId : int {
    EM_SERVO_UNKNOW = 0,
    EM_SERVO_1,
    EM_SERVO_2,
    EM_SERVO_3,
    EM_SERVO_4,
    EM_SERVO_5,
    EM_SERVO_6,
    EM_SERVO_7,
    EM_SERVO_8,
    EM_SERVO_9,
    EM_SERVO_10,
    EM_SERVO_11,
    EM_SERVO_12,
    EM_SERVO_13,
    EM_SERVO_14,
    EM_SERVO_15,
    EM_SERVO_16,
    EM_SERVO_17,
    EM_SERVO_18,
    EM_SERVO_19,
    EM_SERVO_20,
    EM_SERVO_21,
    EM_SERVO_22,
    EM_SERVO_23,
    EM_SERVO_24
};

enum class RunMode : int
{
    EM_RUN_MODE_UNKNOW = 0,
    EM_RUN_MODE_UNIFORM_SPEED,      /**< 匀速 */
    EM_RUN_MODE_CURVE_SPEED         /**< 曲线 */
};

enum class ExpressionFileType : int
{
    EM_EXPRESSION_TYPE_UNKNOW = 0,
    EM_EXPRESSION_TYPE_GIF,         /**< gif */
    EM_EXPRESSION_TYPE_LOTTIE,      /**< lottie draw */
    EM_EXPRESSION_TYPE_SVG,
    EM_EXPRESSION_TYPE_PNG,
};

enum class LayerType : int
{
    EM_LAYER_TYPE_UNKNOW = 0,
    EM_LAYER_TYPE_ACTION,           /**< 动作层 */
    EM_LAYER_TYPE_MUSIC,            /**< 音乐层 */
    EM_LAYER_TYPE_EXPRESSION,       /**< 表情层 */
    EM_LAYER_TYPE_LED,               /**< led层 */
    EM_LAYER_TYPE_HEAD,
    EM_LAYER_TYPE_HAND,
    EM_LAYER_TYPE_FOOT
};

enum class ActionLevel : int
{
    EM_ACTION_LEVEL_UNKNOW = 0,
    EM_ACTION_LEVEL_NORMAL,                /**< 普通动作 */
    EM_ACTION_LEVEL_DANGER                 /**< 高危动作 */
};

class BEHAVIOR_LIB Servo
{
public:
    explicit Servo () : _targetAngle(DEFAULT_ANGLE) {}
    explicit Servo (float targetAngle) : _targetAngle(targetAngle) {}
    ~Servo () {}

public:
    void setTargetAngle (float targetAngle) { _targetAngle = targetAngle; }
    float getTargetAngle () const           { return _targetAngle; }

    void setRunTime (int runTime)            { _runTime = runTime; }
    int  getRunTime () const                 { return _runTime; }

    void setRunMode (RunMode runMode)        { _runMode = runMode; }
    RunMode  getRunMode () const             { return _runMode; }

private:
    BEHCPP_DISABLE_COPY(Servo)
    float   _targetAngle{DEFAULT_ANGLE};
    int     _runTime{0};
    RunMode _runMode{RunMode::EM_RUN_MODE_UNIFORM_SPEED};
};

class BEHAVIOR_LIB Frame
{
public:
    explicit Frame ();
    ~Frame ();

public:
    void setInterval (int interval) { _interval = interval; }
    int  getInterval () const       { return _interval; }

    void addServo (int id, std::shared_ptr<Servo> pServo);
    void delServo (int id);
    std::shared_ptr<Servo> queryServo (int id);
    int servoCount ();
    std::map<int, std::shared_ptr<Servo>>* getServos ();
    RunMode queryRunMode();

private:
    BEHCPP_DISABLE_COPY(Frame)
    int _interval{0};
    std::map<int, std::shared_ptr<Servo>> _servos;
};

class BEHAVIOR_LIB MusicItem
{
public:
    explicit MusicItem () {}

public:
    void setStartTime (int startTime) { _startTime = startTime; }
    int  getStartTime () const        { return _startTime; }

    void setPlayTime (int playTime)   { _playTime = playTime; }
    int  getPlayTime () const         { return _playTime; }

    void setPath (const std::string& path) { _path = path; }
    std::string getPath () const           { return _path; }

private:
    BEHCPP_DISABLE_COPY(MusicItem)
    int _startTime{0};
    int _playTime{0};
    std::string _path {""};
};

class BEHAVIOR_LIB ExpressionItem
{
public:
    explicit ExpressionItem () {}

public:
    void setStartTime (int startTime) { _startTime = startTime; }
    int  getStartTime () const        { return _startTime; }

    void setPlayTime (int playTime)   { _playTime = playTime; }
    int  getPlayTime () const         { return _playTime; }

    void setPath (const std::string& path) { _path = path; }
    std::string getPath () const           { return _path; }

    void setType (ExpressionFileType type) { _type = type; }
    ExpressionFileType  getType () const   { return _type; }

    void setLoops (int loops)          { _loops = loops; }
    int  getLoops () const             { return _loops; }

private:
    BEHCPP_DISABLE_COPY(ExpressionItem)
    int _startTime{0};
    int _playTime{0};
    std::string _path {""};
    ExpressionFileType _type {ExpressionFileType::EM_EXPRESSION_TYPE_UNKNOW};
    int _loops{1};
};

class BEHAVIOR_LIB Layer
{
public:
    explicit Layer () {}
    explicit Layer (const std::string& name) : _name(name) {}
    virtual ~Layer ();

public:
    void setName (const std::string& name) { _name = name; }
    std::string getName () const           { return _name; }

private:
    BEHCPP_DISABLE_COPY(Layer)
    std::string _name{""};
};

class BEHAVIOR_LIB ActionLayer : public Layer
{
public:
    explicit ActionLayer ();
    virtual~ ActionLayer ();

public:
    void addFrame (int id, std::shared_ptr<Frame> frame);
    void delFrame (int id);
    std::shared_ptr<Frame> queryFrame (int id);
    int frameCount ();
    std::map<int, std::shared_ptr<Frame>>* getFrames ();
    std::map<std::string, std::set<int>>* getGroups ();
    /**< 更新所有帧的interval */
    void updateFrameInterval ();
    void addGroup(std::string name, std::set<int> frames);

private:
    std::map<int, std::shared_ptr<Frame>> _frames;
    std::map<std::string, std::set<int>> _groups;
};

class BEHAVIOR_LIB MusicLayer : public Layer
{
public:
    explicit MusicLayer ();
    virtual~ MusicLayer ();

public:
    void addMusicItem (int id, std::shared_ptr<MusicItem> musicItem);
    void delMusicItem (int id);
    std::shared_ptr<MusicItem> queryMusicItem (int id);
    int musicCount ();
    std::map<int, std::shared_ptr<MusicItem>>* getMusicItems ();
    /**< 更新所有音乐的开始时间 */
    void updateMusicItemStartTime ();

private:
    std::map<int, std::shared_ptr<MusicItem>> _musicitems;
};

class BEHAVIOR_LIB ExpressionLayer : public Layer
{
public:
    explicit ExpressionLayer ();
    virtual~ ExpressionLayer ();

public:
    void addExpressionItem (int id, std::shared_ptr<ExpressionItem> expressionitem);
    void delExpressionItem (int id);
    std::shared_ptr<ExpressionItem> queryExpressionItem (int id);
    int expressionCount ();
    std::map<int, std::shared_ptr<ExpressionItem>>* getExpressionItems ();
    /**< 更新所有表情的开始时间 */
    void updateExpressionItemStartTime ();

private:
    std::map<int, std::shared_ptr<ExpressionItem>> _expressionitems;
};

class BEHAVIOR_LIB Label
{
public:
    explicit Label () {}
    explicit Label (std::string desc) : _desc(desc) {}

public:
    void setDesc (const std::string& desc) { _desc = desc; }
    std::string getDesc () const { return _desc; }

private:
    BEHCPP_DISABLE_COPY(Label)
    std::string _desc{""};
};

class BEHAVIOR_LIB Attributes
{
public:
    explicit Attributes ();
    ~Attributes ();

public:
    void addLabel (int id, std::shared_ptr<Label> label);
    void delLabel (int id);
    std::shared_ptr<Label> queryLabel (int id);
    int labelCount ();
    std::map<int, std::shared_ptr<Label>>* getLabels ();

private:
    BEHCPP_DISABLE_COPY(Attributes)
    std::map<int, std::shared_ptr<Label>> _labels;
};

class BEHAVIOR_LIB Behavior
{
public:
    explicit Behavior();
    ~Behavior ();

public:
    bool loadFile (const char* filename);
    bool saveFile (const char* filename);

protected:
    bool ParseActionLayer (tinyxml2::XMLElement* elem, std::shared_ptr<ActionLayer> pLayer);
    bool ParseMusicLayer (tinyxml2::XMLElement* elem, std::shared_ptr<MusicLayer> pLayer);
    bool ParseExpressionLayer (tinyxml2::XMLElement* elem, std::shared_ptr<ExpressionLayer> pLayer);

    bool createActionLayer(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* elem, std::shared_ptr<ActionLayer> pLayer);
    bool createMusicLayer(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* elem, std::shared_ptr<MusicLayer> pLayer);
    bool createExpressionLayer(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* elem, std::shared_ptr<ExpressionLayer> pLayer);

public:
    /**< 舵机相关接口 */
    void addServo (int nFrameId, int nServoId, std::shared_ptr<Servo> pServo);
    void delServo (int nFrameId, int nServoId);
    std::shared_ptr<Servo> queryServo (int nFrameId, int nServoId);
    std::map<int, std::shared_ptr<Servo>>* getServos(int nFrameId);

    /**< 音乐相关接口 */
    void addMusic (int nItemId, std::shared_ptr<MusicItem> pMusic);
    void delMusic (int nItemId);
    std::shared_ptr<MusicItem> queryMusic (int nItemId);
    std::map<int, std::shared_ptr<MusicItem>>* getMusics();

    /**< 表情相关接口 */
    void addExpression (int nItemId, std::shared_ptr<ExpressionItem> pExpression);
    void delExpression (int nItemId);
    std::shared_ptr<ExpressionItem> queryExpression (int nItemId);
    std::map<int, std::shared_ptr<ExpressionItem>>* getExpressions();

    /**< LED灯相关接口 */

    /**< 层相关接口 */
    void addLayer (int id, std::shared_ptr<Layer> pLayer);
    void delLayer (int id);
    std::shared_ptr<Layer> queryLayer (int id);
    std::shared_ptr<ActionLayer> queryActionLayer();
    std::shared_ptr<ActionLayer> queryHeadLayer();
    std::shared_ptr<ActionLayer> queryHandLayer();
    std::shared_ptr<ActionLayer> queryFootLayer();
    std::shared_ptr<MusicLayer> queryMusicLayer();
    std::shared_ptr<ExpressionLayer> queryExpressLayer();
    std::map<int, std::shared_ptr<Layer>>* getLayers ();

    /**< 属性相关接口 */
    void addLabel (int location, std::shared_ptr<Label> pLabel);
    void delLabel (int location);
    std::shared_ptr<Label> queryLabel (int location);
    std::map<int, std::shared_ptr<Label>>* getLabels();

private:
    /**
     * @brief frameRateToDivison  帧率转换分度值(最小刻度值、帧间隔)
     * @param frameRate           帧率
     * @return                    分度值(最小刻度值、帧间隔)
     */
    int frameRateToDivison (int frameRate);

public:
    void setVersion (const std::string& version) { _version = version; }
    std::string getVersion () const { return _version; }

    void setActionLevel (ActionLevel actionLevel) { _actionLevel = actionLevel; }
    ActionLevel getActionLevel () const { return _actionLevel; }

    void setFrameRate (int frameRate);
    int  getFrameRate () const { return _frameRate; }

    int  getDivision () const { return _division; }

    void setStartTime (int startTime) { _startTime = startTime; }
    int  getStartTime () const  { return _startTime; }

    void setRunTime (int runTime) { _runTime = runTime; }
    int  getRunTime () const { return _runTime; }

private:
    BEHCPP_DISABLE_COPY(Behavior)
    std::string _version {u8"1.0.0"};
    ActionLevel _actionLevel{ActionLevel::EM_ACTION_LEVEL_UNKNOW};
    int _frameRate{DEFAULT_FRAME_RATE};
    int _division{DEFAULT_DIVISION};
    int _startTime {0};
    int _runTime {0};
    std::map<int, std::shared_ptr<Layer>> _layers;
    std::shared_ptr<Attributes> _attributes{nullptr};
};

} // namespace BEH

#endif // BEHAVIOR_H
