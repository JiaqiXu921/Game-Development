//
//  HeightMap.hpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/23/21.
//

#ifndef HeightMap_hpp
#define HeightMap_hpp

#include <stdio.h>
#include <vector>
#include "Math.h"
#endif /* HeightMap_hpp */
class HeightMap
{
public:
    HeightMap();
    Vector3 CellToWorld(int row, int col);
    Vector2 WorldToCell(float x,float y);
    bool IsOnTrack(float x, float y);
    float GetHeight(float x, float y);
private:
    std::vector<std::vector<int>> integerStorage;
    bool IsCellOnTrack(int row, int col);
    float GetHeightFromCell(int row, int col);
    float CELL_SIZE = 40.55f;
    float GRID_TOP = 1280.0f;
    float GRID_LEFT = -1641.0f;
};
