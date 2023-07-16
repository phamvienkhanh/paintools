#ifndef SNCROPTOOL_H
#define SNCROPTOOL_H

#include "snabstracttool.h"

class SNCropTool : public SNAbstractTool
{
public:
    explicit SNCropTool(QObject *parent = nullptr);

    // SNAbstractTool interface
public:
    void done(SNImageEditorContext &context) override;
    void doneWithInternal(SNImageEditorContext& context) override;
    SNAbstractTool::Type type() override {return SNAbstractTool::TYPE_CROP;}

private:
    QRect m_cropRect;
};

#endif // SNCROPTOOL_H
