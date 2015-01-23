import QtQuick 2.0
import Grafips 1.0

Item {
    id: activeMetricNames
    property var model
    property var renderer
    height: justForHeight.height
    property var availableColors: ["black", "brown", "blue", "slategrey", "cornflowerblue", "orchid"]

    Text {
        id: justForHeight
        visible: false
        text: "some text"
    }

    function add(name, id) {
        var c = availableColors[activeMetricsModel.count];
        console.log("color: " + c)
        renderer.setColor(id, c);
        activeMetricsModel.append( { "name" : name,
                                     "displayColor" : c })
        return c;
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
            color: displayColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }
    }
}
