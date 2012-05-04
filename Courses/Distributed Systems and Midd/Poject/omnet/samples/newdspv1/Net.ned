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

package newdspv1;



//
// Generated network with random topology (3 nodes, 3 edges, seed=100).
//
network Net {
    parameters:
        int nBrokers = default(4);
        int nClients = default(8);
    submodules:
        brokers[nBrokers]: Broker;
        clients[nClients]: Client;
        internet: Internet;
    connections:
        for i=0..nBrokers-1{
       	brokers[i].toInternet --> internet.in++;
       	brokers[i].fromInternet <-- internet.out++;
       }
       for j=0..nClients-1{
           clients[j].toInternet --> internet.in++;
           clients[j].fromInternet <-- internet.out++;
        }
}
