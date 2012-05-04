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

module Broker
{
    gates:
        input fromInternet;
        output toInternet;
    submodules:
        app: AppBroker {
        }
        consistency: Consistency {
        }
    connections:
        app.fromconsis <-- consistency.toapp;
        app.toconsis --> consistency.fromapp;
        consistency.frominternet<-- fromInternet;
        consistency.tointernet --> toInternet;
}
