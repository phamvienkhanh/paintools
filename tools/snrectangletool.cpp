#include "snrectangletool.h"
#include <QPainter>
#include "snimageeditor.h"

SNRectangleTool::SNRectangleTool(QObject *parent)
    : SNAbstractTool{parent}
{

}

void SNRectangleTool::done(SNImageEditorContext &context)
{
    m_vstartPos = context.viewPortToPix(m_vstartPos);
    m_vendPos = context.viewPortToPix(m_vendPos);

    doneWithInternal(context);
}

void SNRectangleTool::mousePressEvent(QMouseEvent *event)
{
    m_vstartPos = event->position();
    m_vendPos = m_vstartPos;
}

void SNRectangleTool::mouseMoveEvent(QMouseEvent *event)
{
    m_vendPos = event->position();
}

void SNRectangleTool::process(QPainter &painter, const SNImageEditorContext &context)
{
    draw(painter, m_vstartPos, m_vendPos, m_setting);
}

void SNRectangleTool::draw(QPainter &painter, const QPointF& start, const QPointF& end, const SNToolSettings& setting)
{
    QRect rect;
    rect.setTopLeft(start.toPoint());
    rect.setBottomRight(end.toPoint());

    QPen pen;
    pen.setWidth(setting.lineWidth);
    pen.setColor(setting.color);

    painter.save();
    painter.setPen(pen);
    painter.drawRect(rect);
    painter.restore();
}

void SNRectangleTool::doneWithInternal(SNImageEditorContext &context)
{
    QPainter painter(&context.editedPix);
    painter.setRenderHint(QPainter::Antialiasing);
    SNToolSettings setting = m_setting;
    setting.lineWidth = setting.lineWidth*context.viewScale.x();
    draw(painter, m_vstartPos, m_vendPos, setting);
}
