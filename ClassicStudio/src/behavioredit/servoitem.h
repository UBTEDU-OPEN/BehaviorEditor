#ifndef SERVOITEM_H
#define SERVOITEM_H

#include <QWidget>
#include <QSet>
#include <memory>

class ServoItem : public QWidget
{
    Q_OBJECT

public:
    enum eSelectState
    {
        eUnSelected = 0,
        ePartiallySelected = 1,
        eSelected = 2
    };

    const static int kServoItemWidth = 15;
    const static int kServoItemHeight = 15;

public:
    explicit ServoItem(QWidget *parent = nullptr);
//    explicit ServoItem(int frameIdx, int layerIdx, int sId, double sAngle, int sRuntime, QWidget *parent = nullptr);
    explicit ServoItem(int frameIdx, int layerIdx, int srvId, double srvValue, QWidget *parent = nullptr);
    ~ServoItem() override;

    inline bool isVirtualServoItem() const { return (servoId_ == -1); }

    inline void setFrameIndex(int idx) { frameIndex_ = idx; }
    inline int getFrameIndex() const { return frameIndex_; }
    inline void setLayerIndex(int idx) { layerIndex_ = idx; }
    inline int getLayerIndex() const { return layerIndex_; }
    inline void setServoId(int id) { servoId_ = id; }
    inline int getServoId() const { return servoId_; }
    inline void setServoAngle(double servoAngle) { servoAngle_ = servoAngle; }
    inline double getServoAngle() const { return servoAngle_; }

    QSet<ServoItem*> getAncestorNodes() const;
    QSet<ServoItem*> getDescendantNodes() const;
    ServoItem* getDescendantNode(int layerIndex_) const;
    void deleteDescendantNodes();
    inline ServoItem *getParentNode() const { return parentNode_; }
    inline void setParentNode(ServoItem *p) { parentNode_ = p; }
    inline bool hasChildNode(ServoItem *node) const { return childNodes_.contains(node); }
    bool hasChildNode(int layerIndex_) const;
    inline int childNodesCount() const { return childNodes_.count(); }
    inline QSet<ServoItem*> getChildNodes() const { return childNodes_; }
    ServoItem *getChild(int layerIndex_) const;
    inline QSet<ServoItem*>::iterator addChildNode(ServoItem *child) { return childNodes_.insert(child); }
    inline void addChildNodes(const QSet<ServoItem*> &newChildren) { childNodes_ += newChildren; }
    ServoItem *removeChildNode(ServoItem *child);
    ServoItem *removeChildNode(int layerIndex_);
    inline void removeChildNodes(const QSet<ServoItem*> &childrenToRm) { childNodes_ -= childrenToRm;}
    bool hasSibling() const;

    inline void setSelected(eSelectState s) { selectedState_ = s; }
    eSelectState getSelected() { return selectedState_; }

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    int frameIndex_;
    int layerIndex_;
    int servoId_;
    double servoAngle_;

    ServoItem *parentNode_;
    QSet<ServoItem*> childNodes_;
    eSelectState selectedState_;
};

#endif // SERVOITEM_H
