#ifndef __ALOHA_STATION_H_
#define __ALOHA_STATION_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace aloha {

class Station : public cSimpleModule
{
  private:
    // parameters
    double C;         // channel capacity (bytes/s)
    double S;         // message size (bytes)
    double W;         // waiting time for acknowledgment ACK
    int N;            // number of stations
    cPar *T = nullptr; // packet inter-arrival time
    simtime_t slotTime;
    bool isSlotted = false;
    simtime_t D;      // transmission duration = S/C

    // state variables
    int k = 0;
    int pkCounter = 0;
    enum { IDLE = 0, TRANSMIT = 1 } state = IDLE;

    // events
    cMessage *endTxEvent = nullptr;
    cMessage *ackEvent = nullptr;

    cModule *hub = nullptr;

    // station position and propagation delay
    double x, y;
    const double propagationSpeed = 299792458.0; // speed of light in meters per second,
    simtime_t radioDelay;

    // signals
    simsignal_t stateSignal;

    // message delay
    simsignal_t delaySignal;

  public:
    virtual ~Station();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    simtime_t getNextTransmissionTime();
};

};

#endif
