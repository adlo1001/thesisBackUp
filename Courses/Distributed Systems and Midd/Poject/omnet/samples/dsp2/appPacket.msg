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


packet AppPacket {
    int pktType @enum(DSP2_APPLICATION_MESSAGE_TYPES);
    int srcAddress;
    int destAddress;
    
    int originator;
    string topic;
    string content;
    
    int topologyIdx;
    int connectedBrokers[];
}

enum DSP2_APPLICATION_MESSAGE_TYPES{
	DSP2_CLIENT_PUBLISH_MESSAGE = 0;
	DSP2_CLIENT_SUBSCRIBE_MESSAGE = 1;
	DSP2_CLIENT_UNSUBSCRIBE_MESSAGE = 2;
	DSP2_BROKER_CONNECT_MESSAGE = 3;
	DSP2_BROKER_NET_UPDATE_MESSAGE = 4;
	DSP2_BROKER_NOT_READY_MESSAGE = 5;
	DSP2_PUBLISHED_CONTENT_MESSAGE = 6;
};