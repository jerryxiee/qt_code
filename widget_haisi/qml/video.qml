import QtQuick 2.0
import QtQuick.Controls 1.4
import MyTableViewModel 1.0

Rectangle {
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
        TableViewColumn { role: "name"; title: "名称"; width: 100}
        TableViewColumn { role: "gender"; title: "修改时间";width: 100}
        TableViewColumn { role: "na"; title: "文件大小";width: 100}

//        onDoubleClicked: {

//        }


    }
}
