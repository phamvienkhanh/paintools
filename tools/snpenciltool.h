#ifndef SNPENCILTOOL_H
#define SNPENCILTOOL_H

#include "snabstracttool.h"

class SNPencilTool : public SNAbstractTool
{
public:
    explicit SNPencilTool(QObject *parent = nullptr);

    // SNAbstractTool interface
public:
    void done(SNImageEditorContext &context) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void doneWithInternal(SNImageEditorContext &context) override;

    void process(QPainter &painter, const SNImageEditorContext &context) override;
    SNAbstractTool::Type type() override {return SNAbstractTool::TYPE_PENCIL;}

private:
    void draw(QPainter& painter, const QVector<QPointF>& points, const SNToolSettings& setting);

private:
    QVector<QPointF> m_vpoints;
    qreal m_vScale = 1.0;
};

#endif // SNPENCILTOOL_H
