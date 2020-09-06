#ifndef LEVEL_Medium7
#define LEVEL_Medium7

struct BlackBoxMedium7 : public BlackBoxCircuitNode {
    /* Number of inputs: 3 */
    /* Number of outputs: 4 */
    
    bool Evaluate(CircuitNode * callingParent) override {
        bool a = SafeEval(Input[0], this);
        bool b = SafeEval(Input[1], this);
        bool c = SafeEval(Input[2], this);
        if (callingParent == parents[0]) {
            return a && (b || !c);
        } else if (callingParent == parents[1]) {
            return (b != c) == a;
        } else if (callingParent == parents[2]) {
            return !(b || c);
        } else if (callingParent == parents[3]) {
            return !(a && c) || b;
        } else {
            Assert(true);
            return false;
        }
    }
};

struct Medium7Level : public Level {
    void OnUserCreate(olc::PixelGameEngine *_pge) {
        Level::OnUserCreate(_pge);
        editor.OnUserCreate(_pge);
        LevelName = "Medium 7\n(3 - 4)";
        LevelCompleteName = "Medium 7\n(3 - 4)";
        
        int InputCenteringYOffset = 1;
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
        
        CircuitNode *BlackBox = new BlackBoxMedium7();
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
        
        editor.CircuitTrees.push_back(UserInput1);
        editor.CircuitTrees.push_back(UserInput2);
        editor.CircuitTrees.push_back(UserInput3);
        editor.CircuitTrees.push_back(UserOutput1);
        editor.CircuitTrees.push_back(UserOutput2);
        editor.CircuitTrees.push_back(UserOutput3);
        editor.CircuitTrees.push_back(UserOutput4);
        
        Inputs.push_back({Input1, UserInput1});
        Inputs.push_back({Input2, UserInput2});
        Inputs.push_back({Input3, UserInput3});
        Outputs.push_back({Output1, UserOutput1});
        Outputs.push_back({Output2, UserOutput2});
        Outputs.push_back({Output3, UserOutput3});
        Outputs.push_back({Output4, UserOutput4});
    }
};

#endif //LEVEL_Medium7
