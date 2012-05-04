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
message InternetMsg
{
    int source;  //the source ip adress inside the network
    int destination;  //the destination address inside the network
    string content;
    int msgType;  // 
    string topic;
    unsigned long seqNumber;
    double timestamp;   //synchronized global timestamp
    int connectedBrokers[];
    double connectedBrokersTimeStamp[];
    double globaltime;
    double agreedlocaltime;
    double clockdriftrate;
}

