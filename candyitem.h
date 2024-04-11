#ifndef CANDYITEM_H
#define CANDYITEM_H

#include <mutex>
#include <string>
#include <thread>
#include <QListWidgetItem>
#include <queue>

class CandyItem : public QListWidgetItem
{
public:
    explicit CandyItem();
    ~CandyItem();

    void update();
    void shutdown();

private:
    void *candy = nullptr;
    int randomHex();
    QString randomHexString(int length);

public:
    static void startKeepAlive();
    static void stopKeepAlive();

private:
    friend void candyErrorCallback(void *candy);
    static std::thread keepAliveThread;
    static std::queue<void *> candyQueue;
    static bool running;
};

#endif // CANDYITEM_H
