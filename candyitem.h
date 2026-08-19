#ifndef CANDYITEM_H
#define CANDYITEM_H

#include <QString>
#include <atomic>
#include <memory>
#include <string>
#include <thread>

class CandyItem
{
public:
    explicit CandyItem(const QString &name);
    ~CandyItem();
    void update();

    const QString &name() const { return m_name; }

private:
    QString m_name;
    std::string id;
    std::thread clientThread;
    std::shared_ptr<std::atomic<bool>> clientDone;
    bool started = false;
};

#endif // CANDYITEM_H
