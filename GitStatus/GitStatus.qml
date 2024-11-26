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
            id: gitRepo
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

            function gitMenu() {
                if (currentItem) {
                    var pos = currentItem.mapToItem(listView, 0, currentItem.height);
                    contextMenu.popup(pos.x, pos.y)
                }
            }

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

            function gitUpdate() {
                console.log("gitUpdate")
                statusModel.update();
            }

            function handleKeys(event)
            {
                if (event.modifiers !== 0)
                    return;

                switch (event.key )
                {
                case Qt.Key_Space:
                    console.log("listView Qt.Key_Space pressed");
                    listView.gitMenu();
                    event.accepted = true;
                    break;
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

            Item {
                enabled: listView.activeFocus

                Shortcut {
                    sequence: "S"
                    onActivated: listView.gitStage()
                }

                Shortcut {
                    sequence: "R"
                    onActivated: listView.gitRestoreStaged()
                }

                Shortcut {
                    sequence: "C"
                    onActivated: listView.gitCheckoutHead()
                }

                Shortcut {
                    sequences: ["D", StandardKey.Delete]
                    onActivated: {
                        if (listView.currentItem) {
                            deleteConfirmDialog.open()
                        }
                    }
                }

                Shortcut {
                    sequences: ["Ctrl+R", "F5"]
                    onActivated: listView.gitUpdate()
                }

            }

            Menu {
                id: contextMenu

                MenuItem {
                    text: qsTr("Stage") + " (S)"
                    onTriggered: listView.gitStage()
                }
                MenuItem {
                    text: qsTr("Restore staged") + " (R)"
                    onTriggered: listView.gitRestoreStaged()
                }
                MenuItem {
                    text: qsTr("Checkout HEAD") + " (C)"
                    onTriggered: listView.gitCheckoutHead()
                }
                MenuItem {
                    text: qsTr("Remove") + " (DEL)"
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
                    text: qsTr("Remove") + " (D, DEL)"
                }

                Label {
                    text: qsTr("Menu") + " (Space)"
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    id: commitButton
                    text: qsTr("Commit")
                    onClicked: commitDialog.open()
                }
            }

        }
    }

    Dialog {
        id: commitDialog
        title: qsTr("Commit Changes")
        modal: true
        anchors.centerIn: parent
        width: 600
        height: 300

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            TextArea {
                id: commitMessage
                Layout.fillWidth: true
                Layout.fillHeight: true
                placeholderText: qsTr("Enter commit message...")
                wrapMode: TextArea.Wrap
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 10

                Button {
                    text: qsTr("Cancel")
                    onClicked: commitDialog.close()
                }

                Button {
                    text: qsTr("Commit")
                    onClicked: {
                        // TODO: Implement commit functionality
                        console.log("Commit message:", commitMessage.text)
                        commitDialog.close()
                        gitRepo.commit(commitMessage.text)
                        statusModel.update()
                    }
                }
            }
        }
    }

    Dialog {
        id: deleteConfirmDialog
        title: qsTr("Confirm Delete")
        modal: true
        anchors.centerIn: parent
        focus: true

        Label {
            text: listView.currentItem && deleteConfirmDialog.visible ?
                qsTr("Are you sure you want to delete '%1'?").arg(listView.currentItem.fileName) :
                qsTr("No file selected")
        }

        standardButtons: Dialog.Yes | Dialog.No
        onOpened: standardButton(Dialog.Yes).forceActiveFocus()

        onAccepted: listView.gitRemoveFile()
    }
}
