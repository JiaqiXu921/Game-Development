//
//  LaserComponent.cpp
//  Lab10
//
//  Created by Jiaqi Xu on 4/5/21.
//

#include "LaserComponent.hpp"
#include "Actor.h"
#include "Shader.h"
#include "Renderer.h"
#include "Game.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Texture.h"
#include "Player.hpp"
#include "Block.hpp"
#include <iostream>
#include "PlayerMove.hpp"
#include "CheckPoint.hpp"
LaserComponent::LaserComponent(Actor* owner, bool usesAlpha)
    :MeshComponent(owner,true)
{
    SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Draw(class Shader* shader){
    if (mMesh)
    {
        // Set the world transform
        for(unsigned int i=0;i<lineSegments.size();i++){
            shader->SetMatrixUniform("uWorldTransform",
                helper(lineSegments[i]));
            // Set the active texture
            Texture* t = mMesh->GetTexture(mTextureIndex);
            if (t)
            {
                t->SetActive();
            }
            // Set the mesh's vertex array as active
            VertexArray* va = mMesh->GetVertexArray();
            va->SetActive();
            // Draw
            glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
        }
    }
}

void LaserComponent::Update(float deltaTime){
    lineSegments.clear();
    LineSegment ls;
    ls.mStart = mOwner->GetWorldPosition();
    Vector3 fwd = mOwner->GetWorldForward();
    fwd.Normalize();
    ls.mEnd = mOwner->GetWorldPosition()+500*fwd;
    
    CastInfo castInfo;
    bool intersect = SegmentCast(mOwner->GetGame()->getPlayer(), ls, castInfo);
    if(intersect){
        Mix_Chunk* laser=mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav");
        Mix_PlayChannel(-1, laser, 0);
        ls.mEnd = castInfo.mPoint;
        lineSegments.push_back(ls);
        mOwner->GetGame()->getPlayer()->SetPosition(mOwner->GetGame()->getPlayer()->GetRespawnPos());
    }
    else{
        bool blockIntersect = SegmentCast(mOwner->GetGame()->getBlocks(), ls, castInfo,mOwner->GetMParent());
        if(blockIntersect){
            Block* b = static_cast<Block*>(castInfo.mActor);
            ls.mEnd = castInfo.mPoint;
            lineSegments.push_back(ls);
            while(b->GetIsMirror()==true){
                LineSegment newLS;
                newLS.mStart = castInfo.mPoint;
                Vector3 oldFwd = ls.mEnd-ls.mStart;
                Vector3 newFwd=Vector3::Reflect(oldFwd, castInfo.mNormal);
                newLS.mEnd = newLS.mStart+500*newFwd;
                bool intersectAgain =SegmentCast(mOwner->GetGame()->getBlocks(), newLS, castInfo,b);
                if(intersectAgain){
                    b = static_cast<Block*>(castInfo.mActor);
                    newLS.mEnd = castInfo.mPoint;
                    ls=newLS;
                    lineSegments.push_back(ls);
                }
            }
        }
        else{
            lineSegments.push_back(ls);
        }
    }
    
}

Matrix4 LaserComponent::helper(LineSegment lineSeg){
    Quaternion q;
    Vector3 oldFacing = Vector3::UnitX;
    Vector3 newFacing = lineSeg.mEnd-lineSeg.mStart;
    newFacing.Normalize();
    float dot = Vector3::Dot(oldFacing, newFacing);
    if(dot==1){
        q=Quaternion::Identity;
    }
    else if(dot==-1){
        q=Quaternion(Vector3::UnitZ, Math::Pi);
    }
    else{
        float theta = Math::Acos(dot);
        Vector3 axis = Vector3::Cross(oldFacing, newFacing);
        axis.Normalize();
        q=Quaternion(axis,theta);
        
    }
    Matrix4 scale = Matrix4::CreateScale(lineSeg.Length(),1,1);
    Matrix4 rotation = Matrix4::CreateRotationZ(mOwner->GetRotation());
    Matrix4 rotationMatrix = Matrix4::CreateFromQuaternion(q);
    Matrix4 translation = Matrix4::CreateTranslation(lineSeg.PointOnSegment(MIDPOINT));
    Matrix4 worldTransform = scale*rotation*rotationMatrix*translation;
    return worldTransform;
}
