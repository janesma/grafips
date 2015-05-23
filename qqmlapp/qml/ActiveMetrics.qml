import QtQuick 2.0
import Grafips 1.0
import Qt.labs.settings 1.0

Item {
    id: activeMetricNames
    property var model
    property var renderer
    property var publisher
    height: justForHeight.height
    property var availableColors: ["black", "brown", "blue", "slategrey", "cornflowerblue", "orchid"]
    property string offset

    Text {
        id: justForHeight
        visible: false
        text: "some text"
    }

    // keeps track of active metrics from the previous session
    Settings {
        id: metricPrefs
        category: "Metrics_" + activeMetricNames.offset
        // need one entry in the associative array to set the type,
        // otherwise get TypeError when we add to it.
        property var activated: { "" : 0 }
    }

    function add(name, id) {
        // skip the initial entry, { "" : 0 } ,  which doesn't correspond to a metric
        if (name == "")
            return;
        // ensure the item isn't already being graphed
        for (var i = 0; i < activeMetricsModel.count; ++i) {
            var current = activeMetricsModel.get(i);
            if (current["name"] == name) {
                return;
            }
        }

        // we need to save this activated metric in the prefs.  Make a
        // copy of the prefs associative array, then update it, and
        // save the new copy in the Settings.
        var newPref = metricPrefs.activated
        newPref[name] = id
        metricPrefs.activated = newPref

        var c = availableColors[activeMetricsModel.count];
        renderer.setColor(id, c);
        activeMetricsModel.append( { "name" : name,
                                     "id" : id,
                                     "displayColor" : c })
        return c;
    }

    function remove(name) {
        // update the preference Settings to reflect that this metric
        // shouldn't be activated on launch.  Make a copy of the
        // associative array so it can be updated.
        var newPref = metricPrefs.activated

        for (var i = 0; i < activeMetricsModel.count; ++i) {
            var current = activeMetricsModel.get(i);
            if (current["name"] == name) {
                publisher.Deactivate(current["id"])
                activeMetricsModel.remove(i);
                delete newPref[name]
            } 
        }

        // save the new, smaller array
        metricPrefs.activated = newPref
    }

    ListModel {
        id: activeMetricsModel
    }

    Component {
        id: list_delegate
        Item {
            width: textDel.width + textDel.height
            Image {
                id: closeImage
                anchors.left: parent.left
                height: textDel.height
                width: textDel.height
                source: "icons/close_icon.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        activeMetricNames.remove(name)
                    }
                }
            }
            Text {
                anchors.left: closeImage.right
                text: name
                id: textDel
                color: displayColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
            }
        }
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
        delegate: list_delegate
    }
}
