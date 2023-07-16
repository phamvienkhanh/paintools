#ifndef SNRECTANGLETOOL_H
#define SNRECTANGLETOOL_H

#include "snabstracttool.h"

class SNRectangleTool : public SNAbstractTool
{
public:
    explicit SNRectangleTool(QObject *parent = nullptr);

    // SNAbstractTool interface
public:
    void done(SNImageEditorContext &context) override;
    void doneWithInternal(SNImageEditorContext &context) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    SNAbstractTool::Type type() override {return SNAbstractTool::TYPE_RECTANGLE;}
    void process(QPainter &painter, const SNImageEditorContext &context) override;

private:
    void draw(QPainter& painter, const QPointF& start, const QPointF& end, const SNToolSettings& setting);

private:
    QPointF m_vstartPos;
    QPointF m_vendPos;
};

#endif // SNRECTANGLETOOL_H
