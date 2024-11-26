import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore
import Status

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("Git Status")
    width: 1200
    height: 800

    font.pixelSize: 20

    property GitStatus status;

    Component.onCompleted: {
        statusModel.repository.open()
        statusModel.update()
    }

    GitStatusModel {
        id: statusModel

        repository: GitRepository {
            path: "/home/alex/prj/test/GitTest"
        }
    }

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView
            model: statusModel
            activeFocusOnTab: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            function gitStage() {
                if ( currentItem ) {
                    console.log("gitStage: " + currentItem.fileName);
                    statusModel.stageFile(currentItem.fileName);
                    statusModel.update();
                }
                else
                    console.log("gitStage: <NONE>")
            }

            function gitRestoreStaged() {
                if ( currentItem ) {
                    console.log("gitRestoreStaged: " + currentItem.fileName);
                    statusModel.restoreStaged(currentItem.fileName);
                    statusModel.update();
                }
                else
                    console.log("gitRestoreStaged: <NONE>")
            }

            function gitCheckoutHead() {
                if ( currentItem ) {
                    console.log("gitCheckoutHead: " + currentItem.fileName);
                    statusModel.checkoutHead(currentItem.fileName);
                    statusModel.update();
                }
                else
                    console.log("gitCheckoutHead: <NONE>")
            }

            function gitRemoveFile() {
                if ( currentItem ) {
                    console.log("gitRemoveFile: " + currentItem.fileName);
                    statusModel.removeFile(currentItem.fileName);
                    statusModel.update();
                }
                else
                    console.log("gitRemoveFile: <NONE>")
            }

            function handleKeys(event)
            {
                switch (event.key )
                {
                case Qt.Key_Space:
                    console.log("listView Qt.Key_Space pressed");
                    if (currentItem) {
                        var pos = currentItem.mapToItem(listView, 0, currentItem.height);
                        contextMenu.popup(pos.x, pos.y)
                    }
                    event.accepted = true;
                    break;
                case Qt.Key_S:
                    console.log("listView Qt.Key_S pressed");
                    listView.gitStage();
                    event.accepted = true;
                    break;
                case Qt.Key_R:
                    console.log("listView Qt.Key_R pressed");
                    listView.gitRestoreStaged();
                    event.accepted = true;
                    break;
                case Qt.Key_C:
                    console.log("listView Qt.Key_C pressed");
                    listView.gitCheckoutHead();
                    event.accepted = true;
                    break;
                case Qt.Key_Delete:
                    console.log("listView Qt.Key_Delete pressed");
                    listView.gitRemoveFile();
                    event.accepted = true;
                    break;
                case Qt.Key_F5:
                    console.log("listView Qt.Key_F5 pressed");
                    event.accepted = true
                    listView.model.update()
                }
            }

            Keys.onPressed: event => handleKeys(event)

            delegate: Item {

                id: item

                implicitHeight: txt.implicitHeight
                implicitWidth: listView.width

                readonly property point menuPos: Qt.point(x + width / 4, y + height)

                required property int index
                required property string fileName
                required property string fileStatus
                required property color fileColor

                Label {
                    id: txt
                    text: item.fileName + " (" + item.fileStatus + ")"
                    color: item.fileColor
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        listView.currentIndex = item.index;
                        listView.forceActiveFocus();
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onClicked: (mouse) => {
                        listView.currentIndex = item.index
                        var pos = item.mapToItem(listView, mouse.x, mouse.y)
                        contextMenu.x = pos.x
                        contextMenu.y = pos.y
                        contextMenu.open()
                    }
                }
            }

            highlight: Item {

                Rectangle {
                    x: 0
                    y: 0
                    height: parent.height
                    width: listView.width
                    color: "cornflowerblue"
                    radius: 5
                }
            }

            Menu {
                id: contextMenu

                MenuItem {
                    text: "Stage\tS"
                    onTriggered: listView.gitStage()
                }
                MenuItem {
                    text: "Restore staged\tR"
                    onTriggered: listView.gitRestoreStaged()
                }
                MenuItem {
                    text: "Checkout HEAD\tC"
                    onTriggered: listView.gitCheckoutHead()
                }
                MenuItem {
                    text: "Remove\tDEL"
                    onTriggered: listView.gitRemoveFile();
                }
            }
        }

        Rectangle {
            id: hints
            color: "burlywood"
            implicitHeight: rowLayout.implicitHeight + rowLayout.anchors.margins * 2
            Layout.fillWidth: true

            RowLayout {
                id: rowLayout
                anchors.fill: parent
                anchors.margins: 8
                spacing: 26

                Label {
                    text: qsTr("Keys") + ":"
                }

                Label {
                    text: qsTr("Stage") + " (S)"
                }

                Label {
                    text: qsTr("Restore staged") + " (R)"
                }

                Label {
                    text: qsTr("Checkout HEAD") + " (C)"
                }

                Label {
                    text: qsTr("Remove") + " (DEL)"
                }

                Label {
                    text: qsTr("Menu") + " (Space)"
                }

                Item {
                    Layout.fillWidth: true
                }
            }

        }
    }
}
