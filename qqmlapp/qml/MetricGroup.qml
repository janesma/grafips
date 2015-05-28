import QtQuick 2.2
import QtQuick.Layouts 1.1
import Grafips 1.0
import Qt.labs.settings 1.0

Item {
    id: currentGroup
    Layout.fillWidth:  true
    Layout.fillHeight: true
    Layout.minimumHeight:20
    Layout.preferredHeight: 400
    property string color
    property MetricRouter publisher
    property string offset

    function formatFloat(num) {
        if (num < 10) {
            return num.toPrecision(2);
        }
        return num.toFixed(0);
    }

    function refresh() {
        visible = false
        visible = true
    }

    function toggleShown()
    {
        if (Layout.maximumHeight != 20) {
            Layout.maximumHeight = 20
        } else {
            Layout.maximumHeight = Number.POSITIVE_INFINITY
        }
        refresh()
    }

    function resize(wheel) {
        var amount = wheel.angleDelta.y / 4
        if (Layout.preferredHeight + wheel.angleDelta.y < 20)
            Layout.preferredHeight = 20
        else
            Layout.preferredHeight += amount

        refresh()
    }

    function start() {
        publisher.AddGraph(graphView.subscriber);

        // on startup, we want to activate any metrics that were saved
        // from the previous run.
        var savedMetrics = metricPrefs.activated
        for (var name in savedMetrics) {
            if (name == 0)
                // skip the initial entry which sets the type  { "" : 0 }
                continue;
            currentGroup.publisher.ActivateToGraph(savedMetrics[name], graphView.subscriber);
            activeMetricNames.add(name, savedMetrics[name]);
        }
    }

    function stop() {
        graphView.stop();
    }

    Settings {
        id: metricPrefs
        category: "Metrics_" + currentGroup.offset
        // initial entry sets the type of the associative array
        property var activated: { "" : 0 }
    }

    DropArea {
        anchors.fill: currentGroup
        property int id
        property string name
        property var source
        onEntered: {
            id = drag.source.theId
            name = drag.source.name
            source = drag.source
        }
        onDropped: {
            currentGroup.publisher.ActivateToGraph(id, graphView.subscriber);
            activeMetricNames.add(name, id)
            // hide the drag visualizer
            source.x = 0;
            source.y = 0;
            source.visible = false;
        }

        Renderer {
            id: graphView
            visible: true
            anchors.top: parent.top
            anchors.bottom: activeMetricNames.top
            anchors.right: scale.left
            anchors.left: parent.left
            publisher: currentGroup.publisher
            onOnGraphMax : { axisText.visible = true; }
        }

        Rectangle {
            id: scale
            anchors.top: parent.top
            anchors.bottom: activeMetricNames.top
            anchors.right: parent.right
            width: axisText.width
            Text {
                id: axisText
                visible: false
                lineHeightMode: Text.FixedHeight
                lineHeight: scale.height / 5.0
                text: formatFloat(graphView.graphMax) + "\n"
                    + formatFloat(graphView.graphMax * 0.8) + "\n"
                    + formatFloat(graphView.graphMax * 0.6) + "\n"
                    + formatFloat(graphView.graphMax * 0.4) + "\n"
                    + formatFloat(graphView.graphMax * 0.2)
            }
        }
        ActiveMetrics {
            id: activeMetricNames
            offset: currentGroup.offset
            visible: true
            renderer: graphView
            publisher: currentGroup.publisher
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
        }
    }
}
