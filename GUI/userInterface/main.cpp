#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

/*###########################################################################3###############*/
#include "playercontroller.h"

/*######################################  sensors #############################################*/

#include "doorstate.h"
#include "batteryindicator.h"
#include "tempindicator.h"
 /*######################################### client ################################*/

//#include "sensorclient.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
     app.setWindowIcon(QIcon("qrc:/userInterface/assets/icons/app_icon.ico"));


    /**************************************************************************/
     Client * sensorClient{new Client(&app)};

    /*##################################################################*/
    /** declare sensors and exposing them to QML*/
    DoorState *doorSensors{new DoorState(&app)};
   BatteryIndicator* batterySensor{new BatteryIndicator(&app)};
   TempIndicator *  tempSensor{ new TempIndicator(&app)};
   PlayerController *playerController{new PlayerController(&app)};

     qmlRegisterSingletonInstance("com.company.PlayerController", 1, 0, "PlayerController", playerController);
   qmlRegisterSingletonInstance("com.company.DoorState", 1, 0, "DoorState", doorSensors);
   qmlRegisterSingletonInstance("com.company.BatteryIndicator", 1, 0, "BatteryIndicator", batterySensor);
   //qmlRegisterSingletonInstance("com.company.TempIndicator", 1, 0, "TempIndicator", tempSensor);


     /*###################################################################*/



    // connect

    // request


   switch (sensorClient->sensorId()) {
   case 'D':
       doorSensors->setDoor(static_cast<char>(sensorClient->value()));
       break;
   case 'B':
       batterySensor->setBatValue(sensorClient->value());
       break;
   case 'T':
      // tempSensor->setValue(sensorClient->value());
       break;

   }
    //


   QQmlApplicationEngine engine;
   const QUrl url(u"qrc:/userInterface/Main.qml"_qs);
   QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
       &app, [url](QObject *obj, const QUrl &objUrl) {
           if (!obj && url == objUrl)
               QCoreApplication::exit(-1);
       }, Qt::QueuedConnection);
   engine.load(url);

    return app.exec();
}
