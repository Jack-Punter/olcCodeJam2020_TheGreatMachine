#ifndef CIRCUIT_NODE_H
#define CIRCUIT_NODE_H

#include "CircuitNodeAbstract.h"
#include "LogicCircuitNode.h"
#include "IoCircuitNode.h"
#include "BlackBoxCircuitNode.h"

struct CircuitNodeXOR : public LogicCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return ((SafeEval(Input[0], this) && !SafeEval(Input[1], this)) ||
                (!SafeEval(Input[0], this) && SafeEval(Input[1], this)));
    }
};

struct CircuitNodeOR : public LogicCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return SafeEval(Input[0], this) || SafeEval(Input[1], this);
    }
};

struct CircuitNodeAND : public LogicCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return SafeEval(Input[0], this) && SafeEval(Input[1], this);
    }
};

struct CircuitNodeBUFFER : public LogicCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return SafeEval(Input[0], this);
    }
};

struct CircuitNodeXNOR : public LogicCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return !((SafeEval(Input[0], this) && !SafeEval(Input[1], this)) ||
                 (!SafeEval(Input[0], this) && SafeEval(Input[1], this)));
    }
};

struct CircuitNodeNOR : public LogicCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return !(SafeEval(Input[0], this) || SafeEval(Input[1], this));
    }
};

struct CircuitNodeNAND : public LogicCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return !(SafeEval(Input[0], this) && SafeEval(Input[1], this));
    }
};

struct CircuitNodeNOT : public LogicCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return !SafeEval(Input[0], this);
    }
};

struct CircuitNodeBIT : public IoCircuitNode {
    bool value = false;
    bool Evaluate(CircuitNode * callingParent) override {
        return value;
    }
    
    void OnClickEvent() override {
        value = !value;
    };
};

struct CircuitNodeLED : public IoCircuitNode {
    bool Evaluate(CircuitNode * callingParent) override {
        return SafeEval(Input, this);
    }
};

template<typename Type>
CircuitNode* CreateHeldNode(olc::PixelGameEngine *pge, LogicComponent type, olc::vf2d componentSize, olc::Renderable* renderable)
{
    Type* NewNode = 0;
    switch(type)
    {
        case LOGIC_XOR:{
            NewNode = new CircuitNodeXOR;
        }break;
        
        case LOGIC_OR:{
            NewNode = new CircuitNodeOR;
        }break;
        
        case LOGIC_AND:{
            NewNode = new CircuitNodeAND;
        }break;
        
        case LOGIC_BUFFER:{
            NewNode = new CircuitNodeBUFFER;
        }break;
        
        case LOGIC_XNOR:{
            NewNode = new CircuitNodeXNOR;
        }break;
        
        case LOGIC_NOR:{
            NewNode = new CircuitNodeNOR;
        }break;
        
        case LOGIC_NAND:{
            NewNode = new CircuitNodeNAND;
        }break;
        
        case LOGIC_NOT:{
            NewNode = new CircuitNodeNOT;
        }break;
        
        case LOGIC_NONE:{
            Assert(true);
        }break;
    }
    
    NewNode->type = type;
    NewNode->SpriteIndex = (int)NewNode->type;
    NewNode->pge = pge;
    NewNode->pos = pge->GetMousePos() - componentSize;
    NewNode->SpriteSheet = renderable;
    NewNode->SpriteSize = componentSize;
    NewNode->Held = true;
    return NewNode;
}

template<typename Type>
CircuitNode* CreateHeldNode(olc::PixelGameEngine *pge, IoComponent type, olc::vf2d componentSize, olc::Renderable* renderable)
{
    Type* NewNode = 0;
    switch(type)
    {
        case IO_BIT:{
            NewNode = new CircuitNodeBIT;
        }break;
        
        case IO_LED: {
            NewNode = new CircuitNodeLED;
        }break;
    }
    
    NewNode->type = type;
    NewNode->SpriteIndex = (int)NewNode->type;
    NewNode->pge = pge;
    NewNode->pos = pge->GetMousePos() - componentSize;
    NewNode->SpriteSheet = renderable;
    NewNode->SpriteSize = componentSize;
    NewNode->Held = true;
    return NewNode;
}
#endif //CIRCUIT_NODE_H
