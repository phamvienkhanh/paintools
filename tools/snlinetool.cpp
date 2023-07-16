#include "snlinetool.h"
#include <QPainter>
#include "snimageeditor.h"

SNLineTool::SNLineTool(QObject *parent)
    : SNAbstractTool{parent}
{

}

void SNLineTool::done(SNImageEditorContext &context)
{
    QPainter painter(&context.editedPix);
    painter.setRenderHint(QPainter::Antialiasing);
    SNToolSettings setting = m_setting;
    setting.lineWidth = setting.lineWidth*context.viewScale.x();
    draw(painter, context.viewPortToPix(m_vstartPos), context.viewPortToPix(m_vendPos), setting);
}

void SNLineTool::mousePressEvent(QMouseEvent *event)
{
    m_vstartPos = event->position();
    m_vendPos = m_vstartPos;
}

void SNLineTool::mouseMoveEvent(QMouseEvent *event)
{
    m_vendPos = event->position();
}

void SNLineTool::process(QPainter &painter, const SNImageEditorContext &context)
{
    draw(painter, m_vstartPos, m_vendPos, m_setting);
}

void SNLineTool::draw(QPainter &painter, const QPointF& start, const QPointF& end, const SNToolSettings& setting)
{
    QRect rect;
    rect.setTopLeft(start.toPoint());
    rect.setBottomRight(end.toPoint());

    QPen pen;
    pen.setWidth(setting.lineWidth);
    pen.setColor(setting.color);

    painter.save();
    painter.setPen(pen);
    painter.drawLine(start, end);
    painter.restore();
}


void SNLineTool::doneWithInternal(SNImageEditorContext &context)
{
    done(context);
}
