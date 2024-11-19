import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: confirmDialog
    title: qsTr("Delete branch")
    modal: true
    anchors.centerIn: parent

    property string branchName: ""
    property alias model: selectBranch.model

    // Сигнал для передачи результата
    signal confirmed(string branchName)

    // Добавляем функцию для установки текущего элемента
    function setCurrentBranch(name) {
        branchName = name
        // Ищем индекс элемента в модели
        for(let i = 0; i < selectBranch.model.rowCount(); i++) {
            let index = selectBranch.model.index(i, 0)
            let refName = selectBranch.model.data(index, Qt.DisplayRole)
            if(refName === name) {
                selectBranch.currentIndex = i
                break
            }
        }
    }

    ColumnLayout {
        spacing: 20

        ComboBox {
            id: selectBranch
            textRole: "refName"
            valueRole: "refName"
            implicitHeight: 50
            Layout.fillWidth: true

            delegate: ItemDelegate {
                width: selectBranch.width

                // Добавляем фоновый прямоугольник с цветом из модели
                background: Rectangle {
                    color: refColor  // Используем роль refColor из модели
                    radius: 2
                    border.color: "black"
                    border.width: 1
                }

                contentItem: Text {
                    text: refName
                    color: "black"
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 4
                    rightPadding: 4
                }

                highlighted: selectBranch.highlightedIndex === index
            }

            // Опционально: настройка внешнего вида выбранного элемента
            background: Rectangle {
                color: selectBranch.currentIndex >= 0 ? selectBranch.model.data(selectBranch.model.index(selectBranch.currentIndex, 0), Qt.BackgroundRole) : "white"
                border.color: selectBranch.pressed ? "#888888" : "#cccccc"
                border.width: 1
                radius: 2
            }
        }

        Label {
            text: qsTr("Are you sure you want to delete branch '%1'?").arg(branchName)
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        DialogButtonBox {
            Layout.fillWidth: true
            standardButtons: DialogButtonBox.Yes | DialogButtonBox.No

            onAccepted: {
                confirmDialog.confirmed(selectBranch.currentValue)
                confirmDialog.close()
            }
            onRejected: confirmDialog.close()
        }
    }
}
