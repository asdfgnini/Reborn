#pragma once

#include <QWidget>
#include <QString>
#include <QList>
#include "../../Page/RBGAseries/ScanMode/T_ScanMode.h"
#include "../../Page/RBGAseries/LeakMode/T_BaseComponents.h"

#include "../../../include/data.h"




class RGADevice : public QWidget
{
    Q_OBJECT
public:
    explicit RGADevice(QWidget* parent = nullptr);
    ~RGADevice();

    void setDeviceName(QString deviceName); 
    QString getDeviceName() const;

    void setm_scanMode(T_ScanMode* scanMode);
    T_ScanMode* getm_scanMode() const;

    void setm_baseComponents(T_BaseComponents* baseComponents);
    T_BaseComponents* getm_baseComponents() const;

    QList<ModeType> getm_modeName(ModeType modeName) const;

private:
    QString m_deviceName;
    QList<ModeType> m_modeName;
    T_ScanMode* m_scanMode;
    RGA* m_rga;
    T_BaseComponents* m_baseComponents;
    // T_LeakMode* m_leakMode;
    // ... 其他模式
};