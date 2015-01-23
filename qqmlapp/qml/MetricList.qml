import QtQuick 2.0
import Grafips 1.0

Item {
    id: currentList
    property var model
    property MetricRouter publisher
    property ActiveMetrics activeMetrics
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: 200

    ListView {
        anchors.fill: parent
        model: currentList.model
        id: metricList

        delegate: Rectangle {
            id: currentRect
            height: 25
            width: 200
            color: model.enabled ? "green" : "red"
            Text { text: name }
            MouseArea {
                anchors.fill: parent;
                onClicked: { 
                    model.enabled = !model.enabled;
                    currentRect.color = model.enabled ? "green" : "red";
                    if (model.enabled) {
                        publisher.Enable(met_id);
                        activeMetrics.add(name)
                    }
                    else {
                        publisher.Disable(met_id);
                        activeMetrics.remove(name)
                    }
                }
                onWheel: {
                    currentGroup.resize(wheel)
                }
            }
        }
    }
}
