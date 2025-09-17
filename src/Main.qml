
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Layouts
import QtQuick.Shapes
import QtQuick.Effects
import Qt.labs.platform

import Focacciat 1.0

MaskedApplicationWindow {
    id: root
    width: 170
    height: 300
    minimumWidth: 150
    minimumHeight: timerCircle.height + 5
    flags: Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    color: "#00000000" // Note: for translucency, use a rectangle with color instead because blending is broken on MacOS

    SystemPalette {
        id: myPalette;
    }
    MouseArea {
        id: defaultFocus
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        onClicked: defaultFocus.forceActiveFocus();
        onPressed: root.startSystemMove();
    }
    // RESIZE mousearea is at bottom

    visible: true

    Rectangle {
        id: windowBG
        anchors.fill: parent
        anchors.topMargin: timerCircle.height/2
        color: myPalette.window
    }
    TimerCircle {
        id: timerCircle
        anchors.left: parent.left; anchors.right: parent.right;
    }



    ColumnLayout {
        anchors.top: timerCircle.bottom
        anchors.bottom: parent.bottom
        width: timerCircle.width
        TodoView {
            labelText: "Tasks"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
    MouseArea {
        id: resizeArea
        anchors.fill: parent
        anchors.topMargin: timerCircle.height/2
        hoverEnabled: Qt.platform.os !== "osx" // if Qt 7, change to "macos". MacOS enables resizing by default even for those windows.
        acceptedButtons: Qt.LeftButton

        property int edges: 0;
        property int edgeOffest: 5;

        function setEdges(x, y) {
            edges = 0;
            if(Qt.platform.os === "osx") return;
            if(x < edgeOffest) edges |= Qt.LeftEdge;
            if(x > (width - edgeOffest))  edges |= Qt.RightEdge;
            //if(y < edgeOffest) edges |= Qt.TopEdge;
            if(y > (height - edgeOffest)) edges |= Qt.BottomEdge;
        }

        cursorShape: {
            return !containsMouse ? Qt.ArrowCursor:
                   edges == 3 || edges == 12 ? Qt.SizeFDiagCursor :
                   edges == 5 || edges == 10 ? Qt.SizeBDiagCursor :
                   edges & 9 ? Qt.SizeVerCursor :
                   edges & 6 ? Qt.SizeHorCursor : Qt.ArrowCursor;
        }

        onPositionChanged: (mouse)=>{setEdges(mouseX, mouseY); mouse.accepted = false};
        onPressed: (mouse)=>{
            setEdges(mouseX, mouseY);
            if(edges && containsMouse) {
                startSystemResize(edges);
            } else {
                mouse.accepted = false;
            }
        }
    }

    SystemTrayIcon {
        visible: true
        icon.name: "clear"

        menu: Menu {
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }
    }

}
