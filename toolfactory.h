#ifndef TOOLFACTORY_H
#define TOOLFACTORY_H

#include <QObject>
#include "tools/snabstracttool.h"

class SNToolFactory : public QObject
{
    Q_OBJECT
public:
    explicit SNToolFactory(QObject *parent = nullptr);

    SNAbstractTool* createTool(SNAbstractTool::Type type, const SNToolSettings& setting);

};

#endif // TOOLFACTORY_H
