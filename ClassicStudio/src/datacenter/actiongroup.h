#ifndef ACTIONGROUP_H
#define ACTIONGROUP_H

#include <QObject>
#include <QSet>
#include <QMap>
#include <set>

class ActionGroup : public QObject
{
    Q_OBJECT
public:
    ActionGroup(QString groupName, QSet<int> frameIndexes, QObject* parent = nullptr);
    void setGroupName(QString name) { groupName_ = name; }
    QString groupName() { return groupName_; }
    int groupIndex();
    int lastFrameIndex();
    int groupWidth();
    void setGroupIndex(int index);
    void removeIndex(int index);
    void addIndex(int index);
    void changeIndexes(QSet<int> indexes);
    int frameCount() { return frameIndexes_.count(); }
    bool contains(int frameIndex);
    bool hasFrame(int frameIndex);
    QSet<int> frameIndexes() { return frameIndexes_; }
    std::set<int> frameIndexesStdSet();

private:
    QString groupName_;
    QSet<int> frameIndexes_;
};

#endif // ACTIONGROUP_H
