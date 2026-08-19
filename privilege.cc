#include "privilege.h"
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>

#ifdef Q_OS_LINUX
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace privilege {

bool hasNetAdmin()
{
#ifdef Q_OS_LINUX
    QFile f("/proc/self/status");
    if (!f.open(QIODevice::ReadOnly))
        return false;

    const QString data = QString::fromUtf8(f.readAll());
    const auto lines = data.split('\n');
    for (const QString &line : lines) {
        if (!line.startsWith("CapEff:"))
            continue;
        const QString hex = line.mid(7).trimmed();
        bool ok = false;
        const quint64 cap = hex.toULongLong(&ok, 16);
        if (ok)
            return (cap & (1ULL << 12)) != 0;
        return false;
    }
    return false;
#else
    return true;
#endif
}

static QString findSetcap()
{
    QString path = QStandardPaths::findExecutable("setcap");
    if (!path.isEmpty())
        return path;

    const QStringList candidates = {"/usr/sbin/setcap", "/sbin/setcap", "/usr/bin/setcap"};
    for (const QString &candidate : candidates) {
        if (QFileInfo::exists(candidate))
            return candidate;
    }
    return QString();
}

bool fixWithPkexec(const QString &binaryPath)
{
#ifdef Q_OS_LINUX
    const QString setcap = findSetcap();
    if (setcap.isEmpty())
        return false;

    QProcess p;
    p.start("pkexec", {setcap, "cap_net_admin+ep", binaryPath});
    if (!p.waitForStarted(10000))
        return false;
    if (!p.waitForFinished(120000))
        return false;
    return p.exitStatus() == QProcess::NormalExit && p.exitCode() == 0;
#else
    Q_UNUSED(binaryPath);
    return true;
#endif
}

} // namespace privilege