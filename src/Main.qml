
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes

import "components" as MyComponents
import Focacciat 1.0
import Todo 1.0

MaskedApplicationWindow {
    id: root
    width: 170
    height: 250
    flags: Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    color: "#00000000" // Note: for translucency, use a rectangle with color instead because blending is broken on MacOS

    TodoModel{id: todoModel}

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
        anchors.fill: parent
        anchors.topMargin: progressCircle.height/2
        color: myPalette.window
    }
    Shape {
        id: progressCircle
        property int barMargin: 2
        property real progress: todoModel.activeItem.timeElapsed / todoModel.activeItem.timeEstimate
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
    Item {
        height: progressCircle.height
        width: progressCircle.width
        Column {
            anchors.centerIn: parent;
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: FormatUtils.msToTime(todoModel.activeItem.timeRemaining)
                font.features: {"tnum": 1}
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
                    text: todoModel.activeItem.description
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
                        cursorShape: Qt.IBeamCursor
                    }
                }
                Rectangle {
                    width: 24; height: 24
                    Image {
                        source: "media/edit_24.png"
                        width: 24; height: 24
                    }
                    color: "white"
                    radius: 5
                    anchors.centerIn: parent
                    visible: timerLabelArea.containsMouse
                }

                TextField {
                    id: timerLabelEdit
                    visible: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: todoModel.activeItem.description
                    onEditingFinished: () => {
                        todoModel.activeItem.description = text
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
                time: todoModel.activeItem.timeEstimate
                onEditingFinished: todoModel.activeItem.timeEstimate = time
            }
        }
    }
    ColumnLayout {
        anchors.top: progressCircle.bottom
        anchors.bottom: parent.bottom
        width: progressCircle.width
        TodoView {
            labelText: "Tasks"
            Layout.fillWidth: true
            Layout.fillHeight: true
            todoModel: todoModel
            blocklists: BlocklistListModel{}
        }

    }
    MouseArea {
        id: resizeArea
        anchors.fill: parent
        anchors.topMargin: progressCircle.height/2
        hoverEnabled: Qt.platform.os === "osx" // if Qt 7, change to "macos". MacOS enables resizing by default even for those windows.
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

        onPositionChanged: setEdges(mouseX, mouseY);
        onPressed: (mouse)=>{
            setEdges(mouseX, mouseY);
            if(edges && containsMouse) {
                startSystemResize(edges);
            } else {
                mouse.accepted = false;
            }
        }
    }

}
