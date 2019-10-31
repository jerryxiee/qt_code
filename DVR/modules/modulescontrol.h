#ifndef MODULESCONTROL_H
#define MODULESCONTROL_H

#include <QThread>
#include "BasicUsageEnvironment0.hh"


class ModulesControlTaskScheduler: public BasicTaskScheduler0 {
public:
  static ModulesControlTaskScheduler* createNew(unsigned maxSchedulerGranularity = 100000/*microseconds*/);
    // "maxSchedulerGranularity" (default value: 10 ms) specifies the maximum time that we wait (in "select()") before
    // returning to the event loop to handle non-socket or non-timer-based events, such as 'triggered events'.
    // You can change this is you wish (but only if you know what you're doing!), or set it to 0, to specify no such maximum time.
    // (You should set it to 0 only if you know that you will not be using 'event triggers'.)
  virtual ~ModulesControlTaskScheduler();

#ifndef MILLION
#define MILLION 1000000
#endif

protected:
  ModulesControlTaskScheduler(unsigned maxSchedulerGranularity);
      // called only by "createNew()"

  static void schedulerTickTask(void* clientData);
  void schedulerTickTask();

protected:
  // Redefined virtual functions:
  virtual void SingleStep(unsigned maxDelayTime);

  virtual void setBackgroundHandling(int socketNum, int conditionSet, BackgroundHandlerProc* handlerProc, void* clientData){}
  virtual void moveSocketHandling(int oldSocketNum, int newSocketNum){}

private:


protected:
  unsigned fMaxSchedulerGranularity;

  // To implement background operations:

private:

};


class ModulesControl : public QThread
{
    Q_OBJECT
public:
    static ModulesControl *getModulesControl();
    ~ModulesControl();
    TaskScheduler &getTaskScheduler() const;
    void initModules();

protected:
    explicit ModulesControl(QObject *parent = nullptr);
    virtual void run();
signals:

public slots:

private:
    bool mRun;
    ModulesControlTaskScheduler *mModulesControlTask;
    static ModulesControl *mModulesControl;
};

#endif // MODULESCONTROL_H
