import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import "../../../Controls"

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    Connections{
        target: record
        onPressed:{
            if(setindex == index){
                console.log(timesetchnBox.currentIndex)
                console.log(timeCheckbox1.checked)
                VencControl.setVencOpen(timesetchnBox.currentIndex,timeCheckbox1.checked.toString())
                if(timeCheckbox2.checked){
                    VencControl.setVencConfig(timesetchnBox.currentIndex,0,"RecordTime","null")
                }else{
                    VencControl.setVencConfig(timesetchnBox.currentIndex,0,"RecordTime",sett.text+"-"+sett1.text)
                }

            }
        }
    }

    Text{
        id:timesetchn
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 100
        text:qsTr("通道选择")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:timesetchnBox
        width: boxwidth
        anchors.left: timesetchn.right
        anchors.leftMargin: 100
        anchors.top: timesetchn.top
        height:timesetchn.height
        model: ["模拟通道0","模拟通道1","模拟通道2","模拟通道3",
            "模拟通道4","模拟通道5","模拟通道6","模拟通道7"]
    }

    MyCheckBox{
        id:timeCheckbox1
        anchors.top: timesetchn.bottom
        anchors.topMargin: 20
        anchors.left: timesetchn.left
        height: timesetchn.height
        checked: true
        pixSize:fontpixelSize

        str: qsTr("开启录像")
//        style:CheckBoxStyle{
//            Label{
//                font.pixelSize: fontpixelSize
//            }
//        }
//        Font{pixelSize: fontpixelSize}
    }
    ExclusiveGroup{
        id:boxexcl
    }
    MyCheckBox{
        id:timeCheckbox2
        anchors.top: timeCheckbox1.top
        anchors.left: timeCheckbox1.right
        anchors.leftMargin: 100
        height: timesetchn.height
        checked: true
        pixSize:fontpixelSize
        exclusiveGroup:boxexcl

        str: qsTr("全天录像")
    }

    MyCheckBox{
        id:timeCheckbox3
        anchors.top: timeCheckbox1.bottom
        anchors.topMargin: 20
        anchors.left: timeCheckbox1.left

        height: timesetchn.height
        checked: false
        pixSize:fontpixelSize
        exclusiveGroup:boxexcl

        str: qsTr("定时录像")
    }


    MyDateTime{
        id:sett
        anchors.left: timeCheckbox3.right
        anchors.leftMargin: 100
        anchors.verticalCenter: timeCheckbox3.verticalCenter
        type: "time"
        width: 150
        height: timeCheckbox3.height
        text: "00:00:00"//(new Date()).toLocaleString(Qt.locale(), "hh/mm/ss")

//        onPressed: vkb.visible = true;

        onValueChanged: console.log("test value:"+value)
    }

    Label{
        id:lab
        anchors.left: sett.right
        anchors.leftMargin: 20
        anchors.verticalCenter:sett.verticalCenter
        text: "----"

    }
    MyDateTime{
        id:sett1
        anchors.left: lab.right
        anchors.leftMargin: 20
        anchors.verticalCenter: lab.verticalCenter
        type: "time"
        width: 150
        height: sett.height
        text: "23:59:59"//(new Date()).toLocaleString(Qt.locale(), "hh/mm/ss")

//        onPressed: vkb.visible = true;

        onValueChanged: console.log("test value:"+value)
    }

//    SpinBox{
//        id:timehBox
//        minimumValue:1
//        maximumValue: 31
//        anchors.left: timeCheckbox3.right
//        anchors.leftMargin: 100
//        anchors.verticalCenter: timeCheckbox3.verticalCenter

//    }


}
