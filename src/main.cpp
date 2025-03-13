#include "countersview.h"
#include "countersstorage.h"
#include "countersmodel.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
   
    CountersStorage   countersStorage;
    countersStorage.connect();
    std::vector<unsigned int>   countersData = countersStorage.load();

    CountersModel countersModel;
    countersModel.setCounters(countersData);
    countersModel.startIncrementThread();

    CountersView mainView = CountersView();
    mainView.setCountersDataModel(&countersModel);
    mainView.show();
    
    QObject::connect(&countersModel, &CountersModel::frequencyChanged, &mainView, &CountersView::frequencyChanged);
    
    QObject::connect(&mainView, &CountersView::addCounter, &countersModel, &CountersModel::appendCounter);
    QObject::connect(&mainView, &CountersView::removeCounter, &countersModel, &CountersModel::removeCounter);
    QObject::connect(&mainView, &CountersView::saveCounters, [&countersStorage, &countersModel]() {
        countersStorage.save(countersModel.counters());
    });

    int nRetCode = app.exec();

    countersModel.stopIncrementThread();
    countersStorage.disconnect();
    
    return nRetCode;
}
