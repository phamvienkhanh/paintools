#include "snimageeditor.h"
#include "toolfactory.h"

#include <QPainter>
#include <QPixmapCache>
#include <QQuickWindow>
#include <QTimer>

QVector<QPoint> m_points;
bool isPreview = true;

SNImageEditor::SNImageEditor(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemHasContents);
//    setFlag(ItemAcceptsInputMethod);

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(350);
    timer->callOnTimeout([this](){updateViewPix();});

    QPixmap orig;
//    orig.load("/home/noname/Downloads/-1626158764746149978973.webp");
    orig.load("/home/keith/Pictures/wallpapper/peakpx.jpg");

    connect(this, &QQuickItem::widthChanged, this, [this, timer]() { timer->start(); });
    connect(this, &QQuickItem::heightChanged, this, [this, timer]() { timer->start(); });

    connect(this, &QQuickPaintedItem::windowChanged, this, [this](QQuickWindow *wind) {
        if (wind) {
            m_context.deviceRatio = wind->devicePixelRatio();
            updateViewPix();
            qDebug() << "m_context.deviceRatio " << m_context.deviceRatio;
        }
    });

    m_context.originPix = orig;
    m_context.editedPix = orig;
    m_context.viewPix = orig;
    m_settings.color = "red";
}

void SNImageEditor::updateViewPix()
{    
//    QPixmapCache::clear();

    qreal viewportRatio = (qreal)width()/height();
    qreal imgRatio = (qreal)m_context.editedPix.width()/m_context.editedPix.height();

    QPixmap editedPix = m_context.editedPix;
    editedPix.setDevicePixelRatio(m_context.deviceRatio);
    qreal zoom = m_context.zoom;
    if(viewportRatio < imgRatio) {
        if (m_context.editedPix.width() > width() || zoom != 1.0) {
            editedPix = editedPix.scaledToWidth(width() * zoom * m_context.deviceRatio,
                                                Qt::SmoothTransformation);
        }
    }
    else {
        if (m_context.editedPix.height() > height() || zoom != 1.0) {
            editedPix = editedPix.scaledToHeight(height() * zoom * m_context.deviceRatio,
                                                 Qt::SmoothTransformation);
        }
    }

    m_context.viewPix = editedPix;

    m_context.viewScale.setX((qreal) m_context.editedPix.width() / m_context.viewPixSize().width());
    m_context.viewScale.setY((qreal) m_context.editedPix.height()
                             / m_context.viewPixSize().height());

    if (width() >= m_context.viewPixSize().width()) {
        m_context.viewPos.setX((width() - m_context.viewPixSize().width()) / 2.0);
    } else {
        m_context.viewPos.setX(std::min(m_context.viewPos.x(), 0.0));
    }
    if (height() >= m_context.viewPixSize().height()) {
        m_context.viewPos.setY((height() - m_context.viewPixSize().height()) / 2.0);
    } else {
        m_context.viewPos.setY(std::min(m_context.viewPos.y(), 0.0));
    }

    imgPos(m_context.viewPos.toPoint());
    imgSize(m_context.viewPixSize());
    viewPortScale(m_context.viewScale.x());

    update();
}

void SNImageEditor::addText(qreal x, qreal y, qreal fontSize, const QColor& color, const QString& text)
{
    m_settings.fontSize = fontSize;

    SNAbstractTool* textTool = SNToolFactory().createTool(SNAbstractTool::TYPE_TEXT, m_settings);
    m_context.text = text;
    y = y + std::round(fontSize / m_context.viewScale.x()) + 2;
    QMouseEvent event(QEvent::Type::MouseButtonPress, QPointF(x, y), QPointF(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

    textTool->mousePressEvent(&event);
    textTool->done(m_context);
    m_context.stackTools.push(textTool);
    updateViewPix();
}

void SNImageEditor::crop(qreal x, qreal y, qreal w, qreal h)
{
    SNAbstractTool* cropTool = SNToolFactory().createTool(SNAbstractTool::TYPE_CROP, m_settings);
    m_context.cropRect = QRect(x, y, w, h);

    cropTool->done(m_context);
    m_context.stackTools.push(cropTool);
    updateViewPix();
}

void SNImageEditor::save()
{
    m_context.editedPix.save("/home/keith/Pictures/wallpapper/programer_code4coffee_edit.jpg",
                             nullptr,
                             90);
}

void SNImageEditor::undo()
{
    if(!m_context.stackTools.isEmpty()) {
        SNAbstractTool* tool = m_context.stackTools.pop();
        tool->deleteLater();

        m_context.editedPix = m_context.originPix;

        for(auto& itool : m_context.stackTools) {
            updateViewPix();
            itool->doneWithInternal(m_context);
        }

        updateViewPix();
    }
}

void SNImageEditor::activeTool(SNAbstractTool::Type type)
{
    if(m_activeTool) {
        m_activeTool->done(m_context);
        m_context.stackTools.push(m_activeTool);
    }

    m_activeTool = SNToolFactory().createTool(type, m_settings);
}

void SNImageEditor::mousePressEvent(QMouseEvent *event)
{
    m_context.prePoint = event->position();
    if (m_activeTool) {
        m_activeTool->mousePressEvent(event);
    }
    update();
}

void SNImageEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        m_context.viewPos = m_context.viewPos + event->position() - m_context.prePoint;
        m_context.prePoint = event->position();
    }

    if (m_activeTool) {
        m_activeTool->mouseMoveEvent(event);
    }
    update();
}

void SNImageEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_activeTool) {
        m_activeTool->mouseReleaseEvent(event);

        switch (m_activeTool->type()) {
        case SNAbstractTool::TYPE_CIRCLE:
        case SNAbstractTool::TYPE_LINE:
        case SNAbstractTool::TYPE_PENCIL:
        case SNAbstractTool::TYPE_RECTANGLE: {
            m_activeTool->done(m_context);
            m_context.stackTools.push(m_activeTool);
            m_activeTool = nullptr;
        }

        default:
            break;
        }
    }

    updateViewPix();
}

void SNImageEditor::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        static int dw = 0;
        dw += event->pixelDelta().y();
        if (abs(dw) > 120) {
            qreal dtz = dw / 120 * 0.1;
            m_context.zoom = std::max(dtz + m_context.zoom, 0.0);
            updateViewPix();

            dw = dw % 120;
        }
    }
}

void SNImageEditor::paint(QPainter *painter)
{
    painter->drawPixmap(m_context.viewPos, m_context.viewPix);

    painter->setRenderHint(QPainter::Antialiasing);
    if (m_activeTool) {
        m_activeTool->process(*painter, m_context);
    }
}
