
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes

import "components" as MyComponents
import Focacciat 1.0
import Todo 1.0

ApplicationWindow {
    id: root
    width: 170
    height: 250
    //maximumWidth: width; maximumHeight: height; minimumWidth: width; minimumHeight: height;
    flags: Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    color: "#00000000" // Note: for translucency, use a rectangle with color instead because blending is broken on MacOS

    SystemPalette {
        id: myPalette;
    }
    MouseArea {
        id: defaultFocus
        anchors.fill: parent
        onClicked: defaultFocus.forceActiveFocus();
        drag.onActiveChanged: if (drag.active) root.startSystemMove();
        drag.target: defaultFocus
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
        property real progress: 0.7
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
            Column {
                anchors.centerIn: parent;
                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: FormatUtils.msToTime(1543112)
                    font.pixelSize: 24;
                }
                Item {
                    width: timerLabelText.width
                    height: timerLabelText.height
                    id: timerLabel
                    visible: progressCircle.width > 150;
                    state: "DISPLAY"
                    Label {
                        id: timerLabelText
                        anchors.horizontalCenter: parent.horizontalCenter
                        topPadding: 4
                        bottomPadding: topPadding
                        text: "woaaaaaaaaaaaaaaaaaaah"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 14;
                        fontSizeMode: Text.HorizontalFit
                        wrapMode: Text.Wrap
                        elide: Text.ElideRight
                        width: progressCircle.width - 40
                        maximumLineCount: 2
                        MouseArea{
                            id: timerLabelArea
                            anchors.fill: parent
                            onClicked: timerLabel.state = "EDITING"
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
                    Image {
                        source: "media/edit_24.png"
                        width: 24; height: 24
                        anchors.centerIn: parent
                        visible: timerLabelArea.containsMouse
                    }

                    TextField {
                        id: timerLabelEdit
                        visible: false
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.right: parent.right
                        text: "woah 2"
                        onEditingFinished: () => {
                            timerLabel.state = "DISPLAY"
                        }
                    }
                    states: [
                        State {
                            name: "DISPLAY"
                            PropertyChanges {
                                target: timerLabelText
                                visible: true
                            }
                            PropertyChanges {
                                target: timerLabelEdit
                                visible: false
                            }
                        },
                        State {
                            name: "EDITING"
                            PropertyChanges {
                                target: timerLabelText
                                visible: false
                            }
                            PropertyChanges {
                                target: timerLabelEdit
                                visible: true
                            }
                            StateChangeScript {
                                script: timerLabelEdit.forceActiveFocus();
                            }
                        }
                    ]
                }

                MyComponents.TimeInput {
                    visible: progressCircle.width > 100
                    anchors.horizontalCenter: parent.horizontalCenter
                    editorFlags: TimeInput.NoSeconds
                }
            }
        }

    }

}
