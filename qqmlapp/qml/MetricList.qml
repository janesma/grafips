import QtQuick 2.0

Item {
    id: currentGroup
    property var model
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    ListView {
        anchors.fill: parent
        model: currentGroup.model
        id: metricList

        delegate: Rectangle {
            id: currentRect
            height: 25
            width: 200
            color: model.enabled ? "green" : "red"
            Text { text: name  + met_id.toString() }
            MouseArea {
                anchors.fill: parent;
                onClicked: { 
                    model.enabled = !model.enabled;
                    currentRect.color = model.enabled ? "green" : "red";
                }
            }
        }
    }
}
