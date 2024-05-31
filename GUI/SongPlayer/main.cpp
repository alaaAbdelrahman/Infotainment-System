#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include "PlayerController.h"
#include "AudioSearchModel.h"
#include "doorstate.h"
#include "batteryindicator.h"
#include "tempindicator.h"
/*######################################### client ################################*/
#include "client.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/SongPlayer/assets/icons/app_icon.ico"));

    QQmlApplicationEngine engine;


    /**************************************************************************/
    Client * sensorClient{new Client(&app)};

    /*##################################################################*/
    /** declare sensors and exposing them to QML*/
    DoorState *doorSensors{new DoorState(&app)};
    qmlRegisterSingletonInstance("com.company.DoorState", 1, 0, "DoorState", doorSensors);

    BatteryIndicator* batterySensor{new BatteryIndicator(&app)};
    qmlRegisterSingletonInstance("com.company.BatteryIndicator", 1, 0, "BatteryIndicator", batterySensor);


    TempIndicator *  tempSensor{ new TempIndicator(&app)};
    qmlRegisterSingletonInstance("com.company.TempIndicator", 1, 0, "TempIndicator", tempSensor);

    PlayerController *playerController = new PlayerController(&app);
    qmlRegisterSingletonInstance("com.company.PlayerController", 1, 0, "PlayerController", playerController);

    AudioSearchModel *audioSearchModel = new AudioSearchModel(&app);
    qmlRegisterSingletonInstance("com.company.AudioSearchModel", 1, 0, "AudioSearchModel", audioSearchModel);

    int interval = 100; // Example: Run every 1 second


    // connect
    sensorClient->connectToHost("10.204.25.109",8080);
    qInfo()<<sensorClient->sensorId();

    // request
    char currentSensorId ='D';

    // Create a QTimer for periodic execution
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {

        sensorClient->sendData(currentSensorId);
        qInfo()<<currentSensorId;
        // Switch to the next sensor ID
        if (currentSensorId == 'D') {
            currentSensorId = 'T';
        } else if (currentSensorId == 'T') {
            currentSensorId = 'B';
        } else {
            currentSensorId = 'D';
        }

        // Perform switch case logic here
        switch (sensorClient->sensorId()) {

        case 'D':
            doorSensors->setDoor(sensorClient->value());
            break;
        case 'B':
            batterySensor->setBatValue(sensorClient->value());
            break;
        case 'T':
            tempSensor->setValue(sensorClient->value());
            break;
        }


    });

    // Start the timer with a desired interval (in milliseconds)
    timer.start(interval);





    const QUrl url(u"qrc:/SongPlayer/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
