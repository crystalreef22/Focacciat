
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "components" as MyComponents
import Focacciat 1.0
import Todo 1.0

Window {
    id: root
    width: 200
    height: 200
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground
    minimumHeight: height
    maximumHeight: height
    minimumWidth: width
    maximumWidth: width
    color: "#00000000" // Note: for translucency, use a rectangle with color instead because blending is broken on MacOS

    visible: true
    Column {
        anchors.fill: parent
        Label {
            anchors.left: parent.left; anchors.right: parent.right;
            text: "main window not implemented. open with --debugwindow instead"
            wrapMode: Text.Wrap
        }
        Button {
            text: "useless button"
        }
    }
    DragHandler {
        onActiveChanged: if (active) root.startSystemMove();
        target: null
        grabPermissions: PointerHandler.TakeOverForbidden
    }
}
