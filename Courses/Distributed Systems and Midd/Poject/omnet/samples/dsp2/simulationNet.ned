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

network SimulationNet{
    parameters:
        int nBrokers = default(1); //default(4);
        int nClients = default(3); //default(8);
    submodules:
        brokers[nBrokers]: Broker;
        clients[nClients]: Client;
        internet: Internet;
    connections:
        for i=0..nBrokers-1{
        	brokers[i].toNet <--> internet.toNode++;
        }
        for j=0..nClients-1{
            clients[j].toNet <--> internet.toNode++;
        }
}