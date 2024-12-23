/*
 * This file is part of Hexagon
 *
 * Hexagon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hexagon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hexagon. If not, see <https://www.gnu.org/licenses/>.
 */


#include "hexitemsignalhelper.h"
#include "hexitem.h"

HexItemSignalHelper::HexItemSignalHelper(QObject *parent)
    : QObject(parent) {
    // Constructor implementation
}


// The signal itself does not need an implementation in the CPP, as it is handled by the Q_OBJECT macro and the Qt Signal-Slot system.