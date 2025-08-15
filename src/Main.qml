
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Layouts
import QtQuick.Shapes
import QtQuick.Effects

import "components" as MyComponents
import Focacciat 1.0
import Todo 1.0

MaskedApplicationWindow {
    id: root
    width: 170
    height: 300
    minimumWidth: 150
    minimumHeight: 150
    flags: Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    color: "#00000000" // Note: for translucency, use a rectangle with color instead because blending is broken on MacOS
    //opacity: 0.5

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

    }
    function circleSliceLength(x) {
        return 2*Math.sqrt(progressCircle.width*x-x**2) // 2\sqrt{2rs-s^{2}}
    }

    Component{
    id: circleMask
    MultiEffect {
        source: progressContent
        maskEnabled: true
        maskSource: progressCircle
        anchors.fill: progressCircle
    }
    }

    Item {
        layer.enabled: true
        layer.effect: circleMask
        id: progressContent
        width: progressCircle.width
        height: progressCircle.height
        state: todoModel.activeItem ? (todoModel.paused ? "PAUSED" : "TIMER") : "STOPPED"
        Item {
            id: timerViewerWrapper
            anchors.top: parent.top
            width: progressCircle.width
            anchors.bottom: pausedViewer.top
                Column {
                    anchors.centerIn: parent
                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: FormatUtils.msToTime(todoModel.activeItem?.timeRemaining)
                        font.features: {"tnum": 1}
                        font.pixelSize: 24;
                        //width: circleSliceLength(mapToItem(progressCircle, 0, y).y) - 20
                    }
                    Item {
                        width: timerLabelText.width
                        height: timerLabelText.height
                        id: timerLabel
                        visible: circleSliceLength(timerViewerWrapper.height/2) > 150;
                        state: "DISPLAY"
                    Label {
                        id: timerLabelText
                        anchors.horizontalCenter: parent.horizontalCenter
                        topPadding: 4
                        bottomPadding: topPadding
                        text: todoModel.activeItem?.description ?? ""
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 14;
                        fontSizeMode: Text.HorizontalFit
                        wrapMode: Text.Wrap
                        elide: Text.ElideRight
                        width: circleSliceLength(mapToItem(progressCircle, 0, y + timerViewerWrapper.height * 0).y) - 20
                                                                                // ^ update width when this changes

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
                        IconLabel {
                            icon.name: "mail-message-new"
                            icon.width: 24; icon.height: 24
                            icon.color: "black";
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
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: timerLabelText.width
                        text: todoModel.activeItem?.description ?? ""
                        onEditingFinished: () => {
                            if(todoModel.activeItem) {todoModel.activeItem.description = text}
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
                    visible: timerViewerWrapper.height > 100
                    anchors.horizontalCenter: parent.horizontalCenter
                    editorFlags: TimeInput.NoSeconds
                    time: todoModel.activeItem?.timeEstimate / 1000
                    onEditingFinished: ()=>{ if(todoModel.activeItem) {todoModel.activeItem.timeEstimate = time * 1000} }
                }
            }
        }
        Rectangle {
            id: pausedViewer
            width: progressCircle.width
            anchors.bottom: parent.bottom
            color: "yellow"; height: 0; clip: true
            ColumnLayout {
                width: parent.width
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: FormatUtils.msToTime(todoModel.pausedTime)
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    id: pvInfo
                    font.pixelSize: 18;
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    id: pvInfoTaskselTip
                    text: "(right click a task to select)"
                    visible: false
                }

            }
        }
        states: [
            State {
                name: "TIMER"
                PropertyChanges {
                    target: pausedViewer
                    height: 0
                }
            },
            State {
                name: "PAUSED"
                PropertyChanges {
                    target: pausedViewer
                    height: progressCircle.height*0.44
                }
                PropertyChanges {
                    target: pvInfo
                    text: "Paused"
                }
            },
            State {
                name: "STOPPED"
                PropertyChanges {
                    target: pausedViewer
                    height: progressCircle.height
                }
                PropertyChanges {
                    target: timerViewerWrapper
                }
                PropertyChanges {
                    target: pvInfo
                    text: "No task selected"
                    Layout.topMargin: 10
                }
                PropertyChanges {
                    target: pvInfoTaskselTip
                    visible: true
                }
            }
        ]
        transitions: Transition {
            from: "*"
            to: "*"
            NumberAnimation {
                target: pausedViewer
                property: "height"
                duration: 800
                easing.type: Easing.OutQuart
            }
        }
    }
    Shape {
        id: progressCircleProgress
        property int barMargin: 2
        property real progress: todoModel.activeItem ? (todoModel.activeItem.timeElapsed / todoModel.activeItem.timeEstimate) : 0
        anchors.fill: progressCircle
        // anti alias
        layer.enabled: true
        layer.samples: 4
        ShapePath {
            id: pcpath
            strokeColor: myPalette.accent
            fillColor: "#00000000"
            strokeWidth: 8
            capStyle: ShapePath.RoundCap

            PathAngleArc {
                centerX: progressCircleProgress.width/2; centerY: centerX
                radiusX: centerX-pcpath.strokeWidth/2 - progressCircleProgress.barMargin; radiusY: radiusX
                startAngle: -90
                sweepAngle: progressCircleProgress.progress * 360
            }
        }
    }
    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: progressCircle.bottom
        text: todoModel.paused ? "resume" : "pause"
        onClicked: todoModel.paused = !todoModel.paused
        visible: todoModel.activeItem
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
