import QtQuick 2.0
import QtQuick.Controls 1.0
//import Grafips 1.0

Item {
    id: cpuPolicy
    ComboBox {
        editable: false
        model: ListModel {
            id: model
            ListElement { text: "performance" }
            ListElement { text: "powersave" }
            ListElement { text: "max"}
            ListElement { text: "min"}
        }
        onAccepted: {
            assert (find(currentText) !== -1)
            currentIndex = find(editText)
        }
    }
}
