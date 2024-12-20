simple Station
{
    parameters:
        @display("i=device/pc_s");
        
        @signal[messageDelay](type="simtime_t");
        @statistic[messageDelayStats](source=messageDelay; record=mean,min,max,vector);

        @signal[state](type="long");
        @statistic[radioState](source="state";title="Radio state";enum="IDLE=0,TRANSMIT=1";record=vector);

        double C @unit(Bps);  // channel capacity in bytes per second
        int N;                // number of stations
        double W @unit(s);    // acknowledgment wait time
        double S @unit(B);    // message size in bytes
        double T @unit(s);    // message interarrival time (exponential)
        double slotTime @unit(s); // zero means pure ALOHA
        double x @unit(m);    // x coordinate of the station
        double y @unit(m);    // y coordinate of the station

    gates:
        input in @directIn;
}