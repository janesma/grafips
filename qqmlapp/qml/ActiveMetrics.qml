import QtQuick 2.0
import Grafips 1.0

Item {
    id: activeMetricNames
    property var model
    height: justForHeight.height

    Text {
        id: justForHeight
        visible: false
        text: "some text"
    }

    function add(name) {
        activeMetricsModel.append( { "name" : name })
    }

    function remove(name) {
        for (var i = 0; i < activeMetricsModel.count; ++i) {
            var current = activeMetricsModel.get(i);
            if (current["name"] == name) {
                activeMetricsModel.remove(i);
                break;
            }
        }
    }

    ListModel {
        id: activeMetricsModel
    }
    ListView {
        visible: true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height
        model: activeMetricsModel
        id: activeListView
        orientation: ListView.Horizontal
        spacing: 15
        delegate: Text {
            text: name
            id: textDel
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }
    }
}
