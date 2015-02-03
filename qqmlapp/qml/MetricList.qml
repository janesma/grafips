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
            property int theId: model.met_id
            
            color: model.enabled ? "green" : "red"
            Text { text: name }
            Item {
                id: bogusDrag
                property int theId: model.met_id
                property string name: model.name
                property bool dragActive: elementMouse.drag.active
                onDragActiveChanged: {
                    if (dragActive) {
                        print ("drag started");
                        Drag.start();
                        return;
                    }
                    print ("drag finished");
                    Drag.drop();
                }
                //Drag.active: elementMouse.drag.active 
                //Drag.hotSpot.x: 32
                //Drag.hotSpot.y: 32
                //Drag.dragType: Drag.Automatic
            }
            MouseArea {
                id: elementMouse
                anchors.fill: parent
                drag.target: bogusDrag
                onReleased : {
                    console.log("drag release")
                    if (bogusDrag.Drag.target !== null) {
                        console.log("drag target known")
                        //var t = bogusDrag.Drag.target
                        //t.parent.publisher.Enable(met_id)
                    }
                }
                onClicked: { 
                    model.enabled = !model.enabled;
                    currentRect.color = model.enabled ? "green" : "red";
                    if (model.enabled) {
                        var graphColor = activeMetrics.add(name, met_id)
                        publisher.Enable(met_id);
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
