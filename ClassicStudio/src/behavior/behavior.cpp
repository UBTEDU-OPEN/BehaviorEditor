#include "behavior.h"
#include <math.h>
#include "tinyxml2.h"
#include <iostream>
using namespace tinyxml2;

namespace BEH
{

Frame::Frame ()
{
    _servos.clear();
}

Frame::~Frame ()
{
    _servos.clear();
}

void Frame::addServo (int id, std::shared_ptr<Servo> pServo)
{
    _servos[id] = pServo;
}

void Frame::delServo (int id)
{
    auto iter = _servos.find(id);
    if ( iter != _servos.end() ) {
        _servos.erase(iter);
    }
}
std::shared_ptr<Servo> Frame::queryServo (int id)
{
    auto iter = _servos.find(id);
    if ( iter != _servos.end() ) {
        return iter->second;
    }
    return nullptr;
}

int Frame::servoCount ()
{
    return (int)_servos.size();
}

std::map<int, std::shared_ptr<Servo>>* Frame::getServos ()
{
    return &_servos;
}

RunMode Frame::queryRunMode()
{
    if(_servos.empty()){
        return RunMode::EM_RUN_MODE_UNKNOW;
    }
    auto cit = _servos.cbegin();
    auto servo = cit->second;
    if(servo.get()){
      return servo->getRunMode();
    }
    return RunMode::EM_RUN_MODE_UNKNOW;
}

Layer::~Layer (){}

ActionLayer::ActionLayer () :
    Layer("action")
{
    _frames.clear();
}

ActionLayer::~ActionLayer ()
{
    _frames.clear();
}

void ActionLayer::addFrame (int id, std::shared_ptr<Frame> frame)
{
    _frames[id] = frame;
}

void ActionLayer::delFrame (int id)
{
    auto iter = _frames.find(id);
    if ( iter != _frames.end() ) {
        _frames.erase(iter);
    }
}

std::shared_ptr<Frame> ActionLayer::queryFrame (int id)
{
    auto iter = _frames.find(id);
    if ( iter != _frames.end() ) {
        return iter->second;
    }
    return nullptr;
}

int ActionLayer::frameCount()
{
    return (int)_frames.size();
}

std::map<int, std::shared_ptr<Frame>>* ActionLayer::getFrames ()
{
    return &_frames;
}

std::map<std::string, std::set<int>>* ActionLayer::getGroups ()
{
    return &_groups;
}

void ActionLayer::updateFrameInterval ()
{
    int nLastIndex = 0;
    auto it = _frames.begin();
    for ( ; it != _frames.end(); it ++ ) {
        int nId = it->first;
        auto frame = it->second;
        frame->setInterval(nId - nLastIndex);
        nLastIndex = nId;
    }
}

void ActionLayer::addGroup(std::string name, std::set<int> frames)
{
    _groups[name] = frames;
}

MusicLayer::MusicLayer () :
    Layer("music")
{
    _musicitems.clear();
}

MusicLayer::~MusicLayer ()
{
    _musicitems.clear();
}

void MusicLayer::addMusicItem (int id, std::shared_ptr<MusicItem> musicItem)
{
    _musicitems[id] = musicItem;
}

void MusicLayer::delMusicItem (int id)
{
    auto iter = _musicitems.find(id);
    if ( iter != _musicitems.end() ) {
        _musicitems.erase(iter);
    }
}

std::shared_ptr<MusicItem> MusicLayer::queryMusicItem (int id)
{
    auto iter = _musicitems.find(id);
    if ( iter != _musicitems.end() ) {
        return iter->second;
    }
    return nullptr;
}

int MusicLayer::musicCount ()
{
    return (int)_musicitems.size();
}

std::map<int, std::shared_ptr<MusicItem>>* MusicLayer::getMusicItems ()
{
    return &_musicitems;
}

void MusicLayer::updateMusicItemStartTime ()
{
    int nLastIndex = 0;
    int nLastPlayTime = 0;
    auto it = _musicitems.begin();
    for ( ; it != _musicitems.end(); it ++ ) {
        int nId = it->first;
        auto pItem = it->second;
        pItem->setStartTime(nId - nLastIndex - nLastPlayTime);
        nLastPlayTime = pItem->getPlayTime ();
        nLastIndex = nId;
    }
}

ExpressionLayer::ExpressionLayer () :
    Layer("expression")
{
    _expressionitems.clear();
}

ExpressionLayer::~ ExpressionLayer ()
{
    _expressionitems.clear();
}

void ExpressionLayer::addExpressionItem (int id, std::shared_ptr<ExpressionItem> expressionitem)
{
    _expressionitems[id] = expressionitem;
}

void ExpressionLayer::delExpressionItem (int id)
{
    auto iter = _expressionitems.find(id);
    if ( iter != _expressionitems.end() ) {
        _expressionitems.erase(iter);
    }
}

std::shared_ptr<ExpressionItem> ExpressionLayer::queryExpressionItem (int id)
{
    auto iter = _expressionitems.find(id);
    if ( iter != _expressionitems.end() ) {
        return iter->second;
    }
    return nullptr;
}

int ExpressionLayer::expressionCount ()
{
    return (int)_expressionitems.size();
}

std::map<int, std::shared_ptr<ExpressionItem>>* ExpressionLayer::getExpressionItems ()
{
    return &_expressionitems;
}

void ExpressionLayer::updateExpressionItemStartTime ()
{
    int nLastIndex = 0;
    int nLastPlayTime = 0;
    auto it = _expressionitems.begin();
    for ( ; it != _expressionitems.end(); it ++ ) {
        int nId = it->first;
        auto pItem = it->second;
        pItem->setStartTime(nId - nLastIndex - nLastPlayTime);
        nLastPlayTime = pItem->getPlayTime ();
        nLastIndex = nId;
    }
}

Attributes::Attributes ()
{
    _labels.clear();
}

Attributes::~Attributes ()
{
    _labels.clear();
}

void Attributes::addLabel (int id, std::shared_ptr<Label> label)
{
    _labels[id] = label;
}

void Attributes::delLabel (int id)
{
    auto iter = _labels.find(id);
    if ( iter != _labels.end() ) {
        _labels.erase(iter);
    }
}

std::shared_ptr<Label> Attributes::queryLabel (int id)
{
    auto iter = _labels.find(id);
    if ( iter != _labels.end() ) {
        return iter->second;
    }
    return nullptr;
}

int Attributes::labelCount ()
{
    return (int)_labels.size();
}

std::map<int, std::shared_ptr<Label> > *Attributes::getLabels()
{
    return &_labels;
}

Behavior::Behavior()
{
    _layers.clear();
    addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_ACTION),
             std::make_shared<ActionLayer>());
    addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_MUSIC),
             std::make_shared<MusicLayer>());
    addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_EXPRESSION),
             std::make_shared<ExpressionLayer>());
    addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_HEAD),
             std::make_shared<ActionLayer>());
    addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_HAND),
             std::make_shared<ActionLayer>());
    addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_FOOT),
             std::make_shared<ActionLayer>());
}

Behavior::~Behavior ()
{
    _layers.clear();
}

bool Behavior::loadFile (const char* filename)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(filename);
    if (err != XML_SUCCESS ) {
        std::cerr << err << std::endl;
        return false;
    }
    XMLElement* pRoot = doc.RootElement();
    if ( pRoot == nullptr ){
        return false;
    }

    auto version = pRoot->Attribute("version");
    if ( version != nullptr ) {
        setVersion(version);
    }

    auto actionLevel = pRoot->Attribute("actionLevel");
    if ( actionLevel != nullptr ) {
        setActionLevel(static_cast<ActionLevel>(atoi(actionLevel)));
    }

    auto frameRate = pRoot->Attribute("frameRate");
    if ( frameRate != nullptr ) {
        setFrameRate(atoi(frameRate));
    }

    auto startTime = pRoot->Attribute("startTime");
    if ( startTime != nullptr ) {
        setStartTime(atoi(startTime));
    }

    auto runTime = pRoot->Attribute("runTime");
    if ( runTime != nullptr ) {
        setRunTime(atoi(runTime));
    }

    XMLElement* layer = pRoot->FirstChildElement("Layer");
    while ( layer != nullptr ) {
        auto id = layer->Attribute("id");
        if ( id != nullptr ) {
            int nId = atoi(id);
            std::shared_ptr<Layer> pLayer = _layers[nId];
            switch (nId) {
            case static_cast<int>(LayerType::EM_LAYER_TYPE_ACTION):
                ParseActionLayer(layer, std::dynamic_pointer_cast<ActionLayer>(pLayer));
                break;
            case static_cast<int>(LayerType::EM_LAYER_TYPE_MUSIC):
                ParseMusicLayer(layer, std::dynamic_pointer_cast<MusicLayer>(pLayer));
                break;
            case static_cast<int>(LayerType::EM_LAYER_TYPE_EXPRESSION):
                ParseExpressionLayer(layer, std::dynamic_pointer_cast<ExpressionLayer>(pLayer));
                break;
            case static_cast<int>(LayerType::EM_LAYER_TYPE_LED):
                break;
            case static_cast<int>(LayerType::EM_LAYER_TYPE_HEAD):
                ParseActionLayer(layer, std::dynamic_pointer_cast<ActionLayer>(pLayer));
                break;
            case static_cast<int>(LayerType::EM_LAYER_TYPE_HAND):
                ParseActionLayer(layer, std::dynamic_pointer_cast<ActionLayer>(pLayer));
                break;
            case static_cast<int>(LayerType::EM_LAYER_TYPE_FOOT):
                ParseActionLayer(layer, std::dynamic_pointer_cast<ActionLayer>(pLayer));
                break;
            default:
                break;
            }

            if ( pLayer ) {
                auto name = layer->Attribute("name");
                if ( name != nullptr ) {
                    pLayer->setName(name);
                }

                addLayer(nId, pLayer);
            }
        }

        layer = layer->NextSiblingElement("Layer");
    }

    /**< 解析属性 */
    XMLElement* attrElem = pRoot->FirstChildElement("Attributes");
    if ( attrElem != nullptr ) {
        _attributes = std::make_shared<Attributes>();
        XMLElement* labelsElem = attrElem->FirstChildElement("Labels");
        if ( labelsElem != nullptr ) {
            XMLElement* labelElem = labelsElem->FirstChildElement("Label");
            while (labelElem != nullptr) {
                auto pLabel = std::make_shared<Label>();
                auto desc = labelElem->Attribute("desc");
                if ( desc != nullptr ) {
                    pLabel->setDesc(desc);
                }

                auto location = labelElem->Attribute("location");
                if ( location != nullptr ) {
                    _attributes->addLabel(atoi(location), pLabel);
                }

                labelElem = labelElem->NextSiblingElement("Label");
            }
        }
    }
    return true;
}

bool Behavior::ParseActionLayer (XMLElement* elem, std::shared_ptr<ActionLayer> pLayer)
{
    XMLElement* groupElem = elem->FirstChildElement("Group");
    while( groupElem != nullptr ) {
        auto groupName = groupElem->Attribute("name");
        std::set<int> frames;
        XMLElement* frameElem = groupElem->FirstChildElement("Frame");
        while ( frameElem != nullptr ) {
            auto pFrame = std::make_shared<Frame>();
            auto interval = frameElem->Attribute("interval");
            if ( interval != nullptr ) {
                pFrame->setInterval(atoi(interval));
            }

            std::shared_ptr<Servo> pServo;
            XMLElement* servoElem = frameElem->FirstChildElement("Servo");
            while ( servoElem != nullptr ) {
                pServo = std::make_shared<Servo>();
                auto targetAngle = servoElem->Attribute("targetAngle");
                if ( targetAngle != nullptr ) {
                    pServo->setTargetAngle((float)atof(targetAngle));
                }

                auto runTime = servoElem->Attribute("runTime");
                if ( runTime != nullptr ) {
                    pServo->setRunTime(atoi(runTime));
                }

                auto runMode = servoElem->Attribute("runMode");
                if ( runMode != nullptr ) {
                    pServo->setRunMode(static_cast<RunMode>(atoi(runMode)));
                }

                auto id = servoElem->Attribute("id");
                if ( id != nullptr ) {
                    pFrame->addServo(atoi(id), pServo);
                }
                servoElem = servoElem->NextSiblingElement("Servo");
            }

            if ( pServo != nullptr ) {
                auto location = frameElem->Attribute("location");
                if ( location != nullptr ) {
                    pLayer->addFrame(atoi(location), pFrame);
                    frames.insert(atoi(location));
                }
            }
            frameElem = frameElem->NextSiblingElement("Frame");
        }
        if(frames.size() > 0) {
            pLayer->addGroup(groupName,frames);
        }
        groupElem = groupElem->NextSiblingElement("Group");
    }

    return true;
}

bool Behavior::ParseMusicLayer (XMLElement* elem, std::shared_ptr<MusicLayer> pLayer)
{
    XMLElement* musicElem = elem->FirstChildElement("Item");
    while ( musicElem != nullptr ) {
        auto pItem = std::make_shared<MusicItem>();
        auto startTime = musicElem->Attribute("startTime");
        if ( startTime != nullptr ) {
            pItem->setStartTime(atoi(startTime));
        }

        auto playTime = musicElem->Attribute("playTime");
        if ( playTime != nullptr ) {
            pItem->setPlayTime(atoi(playTime));
        }

        auto path = musicElem->Attribute("path");
        if ( path != nullptr ) {
            pItem->setPath(path);
        }

        auto id = musicElem->Attribute("id");
        if ( id != nullptr ) {
            pLayer->addMusicItem(atoi(id), pItem);
        }

        musicElem = musicElem->NextSiblingElement("Item");
    }
    return true;
}

bool Behavior::ParseExpressionLayer (XMLElement* elem, std::shared_ptr<ExpressionLayer> pLayer)
{
    XMLElement* expressionElem = elem->FirstChildElement("Item");
    while ( expressionElem != nullptr ) {
        auto pItem = std::make_shared<ExpressionItem>();
        auto startTime = expressionElem->Attribute("startTime");
        if ( startTime != nullptr ) {
            pItem->setStartTime(atoi(startTime));
        }

        auto playTime = expressionElem->Attribute("playTime");
        if ( playTime != nullptr ) {
            pItem->setPlayTime(atoi(playTime));
        }

        auto path = expressionElem->Attribute("path");
        if ( path != nullptr ) {
            pItem->setPath(path);
        }

        auto type = expressionElem->Attribute("type");
        if ( type != nullptr ) {
            pItem->setType(static_cast<ExpressionFileType>(atoi(type)));
        }

        auto loops = expressionElem->Attribute("loops");
        if ( loops != nullptr ) {
            pItem->setLoops(atoi(loops));
        }

        auto id = expressionElem->Attribute("id");
        if ( id != nullptr ) {
            pLayer->addExpressionItem(atoi(id), pItem);
        }

        expressionElem = expressionElem->NextSiblingElement("Item");
    }
    return true;
}

bool Behavior::saveFile (const char* filename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    doc.InsertEndChild(doc.NewComment(u8"\n"
                                      "    @version        协议版本\n"
                                      "    @actionLevel    动作级别(常规动作=1, 高危动作=2)\n"
                                      "    @frameRate      帧率,单位:f/s\n"
                                      "    @startTime      开始时间(单位:ms)\n"
                                      "    @runTime        运行时间(单位:ms)"
                                      "\n"));
    XMLElement* pRootElem = doc.NewElement("BCL");
    doc.InsertEndChild(pRootElem);
    pRootElem->SetAttribute("version", getVersion().c_str());
    pRootElem->SetAttribute("actionLevel", static_cast<int>(getActionLevel()));
    pRootElem->SetAttribute("frameRate", getFrameRate());
    pRootElem->SetAttribute("startTime", getStartTime());
    pRootElem->SetAttribute("runTime", getRunTime());

    pRootElem->InsertEndChild(doc.NewComment(u8"\n"
                                             "        @id    层ID(动作层=1, 音乐层=2, 表情层=3)\n"
                                             "        @name  层名称"
                                             "\n    "));
    auto it = _layers.begin();
    for (; it != _layers.end(); it ++ ) {
        auto nId = it->first;
        auto pLayer = it->second;
        XMLElement* pLayerElem = doc.NewElement("Layer");
        pRootElem->InsertEndChild(pLayerElem);
        pLayerElem->SetAttribute("id", nId);
        pLayerElem->SetAttribute("name", pLayer->getName().c_str());

        switch (nId) {
        case static_cast<int>(LayerType::EM_LAYER_TYPE_ACTION):
            createActionLayer(&doc, pLayerElem, std::dynamic_pointer_cast<ActionLayer>(pLayer));
            break;
        case static_cast<int>(LayerType::EM_LAYER_TYPE_MUSIC):
            createMusicLayer(&doc, pLayerElem, std::dynamic_pointer_cast<MusicLayer>(pLayer));
            break;
        case static_cast<int>(LayerType::EM_LAYER_TYPE_EXPRESSION):
            createExpressionLayer(&doc, pLayerElem, std::dynamic_pointer_cast<ExpressionLayer>(pLayer));
            break;
        case static_cast<int>(LayerType::EM_LAYER_TYPE_LED):
            break;
        default:
            break;
        }
    }

    /**< 保存属性 */
    if ( _attributes ) {
        int nLabelCnt = _attributes->labelCount();
        if ( nLabelCnt > 0 ) {
            XMLElement* pAttrElem = doc.NewElement("Attributes");
            pRootElem->InsertEndChild(pAttrElem);
            XMLElement* pLabelsElem = doc.NewElement("Labels");
            pAttrElem->InsertEndChild(pLabelsElem);
            auto it = _attributes->getLabels()->begin();
            for ( ; it != _attributes->getLabels()->end(); it ++ ) {
                  int location = it->first;
                  auto pLabel = it->second;
                  XMLElement* pLabelElem = doc.NewElement("Label");
                  pLabelsElem->InsertEndChild(pLabelElem);
                  pLabelElem->SetAttribute("location", location);
                  pLabelElem->SetAttribute("desc", pLabel->getDesc().c_str());
            }
        }
    }

    if ( doc.SaveFile(filename) != XML_SUCCESS ) {
        return false;
    }

    return true;
}

bool Behavior::createActionLayer(XMLDocument* doc, XMLElement* elem, std::shared_ptr<ActionLayer> pLayer)
{
    if ( pLayer->frameCount() == 0 ) {
        return false;
    }

    elem->InsertEndChild(doc->NewComment(u8"\n"
                                         "            @location  关键帧位置\n"
                                         "            @interval  帧间隔时间(此帧与上一帧的间隔时间)"
                                         "\n        "));

    pLayer->updateFrameInterval();

    auto frames = pLayer->getFrames();

    auto it = pLayer->getGroups()->begin();
    for ( ; it != pLayer->getGroups()->end(); it ++ ) {
        std::string groupName = it->first;
        auto frameSet = it->second;
        XMLElement* pGroupElem = doc->NewElement("Group");
        pGroupElem->SetAttribute("name", groupName.c_str());
        elem->InsertEndChild(pGroupElem);

        for(auto frameIndex : frameSet) {
            int nId = frameIndex;
            auto pFrame = (*frames)[frameIndex];
            XMLElement* pFrameElem = doc->NewElement("Frame");
            pGroupElem->InsertEndChild(pFrameElem);
            pFrameElem->SetAttribute("location", nId);
            pFrameElem->SetAttribute("interval", pFrame->getInterval());
            /**< 仅在第一个舵机标注描述信息 */
            if ( frameIndex == pLayer->getFrames()->begin()->first ) {
                pFrameElem->InsertEndChild(doc->NewComment(u8"\n"
                                                           "                @id           舵机ID\n"
                                                           "                @targetAngle  舵机目标角度\n"
                                                           "                @runTime      运行时间\n"
                                                           "                @runMode      运行模式(匀速运动=1, 曲线运动=2)"
                                                           "\n            "));
            }

            auto iter = pFrame->getServos()->begin();
            for ( ; iter != pFrame->getServos()->end(); iter ++ ) {
                int nServoId = iter->first;
                auto pServo = iter->second;
                XMLElement* pServoElem = doc->NewElement("Servo");
                pFrameElem->InsertEndChild(pServoElem);
                pServoElem->SetAttribute("id", nServoId);
                float angle = pServo->getTargetAngle();
                char buf[200]; /**< 此处取值200，原因是tinyxml中SetAttribute中给定的BUF_SIZE=200 */
                sprintf(buf, "%.1f", angle); /**< 控制舵机角度精度为0.1度 */
                pServoElem->SetAttribute("targetAngle", buf);
                pServoElem->SetAttribute("runTime", pServo->getRunTime());
                pServoElem->SetAttribute("runMode", static_cast<int>(pServo->getRunMode()));
            }


        }
    }

    return true;
}

bool Behavior::createMusicLayer(XMLDocument* doc, XMLElement* elem, std::shared_ptr<MusicLayer> pLayer)
{
    if ( pLayer->musicCount() == 0 ) {
        return false;
    }

    elem->InsertEndChild(doc->NewComment(u8"\n"
                                         "            @id         音乐ID\n"
                                         "            @startTime  开始时间\n"
                                         "            @playTime   播放时间\n"
                                         "            @path       音乐文件路径"
                                         "\n        "));
    pLayer->updateMusicItemStartTime();
    auto it = pLayer->getMusicItems()->begin();
    for ( ; it != pLayer->getMusicItems()->end(); it ++ ) {
        int nId = it->first;
        auto pItem = it->second;
        XMLElement* pMusicElem = doc->NewElement("Item");
        elem->InsertEndChild(pMusicElem);
        pMusicElem->SetAttribute("id", nId);
        pMusicElem->SetAttribute("startTime", pItem->getStartTime());
        pMusicElem->SetAttribute("playTime", pItem->getPlayTime());
        pMusicElem->SetAttribute("path", pItem->getPath().c_str());
    }

    return true;
}

bool Behavior::createExpressionLayer(XMLDocument* doc, XMLElement* elem, std::shared_ptr<ExpressionLayer> pLayer)
{
    if ( pLayer->expressionCount() == 0 ) {
        return false;
    }
    elem->InsertEndChild(doc->NewComment(u8"\n"
                                         "            @id         表情ID\n"
                                         "            @startTime  开始时间\n"
                                         "            @playTime   播放时间\n"
                                         "            @path       表情文件路径\n"
                                         "            @type       动画类型(gif=1,lottie=2,svg=3,png=4)\n"
                                         "            @loops      循环播放次数"
                                         "\n        "));
    pLayer->updateExpressionItemStartTime();
    auto it = pLayer->getExpressionItems()->begin();
    for ( ; it != pLayer->getExpressionItems()->end(); it ++ ) {
        int nId = it->first;
        auto pItem = it->second;
        XMLElement* pExpressionElem = doc->NewElement("Item");
        elem->InsertEndChild(pExpressionElem);
        pExpressionElem->SetAttribute("id", nId);
        pExpressionElem->SetAttribute("startTime", pItem->getStartTime());
        pExpressionElem->SetAttribute("playTime", pItem->getPlayTime());
        pExpressionElem->SetAttribute("path", pItem->getPath().c_str());
        pExpressionElem->SetAttribute("type", static_cast<int>(pItem->getType()));
        pExpressionElem->SetAttribute("loops", pItem->getLoops());
    }

    return true;
}

void Behavior::addServo (int nFrameId, int nServoId, std::shared_ptr<Servo> pServo)
{
    auto pLayer = queryActionLayer();
    if ( pLayer == nullptr ) {
        pLayer = std::make_shared<ActionLayer>();
        addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_ACTION), pLayer);
    }

    auto pFrame = pLayer->queryFrame(nFrameId);
    if ( pFrame == nullptr ) {
        pFrame = std::make_shared<Frame>();
        pLayer->addFrame(nFrameId, pFrame);
    }
    pFrame->addServo(nServoId, pServo);
}

void Behavior::delServo (int nFrameId, int nServoId)
{
    auto pLayer = queryActionLayer();
    if ( pLayer ) {
        auto pFrame = pLayer->queryFrame(nFrameId);
        if ( pFrame ) {
            pFrame->delServo(nServoId);
            if ( pFrame->servoCount() == 0 ) {
                pLayer->delFrame(nFrameId);
            }
        }
    }
}

std::shared_ptr<Servo> Behavior::queryServo(int nFrameId, int nServoId)
{
    auto pLayer = queryActionLayer();
    if ( pLayer ) {
        auto pFrame = pLayer->queryFrame(nFrameId);
        if ( pFrame ) {
            return pFrame->queryServo(nServoId);
        }
    }
    return nullptr;
}

std::map<int, std::shared_ptr<Servo>>* Behavior::getServos(int nFrameId)
{
    auto pLayer = queryActionLayer();
    if ( pLayer ) {
        auto pFrame = pLayer->queryFrame(nFrameId);
        if ( pFrame ) {
            return pFrame->getServos();
        }
    }
    return nullptr;
}

void Behavior::addMusic (int nItemId, std::shared_ptr<MusicItem> pMusic)
{
    auto pLayer = queryMusicLayer();
    if ( pLayer == nullptr ) {
        pLayer = std::make_shared<MusicLayer>();
        addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_MUSIC), pLayer);
    }
    pLayer->addMusicItem(nItemId, pMusic);
}

void Behavior::delMusic (int nItemId)
{
    auto pLayer = queryMusicLayer();
    if ( pLayer ) {
        pLayer->delMusicItem(nItemId);
    }
}

std::shared_ptr<MusicItem> Behavior::queryMusic (int nItemId)
{
    auto pLayer = queryMusicLayer();
    if ( pLayer ) {
        return pLayer->queryMusicItem(nItemId);
    }
    return nullptr;
}

std::map<int, std::shared_ptr<MusicItem>>* Behavior::getMusics()
{
    auto pLayer = queryMusicLayer();
    if ( pLayer ) {
        return pLayer->getMusicItems();
    }
    return nullptr;
}

void Behavior::addExpression (int nItemId, std::shared_ptr<ExpressionItem> pExpression)
{
    auto pLayer = queryExpressLayer();
    if ( pLayer == nullptr ) {
        pLayer = std::make_shared<ExpressionLayer>();
        addLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_EXPRESSION), pLayer);
    }
    pLayer->addExpressionItem(nItemId, pExpression);
}

void Behavior::delExpression (int nItemId)
{
    auto pLayer = queryExpressLayer();
    if ( pLayer ) {
        pLayer->delExpressionItem(nItemId);
    }
}

std::shared_ptr<ExpressionItem> Behavior::queryExpression (int nItemId)
{
    auto pLayer = queryExpressLayer();
    if ( pLayer ) {
        return pLayer->queryExpressionItem(nItemId);
    }
    return nullptr;
}

std::map<int, std::shared_ptr<ExpressionItem>>* Behavior::getExpressions()
{
    auto pLayer = queryExpressLayer();
    if ( pLayer ) {
        return pLayer->getExpressionItems();
    }
    return nullptr;
}

void Behavior::addLayer (int id, std::shared_ptr<Layer> pLayer)
{
    _layers[id] = pLayer;
}

void Behavior::delLayer (int id)
{
    auto iter = _layers.find(id);
    if ( iter != _layers.end() ) {
        _layers.erase(iter);
    }
}

std::shared_ptr<Layer> Behavior::queryLayer (int id)
{
    auto iter = _layers.find(id);
    if ( iter != _layers.end() ) {
        return iter->second;
    }
    return nullptr;
}

std::shared_ptr<ActionLayer> Behavior::queryActionLayer()
{
    auto pLayer = std::dynamic_pointer_cast<ActionLayer>(
                queryLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_ACTION))
                );
    return pLayer;
}

std::shared_ptr<ActionLayer> Behavior::queryHeadLayer()
{
    auto pLayer = std::dynamic_pointer_cast<ActionLayer>(
                queryLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_HEAD))
                );
    return pLayer;
}

std::shared_ptr<ActionLayer> Behavior::queryHandLayer()
{
    auto pLayer = std::dynamic_pointer_cast<ActionLayer>(
                queryLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_HAND))
                );
    return pLayer;
}

std::shared_ptr<ActionLayer> Behavior::queryFootLayer()
{
    auto pLayer = std::dynamic_pointer_cast<ActionLayer>(
                queryLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_FOOT))
                );
    return pLayer;
}

std::shared_ptr<MusicLayer> Behavior::queryMusicLayer()
{
    auto pLayer = std::dynamic_pointer_cast<MusicLayer>(
                queryLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_MUSIC))
                );
    return pLayer;
}

std::shared_ptr<ExpressionLayer> Behavior::queryExpressLayer()
{
    auto pLayer = std::dynamic_pointer_cast<ExpressionLayer>(
                queryLayer(static_cast<int>(LayerType::EM_LAYER_TYPE_EXPRESSION))
                );
    return pLayer;
}

std::map<int, std::shared_ptr<Layer>>* Behavior::getLayers ()
{
    return &_layers;
}

void Behavior::addLabel (int location, std::shared_ptr<Label> pLabel)
{
    if ( _attributes == nullptr ) {
        _attributes = std::make_shared<Attributes>();
    }
    _attributes->addLabel(location, pLabel);
}

void Behavior::delLabel (int location)
{
    if ( _attributes ) {
        _attributes->delLabel(location);
        if ( _attributes->labelCount() == 0 ) {
            _attributes.reset();
        }
    }
}

std::shared_ptr<Label> Behavior::queryLabel (int location)
{
    if ( _attributes ) {
        return _attributes->queryLabel(location);
    }
    return nullptr;
}

std::map<int, std::shared_ptr<Label>>* Behavior::getLabels()
{
    if ( _attributes ) {
        return _attributes->getLabels();
    }
    return nullptr;
}

void Behavior::setFrameRate (int frameRate)
{
    _frameRate = frameRate;
    _division = frameRateToDivison(frameRate);
}

int Behavior::frameRateToDivison (int frameRate)
{
    return (int)ceil(1000.0 / frameRate);
}

} // namespace BEH
