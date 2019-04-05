/*
* Liam Ashdown
* Copyright (C) 2019
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Habbo.h"
#include "PathFinder.h"

namespace SteerStone
{
    /// Constructor
    TileInstance::TileInstance(int16 const p_X, int16 const p_Y) : m_TileX(p_X), m_TileY(p_Y), m_TileHeight(0), m_TileState(TileState::TILE_STATE_CLOSED)
    {
        m_Habbo.reset();
    }

    /// Deconstructor
    TileInstance::~TileInstance()
    {
    }

    /// AddItem
    /// @p_Item : Item being added to tile
    void TileInstance::AddItem(Item* p_Item)
    {
        m_Item = p_Item;
    }

    /// GetItem
    Item* TileInstance::GetItem()
    {
        if (m_Item)
            return m_Item;
        else
            return nullptr;
    }

    /// GetTileState
    /// Returns current tile state
    int16 TileInstance::GetTileState() const
    {
        return m_TileState;
    }

    /// GetTileHeight
    /// Returns max tile height
    int16 TileInstance::GetTileHeight() const
    {
        return m_TileHeight;
    }

    /// SetTileOccupied
    /// Set the tile is being used by another habbo or object
    /// @p_Occupied : Set tile is being occupied or not
    /// @p_Habbo : Habbo user being added to the tile
    void TileInstance::SetTileOccupied(bool p_Occupied, Habbo* p_Habbo)
    {
        /// Lock guard this function because this can be accessed from PathFinder
        /// and from Room Update
        std::lock_guard<std::mutex> l_Guard(m_Mutex);

        if (p_Occupied)
            m_Habbo = p_Habbo;
        else
            m_Habbo.reset();
    }

    /// CanWalkOnTile
    /// Can Habbo walk on tile
    bool TileInstance::CanWalkOnTile()
    {
        if (m_Habbo.is_initialized())
        {
            if (Habbo* l_Habbo = m_Habbo.get())
            {
                if (GetTilePositionX() != l_Habbo->GetPositionX() || GetTilePositionY() != l_Habbo->GetPositionY()) ///< User is no longer on the tile
                    SetTileOccupied(false);
                else
                    return false;
            }
        }
        else
        {
            if (Item* l_Item = GetItem())
                if (!l_Item->CanBeWalkedOn())
                    return false;
        }

        return true;
    }

} ///< NAMESPACE STEERSTONE
