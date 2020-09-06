#ifndef LEVEL_Hard4
#define LEVEL_Hard4

struct BlackBoxHard4 : public BlackBoxCircuitNode {
    /* Number of inputs: 3 */
    /* Number of outputs: 6 */
    
    bool Evaluate(CircuitNode * callingParent) override {
        bool a = SafeEval(Input[0], this);
        bool b = SafeEval(Input[1], this);
        bool c = SafeEval(Input[2], this);
        
        bool out1 = a != b;
        bool out2 = !a || (c && b);
        bool out3 = b == c;
        
        if (callingParent == parents[0]) {
            return out1;
        } else if (callingParent == parents[1]) {
            return (out2 != out3) == out1;
        } else if (callingParent == parents[2]) {
            return out2;
        } else if (callingParent == parents[3]) {
            return !(out3 && out1);
        } else if (callingParent == parents[4]) {
            return out3;
        } else if (callingParent == parents[5]) {
            return out2 || out3;
        } else {
            Assert(true);
            return false;
        }
    }
};

struct Hard4Level : public Level {
    void OnUserCreate(olc::PixelGameEngine *_pge) {
        Level::OnUserCreate(_pge);
        editor.OnUserCreate(_pge);
        LevelName = "Hard 4\n(3 - 6)";
        LevelCompleteName = "Hard 4\n(3 - 6)";
        
        int InputCenteringYOffset = 3;
        int OutputCenteringYOffset = 0;
        
        CircuitNodeBIT *Input1 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        Input1->Held = false;
        Input1->IsStatic = true;
        Input1->pos = (2 * editor.IoComponentSize) + olc::vi2d{0, (int)editor.IoComponentSize.y * (0 + InputCenteringYOffset)};
        
        CircuitNodeBIT *Input2 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        Input2->Held = false;
        Input2->IsStatic = true;
        Input2->pos = (2 * editor.IoComponentSize) + olc::vi2d{0, (int)editor.IoComponentSize.y * (2 + InputCenteringYOffset)};
        
        CircuitNodeBIT *Input3 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        Input3->Held = false;
        Input3->IsStatic = true;
        Input3->pos = (2 * editor.IoComponentSize) + olc::vi2d{0, (int)editor.IoComponentSize.y * (4 + InputCenteringYOffset)};
        
        CircuitNode *BlackBox = new BlackBoxHard4();
        BlackBox->SpriteIndex = (int)LOGIC_NONE;
        BlackBox->IsStatic = true;
        BlackBox->pge = pge;
        BlackBox->pos = {((pge->ScreenWidth() - 300) / 2 - (int)editor.LogicComponentSize.x), (Input1->pos.y + (Input3->pos.y + 2 * (int)Input3->SpriteSize.y)) / 2 - (int)editor.LogicComponentSize.y};
        BlackBox->SpriteSheet = editor.LogicComponentRenderable;
        BlackBox->SpriteSize = editor.LogicComponentSize;
        BlackBox->Held = false;
        
        BlackBox->ConnectChild(Input1);
        BlackBox->ConnectChild(Input2);
        BlackBox->ConnectChild(Input3);
        
        CircuitNode *Output1 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        Output1->Held = false;
        Output1->IsStatic = true;
        Output1->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, (int)editor.IoComponentSize.y * (2 + OutputCenteringYOffset)};
        
        Output1->ConnectChild(BlackBox);
        editor.CircuitTrees.push_back(Output1);
        
        CircuitNode *Output2 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        Output2->Held = false;
        Output2->IsStatic = true;
        Output2->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, (int)editor.IoComponentSize.y * (4 + OutputCenteringYOffset)};
        
        Output2->ConnectChild(BlackBox);
        editor.CircuitTrees.push_back(Output2);
        
        CircuitNode *Output3 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        Output3->Held = false;
        Output3->IsStatic = true;
        Output3->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, (int)editor.IoComponentSize.y * (6 + OutputCenteringYOffset)};
        
        Output3->ConnectChild(BlackBox);
        editor.CircuitTrees.push_back(Output3);
        
        CircuitNode *Output4 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        Output4->Held = false;
        Output4->IsStatic = true;
        Output4->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, (int)editor.IoComponentSize.y * (8 + OutputCenteringYOffset)};
        
        Output4->ConnectChild(BlackBox);
        editor.CircuitTrees.push_back(Output4);
        
        CircuitNode *Output5 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        Output5->Held = false;
        Output5->IsStatic = true;
        Output5->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, (int)editor.IoComponentSize.y * (10 + OutputCenteringYOffset)};
        
        Output5->ConnectChild(BlackBox);
        editor.CircuitTrees.push_back(Output5);
        
        CircuitNode *Output6 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        Output6->Held = false;
        Output6->IsStatic = true;
        Output6->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, (int)editor.IoComponentSize.y * (12 + OutputCenteringYOffset)};
        
        Output6->ConnectChild(BlackBox);
        editor.CircuitTrees.push_back(Output6);
        
        CircuitNodeBIT *UserInput1 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        UserInput1->Held = false;
        UserInput1->IsStatic = true;
        UserInput1->pos = {(2 * (int)editor.IoComponentSize.x), pge->ScreenHeight() / 2 + (0 + InputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNodeBIT *UserInput2 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        UserInput2->Held = false;
        UserInput2->IsStatic = true;
        UserInput2->pos = {(2 * (int)editor.IoComponentSize.x), pge->ScreenHeight() / 2 + (2 + InputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNodeBIT *UserInput3 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        UserInput3->Held = false;
        UserInput3->IsStatic = true;
        UserInput3->pos = {(2 * (int)editor.IoComponentSize.x), pge->ScreenHeight() / 2 + (4 + InputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNode *UserOutput1 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        UserOutput1->Held = false;
        UserOutput1->IsStatic = true;
        UserOutput1->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, pge->ScreenHeight() / 2 + (0 + OutputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNode *UserOutput2 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        UserOutput2->Held = false;
        UserOutput2->IsStatic = true;
        UserOutput2->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, pge->ScreenHeight() / 2 + (2 + OutputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNode *UserOutput3 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        UserOutput3->Held = false;
        UserOutput3->IsStatic = true;
        UserOutput3->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, pge->ScreenHeight() / 2 + (4 + OutputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNode *UserOutput4 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        UserOutput4->Held = false;
        UserOutput4->IsStatic = true;
        UserOutput4->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, pge->ScreenHeight() / 2 + (6 + OutputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNode *UserOutput5 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        UserOutput5->Held = false;
        UserOutput5->IsStatic = true;
        UserOutput5->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, pge->ScreenHeight() / 2 + (8 + OutputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNode *UserOutput6 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        UserOutput6->Held = false;
        UserOutput6->IsStatic = true;
        UserOutput6->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, pge->ScreenHeight() / 2 + (10 + OutputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        editor.CircuitTrees.push_back(UserInput1);
        editor.CircuitTrees.push_back(UserInput2);
        editor.CircuitTrees.push_back(UserInput3);
        editor.CircuitTrees.push_back(UserOutput1);
        editor.CircuitTrees.push_back(UserOutput2);
        editor.CircuitTrees.push_back(UserOutput3);
        editor.CircuitTrees.push_back(UserOutput4);
        editor.CircuitTrees.push_back(UserOutput5);
        editor.CircuitTrees.push_back(UserOutput6);
        
        Inputs.push_back({Input1, UserInput1});
        Inputs.push_back({Input2, UserInput2});
        Inputs.push_back({Input3, UserInput3});
        Outputs.push_back({Output1, UserOutput1});
        Outputs.push_back({Output2, UserOutput2});
        Outputs.push_back({Output3, UserOutput3});
        Outputs.push_back({Output4, UserOutput4});
        Outputs.push_back({Output5, UserOutput5});
        Outputs.push_back({Output6, UserOutput6});
    }
};

#endif //LEVEL_Hard4
