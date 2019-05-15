import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import MyTableViewModel 1.0

//Window{
Rectangle {

    visible: true
    width: 640
    height: 480

    signal refrushModel()

    MyTableViewModel {
        id:dataModel
        roles: ["name","gender","na"]
    }

    TableView {
        anchors.left: parent.left
//        anchors.fill: parent
        model: dataModel
        sortIndicatorVisible:true
        width: 200
        height: 300
        TableViewColumn { role: "name"; title: "Title"; width: 100}
        TableViewColumn { role: "gender"; title: "Gender";width: 100}
        TableViewColumn { role: "na"; title: "Na";width: 100}


    }

    Button{
        text: qsTr("press")
        x:400;y:400
        onClicked:{

//            dataModel.roles = ["dfgf","dsgffdg"]
            refrushModel()
        }
    }


}
