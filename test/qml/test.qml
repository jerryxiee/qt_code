import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import MyTableViewModel 1.0

//Window{
Rectangle {

    MyTableViewModel {
        id:dataModel
        roles: ["name","gender","na"]
        onDispVideo:console.log(name)

    }


    TableView {
//        anchors.left: parent.left
        anchors.fill: parent
        model: dataModel
        sortIndicatorVisible:true
//        width: 200
//        height: 300
        TableViewColumn { role: "name"; title: "名称"}
        TableViewColumn { role: "gender"; title: "修改时间"}
        TableViewColumn { role: "na"; title: "文件大小"}

        onDoubleClicked: {
            dataModel.oncellDoubleClickedSlot(row,0)
        }


    }


}
