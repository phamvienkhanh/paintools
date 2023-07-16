import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Item {
    id: root

    signal beginSelect
    signal endSelect
    signal beginMove
    signal endMove

    signal cancel
    signal crop (real x, real y, real w, real h)

    property size limitSize: Qt.size(1, 1)

    property color _pointColor: "pink"
    property int _pointSize: 10
    property int _borderSize: 2
    property int _offsetHoverBoder: 4

    enabled: false
    visible: enabled

    function clamp (val, min, max) {
        return Math.min(Math.max(val, min), max)
    }

    onWidthChanged: {
        root.returnToBound()
    }
    onHeightChanged: {
        root.returnToBound()
    }

    function returnToBound () {
        selection.x = root.clamp(selection.x, 0, root.width - selection.width)
        selection.y = root.clamp(selection.y, 0, root.height - selection.height)

        selection.width = root.clamp(selection.width, root.limitSize.width, root.width)
        selection.height = root.clamp(selection.height, root.limitSize.height, root.height)

        selection.width = Math.min(selection.width, root.width - selection.x)
        selection.height = Math.min(selection.height, root.height - selection.y)
    }

    function adjustPostion (x, y) {
        const rx = root.clamp(x, 0, root.width)
        const ry = root.clamp(y, 0, root.height)

        return Qt.point(rx, ry)
    }

    Rectangle {
        id: rectOverlay
        color: "#AF000000"
        anchors.fill: parent
        visible: false
    }

    MouseArea {
        property point originPos: Qt.point(0, 0)

        anchors.fill: parent
        onPressed: function (mouse) {
            originPos = Qt.point(mouse.x, mouse.y)
            beginSelect()
        }
        onPositionChanged: function (mouse) {
            const mPos = root.adjustPostion(mouse.x, mouse.y)

            selection.x = Math.min(mPos.x, originPos.x)
            selection.y = Math.min(mPos.y, originPos.y)

            const w = Math.abs(mPos.x - originPos.x)
            const h = Math.abs(mPos.y - originPos.y)
            selection.width = w
            selection.height = h
        }
        onReleased: function (mouse) {
            endSelect()
        }
    }

    Rectangle {
        id: maskRect
        anchors.fill: rectOverlay
        visible: false
        color: "transparent"

        Rectangle {
            id: selection
            width: 0
            height: 0
            x: 0
            y: 0
        }
    }

    OpacityMask {
        anchors.fill: rectOverlay
        maskSource: maskRect
        source: rectOverlay
        cached: false
        invert: true
    }

    Rectangle {
        id: visualSelection

        x: selection.x
        y: selection.y
        width: selection.width
        height: selection.height
        color: "transparent"
        border.width: 2
        border.color: "pink"

        MouseArea {
            id: dragSelection

            property point originPoint: Qt.point(0, 0)

            anchors.fill: parent
            onPressed: function (mouse) {
                cursorShape = Qt.ClosedHandCursor
                originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
                beginMove()
            }
            onPositionChanged: function (mouse) {
                if (containsPress) {
                    const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                    const dx = curPos.x - originPoint.x
                    const dy = curPos.y - originPoint.y
                    originPoint = curPos
                    selection.x += dx
                    selection.y += dy
                    root.returnToBound()
                }
            }
            onReleased: function (mouse) {
                cursorShape = Qt.ArrowCursor
                endMove()
            }
        }
    }

    MouseArea {
        id: leftEdge

        property point originPoint: Qt.point(0, 0)

        height: visualSelection.height
        width: root._borderSize + root._offsetHoverBoder
        x: visualSelection.x - width / 2
        y: visualSelection.y

        hoverEnabled: true
        cursorShape: containsMouse ? Qt.SizeHorCursor : Qt.ArrowCursor
        onPressed: function (mouse) {
            originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
            originPoint = root.adjustPostion(originPoint.x, originPoint.y)
            beginMove()
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                const mPos = root.adjustPostion(curPos.x, curPos.y)
                const dx = mPos.x - originPoint.x
                originPoint = mPos
                selection.x += dx
                selection.width -= dx
                root.returnToBound()
            }
        }
        onReleased: function (mouse) {
            endMove()
        }
    }

    MouseArea {
        id: rightEdge

        property point originPoint: Qt.point(0, 0)

        height: visualSelection.height
        width: root._borderSize + root._offsetHoverBoder
        x: visualSelection.x + visualSelection.width - width / 2
        y: visualSelection.y

        hoverEnabled: true
        cursorShape: containsMouse ? Qt.SizeHorCursor : Qt.ArrowCursor
        onPressed: function (mouse) {
            originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
            originPoint = root.adjustPostion(originPoint.x, originPoint.y)
            beginMove()
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                const mPos = root.adjustPostion(curPos.x, curPos.y)

                const dx = mPos.x - originPoint.x
                originPoint = mPos

                selection.width += dx
                root.returnToBound()
            }
        }
        onReleased: function (mouse) {
            endMove()
        }
    }

    MouseArea {
        id: topEdge

        property point originPoint: Qt.point(0, 0)

        height: root._borderSize + root._offsetHoverBoder
        width: visualSelection.width
        x: visualSelection.x
        y: visualSelection.y - height / 2

        hoverEnabled: true
        cursorShape: containsMouse ? Qt.SizeVerCursor : Qt.ArrowCursor
        onPressed: function (mouse) {
            originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
            originPoint = root.adjustPostion(originPoint.x, originPoint.y)
            beginMove()
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                const mPos = root.adjustPostion(curPos.x, curPos.y)
                const dy = mPos.y - originPoint.y
                originPoint = mPos
                selection.y += dy
                selection.height -= dy
                root.returnToBound()
            }
        }
        onReleased: function (mouse) {
            endMove()
        }
    }

    MouseArea {
        id: bottomEdge

        property point originPoint: Qt.point(0, 0)

        height: root._borderSize + root._offsetHoverBoder
        width: visualSelection.width
        x: visualSelection.x
        y: visualSelection.y + visualSelection.height - height / 2

        hoverEnabled: true
        cursorShape: containsMouse ? Qt.SizeVerCursor : Qt.ArrowCursor
        onPressed: function (mouse) {
            originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
            originPoint = root.adjustPostion(originPoint.x, originPoint.y)
            beginMove()
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                const mPos = root.adjustPostion(curPos.x, curPos.y)

                const dy = mPos.y - originPoint.y

                originPoint = mPos

                selection.height += dy
                root.returnToBound()
            }
        }
        onReleased: function (mouse) {
            endMove()
        }
    }

    MouseArea {
        id: topleftCorner

        property point originPoint: Qt.point(0, 0)

        height: root._pointSize
        width: root._pointSize
        x: visualSelection.x - (width - root._borderSize) / 2
        y: visualSelection.y - (height - root._borderSize) / 2

        hoverEnabled: true
        cursorShape: containsMouse ? Qt.SizeFDiagCursor : Qt.ArrowCursor
        onPressed: function (mouse) {
            originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
            originPoint = root.adjustPostion(originPoint.x, originPoint.y)
            beginMove()
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                const mPos = root.adjustPostion(curPos.x, curPos.y)

                const dy = mPos.y - originPoint.y
                const dx = mPos.x - originPoint.x

                originPoint = mPos

                selection.x += dx
                selection.y += dy
                selection.width -= dx
                selection.height -= dy

                root.returnToBound()
            }
        }
        onReleased: function (mouse) {
            endMove()
        }

        Rectangle {
            anchors.fill: parent
            color: root._pointColor
            radius: width / 2
        }
    }

    MouseArea {
        id: toprightCorner

        property point originPoint: Qt.point(0, 0)

        height: root._pointSize
        width: root._pointSize
        x: visualSelection.x + visualSelection.width - (width + root._borderSize) / 2
        y: visualSelection.y - (height - root._borderSize) / 2

        hoverEnabled: true
        cursorShape: containsMouse ? Qt.SizeBDiagCursor : Qt.ArrowCursor
        onPressed: function (mouse) {
            originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
            originPoint = root.adjustPostion(originPoint.x, originPoint.y)
            beginMove()
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                const mPos = root.adjustPostion(curPos.x, curPos.y)

                const dy = mPos.y - originPoint.y
                const dx = mPos.x - originPoint.x

                originPoint = mPos

                selection.y += dy

                selection.width += dx
                selection.height -= dy
                root.returnToBound()
            }
        }
        onReleased: function (mouse) {
            endMove()
        }

        Rectangle {
            anchors.fill: parent
            color: root._pointColor
            radius: width / 2
        }
    }

    MouseArea {
        id: bottomrightCorner

        property point originPoint: Qt.point(0, 0)

        height: root._pointSize
        width: root._pointSize
        x: visualSelection.x + visualSelection.width - (width + root._borderSize) / 2
        y: visualSelection.y + visualSelection.height - (height + root._borderSize) / 2

        hoverEnabled: true
        cursorShape: containsMouse ? Qt.SizeFDiagCursor : Qt.ArrowCursor
        onPressed: function (mouse) {
            originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
            originPoint = root.adjustPostion(originPoint.x, originPoint.y)
            beginMove()
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                const mPos = root.adjustPostion(curPos.x, curPos.y)

                const dy = mPos.y - originPoint.y
                const dx = mPos.x - originPoint.x

                originPoint = mPos

                selection.width += dx
                selection.height += dy
                root.returnToBound()
            }
        }
        onReleased: function (mouse) {
            endMove()
        }

        Rectangle {
            anchors.fill: parent
            color: root._pointColor
            radius: width / 2
        }
    }

    MouseArea {
        id: bottomleftCorner

        property point originPoint: Qt.point(0, 0)

        height: root._pointSize
        width: root._pointSize
        x: visualSelection.x - (width - root._borderSize) / 2
        y: visualSelection.y + visualSelection.height - (height + root._borderSize) / 2

        hoverEnabled: true
        cursorShape: containsMouse ? Qt.SizeBDiagCursor : Qt.ArrowCursor
        onPressed: function (mouse) {
            originPoint = mapToItem(root, Qt.point(mouse.x, mouse.y))
            originPoint = root.adjustPostion(originPoint.x, originPoint.y)
            console.log("originPoint ", originPoint)
            beginMove()
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                const curPos = mapToItem(root, Qt.point(mouse.x, mouse.y))
                const mPos = root.adjustPostion(curPos.x, curPos.y)

                const dy = mPos.y - originPoint.y
                const dx = mPos.x - originPoint.x

                originPoint = mPos

                selection.x += dx

                selection.width -= dx
                selection.height += dy
                root.returnToBound()
            }
        }
        onReleased: function (mouse) {
            endMove()
        }

        Rectangle {
            anchors.fill: parent
            color: root._pointColor
            radius: width / 2
        }
    }

    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.5)
        radius: 4
        width: rlayout.width
        height: 20
        x: selection.x + selection.width - width
        y: selection.y + selection.height + 3

        Row {
            id: rlayout

            anchors.verticalCenter: parent.verticalCenter
            leftPadding: 6
            rightPadding: 6
            spacing: 4

            Text {
                verticalAlignment: Text.AlignVCenter
                rightPadding: 6
                text: "%1x%2 - (%3,%4)".arg(selection.width).arg(selection.height).arg(selection.x).arg(selection.y)

            }
            Rectangle {
                width: 1
                height: parent.height
                opacity: 0.3
            }

            Row {
                anchors.verticalCenter: parent.verticalCenter
                width: 36
                height: 14
                spacing: 8
                Rectangle {
                    width: 14
                    height: width
                    color: "red"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.cancel()
                        }
                    }
                }
                Rectangle {
                    width: 14
                    height: width
                    color: "green"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.crop(selection.x, selection.y, selection.width, selection.height)
                        }
                    }
                }
            }
        }
    }
}
