#ifndef LEVEL_Hard2
#define LEVEL_Hard2

struct BlackBoxHard2 : public BlackBoxCircuitNode {
    /* Number of inputs: 4 */
    /* Number of outputs: 1 */
    
    bool Evaluate(CircuitNode * callingParent) override {
        bool a = SafeEval(Input[0], this);
        bool b = SafeEval(Input[1], this);
        bool c = SafeEval(Input[2], this);
        bool d = SafeEval(Input[3], this);
        int n = (a?8:0) + (b?4:0) + (c?2:0) + (d?1:0);
        if (callingParent == parents[0]) {
            return n==2 || n==3 || n==5 || n==7 || n==11 || n==13;
        } else {
            Assert(true);
            return false;
        }
    }
};

struct Hard2Level : public Level {
    void OnUserCreate(olc::PixelGameEngine *_pge) {
        Level::OnUserCreate(_pge);
        editor.OnUserCreate(_pge);
        LevelName = "Hard 4\n(4 - 1)";
        LevelCompleteName = "Hard 4\n(4 - 1)\nPrimes";
        
        int InputCenteringYOffset = 0;
        int OutputCenteringYOffset = 3;
        
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
        
        CircuitNodeBIT *Input4 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        Input4->Held = false;
        Input4->IsStatic = true;
        Input4->pos = (2 * editor.IoComponentSize) + olc::vi2d{0, (int)editor.IoComponentSize.y * (6 + InputCenteringYOffset)};
        
        CircuitNode *BlackBox = new BlackBoxHard2();
        BlackBox->SpriteIndex = (int)LOGIC_NONE;
        BlackBox->IsStatic = true;
        BlackBox->pge = pge;
        BlackBox->pos = {((pge->ScreenWidth() - 300) / 2 - (int)editor.LogicComponentSize.x), (Input1->pos.y + (Input4->pos.y + 2 * (int)Input4->SpriteSize.y)) / 2 - (int)editor.LogicComponentSize.y};
        BlackBox->SpriteSheet = editor.LogicComponentRenderable;
        BlackBox->SpriteSize = editor.LogicComponentSize;
        BlackBox->Held = false;
        
        BlackBox->ConnectChild(Input1);
        BlackBox->ConnectChild(Input2);
        BlackBox->ConnectChild(Input3);
        BlackBox->ConnectChild(Input4);
        
        CircuitNode *Output1 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        Output1->Held = false;
        Output1->IsStatic = true;
        Output1->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, (int)editor.IoComponentSize.y * (2 + OutputCenteringYOffset)};
        
        Output1->ConnectChild(BlackBox);
        editor.CircuitTrees.push_back(Output1);
        
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
        
        CircuitNodeBIT *UserInput4 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        UserInput4->Held = false;
        UserInput4->IsStatic = true;
        UserInput4->pos = {(2 * (int)editor.IoComponentSize.x), pge->ScreenHeight() / 2 + (6 + InputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        CircuitNode *UserOutput1 = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);
        UserOutput1->Held = false;
        UserOutput1->IsStatic = true;
        UserOutput1->pos = {pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, pge->ScreenHeight() / 2 + (0 + OutputCenteringYOffset) * (int)editor.IoComponentSize.y};
        
        editor.CircuitTrees.push_back(UserInput1);
        editor.CircuitTrees.push_back(UserInput2);
        editor.CircuitTrees.push_back(UserInput3);
        editor.CircuitTrees.push_back(UserInput4);
        editor.CircuitTrees.push_back(UserOutput1);
        
        Inputs.push_back({Input1, UserInput1});
        Inputs.push_back({Input2, UserInput2});
        Inputs.push_back({Input3, UserInput3});
        Inputs.push_back({Input4, UserInput4});
        Outputs.push_back({Output1, UserOutput1});
    }
};

#endif //LEVEL_Hard2
