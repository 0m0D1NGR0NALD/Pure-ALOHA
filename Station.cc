#include <algorithm>
#include "Station.h"

namespace aloha {

Define_Module(Station);

Station::~Station()
{
    cancelAndDelete(endTxEvent);
    cancelAndDelete(ackEvent);
}

void Station::initialize()
{
    stateSignal = registerSignal("state");
    delaySignal = registerSignal("messageDelay");

    hub = getModuleByPath("hub");

    C = par("C").doubleValue(); // channel capacity in bytes/s
    S = par("S").doubleValue(); // message size in bytes
    T = &par("T");              // exponential distribution for inter-arrival time
    W = par("W").doubleValue(); // waiting time for ACK

    D = S / C;  // transmission duration in seconds

    slotTime = par("slotTime");
    isSlotted = slotTime > 0;

    pkCounter = 0;
    k = 0; // initialize collision count

    endTxEvent = new cMessage("send/endTx");
    ackEvent = new cMessage("ackEvent");

    state = IDLE;
    emit(stateSignal, state);

    x = par("x").doubleValue();
    y = par("y").doubleValue();
    double hubX = hub->par("x").doubleValue();
    double hubY = hub->par("y").doubleValue();
    double dist = std::sqrt((x - hubX)*(x - hubX) + (y - hubY)*(y - hubY));
    radioDelay = dist / propagationSpeed;

    getDisplayString().setTagArg("p", 0, x);
    getDisplayString().setTagArg("p", 1, y);

    scheduleAt(getNextTransmissionTime(), endTxEvent);
}

void Station::handleMessage(cMessage *msg)
{
    if (msg == endTxEvent && state == IDLE) {
        // send a new packet
        char pkname[40];
        snprintf(pkname, sizeof(pkname), "pk-%d-#%d", getId(), pkCounter++);
        EV << "generating packet " << pkname << endl;

        cPacket *pk = new cPacket(pkname);
        pk->setBitLength((long)(S * 8)); // convert bytes to bits for setBitLength
        pk->addPar("creationTime") = SIMTIME_DBL(simTime()); // store message creation time as double
        simtime_t pktDuration = pk->getBitLength() / (C * 8.0);
        // NOTE: If C is in bytes/s, then C*8 is bits/s. pktDuration = packet_size_in_bits / capacity_in_bits_per_s

        // begin transmission
        state = TRANSMIT;
        emit(stateSignal, state);
        sendDirect(pk, radioDelay, pktDuration, hub->gate("in"));
        // scheduling ackEvent as a timeout for ACK reception
        // waiting time (D + W), using "pktDuration" for D to be consistent with actual packet time
        scheduleAt(simTime() + pktDuration + W, ackEvent);
    }
    else if (msg == ackEvent) {
        // if ackEvent fires, it means no ACK received in time
        EV << "no ACK received by time W, applying back-off\n";
        k++;

        const int k_max = N; // max limit for the collision attempts (equivalent to number of stations)
        if (k >= k_max) {
            EV << "maximum collision attempts reached. dropping packet.\n";
            k = 0;
            state = IDLE;
            // schedule next packet generation
            scheduleAt(getNextTransmissionTime(), endTxEvent);
            return;
        }

        // apply back-off: B ~ U[0, 2^(k+1)*D]
        simtime_t backoff = uniform(0, D * pow(2, k + 1));
        EV << "back-off time: " << backoff << " seconds" << endl;
        state = IDLE;
        scheduleAt(simTime() + backoff, endTxEvent);
    }
    else if (cPacket *pkt = dynamic_cast<cPacket*>(msg)) {
        // Received a packet from the hub (rebroadcast)
        EV << "station " << getId() << " received packet: " << pkt->getName() << endl;

        // check if this is our packet (simple name check)
        char expectedName[40];
        snprintf(expectedName, sizeof(expectedName), "pk-%d-#%d", getId(), pkCounter-1);

        if (strcmp(pkt->getName(), expectedName) == 0) {
            // ACK received
            if (ackEvent->isScheduled())
                cancelEvent(ackEvent);

            EV << "transmission acknowledged\n";
            k = 0;
            state = IDLE;
            emit(stateSignal, state);

            // compute delay
            simtime_t creationTime = pkt->par("creationTime").doubleValue();
            simtime_t delay = simTime() - creationTime; // current simulation time - message creation time

            // emit the delay signal
            emit(delaySignal, delay);

            // schedule next packet generation
            scheduleAt(getNextTransmissionTime(), endTxEvent);
        }
        delete pkt;
    }
    else {
        throw cRuntimeError("unknown message received by station");
    }
}

simtime_t Station::getNextTransmissionTime()
{
    simtime_t t = simTime() + T->doubleValue();

    if (!isSlotted)
        return t;
    else
        // align time of next transmission to a slot boundary
        return slotTime * ceil(t/slotTime);
}

};
