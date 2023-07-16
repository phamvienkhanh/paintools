#ifndef SNIMAGEEDITOR_H
#define SNIMAGEEDITOR_H

#include <QPixmap>
#include <QQuickPaintedItem>
#include <QStack>
#include "tools/snabstracttool.h"

#define Q_PROPERTY_AUTO(TYPE, M)                     \
Q_PROPERTY(TYPE M MEMBER m_##M NOTIFY M##Changed) \
    public:                                           \
    Q_SIGNAL void M##Changed();                      \
    void M(TYPE in_##M)                              \
{                                                \
        m_##M = in_##M;                               \
        Q_EMIT M##Changed();                         \
}                                                \
    TYPE M() { return m_##M; }                        \
                                                     \
    private:                                          \
    TYPE m_##M;

struct SNImageEditorContext
{
    QPixmap originPix;
    QPixmap editedPix;

    // for text tool
    QString text;

    // for crop tool
    QRect cropRect;

    // for render view
    QPixmap viewPix;
    QPointF viewScale{1.0, 1.0};
    QPointF viewPos{0.0, 0.0};
    QPointF prePoint; // todo: delete later
    qreal zoom = 1.0;
    qreal deviceRatio = 1.0;

    QStack<SNAbstractTool* > stackTools;

    QPointF viewPortToPix(const QPointF &point)
    {
        return QPointF((point.x() - viewPos.x())*viewScale.x(), (point.y() - viewPos.y())*viewScale.y());
    }

    QSize viewPixSize() { return viewPix.size() / deviceRatio; }
};

class SNImageEditor : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY_AUTO(qreal, viewPortScale)
    Q_PROPERTY_AUTO(QSize, imgSize)
    Q_PROPERTY_AUTO(QPoint, imgPos)

public:
    SNImageEditor(QQuickItem *parent = nullptr);
    void updateViewPix();

    Q_INVOKABLE void setLineWidth(qint32 w) { m_settings.lineWidth = w; }
    Q_INVOKABLE void addText(qreal x, qreal y, qreal fontSize, const QColor& color, const QString& text);
    Q_INVOKABLE void crop(qreal x, qreal y, qreal w, qreal h);
    Q_INVOKABLE void save();
    Q_INVOKABLE void undo();
    Q_INVOKABLE void activeTool(SNAbstractTool::Type type);

    // QQuickItem interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;

private:
    SNAbstractTool* m_activeTool = nullptr;    
    SNImageEditorContext m_context;
    SNToolSettings m_settings;
};

#endif // SNIMAGEEDITOR_H
