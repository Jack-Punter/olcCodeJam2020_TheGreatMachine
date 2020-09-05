#ifndef IO_CIRCUIT_NODE_H
#define IO_CIRCUIT_NODE_H

struct IoCircuitNode : public CircuitNode {
    IoComponent type;
    CircuitNode* Input = {};
    ~IoCircuitNode() override
    {
        printf("Deleting %s Node\n", IoComponentStrings[(int)type]);
        RemoveFromParents();
        if (Input) {
            Input->RemoveParent(this);
            if(Input->parents.size() == 0) {
                delete Input;
            }
        }
    }
    
    void Draw(olc::Pixel TintCol = olc::WHITE) override
    {
        if(Input) {
            olc::vi2d leftCentre = pos + olc::vi2d{0, (int)SpriteSize.y};
            olc::vi2d inputConnectPos = Input->GetConnectionPoint(this);
            
            olc::Pixel lineColor = Input->Evaluate(this) ? olc::YELLOW : olc::BLUE;
            pge->DrawLine(leftCentre, inputConnectPos, lineColor);
            Input->Draw(TintCol);
        }
        
        SpriteIndex = (int)type + (Evaluate(0) ? SpriteSheet->Sprite()->width / (int)SpriteSize.x : 0);
        CircuitNode::Draw(TintCol);
    }
    
    void RemoveInput(CircuitNode *node) override {
        if (Input == node) {
            node->RemoveParent(this);
            Input = 0;
        }
    }
    
    
    std::vector<CircuitNode *> IntersectionsWithRect(olc::vf2d tLeft, olc::vf2d bRight) override
    {
        std::vector<CircuitNode *> Collisions;
        
        std::vector<CircuitNode *> ThisCollision = CircuitNode::IntersectionsWithRect(tLeft, bRight);
        std::vector<CircuitNode *> ChildCollision = SafeIntersectionsWithRect(Input, tLeft, bRight);
        
        Collisions.reserve(ThisCollision.size() + ChildCollision.size());
        Collisions.insert(std::end(Collisions), std::begin(ThisCollision), std::end(ThisCollision));
        Collisions.insert(std::end(Collisions), std::begin(ChildCollision), std::end(ChildCollision));
        
        return Collisions;
    }
    
    CircuitNode *GetNodeAt(olc::vi2d p) override
    {
        CircuitNode *Result;
        Result = CircuitNode::GetNodeAt(p);
        if (Result) return Result;
        
        Result = SafeGetNodeAt(Input, p);
        if (Result) return Result;
        
        return 0;
    }
    
    bool ConnectChild(CircuitNode *node) override
    {
        bool result = false;
        if (node == this || node->HasChildRecursive(this)) {
            return false;
        }
        
        if (type == IO_LED && !Input) {
            Input = node;
            result = true;
        }
        if (result) {
            node->parents.push_back(this);
        }
        return result;
    }
    
    bool HasChildRecursive(CircuitNode *node) override
    {
        bool Result = false;
        if(Input) {
            Result = Input == node || Input->HasChildRecursive(node);
        }
        
        return Result;
    }
    
    std::vector<CircuitNode *> GetChildren() override {
        std::vector<CircuitNode *> result;
        if(Input) {
            result.push_back(Input);
        }
        return result;
    }
    
    std::vector<CircuitNode *> RemoveStaticChildrenRecursive() override
    {
        std::vector<CircuitNode *> Result;
        if (Input) {
            if (Input->IsStatic) {
                RemoveErase(&Input->parents, this);
                Result.push_back(Input);
                Input = 0;
            } else {
                std::vector<CircuitNode *> tmp = Input->RemoveStaticChildrenRecursive();
                Result.reserve(Result.size() + tmp.size());
                Result.insert(std::end(Result), std::begin(tmp), std::end(tmp));
            }
        }
        return Result;
    }
    
};

#endif //_IO_CIRCUIT_NODE_H
