//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.hpp"
#include "SideBlock.hpp"
#include "MeshComponent.h"
#include "Block.hpp"
Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
    mRenderer = new Renderer(this);
    bool didInit = mRenderer->Initialize(1024.0f, 768.0f);
    if(didInit==false){
        return false;
    }
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();
	
	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	// TODO: tell renderer to draw
    mRenderer->Draw();
}

void Game::LoadData()
{
    Mix_Chunk* music=this->GetSound("Assets/Sounds/ShipLoop.wav");
    backgroundSound = Mix_PlayChannel(-1, music, -1);
    player= new Player(this);
    Matrix4 projectionMatrix = Matrix4::CreatePerspectiveFOV(RAD, VIEW_WIDTH, VIEW_HEIGHT, NEAR_PLANE_DIS, FAR_PLANE_DIS);
    mRenderer->SetProjectionMatrix(projectionMatrix);
    Matrix4 viewMatrix = Matrix4::CreateLookAt(Vector3(-300,0,100), Vector3(20,0,0), Vector3::UnitZ);
    mRenderer->SetViewMatrix(viewMatrix);
    //dynamic block spawning
    for(unsigned int i=0;i<6;i++){
            SideBlock* blockLeft = new SideBlock(this);
            SideBlock* blockRight = new SideBlock(this);
            blockLeft->SetPosition(Vector3(500*i,500,0));
            blockRight->SetPosition(Vector3(500*i,-500,0));
            if(colorIndex>COLOR_TOTAL){
                colorIndex=1;
            }
            blockLeft->getMC()->SetTextureIndex(colorIndex);
            blockRight->getMC()->SetTextureIndex(colorIndex);
            colorIndex++;
    }
    
    LoadBlocks("Assets/Blocks/1.txt");
    xGap+=GAP_SIZE;
    LoadBlocks("Assets/Blocks/2.txt");
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	// TODO: Delete renderer
    mRenderer->Shutdown();
    delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}

void Game::AddBlock(Block* block){
    blocks.push_back(block);
}

void Game::RemoveBlock(Block* block){
    std::vector<Block*>::iterator sctoErase=std::find(blocks.begin(), blocks.end(), block);
    if(sctoErase!=blocks.end()){
        blocks.erase(sctoErase);
    }
}

void Game::LoadBlocks(std::string fileName){
    blockZ = INIT_Z;
    std:: ifstream fileInput(fileName);
    std::string name;
    if(fileInput.is_open()){
        while(!fileInput.eof()){
            std::string line;
            std::getline(fileInput, line);
            if(!line.empty()){
                for (unsigned int i=0;i<line.size();i++){
                    if(line.at(i) == '.'){
                        
                    }
                    else if(line.at(i)=='A'){
                        Block* block = new Block(this);
                        block->getMC()->SetTextureIndex(2);
                        block->SetPosition(Vector3(xGap,blockY,blockZ));
                    }
                    else if(line.at(i)=='B'){
                        Block* block = new Block(this);
                        block->setExplode(true);
                        block->getMC()->SetTextureIndex(4);
                        block->SetPosition(Vector3(xGap,blockY,blockZ));
                    }
                    blockY += BLOCK_SIZE;
                }
                blockZ -= BLOCK_SIZE;
                blockY = INIT_Y;
            }
        }
    }
}

