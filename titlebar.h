#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QLabel>
#include <QPoint>
#include <QWidget>

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(const QString &title, QWidget *parent = nullptr);

    void setTitle(const QString &title);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QLabel *titleLabel;
    QPoint dragPosition;
};

#endif // TITLEBAR_H
