#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QColor>
#include <QObject>
#include <QTimer>
#include <QWidget>

class SwitchButton : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = nullptr);

signals:
    void statusChanged(bool checked);

public slots:

private slots:
    void UpdateValue();

private:
    void drawBackGround(QPainter *painter);
    void drawSlider(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    //滑块距离边界距离
    int m_space;
    //圆角角度
    int m_radius;
    //是否选中
    bool m_checked;
    //是否显示文本
    bool m_showText;
    //是否显示圆
    bool m_showCircle;
    //是否使用动画
    bool m_animation;

    //打开时候背景色
    QColor m_bgColorOn;
    //关闭时候背景色
    QColor m_bgColorOff;
    //打开时候滑块颜色
    QColor m_sliderColorOn;
    //关闭时候滑块颜色
    QColor m_sliderColorOff;
    //文字颜色
    QColor m_textColor;

    //打开时候文字
    QString m_textOn;
    //关闭时候文字
    QString m_textOff;

    //动画定时器
    QTimer *m_timer;
    //动画步长
    int m_step;
    //滑块开始X轴坐标
    int m_startX;
    //滑块介绍X轴坐标
    int m_endX;

public:
    int space() const;
    int radius() const;
    bool checked() const;
    bool showText() const;
    bool showCircle() const;
    bool animation() const;

    QColor bgColorOn() const;
    QColor bgColorOff() const;
    QColor sliderColorOn() const;
    QColor sliderColorOff() const;
    QColor textColor() const;

    QString textOn() const;
    QString textOff() const;

    int step() const;
    int startX() const;
    int endX() const;

public Q_SLOTS:
    void setSpace(int space);
    void setRadius(int radius);
    void setChecked(bool checked);
    void setShowText(bool show);
    void setShowCircle(bool show);
    void setAnimation(bool ok);

    void setBgColorOn(const QColor &color);
    void setBgColorOff(const QColor &color);
    void setSliderColorOn(const QColor &color);
    void setSliderColorOff(const QColor &color);
    void setTextColor(const QColor &color);

    void setTextOn(const QString &text);
    void setTextOff(const QString &text);
};

#endif // SWITCHBUTTON_H
