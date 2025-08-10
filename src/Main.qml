
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes

import "components" as MyComponents
import Focacciat 1.0
import Todo 1.0

Window {
    id: root
    width: 170
    height: 250
    maximumWidth: width; maximumHeight: height; minimumWidth: width; minimumHeight: height;
    flags: Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    color: "#00000000" // Note: for translucency, use a rectangle with color instead because blending is broken on MacOS

    SystemPalette {
        id: myPalette;
    }

    visible: true

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: progressCircle.height/2
        color: myPalette.window
    }
    Shape {
        id: progressCircle
        property int barMargin: 2
        property real progress: 0.0
        anchors.left: parent.left; anchors.right: parent.right;
        height: width
        // anti alias
        layer.enabled: true
        layer.samples: 4
        ShapePath {
            fillColor: myPalette.base
            strokeWidth: 0
            PathAngleArc {
                centerX: progressCircle.width/2; centerY: centerX
                radiusY: centerX; radiusX: centerX
                sweepAngle: 360
            }
        }

        ShapePath {
            id: pcpath
            fillColor: "#00000000"
            strokeColor: myPalette.accent
            strokeWidth: 8
            capStyle: ShapePath.RoundCap

            PathAngleArc {
                centerX: progressCircle.width/2; centerY: centerX
                radiusX: centerX-pcpath.strokeWidth/2 - progressCircle.barMargin; radiusY: radiusX
                startAngle: -90
                sweepAngle: progressCircle.progress * 360
            }
        }
    }
    Column {
        Item {
            height: progressCircle.height
            width: progressCircle.width
            MyComponents.TimeInput {
                y: 95
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Button {
            text: 'not so useless button'
            onClicked: progressCircle.progress += 0.01
        }
    }

    DragHandler {
        onActiveChanged: if (active) root.startSystemMove();
        target: null
        grabPermissions: PointerHandler.TakeOverForbidden
    }
}
