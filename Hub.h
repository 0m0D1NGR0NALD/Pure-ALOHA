#ifndef __ALOHA_HUB_H_
#define __ALOHA_HUB_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace aloha {

class Hub : public cSimpleModule
{
  private:
    // state variables
    bool channelBusy = false;
    cMessage *endRxEvent = nullptr;
    cPacket *currentlyReceivedPacket = nullptr;
    int currentCollisionNumFrames = 0;
    int receiveCounter = 0;
    simtime_t recvStartTime;

    enum { IDLE = 0, TRANSMISSION = 1, COLLISION = 2 };

    // signals
    simsignal_t channelStateSignal;
    simsignal_t receiveBeginSignal;
    simsignal_t receiveSignal;
    simsignal_t collisionLengthSignal;
    simsignal_t collisionSignal;

    // throughput
    simsignal_t throughputSignal;
    long numReceived = 0; // count of received (successful) packets

  public:
    virtual ~Hub();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

};
#endif
