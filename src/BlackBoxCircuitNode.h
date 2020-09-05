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
    
    olc::vi2d GetConnectionPoint(CircuitNode *parent) override {
        int ParentIndex = 0;
        for(; parents[ParentIndex] != parent; ++ParentIndex);
        return pos + olc::vi2d{2 * (int)SpriteSize.x, (int)((1 + ParentIndex) * (2 * SpriteSize.y / (1 + parents.size())))};
    }
    
    void Draw(olc::Pixel TintCol = olc::WHITE) override
    {
        int counter = 1;
        
        for (auto *node : Input) {
            olc::vi2d inputConnectionPos = node->GetConnectionPoint(this);
            
            olc::vi2d currentInput = pos + olc::vi2d{0, (int)(counter * (2 * SpriteSize.y / (1 + Input.size())))};
            
            olc::Pixel lineColor = node->Evaluate(this) ? olc::YELLOW : olc::BLUE;
            
            DrawConnectorLine(currentInput, inputConnectionPos, lineColor);
            
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
    
    CircuitNode *GetNodeAt(olc::vi2d p) override
    {
        CircuitNode *Result;
        Result = CircuitNode::GetNodeAt(p);
        if (Result) return Result;
        
        for (auto *node : Input) {
            Result = SafeGetNodeAt(node, p);
            if (Result) return Result;
        }
        
        return 0;
    }
    
    bool CanConnectChild(CircuitNode * node) override
    {
        bool Result = true;
        
        if (!node) {
            Result = false;
        } else {
            if (node == this || node->HasChildRecursive(this)) {
                Result = false;
            }
        }
        
        return Result;
    }
    
    bool ConnectChild(CircuitNode *node) override
    {
        bool result = CanConnectChild(node);
        if (result) {
            Input.push_back(node);
            node->parents.push_back(this);
        }
        return result;
    };
    
    bool HasChildRecursive(CircuitNode *node) override
    {
        bool Result = false;
        for(auto *i : Input)
        {
            if(!Result && i) {
                Result = i == node || i->HasChildRecursive(node);
            }
        }
        
        return Result;
    }
    
    std::vector<CircuitNode *> GetChildren() override {
        return Input;
    }
    
    std::vector<CircuitNode *> RemoveStaticChildrenRecursive()
    {
        // NOTE(jack): This code doesn't ever get called as Black Boxes are always static
        std::vector<CircuitNode *> Result;
        return Result;
    }
};

#endif //BLACK_BOX_CIRCUIT_NODE_H
