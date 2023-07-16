#include "snpenciltool.h"
#include "snimageeditor.h"
#include <QPainter>

SNPencilTool::SNPencilTool(QObject *parent)
    : SNAbstractTool{parent}
{

}

void SNPencilTool::done(SNImageEditorContext &context)
{    
    for(auto& p : m_vpoints) {
        p = context.viewPortToPix(p);
    }
    doneWithInternal(context);
}

void SNPencilTool::mousePressEvent(QMouseEvent *event)
{
    m_vpoints.append(event->position());
}

void SNPencilTool::mouseMoveEvent(QMouseEvent *event)
{
    QPointF p = event->position();

    if(m_vpoints.size() > 0) {
        QPointF lp = m_vpoints.last();
        p.setX(p.x()*0.15f + lp.x()*0.85f);
        p.setY(p.y()*0.15f + lp.y()*0.85f);
    }
    m_vpoints.append(p);
}

void SNPencilTool::process(QPainter &painter, const SNImageEditorContext &context)
{
    painter.save();
    auto setting = m_setting;
    setting.lineWidth = std::ceil(setting.lineWidth / context.viewScale.x());
    qDebug() << "process " << setting.lineWidth;
    draw(painter, m_vpoints, setting);
    painter.restore();
}

void SNPencilTool::draw(QPainter &painter, const QVector<QPointF>& points, const SNToolSettings& setting)
{
    QPen pen;
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(setting.lineWidth);
    pen.setColor(setting.color);
    painter.setPen(pen);

    painter.drawPolyline(points);
}

void SNPencilTool::doneWithInternal(SNImageEditorContext &context)
{
    QPainter painter(&context.editedPix);
    painter.setRenderHint(QPainter::Antialiasing);

    qDebug() << "doneWithInternal " << m_setting.lineWidth;

    draw(painter, m_vpoints, m_setting);
}
