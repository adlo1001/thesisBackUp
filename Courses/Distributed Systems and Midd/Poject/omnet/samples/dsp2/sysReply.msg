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

message SysReply {
    int repType @enum(DSP2_SYSTEM_REPLY_TYPES);
    int originator;
    string topic;
    string content;
}

enum DSP2_SYSTEM_REPLY_TYPES{
	DSP2_BROKER_NOT_READY = 0;
	DSP2_PUBLISHED_MESSAGE = 1;
};