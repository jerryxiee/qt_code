import QtQuick 2.0
import QtQuick.Controls 1.2
//import QtQuick.Controls 2.2 as Control2
import "../../../Controls"

Item {
    property int fontpixelSize : 32
    property int boxwidth: 150

    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "Normal"
            Item{

                ExclusiveGroup{
                    id:boxexcl
                }

                MyCheckBox{
                    id:timeauto
                    anchors.top: parent.top
                    anchors.topMargin: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 100
                    checked: true
                    exclusiveGroup:boxexcl
                    pixSize:fontpixelSize

                    str:qsTr("自动获取网络时间")

                }

                MyCheckBox{
                    id:timemanual
                    anchors.top: timeauto.top
                    anchors.left: timeauto.right
                    anchors.leftMargin: 50
                    exclusiveGroup:boxexcl
                    pixSize:fontpixelSize


                    str:qsTr("手动设置时间")

                }

                Text {
                    id: texttime
                    width: boxwidth
                    text: qsTr("选择时区")
                    anchors.top: timeauto.bottom
                    anchors.topMargin: 20
                    anchors.left: timeauto.left
                    font.pixelSize:fontpixelSize
                    enabled: timemanual.checked
                }

                ComboBox{
                    id:texttimeBox
                    width: boxwidth
                    height:texttime.height
//                    anchors.top: texttime.top
                    anchors.left: texttime.right
                    anchors.leftMargin: 50
                    anchors.verticalCenter: texttime.verticalCenter
                    currentIndex: 8
                    enabled: timemanual.checked

                    model: ["GMT+0","GMT+1","GMT+2","GMT+3","GMT+4","GMT+5","GMT+6","GMT+7","GMT+8","GMT+9","GMT+10","GMT+11",
                    "GMT-1","GMT-2","GMT-3","GMT-4","GMT-5","GMT-6","GMT-7","GMT-8","GMT-9","GMT-10","GMT-11"]

                }

                Text {
                    id: dateset
                    text: qsTr("设置日期")
                    width:boxwidth
                    anchors.top: texttime.bottom
                    anchors.topMargin: 20
                    anchors.left: texttime.left
                    font.pixelSize:fontpixelSize
                    enabled: timemanual.checked
                }

                MyDateTime{
                    id:startd
                    width: boxwidth
                    height: dateset.height
                    anchors.left: dateset.right
                    anchors.leftMargin:50
                    anchors.verticalCenter: dateset.verticalCenter
                    text: (new Date()).toLocaleString(Qt.locale(), "yyyy/MM/dd")
                    enabled: timemanual.checked

                    onValueChanged: console.log("test value:"+value)
                }

                Text {
                    id: timeset
                    text: qsTr("设置时间")
                    width:boxwidth
                    anchors.top: dateset.bottom
                    anchors.topMargin: 20
                    anchors.left: dateset.left
                    font.pixelSize:fontpixelSize
                    enabled: timemanual.checked
                }

                MyDateTime{
                    id:startt
                    anchors.left: timeset.right
                    anchors.leftMargin:50
                    anchors.verticalCenter: timeset.verticalCenter
                    type: "time"
                    width: boxwidth
                    height:timeset.height
                    enabled: timemanual.checked
                    text: (new Date()).toLocaleString(Qt.locale(), "hh/mm/ss")
                    onValueChanged: console.log("test value:"+value)
                }

                MyCheckBox{
                    id:timetype
                    anchors.top: timeset.bottom
                    anchors.topMargin: 20
                    anchors.left: timeset.left
                    checked: true

                    pixSize:fontpixelSize

                    str:qsTr("24小时制")

                }

            }
        }
    }

    Connections{
        target: setparent
        onSuresignal:{
//            pressed(tabView.currentIndex)
            console.log("time set")
        }
        onCancelsignal:{

            console.log("cancel press")
        }
    }

}
