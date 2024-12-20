simple Hub
{
    parameters:
        @display("i=device/antennatower_l");

        int N;                    // number of stations
        double x @unit(m);        // x coordinate
        double y @unit(m);        // y coordinate

		@signal[throughput](type="double");
        @statistic[throughputStats](source=throughput; record=sum,count,mean,vector);

        @signal[receiveBegin](type="long");
        @signal[receive](type="long");
        @signal[collision](type="long");
        @signal[collisionLength](type="simtime_t");
        @signal[channelState](type="long");

        @statistic[serverChannelState](source="channelState"; title="Channel state"; enum="IDLE=0,TRANSMISSION=1,COLLISION=2"; record=vector);
        @statistic[receiveBegin](source="receiveBegin"; record=vector?; interpolationmode=sample-hold; title="receive begin");
        @statistic[channelUtilization](source="timeavg(receive)"; record=last; interpolationmode=linear; title="channel utilization");
        @statistic[collisionMultiplicity](source=collision; record=vector?,histogram; title="collision multiplicity");
        @statistic[collisionLength](record=vector?,histogram,mean,sum,max; title="collision length");
        @statistic[receivedFrames](source="sum(receive)"; record=last; title="received frames");
        @statistic[collidedFrames](source="sum(collision)"; record=last; title="collided frames");

    gates:
        input in @directIn;
        output out[N] @directOut;
}
