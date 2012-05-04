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
// TODO auto-generated module
//
simple AppBroker
{
    parameters:
        volatile double actionDelay @unit(s) = exponential(6s);
        double joinTimeout @unit(s) = default(10s);
        double netUpdateTimeout @unit(s) = default(10s);
    gates:
        input fromconsis;
        output toconsis;
}
