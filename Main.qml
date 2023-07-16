import QtQuick
import QtQuick.Controls
import QtQuick.Window

import painttools

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    //    Component.onCompleted: {
    //        console.log("screen.devicePixelRatio ", screen.devicePixelRatio)
    //    }
    Rectangle {
        anchors.fill: imgEditor
        border.width: 1
        border.color: "red"
    }

    SNImageEditor {
        id: imgEditor
        width: parent.width - 50
        height: parent.height - 50

        anchors.centerIn: parent

        TextInputZone {
            id: txtEditZone

            textInput.font.pixelSize: 60
            textInput.scale: 1 / imgEditor.viewPortScale
            textInput.transformOrigin: Item.TopLeft
            onNewText: function (x, y, text) {
                imgEditor.addText(x, y, 60, txtEditZone.textInput.color, text)
                txtEditZone.enabled = false
            }
        }

        RectangleSelectZone {
            id: cropZone

            width: imgEditor.imgSize.width
            height: imgEditor.imgSize.height
            x: imgEditor.imgPos.x
            y: imgEditor.imgPos.y

            onCrop: function (x, y, w, h) {
                imgEditor.crop(x, y, w, h)
                cropZone.enabled = false
            }

            onCancel: function (x, y, w, h) {
                cropZone.enabled = false
            }
        }
    }

    Row {
        spacing: 5

        Button {
            text: "save"
            onClicked: {
                imgEditor.save()
            }
        }

        Button {
            text: "pencil"
            onClicked: {
                imgEditor.activeTool(SNAbstractTool.TYPE_PENCIL)
                txtEditZone.enabled = false
            }
        }

        Button {
            text: "circle"
            onClicked: {
                imgEditor.activeTool(SNAbstractTool.TYPE_CIRCLE)
                txtEditZone.enabled = false
            }
        }

        Button {
            text: "rectangle"
            onClicked: {
                imgEditor.activeTool(SNAbstractTool.TYPE_RECTANGLE)
                txtEditZone.enabled = false
            }
        }

        Button {
            text: "line"
            onClicked: {
                imgEditor.activeTool(SNAbstractTool.TYPE_LINE)
                txtEditZone.enabled = false
            }
        }

        Button {
            text: "text"
            onClicked: {
                imgEditor.activeTool(SNAbstractTool.TYPE_TEXT)
                txtEditZone.enabled = true
            }
        }

        Button {
            text: "crop"
            onClicked: {
                cropZone.enabled = true
            }
        }

        Button {
            text: "undo"
            onClicked: {
                imgEditor.undo()
            }
        }

        Slider {
            width: 100
            value: 10
            from: 4
            to: 100
            onValueChanged: {
                imgEditor.setLineWidth(value)
            }
        }
    }
}
