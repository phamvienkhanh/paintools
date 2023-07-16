#include "sncircletool.h"
#include <QPainter>
#include "snimageeditor.h"

SNCircleTool::SNCircleTool(QObject *parent)
    : SNAbstractTool{parent}
{

}

void SNCircleTool::done(SNImageEditorContext &context)
{
    QPainter painter(&context.editedPix);
    painter.setRenderHint(QPainter::Antialiasing);
    SNToolSettings setting = m_setting;
    setting.lineWidth = setting.lineWidth*context.viewScale.x();
    draw(painter, context.viewPortToPix(m_vstartPos), context.viewPortToPix(m_vendPos), setting);
}

void SNCircleTool::mousePressEvent(QMouseEvent *event)
{
    m_vstartPos = event->position();
    m_vendPos = m_vstartPos;
}

void SNCircleTool::mouseMoveEvent(QMouseEvent *event)
{
    m_vendPos = event->position();
}

void SNCircleTool::process(QPainter &painter, const SNImageEditorContext &context)
{
    draw(painter, m_vstartPos, m_vendPos, m_setting);
}

void SNCircleTool::draw(QPainter &painter, const QPointF& start, const QPointF& end, const SNToolSettings& setting)
{
    QRect rect;
    rect.setTopLeft(start.toPoint());
    rect.setBottomRight(end.toPoint());

    QPen pen;
    pen.setWidth(setting.lineWidth);
    pen.setColor(setting.color);

    painter.save();
    painter.setPen(pen);
    painter.setBrush(setting.color);
    painter.drawEllipse(rect);
    painter.restore();
}

void SNCircleTool::doneWithInternal(SNImageEditorContext &context)
{
    done(context);
}
