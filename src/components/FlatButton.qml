import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Controls.impl

AbstractButton {
    id: control
    implicitWidth: 24 + margins;
    implicitHeight: 24 + margins;
    hoverEnabled: true;
    property bool holdable: false;
    property real holdTime: 1000;
    property real margins: 1;
    signal holdDone();
    SystemPalette {
        id: myPalette;
    }
    icon.color: myPalette.text
    icon.width: 16;
    icon.height: 16;
    contentItem: IconLabel {
        id: il
        icon: control.icon
        anchors.centerIn: parent
    }
    background: Item {
        anchors.fill: parent
        anchors.margins: control.margins
        Rectangle {
            anchors.fill: parent
            id: bgrect
            layer.enabled: true
            color: "#606060"
            opacity: 0
            radius: 2000
            states: [
                State{
                    name: "pressed"; when: control.pressed;
                    PropertyChanges {
                        target: bgrect
                        opacity: 0.5
                    }
                },
                State{
                    name: "hover"; when: control.hovered;
                    PropertyChanges {
                        target: bgrect
                        opacity: 0.2
                    }
                }
            ]

            transitions: Transition {
                from: ""; to: "hover";
                reversible: true;
                NumberAnimation { properties: "opacity"; easing.type: Easing.Linear; duration: 50 }
            }
        }
        Component{
        id: circleMask
        MultiEffect {
            maskEnabled: true
            maskSource: bgrect
            anchors.fill: bgrect
        }
        }
        Item {
            layer.enabled: true
            layer.effect: circleMask
            visible: holdable;
            anchors.fill: bgrect;
            Rectangle {
                id: bgindicator
                anchors.fill: parent;
                anchors.topMargin: bgrect.height;
                color: "red";
                states: [
                    State {
                        name: "pressed";
                        PropertyChanges {
                            target: bgindicator
                            anchors.topMargin: 0;
                        }
                    },
                    State {
                        name: "completed";
                        PropertyChanges {
                            target: bgindicator
                            color: "#0e0";
                        }
                    }

                ]
                Connections {
                    target: control
                    function onPressedChanged() {
                        if (bgindicator.state !== "completed") {
                            bgindicator.state = pressed ? "pressed" : "";
                        }
                    }
                }

                transitions: [
                    Transition {
                        from: "*"; to: "pressed";
                        SequentialAnimation {
                            NumberAnimation { property: "anchors.topMargin"; easing.type: Easing.Linear; duration: holdTime}
                            ScriptAction { script: bgindicator.state = "completed"; }
                        }
                    },
                    Transition {
                        from: "pressed"; to: "";
                        NumberAnimation { property: "anchors.topMargin"; easing.type: Easing.OutQuart; duration: 100}
                    },
                    Transition {
                        from: "pressed"; to: "completed";
                        SequentialAnimation {
                            ScriptAction { script: control.holdDone(); }
                            PropertyAnimation {property: "color"; easing.type: Easing.OutQuart; duration: 300}
                            NumberAnimation { property: "anchors.topMargin"; easing.type: Easing.OutQuart; duration: 100}
                            ScriptAction { script: bgindicator.state = ""; }
                        }
                    }

                ]
            }
        }
    }
}
