#ifndef PRIVILEGE_H
#define PRIVILEGE_H

#include <QString>

namespace privilege {

bool hasNetAdmin();
bool fixWithPkexec(const QString &binaryPath);

} // namespace privilege

#endif // PRIVILEGE_H