import QtQuick
import QtQuick.Shapes
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Controls.impl

Item {
    height: width;
    Shape {
        id: progressCircle
        anchors.fill: parent
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
        state: GlobalState.todoModel.activeItem ? (GlobalState.todoModel.paused ? "PAUSED" : "TIMER") : "STOPPED"
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
                        text: FormatUtils.msToTime(GlobalState.todoModel.activeItem?.timeRemaining)
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
                        text: GlobalState.todoModel.activeItem?.description ?? ""
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
                        text: GlobalState.todoModel.activeItem?.description ?? ""
                        onEditingFinished: () => {
                            if(GlobalState.todoModel.activeItem) {GlobalState.todoModel.activeItem.description = text}
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

                TimeInput {
                    visible: timerViewerWrapper.height > 100
                    anchors.horizontalCenter: parent.horizontalCenter
                    editorFlags: AbstractTimeInput.NoSeconds
                    time: GlobalState.todoModel.activeItem?.timeEstimate / 1000
                    onEditingFinished: ()=>{ if(GlobalState.todoModel.activeItem) {GlobalState.todoModel.activeItem.timeEstimate = time * 1000} }
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
                    text: FormatUtils.msToTime(GlobalState.todoModel.pausedTime)
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
        property real progress: GlobalState.todoModel.activeItem ? (GlobalState.todoModel.activeItem.timeElapsed / GlobalState.todoModel.activeItem.timeEstimate) : 0
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
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: progressCircle.bottom
        anchors.bottomMargin: 4
        visible: GlobalState.todoModel.activeItem
        FlatButton{
            id: pauseButton
            icon.name: GlobalState.todoModel.paused ? "play" : "pause"
            onClicked: GlobalState.todoModel.paused = !GlobalState.todoModel.paused;
            margins: 2
        }
        FlatButton{
            id: stopButton
            enabled: GlobalState.todoModel.activeItem.timeElapsed !== 0;
            icon.name: "stop"
            holdable: true
            onHoldDone: () => {
                GlobalState.todoModel.paused = true;
                GlobalState.todoModel.pausedTime += GlobalState.todoModel.activeItem.timeElapsed;
                GlobalState.todoModel.activeItem.timeElapsed = 0;
            };
            margins: 2
        }
    }
}
