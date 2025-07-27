import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import FocusAssist9 1.0

Popup {
    id: popup
    anchors.centerIn: Overlay.overlay;
    implicitWidth: 500
    implicitHeight: 400
    modal: true
    focus: true
    popupType: Popup.Item
    closePolicy: Popup.NoAutoClose

    property BlocklistListModel blocklists;


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
            property int currentIndex: 0;
            spacing: 2
            Repeater {
                id: tabbarRepeater
                model: blocklists
                onItemAdded: (index, item)=>{
                    item.checked = true
                    tabbar.currentIndex = index
                }
                onItemRemoved: (index, item)=>{
                    if (index === tabbar.currentIndex) {
                        if (index >= tabbarRepeater.count) {
                            tabbar.currentIndex -= 1;
                        }
                        tabbarRepeater.itemAt(tabbar.currentIndex).checked = true;
                    }
                }

                TabButton {
                    text: model.name
                    onToggled: tabbar.currentIndex = index;
                }
            }
            TabButton {
                text: "+"
                onToggled: blocklists.appendItem()
            }

            Component.onCompleted: () => {
                tabbarRepeater.itemAt(0).checked = true;
            }

        }

        StackLayout {
            id: stackLayout
            currentIndex: tabbar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true
            Repeater {
                model: blocklists
                delegate: Item {
                    ColumnLayout {
                        anchors.fill: parent;
                        TextInput {
                            text: model.name
                            onTextEdited: model.name = text
                        }
                        Button {
                            text: "delete"
                            onClicked: blocklists.removeItem(index)
                        }

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            TextArea {
                                id: websiteListEditor
                                text: model.item.websiteList
                                onEditingFinished: model.item.websiteList = text
                                Keys.onTabPressed: nextItemInFocusChain().forceActiveFocus(Qt.TabFocusReason)
                            }
                        }
                        Button {
                            text: "Test blocklist"
                            onClicked: ()=>{
                                websiteListEditor.editingFinished()
                                model.item.applyBlocks()
                            }
                        }
                    }
                }
            }
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
