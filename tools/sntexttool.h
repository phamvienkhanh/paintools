#ifndef SNTEXTTOOL_H
#define SNTEXTTOOL_H

#include "snabstracttool.h"

class SNTextTool : public SNAbstractTool
{
public:
    explicit SNTextTool(QObject *parent = nullptr);

    // SNAbstractTool interface
public:
    void done(SNImageEditorContext &context) override;
    void doneWithInternal(SNImageEditorContext &context) override;
    void mousePressEvent(QMouseEvent *event) override;

    SNAbstractTool::Type type() override {return SNAbstractTool::TYPE_TEXT;}

private:
    QPointF m_vposBaseline;
    QString m_text;
};

#endif // SNTEXTTOOL_H
