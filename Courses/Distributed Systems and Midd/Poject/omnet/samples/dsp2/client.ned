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

module Client
{
    parameters:
        @display("i=block/users"); // add a default icon
    			//@display("bgb=376,99;bgl=3");    
    gates:
        inout toNet;
    submodules:
        user: User {
            @display("p=41,52");
        }
        app: AppClient {
            @display("p=139,52");
        }
        consistency: CausalConsistency {
            @display("p=237,52");
        }
        fault: NetFault {
            @display("p=332,52");
        }
    connections:
        user.application <--> app.user;
        app.middleware <--> consistency.upLayer;
        consistency.downLayer <--> fault.upLayer;
        fault.downLayer <--> toNet;
}
