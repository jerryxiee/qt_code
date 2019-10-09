import QtQuick 2.0
import QtQuick.Controls 1.2

Item {
    id:bubiaoconfig
    signal pressed(int setindex)


    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "驾驶设置"
            DriverSet{
                index:0

            }
        }

        Tab{
            title: "电话设置"
            PhoneSet{
                index:1
            }
        }
        Tab{
            title: "位置设置"
            PositionSet{
                index:2
            }
        }
        Tab{
            title: "IC卡认证"
            ICSet{
                index:3
            }
        }
    }

    Connections{
        target: platformroot
        onSuresignal:{
            pressed(tabView.currentIndex)
            console.log("sure press")
        }
        onCancelsignal:{

            console.log("cancel press")
        }
    }

//    onDestroyed: console.log("chnset destroyed")

}
