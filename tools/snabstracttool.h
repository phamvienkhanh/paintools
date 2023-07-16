#ifndef SNABSTRACTTOOL_H
#define SNABSTRACTTOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QQmlEngine>
#include <QColor>

struct SNImageEditorContext;
struct SNToolSettings
{
    qreal lineWidth = 10;
    qreal fontSize = 24;
    QColor color;
};

class SNAbstractTool : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
public:
    enum Type {
        NONE = -1,
        TYPE_PENCIL = 0,
        TYPE_CIRCLE = 1,
        TYPE_RECTANGLE = 2,
        TYPE_LINE = 3,
        TYPE_TEXT = 4,
        TYPE_CROP = 5,
    };
    Q_ENUM(Type);

public:
    explicit SNAbstractTool(QObject *parent = nullptr);

    virtual void done(SNImageEditorContext& context) = 0;
    virtual void doneWithInternal(SNImageEditorContext& context){};
    virtual void process(QPainter &painter, const SNImageEditorContext &context){};

    virtual void mousePressEvent(QMouseEvent *event){};
    virtual void mouseMoveEvent(QMouseEvent *event){};
    virtual void mouseReleaseEvent(QMouseEvent *event){};
    virtual void wheelEvent(QWheelEvent *event){};

    virtual SNAbstractTool::Type type() = 0;
    void applySetting(const SNToolSettings& setting);

protected:
    SNToolSettings m_setting;
};

#endif // SNABSTRACTTOOL_H
