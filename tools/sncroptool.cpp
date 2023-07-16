#include "sncroptool.h"
#include "snimageeditor.h"

SNCropTool::SNCropTool(QObject *parent)
    : SNAbstractTool{parent}
{

}

void SNCropTool::done(SNImageEditorContext &context)
{
    QPixmap &editPix = context.editedPix;
    QRect &cropRect = context.cropRect;
    QPoint pixPoint = QPoint(cropRect.x()*context.viewScale.x(), cropRect.y()*context.viewScale.y());
    QSize pixSize = QSize(cropRect.width() * context.viewScale.x(),
                          cropRect.height() * context.viewScale.y());

    m_cropRect = QRect(pixPoint, pixSize);
    doneWithInternal(context);
}

void SNCropTool::doneWithInternal(SNImageEditorContext& context)
{
    context.editedPix = context.editedPix.copy(m_cropRect);
}
