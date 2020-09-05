#ifndef LOGIC_CIRCUIT_NODE_H
#define LOGIC_CIRCUIT_NODE_H

struct LogicCircuitNode : public CircuitNode {
    LogicComponent type;
    CircuitNode *Input[2] = { 0 };
    
    ~LogicCircuitNode() override
    {
        printf("Deleting %s Node\n", LogicComponentStrings[(int)type]);
        RemoveFromParents();
        if (Input[0]) {
            Input[0]->RemoveParent(this);
            if(Input[0]->parents.size() == 0) {
                delete Input[0];
            }
        }
        if (Input[1] && Input[1] != Input[0]) {
            Input[1]->RemoveParent(this);
            if(Input[1]->parents.size() == 0) {
                delete Input[1];
            }
        }
    }
    
    void RemoveInput(CircuitNode *node) override {
        if (Input[0] == node) {
            node->RemoveParent(this);
            Input[0] = 0;
        }
        if (Input[1] == node) {
            node->RemoveParent(this);
            Input[1] = 0;
        }
    }
    
    void Draw(olc::Pixel TintCol = olc::WHITE) override
    {
        if(Input[0]) {
            olc::vi2d leftCentre = pos + olc::vi2d{0, (int)SpriteSize.y};
            olc::vi2d inputConnectionPos = Input[0]->GetConnectionPoint(this);
            
            olc::vi2d leftUpperInput = leftCentre - olc::vi2d{0, (int)SpriteSize.y / 2};
            olc::vi2d leftLowerInput = leftCentre + olc::vi2d{0, (int)SpriteSize.y / 2};
            olc::Pixel lineColor = Input[0]->Evaluate(this) ? olc::YELLOW : olc::BLUE;
            if(type == LOGIC_BUFFER || type == LOGIC_NOT) {
                DrawConnectorLine(leftCentre, inputConnectionPos, lineColor);
            } else {
                DrawConnectorLine(leftUpperInput, inputConnectionPos, lineColor);
            }
            Input[0]->Draw(TintCol);
        }
        if(Input[1]) {
            olc::vi2d leftCentre = pos + olc::vi2d{0, (int)SpriteSize.y};
            olc::vi2d inputConnectionPos = Input[1]->GetConnectionPoint(this);
            
            olc::vi2d leftUpperInput = leftCentre - olc::vi2d{0, (int)SpriteSize.y / 2};
            olc::vi2d leftLowerInput = leftCentre + olc::vi2d{0, (int)SpriteSize.y / 2};
            
            olc::Pixel lineColor = Input[1]->Evaluate(this) ? olc::YELLOW : olc::BLUE;
            
            DrawConnectorLine(leftLowerInput, inputConnectionPos, lineColor);
            
            Input[1]->Draw(TintCol);
        }
        CircuitNode::Draw(TintCol);
    }
    
    std::vector<CircuitNode *> IntersectionsWithRect(olc::vf2d tLeft, olc::vf2d bRight) override
    {
        std::vector<CircuitNode *> connections;
        
        std::vector<CircuitNode *> ThisCollision = CircuitNode::IntersectionsWithRect(tLeft, bRight);
        std::vector<CircuitNode *> Child0Collision = SafeIntersectionsWithRect(Input[0], tLeft, bRight);
        std::vector<CircuitNode *> Child1Collision = SafeIntersectionsWithRect(Input[1], tLeft, bRight);
        
        connections.reserve(ThisCollision.size() + Child0Collision.size() + Child1Collision.size());
        connections.insert(std::end(connections), std::begin(ThisCollision), std::end(ThisCollision));
        connections.insert(std::end(connections), std::begin(Child0Collision), std::end(Child0Collision));
        connections.insert(std::end(connections), std::begin(Child1Collision), std::end(Child1Collision));
        
        return connections;
    }
    
    CircuitNode *GetNodeAt(olc::vi2d p) override {
        CircuitNode *Result;
        Result = CircuitNode::GetNodeAt(p);
        if (Result) return Result;
        
        Result = SafeGetNodeAt(Input[0], p);
        if (Result) return Result;
        
        Result = SafeGetNodeAt(Input[1], p);
        if (Result) return Result;
        return 0;
    }
    
    bool CanConnectChild(CircuitNode *node) override
    {
        bool result = true;
        
        if (!(!Input[0] || (!Input[1] && type != LOGIC_BUFFER && type != LOGIC_NOT))) {
            result = false;
        }
        
        if (node == this || node->HasChildRecursive(this)) {
            result = false;
        }
        
        return result;
    }
    
    bool ConnectChild(CircuitNode *node) override
    {
        bool result = CanConnectChild(node);
        if (result) {
            if (!Input[0]) {
                Input[0] = node;
            } else if (type != LOGIC_BUFFER && type != LOGIC_NOT && !Input[1]) {
                Input[1] = node;
            }
            node->parents.push_back(this);
        }
        return result;
    };
    
    bool HasChildRecursive(CircuitNode *node) override
    {
        bool Result = false;
        if(Input[0]) {
            Result = Input[0] == node || Input[0]->HasChildRecursive(node);
        }
        if(!Result && Input[1]) {
            Result = Input[1] == node || Input[1]->HasChildRecursive(node);
        }
        return Result;
    }
    
    std::vector<CircuitNode *> RemoveStaticChildrenRecursive()
    {
        std::vector<CircuitNode *> Result;
        if (Input[0]) {
            if (Input[0]->IsStatic) {
                RemoveErase(&Input[0]->parents, this);
                Result.push_back(Input[0]);
                Input[0] = 0;
            }
            else {
                std::vector<CircuitNode *> tmp = Input[0]->RemoveStaticChildrenRecursive();
                Result.reserve(Result.size() + tmp.size());
                Result.insert(std::end(Result), std::begin(tmp), std::end(tmp));
            }
        }
        if(Input[1] && type != LOGIC_BUFFER && type != LOGIC_NOT) {
            if (Input[1]->IsStatic) {
                RemoveErase(&Input[1]->parents, this);
                Result.push_back(Input[1]);
                Input[1] = 0;
            }
            else {
                std::vector<CircuitNode *> tmp = Input[1]->RemoveStaticChildrenRecursive();
                Result.reserve(Result.size() + tmp.size());
                Result.insert(std::end(Result), std::begin(tmp), std::end(tmp));
            }
        }
        return Result;
    }
    
    std::vector<CircuitNode *> GetChildren() override {
        std::vector<CircuitNode *> result;
        if(Input[0]) {
            result.push_back(Input[0]);
        }
        if(Input[1] && Input[1] != Input[0] && type != LOGIC_BUFFER && type != LOGIC_NOT) {
            result.push_back(Input[1]);
        }
        return result;
    }
};

#endif //LOGIC_CIRCUIT_NODE_H
