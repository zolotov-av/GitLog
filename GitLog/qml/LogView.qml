import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore

Rectangle {
    color: logView.activeFocus ? "lightblue" : "lightgrey"
    clip: true

    property alias currentIndex: logView.currentIndex

    DeleteBranchDialog {
        id: confirmDialog

        onConfirmed: function(branchName) {
            gitlog.removeRef(branchName)
        }
    }

    Menu {
        id: refContextMenu

        property string selectedRefName: ""
        property int commitIndex: -1
        property var refsModel: null

        MenuItem {
            text: qsTr("Checkout branch")
            //visible: refContextMenu.selectedRefName.startsWith("refs/heads/")
            onTriggered: {
                // Логика переключения на ветку
                console.log("Checkout branch:", refContextMenu.selectedRefName)
            }
        }

        MenuItem {
            text: qsTr("Delete branch")
            // Показывать только для веток
            //visible: refContextMenu.selectedRefName.startsWith("refs/heads/")
            onTriggered: {
                confirmDialog.model = refContextMenu.refsModel
                confirmDialog.setCurrentBranch(refContextMenu.selectedRefName)
                console.log("refsModel: " + refContextMenu.refsModel.rowCount())
                confirmDialog.open()
            }
        }

    }

    ListView {
        id: logView
        model: gitlog.logModel
        activeFocusOnTab: true
        anchors.fill: parent

        delegate: Item {

            implicitHeight: txt.implicitHeight * 1.2
            implicitWidth: logView.width

            RowLayout {
                anchors.fill: parent

                GraphItem {
                    implicitWidth: 100
                    implicitHeight: txt.implicitHeight
                    Layout.fillHeight: true

                    model: logView.model
                    time: commitTime
                    itemIndex: index
                }

                Item {
                    implicitWidth: refs.contentWidth
                    implicitHeight: parent.height
                    clip: true

                    ListView {
                        id: refs
                        model: commitRefs
                        orientation: ListView.Horizontal
                        anchors.fill: parent

                        delegate: Item {
                            implicitHeight: refs.height
                            implicitWidth: name.implicitWidth + 8

                            Rectangle {
                                y: 1
                                width: name.implicitWidth + 4
                                height: parent.height - 2
                                color: refColor
                                border.color: "black"
                                border.width: 1

                                Text {
                                    id: name
                                    text: refName
                                    font.family: root.fontFamily
                                    font.pixelSize: root.fontSize - 4
                                    anchors.centerIn: parent
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton  // Принимаем обе кнопки

                                onClicked: function(mouse) {
                                    if (mouse.button === Qt.RightButton) {
                                        // Установим данные для контекстного меню
                                        refContextMenu.refsModel = commitRefs
                                        refContextMenu.selectedRefName = refName
                                        refContextMenu.commitIndex = index
                                        // Показываем меню в позиции клика
                                        refContextMenu.popup()
                                    }
                                }

                                // Предотвращаем распространение события к внешнему MouseArea
                                onPressed: function(mouse) {
                                    mouse.accepted = true
                                }
                                onReleased: function(mouse) {
                                    mouse.accepted = true
                                }
                            }

                        }
                    }
                }

                Text {
                    id: txt
                    text: commitMessage
                    font.family: root.fontFamily
                    font.pixelSize: root.fontSize
                    Layout.fillWidth: true
                }

                Rectangle{
                    visible: false
                    Layout.fillWidth: true;
                    implicitHeight: parent.height

                    RowLayout {
                        anchors.fill: parent

                        clip: true

                    }
                }

            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    logView.currentIndex = index;
                    gitlog.showCommit(index);
                    logView.forceActiveFocus();
                }

                onDoubleClicked: {
                    logView.currentIndex = index;
                    logView.activeItem(index);
                }
            }
        }

        highlight: Item {

            Rectangle {
                x: 0
                y: 0
                height: parent.height
                width: logView.width
                color: "cornflowerblue"
                radius: 5
            }
        }

    }

}

