import QtQuick 2.0
import QtQuick.Controls 1.4
import MyTableViewModel 1.0

Rectangle {

    MyTableViewModel {
        id:dataModel
        roles: ["name","gender","na"]
        onFileNameChanged:{
            videoDispSignal(name)
            console.log("display"+name)
        }

    }

    TableView {
        anchors.left: parent.left
        anchors.fill: parent
        model: dataModel
        sortIndicatorVisible:true
//        width: 200
//        height: 300

        TableViewColumn { role: "name"; title: "名称"; /*width: 100;*/delegate:itemDelegateText}
        TableViewColumn { role: "gender"; title: "修改时间";/*width: 100;*/delegate:itemDelegateText}
        TableViewColumn { role: "na"; title: "文件大小";/*width: 100;*/delegate:itemDelegateText}

        onDoubleClicked: {
            dataModel.oncellDoubleClickedSlot(row,0)
        }

//        onClicked: console.log()
        Component{
            id:itemDelegateText
            Text {
                id: name
                text: styleData.value
                font.pointSize: 12
            }

        }


    }
}
