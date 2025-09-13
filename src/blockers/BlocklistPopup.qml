import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Focacciat 1.0

Popup {
    id: popup
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
            property int currentIndex: 0;
            spacing: 2
            Repeater {
                id: tabbarRepeater
                model: GlobalState.blocklistListModel
                onItemAdded: (index, item)=>{
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
                    checked: tabbar.currentIndex === index;
                }
            }
            TabButton {
                text: "+"
                onToggled: GlobalState.blocklistListModel.appendItem()
            }

        }

        StackLayout {
            id: stackLayout
            currentIndex: tabbar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true
            Repeater {
                model: GlobalState.blocklistListModel
                delegate: Item {
                    ColumnLayout {
                        anchors.fill: parent;
                        Label {
                            text: model.item.UUID.toString()
                        }

                        TextField {
                            text: model.name
                            onTextEdited: model.name = text
                        }
                        Button {
                            text: "delete"
                            onClicked: GlobalState.blocklistListModel.removeItem(index)
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
