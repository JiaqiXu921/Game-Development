//
//  HeightMap.cpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/23/21.
//

#include "HeightMap.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "CSVHelper.h"
HeightMap::HeightMap()
{
    std:: ifstream fileInput("Assets/HeightMap/HeightMap.csv");
    if(fileInput.is_open()){
        while(!fileInput.eof()){
            std::string line;
            std::getline(fileInput, line);
            std::vector<int> oneIntLine;
            std::vector<std::string> oneStringLine;
            if(line.size()!=0){
                oneStringLine=CSVHelper::Split(line);
                for(unsigned int i=0;i<oneStringLine.size();i++){
                    oneIntLine.push_back(std::atoi(oneStringLine[i].c_str()));
                }
                integerStorage.push_back(oneIntLine);
            }
        }
    }
}

bool HeightMap::IsCellOnTrack(int row, int col){
    bool result = false;
    if(integerStorage[row][col] == -1 || row > 63 || col > 63){
        result = false;
    }
    else{
        result = true;
    }
    return result;
}

float HeightMap::GetHeightFromCell(int row, int col){
    int cellValue = integerStorage[row][col];
    return -40.0f +cellValue * 5.0f;
}

Vector3 HeightMap::CellToWorld(int row, int col){
    Vector3 result = Vector3::Zero;
    if(IsCellOnTrack(row, col)==false){
        result = Vector3::Zero;

    }
    else{
        result = Vector3(GRID_TOP-CELL_SIZE*row, GRID_LEFT+CELL_SIZE*col,GetHeightFromCell(row, col));
    }
    return result;
}

Vector2 HeightMap::WorldToCell(float x,float y){
    Vector2 result = Vector2::Zero;
    result.x = (abs(x-GRID_TOP)+CELL_SIZE/2.0f) / CELL_SIZE;
    result.y = (abs(y-GRID_LEFT)+CELL_SIZE/2.0f) / CELL_SIZE;
    return result;
}

bool HeightMap::IsOnTrack(float x, float y){
    Vector2 cell = WorldToCell(x, y);
    bool result = IsCellOnTrack((int)cell.x, (int)cell.y);
    return result;
}

float HeightMap::GetHeight(float x, float y){
    float result = -1000.0f;
    if(IsOnTrack(x, y)){
        Vector2 pos = WorldToCell(x, y);
        result = GetHeightFromCell((int)pos.x, (int)pos.y);
    }
    return  result;
}

