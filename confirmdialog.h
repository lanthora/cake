#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include "framelessdialog.h"
#include <QObject>

class QHBoxLayout;

class ConfirmDialog : public FramelessDialog
{
    Q_OBJECT
public:
    enum ButtonStyle
    {
        PrimaryButton,
        DangerButton
    };

    ConfirmDialog(const QString &title, const QString &text, QWidget *parent = nullptr);

    void setConfirmButton(const QString &text, ButtonStyle style = PrimaryButton);
    void setCancelButton(const QString &text);

private:
    QHBoxLayout *buttonLayout;
};

#endif // CONFIRMDIALOG_H
