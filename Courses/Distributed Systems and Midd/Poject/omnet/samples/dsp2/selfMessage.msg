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

message SelfMessage {
    int type @enum(DSP2_SELF_MESSAGE_TYPES);
}

enum DSP2_SELF_MESSAGE_TYPES{
	DSP2_INTERNET_MESSAGE_FORWARD_TIMER = 0;
	DSP2_BROKER_POWER_ON_TIMER = 1;
	DSP2_USER_START_TIMER = 2;
	DSP2_USER_NEXT_ACTION_TIMER = 3;

};