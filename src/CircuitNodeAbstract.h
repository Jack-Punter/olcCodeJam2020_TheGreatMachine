#ifndef CICRUIT_NODE_ABSTRACT_H
#define CICRUIT_NODE_ABSTRACT_H

#include "generated\generated.h"
#include "olcPixelGameEngine.h"
#include <vector>
#include <optional>
#include <algorithm>

struct CircuitNode;

inline bool SafeEval(CircuitNode *node, CircuitNode * callingParent);
inline CircuitNode *SafeGetNodeAt(CircuitNode *node, olc::vi2d p);
inline std::vector<CircuitNode *> SafeIntersectionsWithRect(CircuitNode *node, olc::vf2d tLeft, olc::vf2d bRight);

struct CircuitNode {
    int SpriteIndex;
    std::vector<CircuitNode*> parents;
    olc::PixelGameEngine *pge;
    olc::Renderable *SpriteSheet;
    olc::vi2d pos;
    olc::vf2d SpriteSize;
    bool Held;
    bool IsStatic = false;
    
    virtual ~CircuitNode() {
        RemoveFromParents();
    };
    
    void RemoveParent(CircuitNode *node) {
        RemoveErase(&parents, node);
    }
    
    void RemoveFromParents() {
        for (CircuitNode *n : parents) {
            n->RemoveInput(this);
        }
    }
    
    virtual void RemoveInput(CircuitNode *node) = 0;
    virtual olc::vi2d GetConnectionPoint(CircuitNode *parent) {
        return pos + SpriteSize + olc::vi2d{(int)SpriteSize.x, 0};
    }
    
    virtual void Draw(olc::Pixel TintCol = olc::WHITE)
    {
        float TargetPosX = (float)((SpriteIndex * (int)SpriteSize.x) % SpriteSheet->Sprite()->width);
        // Get the Y sprite offset
        float TargetPosY = (float)((SpriteIndex * (int)SpriteSize.x) / SpriteSheet->Sprite()->width);
        TargetPosY *= SpriteSize.y;
        if (Held) {
            if (TintCol != olc::WHITE) {
                pge->DrawPartialDecal(pos, SpriteSheet->Decal(), {TargetPosX, TargetPosY}, SpriteSize, {2.0f, 2.0f}, TintCol);
            }
        } else {
            pge->DrawPartialSprite({(int)pos.x, (int)pos.y}, SpriteSheet->Sprite(), {(int)TargetPosX, (int)TargetPosY}, SpriteSize, 2);
        }
    }
    // This implements a single node intesection,
    // Base classes call this to check thier collision box
    virtual std::vector<CircuitNode *> IntersectionsWithRect(olc::vf2d tLeft, olc::vf2d bRight)
    {
        std::vector<CircuitNode *> connections;
        olc::vf2d MyTLeft = pos;
        olc::vf2d MyBRight = pos + 2 * SpriteSize;
        if (MyBRight.x > tLeft.x && MyTLeft.x < bRight.x && MyBRight.y > tLeft.y && MyTLeft.y < bRight.y) {
            connections.push_back(this);
        }
        return connections;
    }
    
    virtual void OnClickEvent() {};
    
    virtual bool Evaluate(CircuitNode * callingParent) = 0;
    virtual bool ConnectChild(CircuitNode *node) = 0;
    virtual bool HasChildRecursive(CircuitNode *node) = 0;
    
    virtual std::vector<CircuitNode *> GetChildren() = 0;
    virtual std::vector<CircuitNode *> RemoveStaticChildrenRecursive() = 0;
    
    virtual CircuitNode *GetNodeAt(olc::vi2d p) {
        olc::vf2d tLeft = pos;
        olc::vf2d bRight = pos + 2 * SpriteSize;
        if (p.x >= tLeft.x && p.x < bRight.x &&
            p.y >= tLeft.y && p.y < bRight.y)
        {
            return this;
        }
        return 0;
    }
};

inline bool SafeEval(CircuitNode *node, CircuitNode * callingParent) {
    return (node ? node->Evaluate(callingParent) : false);
}

inline CircuitNode *SafeGetNodeAt(CircuitNode *node, olc::vi2d p) {
    return (node ? node->GetNodeAt(p) : 0);
}

inline std::vector<CircuitNode *> SafeIntersectionsWithRect(CircuitNode *node, olc::vf2d tLeft, olc::vf2d bRight) {
    return node ? node->IntersectionsWithRect(tLeft, bRight) : std::vector<CircuitNode *>();
}

#endif //CICRUIT_NODE_ABSTRACT_H
