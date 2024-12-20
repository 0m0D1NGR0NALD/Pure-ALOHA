#include "Hub.h"

namespace aloha {

Define_Module(Hub);

Hub::~Hub()
{
    cancelAndDelete(endRxEvent);
}

void Hub::initialize()
{
    channelStateSignal = registerSignal("channelState");
    throughputSignal = registerSignal("throughput");
    endRxEvent = new cMessage("end-reception");
    channelBusy = false;
    emit(channelStateSignal, IDLE);

    gate("in")->setDeliverImmediately(true);

    currentlyReceivedPacket = nullptr;
    currentCollisionNumFrames = 0;
    receiveCounter = 0;
    WATCH(currentCollisionNumFrames);

    receiveBeginSignal = registerSignal("receiveBegin");
    receiveSignal = registerSignal("receive");
    collisionSignal = registerSignal("collision");
    collisionLengthSignal = registerSignal("collisionLength");

    emit(receiveSignal, 0L);
    emit(receiveBeginSignal, 0L);

    getDisplayString().setTagArg("p", 0, par("x").doubleValue());
    getDisplayString().setTagArg("p", 1, par("y").doubleValue());
}

void Hub::handleMessage(cMessage *msg)
{
    if (msg == endRxEvent) {
        // reception just ended
        EV << "reception finished\n";
        channelBusy = false;
        emit(channelStateSignal, IDLE);

        simtime_t dt = simTime() - recvStartTime;
        if (currentCollisionNumFrames == 0 && currentlyReceivedPacket != nullptr) {
            // successful reception of a single frame

            numReceived++;
            emit(throughputSignal, (double)numReceived);

            EV << "re-broadcasting packet: " << currentlyReceivedPacket->getName() << endl;
            for (int i = 0; i < gateSize("out"); i++) {
                send(currentlyReceivedPacket->dup(), "out", i);
            }
            delete currentlyReceivedPacket;
            currentlyReceivedPacket = nullptr;
        } else {
            // collision occurred
            EV << "collision detected\n";
            emit(collisionSignal, currentCollisionNumFrames);
            emit(collisionLengthSignal, dt);

            // discard any partially stored packet
            if (currentlyReceivedPacket) {
                delete currentlyReceivedPacket;
                currentlyReceivedPacket = nullptr;
            }
        }

        currentCollisionNumFrames = 0;
        receiveCounter = 0;
        emit(receiveBeginSignal, receiveCounter);
    }
    else {
        // new packet arrival
        cPacket *pkt = check_and_cast<cPacket *>(msg);

        ASSERT(pkt->isReceptionStart());
        simtime_t endReceptionTime = simTime() + pkt->getDuration();
        emit(receiveBeginSignal, ++receiveCounter);

        if (!channelBusy) {
            // start receiving this packet
            EV << "started receiving\n";
            recvStartTime = simTime();
            channelBusy = true;
            emit(channelStateSignal, TRANSMISSION);

            currentlyReceivedPacket = pkt; // store the currently received packet
            scheduleAt(endReceptionTime, endRxEvent);
        }
        else {
            // collision: another packet arrived while channel busy
            EV << "another frame arrived while receiving -- collision!\n";
            emit(channelStateSignal, COLLISION);

            if (currentCollisionNumFrames == 0)
                currentCollisionNumFrames = 2; // at least two packets collided
            else
                currentCollisionNumFrames++;

            // extend reception time if this packet ends later
            if (endReceptionTime > endRxEvent->getArrivalTime()) {
                cancelEvent(endRxEvent);
                scheduleAt(endReceptionTime, endRxEvent);
            }

            // this packet is lost due to collision
            delete pkt;
        }
        // channelBusy remains true until endRxEvent
    }
}

void Hub::finish()
{
    EV << "Duration: " << simTime() << endl;
    recordScalar("Duration", simTime());
}

};
