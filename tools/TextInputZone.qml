import QtQuick 2.15

import painttools

MouseArea {
    id: root

    signal newText(int x, int y, string text)

    property alias textInput: tex

    enabled: false
    anchors.fill: parent
    onClicked: function (mouse) {
        tex.x = mouse.x
        tex.y = mouse.y

        tex.forceActiveFocus()
        tex.visible = true

        mouse.accepted = false
    }

    TextInput {
        id: tex
        width: Math.max(implicitWidth, 20)
        height: Math.max(implicitHeight, 20)
        padding: 0
        bottomPadding: 0
        visible: parent.enabled
        onAccepted: {
            root.newText(x, y, tex.text)

            tex.clear()
            tex.visible = false
        }
    }
}
