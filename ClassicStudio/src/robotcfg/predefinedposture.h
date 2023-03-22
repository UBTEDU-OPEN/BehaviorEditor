#ifndef PREDEFINEDPOSTURE_H
#define PREDEFINEDPOSTURE_H

#include "robotcfg_global.h"
#include <QVector>
#include <QMap>

class ROBOTCFGSHARED_EXPORT Posture
{
public:
    explicit Posture(const QString name): m_name(name) {}
    QString getName() { return m_name; }
    void addServo(const int id, const double value) { m_servosValue.insert(id,value); }
    const QMap<int,double>& getServosValue() { return m_servosValue; }

private:
    QString m_name;
    QMap<int,double> m_servosValue;
};

class ROBOTCFGSHARED_EXPORT PredefinedPosture
{
private:
    PredefinedPosture();

public:
    static PredefinedPosture* getInstance() { return s_instance; }
    bool loadFile(const QString& filePath);
    const QVector<Posture*>& getPostures() { return m_postures; }

private:
    static PredefinedPosture* s_instance;
    QVector<Posture*> m_postures;
};


#endif // PREDEFINEDPOSTURE_H
