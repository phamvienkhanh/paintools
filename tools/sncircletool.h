#ifndef SNCIRCLETOOL_H
#define SNCIRCLETOOL_H

#include "snabstracttool.h"

class SNCircleTool : public SNAbstractTool
{
public:
    explicit SNCircleTool(QObject *parent = nullptr);

    // SNAbstractTool interface
public:
    void done(SNImageEditorContext &context) override;
    void doneWithInternal(SNImageEditorContext &context) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void process(QPainter &painter, const SNImageEditorContext &context) override;

    SNAbstractTool::Type type() override {return SNAbstractTool::TYPE_CIRCLE;}
    void draw(QPainter &painter, const QPointF& start, const QPointF& end, const SNToolSettings& setting);

private:
    QPointF m_vstartPos;
    QPointF m_vendPos;
};

#endif // SNCIRCLETOOL_H
