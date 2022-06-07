#include "LevelLoader.h"
#include <rapidjson/document.h>
#include "Math.h"
#include <SDL2/SDL.h>
#include <fstream>
#include <sstream>
#include "Actor.h"
#include "MeshComponent.h"
#include "Block.hpp"
#include "Player.hpp"
#include "Game.h"
#include "LaserMine.hpp"
#include "CheckPoint.hpp"
#include "Coin.hpp"
#include "SecurityCamera.hpp"
namespace
{
	// Helper functions to get other types
	bool GetFloatFromJSON(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	bool GetIntFromJSON(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
	bool GetStringFromJSON(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr);
	bool GetBoolFromJSON(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
	bool GetVectorFromJSON(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector);
	bool GetQuaternionFromJSON(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat);
}

void LoadActor(const rapidjson::Value& actorValue, Game* game, Actor* parent)
{
	if (actorValue.IsObject())
	{
		// Lookup actor type
		std::string type = actorValue["type"].GetString();
		Actor* actor = nullptr;

		if (type == "Block")
		{
			Block* block = new Block(game,parent);
			actor = block;
            bool isMirror;
            bool isRotate;
            if(GetBoolFromJSON(actorValue, "mirror", isMirror)){
                block->SetIsMirror(isMirror);
            }
            if(GetBoolFromJSON(actorValue, "rotates", isRotate)){
                block->SetIsRotate(isRotate);
            }
		}
		else if (type == "Player")
		{
			// TODO: Handle construction of a player!
            Player* ac = new Player(game,parent);
            actor = ac;
            game->setPlayer(ac);
            
            Vector3 pos;
            if (GetVectorFromJSON(actorValue, "pos", pos))
            {
             ac->SetRespawnPos(pos);
            }
		}
        else if(type == "Checkpoint"){
            CheckPoint* cp = new CheckPoint(game,parent);
            actor = cp;
            game->checkPoints.push(cp);
            std::string string;
            if(GetStringFromJSON(actorValue, "level", string)){
                cp->SetLevelString(string);
            }
            std::string checkPointText;
            if(GetStringFromJSON(actorValue, "text", checkPointText)){
                cp->SetCheckPointText(checkPointText);
            }
        }
        else if(type == "SecurityCamera"){
            SecurityCamera* sc = new SecurityCamera(game,parent);
            actor = sc;
            Quaternion startQ;
            Quaternion endQ;
            float interpTime;
            float pauseTime;
            if(GetQuaternionFromJSON(actorValue, "startQ", startQ)){
                sc->SetStartQ(startQ);
            }
            if(GetQuaternionFromJSON(actorValue, "endQ", endQ)){
                sc->SetEndQ(endQ);
            }
            if(GetFloatFromJSON(actorValue, "interpTime", interpTime)){
                sc->SetInterpTime(interpTime);
            }
            if(GetFloatFromJSON(actorValue, "pauseTime", pauseTime)){
                sc->SetPauseTime(pauseTime);
            }
        }
		// TODO: Add else ifs for other actor types
        else if(type == "LaserMine"){
            LaserMine* lm = new LaserMine(game,parent);
            actor = lm;
        }
        else if(type == "Coin"){
            Coin* coin = new Coin(game,parent);
            actor = coin;
        }
		// Set properties of actor
		if (actor)
		{
			Vector3 pos;
			if (GetVectorFromJSON(actorValue, "pos", pos))
			{
				actor->SetPosition(pos);
			}

			float scale = 1.0f;
			if (GetFloatFromJSON(actorValue, "scale", scale))
			{
				actor->SetScale(scale);
			}

			float rot = 0.0f;
			if (GetFloatFromJSON(actorValue, "rot", rot))
			{
				actor->SetRotation(rot);
			}

			Quaternion q;
			if (GetQuaternionFromJSON(actorValue, "quat", q))
			{
				// TODO: Set actor's quaternion member to q
                actor->SetQuaternion(q);
			}

			int textureIdx = 0;
			if (GetIntFromJSON(actorValue, "texture", textureIdx))
			{
				MeshComponent* mesh = actor->GetComponent<MeshComponent>();
				if (mesh)
				{
					mesh->SetTextureIndex(static_cast<int>(textureIdx));
				}
			}

			// See if we have any children
			auto childIter = actorValue.FindMember("children");
			if (childIter != actorValue.MemberEnd())
			{
				auto& childArr = childIter->value;
				if (childArr.IsArray())
				{
					for (rapidjson::SizeType i = 0; i < childArr.Size(); i++)
					{
						if (childArr[i].IsObject())
						{
							LoadActor(childArr[i], game, actor);
						}
					}
				}
			}
		}
	}
}

bool LevelLoader::Load(class Game* game, const std::string & fileName)
{
	std::ifstream file(fileName);

	if (!file.is_open())
	{
		SDL_Log("Level file %s not found", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Level file %s is not valid JSON", fileName.c_str());
		return false;
	}

	// Loop through "actors" array
	const rapidjson::Value& actors = doc["actors"];
	if (actors.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < actors.Size(); i++)
		{
			// This index should be an object
			LoadActor(actors[i], game, nullptr);
		}
	}

	return true;
}

namespace
{

	bool GetFloatFromJSON(const rapidjson::Value& inObject, const char* inProperty, float& outFloat)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			return false;
		}

		auto& property = itr->value;
		if (!property.IsDouble())
		{
			return false;
		}

		outFloat = static_cast<float>(property.GetDouble());
		return true;
	}

	bool GetIntFromJSON(const rapidjson::Value& inObject, const char* inProperty, int& outInt)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			return false;
		}

		auto& property = itr->value;
		if (!property.IsInt())
		{
			return false;
		}

		outInt = property.GetInt();
		return true;
	}

	bool GetStringFromJSON(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			return false;
		}

		auto& property = itr->value;
		if (!property.IsString())
		{
			return false;
		}

		outStr = property.GetString();
		return true;
	}

	bool GetBoolFromJSON(const rapidjson::Value& inObject, const char* inProperty, bool& outBool)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			return false;
		}

		auto& property = itr->value;
		if (!property.IsBool())
		{
			return false;
		}

		outBool = property.GetBool();
		return true;
	}

	bool GetVectorFromJSON(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			return false;
		}

		auto& property = itr->value;
		if (!property.IsArray() || property.Size() != 3)
		{
			return false;
		}

		for (rapidjson::SizeType i = 0; i < 3; i++)
		{
			if (!property[i].IsDouble())
			{
				return false;
			}
		}

		outVector.x = static_cast<float>(property[0].GetDouble());
		outVector.y = static_cast<float>(property[1].GetDouble());
		outVector.z = static_cast<float>(property[2].GetDouble());

		return true;
	}

	bool GetQuaternionFromJSON(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			return false;
		}

		auto& property = itr->value;
		if (!property.IsArray() || property.Size() != 4)
		{
			return false;
		}

		for (rapidjson::SizeType i = 0; i < 4; i++)
		{
			if (!property[i].IsDouble())
			{
				return false;
			}
		}

		outQuat.x = static_cast<float>(property[0].GetDouble());
		outQuat.y = static_cast<float>(property[1].GetDouble());
		outQuat.z = static_cast<float>(property[2].GetDouble());
		outQuat.w = static_cast<float>(property[3].GetDouble());

		return true;
	}
}