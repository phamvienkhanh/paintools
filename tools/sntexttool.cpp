#include "sntexttool.h"
#include <QPainter>
#include "snimageeditor.h"

SNTextTool::SNTextTool(QObject *parent)
    : SNAbstractTool{parent}
{

}

void SNTextTool::done(SNImageEditorContext& context)
{
    m_text = context.text;
    doneWithInternal(context);
}

void SNTextTool::doneWithInternal(SNImageEditorContext &context)
{
    QPainter painter(&context.editedPix);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font;
    font.setPixelSize(m_setting.fontSize);
    painter.setFont(font);

    QPen pen;
    pen.setWidth(10);
    painter.setPen(pen);

    painter.drawText(context.viewPortToPix(m_vposBaseline), m_text);
}

void SNTextTool::mousePressEvent(QMouseEvent *event)
{
    m_vposBaseline = event->position();
}
