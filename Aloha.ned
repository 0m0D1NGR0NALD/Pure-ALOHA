network Aloha
{
    parameters:
        int N;                // number of stations
        double C @unit(Bps);  // channel capacity
        double slotTime @unit(s); // slot time
        @display("bgi=background/terrain,s;bgb=1116,1155");

    submodules:
        hub: Hub {
            N = parent.N;
            @display("p=512,645");
        }

        station[N]: Station {
            C = parent.C;
            N = parent.N;
            slotTime = parent.slotTime;
            @display("p=247,175");
        }

    connections:
        for i=0..N-1 {
            hub.out[i] --> station[i].in;
        }
}