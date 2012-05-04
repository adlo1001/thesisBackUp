package token_ring;

simple TRNode {
    parameters:
        volatile double requestPeriod @unit(s) = default(uniform(0s,60s));
        volatile double usePeriod @unit(s) = default(uniform(0s,5s));
        @display("i=misc/node_vs");
    gates:
        input prev;
        output next;
}

channel MyChannel extends ned.DatarateChannel {
    delay = uniform(90ms,110ms);
    datarate = normal(1Mbps,1Kbps);
}

network TokenRing
{
    parameters:
        int numNodes = default(12);
    submodules:
        node[numNodes]: TRNode ;

    connections:
        for i=0..numNodes-1 {
            node[i].next --> MyChannel --> node[(i+1)%numNodes].prev;
        }
}
