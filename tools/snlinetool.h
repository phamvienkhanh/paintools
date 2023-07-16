#ifndef SNLINETOOL_H
#define SNLINETOOL_H

#include "snabstracttool.h"

class SNLineTool : public SNAbstractTool
{
public:
    explicit SNLineTool(QObject *parent = nullptr);

    // SNAbstractTool interface
public:
    void done(SNImageEditorContext &context) override;
    void doneWithInternal(SNImageEditorContext &context) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void process(QPainter &painter, const SNImageEditorContext &context) override;

    SNAbstractTool::Type type() override {return SNAbstractTool::TYPE_LINE;}

private:
    void draw(QPainter& painter, const QPointF& start, const QPointF& end, const SNToolSettings& setting);

private:
    QPointF m_vstartPos;
    QPointF m_vendPos;
};

#endif // SNLINETOOL_H
