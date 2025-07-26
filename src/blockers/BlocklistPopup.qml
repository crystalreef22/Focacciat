import QtQuick
import QtQuick.Controls
import QtQuick.Controls
import QtQuick.Layouts

import FocusAssist9 1.0

Popup {
    id: popup
    property Blocklist blocklist
    anchors.centerIn: Overlay.overlay;
    implicitWidth: 500
    implicitHeight: 400
    modal: true
    focus: true
    popupType: Popup.Item
    closePolicy: Popup.NoAutoClose

    MouseArea {
        id: clickToUnfocus
        anchors.fill: parent;
        onClicked: forceActiveFocus()
    }

    ColumnLayout{
        anchors.fill: parent;
        Flow {
            id: tabbar
            Layout.fillWidth: true
            spacing: 2

            Repeater {
                id: tabbarRepeater
                model: ["First", "Second", "Third", "Fourth", "Fifth", "tesr", "asd", "tasdkjalskd"]

                TabButton {
                    text: modelData
                    MouseArea {
                        id: modelDataMouseArea
                        propagateComposedEvents: true
                        anchors.fill: parent
                        drag.target: parent
                        drag.filterChildren: true
                        drag.axis: Drag.XAndYAxis
                        drag.minimumX: 0
                        drag.minimumY: tabbarRepeater.y
                        drag.maximumX: tabbarRepeater.x + tabbarRepeater.width
                        drag.maximumY: tabbarRepeater.y + tabbarRepeater.height
                        onPressed: parent.checked = true;
                    }
                }
            }

            Component.onCompleted: () => {
                tabbarRepeater.itemAt(0).checked = true;
           }
        }
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            TextArea {
                id: websiteListEditor
                text: blocklist.websiteList
                onEditingFinished: blocklist.websiteList = text
            }
        }

        Button {
            text: "activate"
        }
        RowLayout {
            Layout.fillWidth: true
            Button {
                text: "close"
                onClicked: popup.close()
            }
        }
    }
}
