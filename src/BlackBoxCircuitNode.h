#ifndef BLACK_BOX_CIRCUIT_NODE_H
#define BLACK_BOX_CIRCUIT_NODE_H

#include "CircuitNodeAbstract.h"

struct BlackBoxCircuitNode : public CircuitNode {
    std::vector<CircuitNode *> Input;
    ~BlackBoxCircuitNode() override {
        RemoveFromParents();
        for(auto * node : Input) {
            node->RemoveParent(this);
            if (node->parents.size() == 0) {
                delete node;
            }
        }
    }
    
    void RemoveInput(CircuitNode *node) override {
        for (auto *i : Input) {
            if (i == node) {
                node->RemoveParent(this);
            }
        }
        RemoveErase(&Input, node);
    }
    
    void Draw(olc::Pixel TintCol = olc::WHITE) override
    {
        int counter = 1;
        
        for (auto *node : Input) {
            olc::vi2d inputRightCentre = node->pos +
                olc::vi2d{2 * (int)node->SpriteSize.x, (int)node->SpriteSize.y};
            
            olc::vi2d currentInput = pos + olc::vi2d{0, (int)(counter * (2 * SpriteSize.y / (1 + Input.size())))};
            
            olc::Pixel lineColor = node->Evaluate(this) ? olc::YELLOW : olc::BLUE;
            pge->DrawLine(currentInput, inputRightCentre, lineColor);
            
            node->Draw(TintCol);
            ++counter;
        }
        CircuitNode::Draw(TintCol);
    }
    
    std::vector<CircuitNode *> IntersectionsWithRect(olc::vf2d tLeft, olc::vf2d bRight) override
    {
        std::vector<CircuitNode *> Collisions = CircuitNode::IntersectionsWithRect(tLeft, bRight);
        
        for (auto *node : Input) {
            std::vector<CircuitNode *> NodeCollisions = SafeIntersectionsWithRect(node, tLeft, bRight);
            Collisions.reserve(Collisions.size() + NodeCollisions.size());
            Collisions.insert(std::end(Collisions), std::begin(NodeCollisions), std::end(NodeCollisions));
        }
        return Collisions;
    }
    
    CircuitNode *GetNodeAt(olc::vi2d p) override {
        CircuitNode *Result;
        Result = CircuitNode::GetNodeAt(p);
        if (Result) return Result;
        
        for (auto *node : Input) {
            Result = SafeGetNodeAt(node, p);
            if (Result) return Result;
        }
        
        return 0;
    }
    
    bool ConnectChild(CircuitNode *node) override {
        if (node) {
            Input.push_back(node);
        }
        return true;
    };
    
    std::vector<CircuitNode *> GetChildren() override {
        return Input;
    }
    
    std::vector<CircuitNode *> RemoveStaticChildrenRecursive()
    {
        std::vector<CircuitNode *> Result;
        std::vector<CircuitNode *>::iterator oldEnd = std::end(Result);
        std::vector<CircuitNode *>::iterator newEnd = std::end(Result);
        
        for(auto It = std::begin(Input); It !=  newEnd;)
        {
            if ((*It)->IsStatic) {
                Result.push_back(*It);
                newEnd = std::remove(std::begin(Result), oldEnd, *It);
                // If we have not removed an element we dont increment so we dont skip an input
                if (newEnd == oldEnd) {
                    ++It;
                } else {
                    oldEnd = newEnd;
                }
            } else {
                std::vector<CircuitNode *> tmp = (*It)->RemoveStaticChildrenRecursive();
                Result.reserve(Result.size() + tmp.size());
                Result.insert(std::end(Result), std::begin(tmp), std::end(tmp));
                ++It;
            }
        }
        // erase all of the removed Inputs
        Input.erase(newEnd, std::end(Input));
        return Result;
    }
};

#endif //BLACK_BOX_CIRCUIT_NODE_H
