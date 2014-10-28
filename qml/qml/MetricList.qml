import QtQuick 2.0

Item {
    ListView {
        anchors.fill: parent
        model: subscriber.metrics
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
