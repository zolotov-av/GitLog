import QtQuick
import QtQuick.Controls

ComboBox {
    id: root
    textRole: "refName"
    valueRole: "refName"
    implicitHeight: 50

    delegate: ItemDelegate {
        width: root.width

        background: Rectangle {
            color: refColor
            radius: 2
            border.color: "lightgrey"
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

        highlighted: root.highlightedIndex === index
    }

    // Опционально: настройка внешнего вида выбранного элемента
    background: Rectangle {
        color: root.currentIndex >= 0 ? root.model.data(root.model.index(root.currentIndex, 0), Qt.BackgroundRole) : "white"
        border.color: root.pressed ? "#888888" : "#cccccc"
        border.width: 1
        radius: 2
    }
}
