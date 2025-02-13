#ifndef XCUSTOMPLOT_H
#define XCUSTOMPLOT_H

#include "../qcustomplot.h"
#include <QObject>
#include <QList>
#include "Def.h"
#include <QTimer>




class CustomPlot:public QCustomPlot
{
    Q_OBJECT

public:
    CustomPlot(QWidget *parent = 0);
    ~CustomPlot();
    void init();
    void draw(const QVector<double>& x, const QVector<double>& y, const QString& name, const QColor& color, bool fill);
    void updateData();

signals:
    // 重新声明需要的信号
    void mouseMove(QMouseEvent* event);
    void mouseLeave(QEvent* event);
private slots:
    void setPlotColors(ElaThemeType::ThemeMode themeMode);

protected:
    // 重写鼠标事件处理函数
    virtual void mouseMoveEvent(QMouseEvent* event) override {
        QCustomPlot::mouseMoveEvent(event);  // 调用父类的处理
        emit mouseMove(event);               // 发送信号
    }

    virtual void leaveEvent(QEvent* event) override {
        QCustomPlot::leaveEvent(event);      // 调用父类的处理
        emit mouseLeave(event);              // 发送信号
    }

};

#endif // XCUSTOMPLOT_H
