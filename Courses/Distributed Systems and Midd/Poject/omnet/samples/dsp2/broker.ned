//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package dsp2;

module Broker
{    
    parameters:
                @display("i=block/server"); // add a default icon        
    gates:
        inout toNet;
    submodules:
        app: AppBroker {
            @display("p=29,52");
        }
        consistency: CausalConsistency {
            @display("p=137,52");
        }
        fault: NetFault {
            @display("p=263,52");
        }
    connections:
        app.middleware <--> consistency.upLayer;
        consistency.downLayer <--> fault.upLayer;
        fault.downLayer <--> toNet;
}
