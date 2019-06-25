import QtQuick 2.0
import QtQuick.Controls 1.2

Item {
    id:channelset
    signal pressed(int setindex)


    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "Normal"
            NormalSet{
                index:0

            }
        }

        Tab{
            title: "Position"
            Position{
                index:1
            }
        }
        Tab{
            title: "Other"
            OtherSet{
                index:2
            }
        }
    }

    Connections{
        target: setparent
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
