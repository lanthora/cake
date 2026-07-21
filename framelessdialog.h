#ifndef FRAMELESSDIALOG_H
#define FRAMELESSDIALOG_H

#include <QDialog>
#include <QObject>

class TitleBar;
class QVBoxLayout;

class FramelessDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FramelessDialog(const QString &title, QWidget *parent = nullptr);

    QVBoxLayout *contentLayout() const;

protected:
    TitleBar *titleBar;

private:
    QVBoxLayout *m_contentLayout;
};

#endif // FRAMELESSDIALOG_H
