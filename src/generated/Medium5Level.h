#ifndef LEVEL_Medium2
#define LEVEL_Medium2

struct BlackBoxMedium5 : public BlackBoxCircuitNode {
    /* Number of inputs: 6 */
    /* Number of outputs: 5 */

    bool Evaluate(CircuitNode * callingParent) override {
        bool a = SafeEval(Input[0], this);
        bool b = SafeEval(Input[1], this);
        bool c = SafeEval(Input[2], this);
        bool d = SafeEval(Input[3], this);
        bool e = SafeEval(Input[4], this);
        bool f = SafeEval(Input[5], this);
        bool out1 = a && b;
        bool out3 = (c != d);
        bool out5 = e || f;
        if (callingParent == parents[0]) {
            return out1;
        } else if (callingParent == parents[1]) {
            return out1 || out3;
        } else if (callingParent == parents[2]) {
            return out3;
        } else if (callingParent == parents[3]) {
            return out3 && out5;
        } else if (callingParent == parents[4]) {
            return out5;
        } else {
            Assert(true);
            return false;
        }
    }
};

struct Medium5Level : public Level {
    void OnUserCreate(olc::PixelGameEngine *_pge) {
        Level::OnUserCreate(_pge);
        editor.OnUserCreate(_pge);
        LevelName = "Medium 5\n(6 - 5)";
        LevelCompleteName = "Medium 5\n(6 - 5)";

        int InputCenteringYOffset = 0;
        int OutputCenteringYOffset = 1;

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

        CircuitNodeBIT *Input5 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        Input5->Held = false;
        Input5->IsStatic = true;
        Input5->pos = (2 * editor.IoComponentSize) + olc::vi2d{0, (int)editor.IoComponentSize.y * (8 + InputCenteringYOffset)};

        CircuitNodeBIT *Input6 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        Input6->Held = false;
        Input6->IsStatic = true;
        Input6->pos = (2 * editor.IoComponentSize) + olc::vi2d{0, (int)editor.IoComponentSize.y * (10 + InputCenteringYOffset)};

        CircuitNode *BlackBox = new BlackBoxMedium5();
        BlackBox->SpriteIndex = (int)LOGIC_NONE;
        BlackBox->IsStatic = true;
        BlackBox->pge = pge;
        BlackBox->pos = {((pge->ScreenWidth() - 300) / 2 - (int)editor.LogicComponentSize.x), (Input1->pos.y + (Input6->pos.y + 2 * (int)Input6->SpriteSize.y)) / 2 - (int)editor.LogicComponentSize.y};
        BlackBox->SpriteSheet = editor.LogicComponentRenderable;
        BlackBox->SpriteSize = editor.LogicComponentSize;
        BlackBox->Held = false;

        BlackBox->ConnectChild(Input1);
        BlackBox->ConnectChild(Input2);
        BlackBox->ConnectChild(Input3);
        BlackBox->ConnectChild(Input4);
        BlackBox->ConnectChild(Input5);
        BlackBox->ConnectChild(Input6);

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

        CircuitNodeBIT *UserInput5 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        UserInput5->Held = false;
        UserInput5->IsStatic = true;
        UserInput5->pos = {(2 * (int)editor.IoComponentSize.x), pge->ScreenHeight() / 2 + (8 + InputCenteringYOffset) * (int)editor.IoComponentSize.y};

        CircuitNodeBIT *UserInput6 = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);
        UserInput6->Held = false;
        UserInput6->IsStatic = true;
        UserInput6->pos = {(2 * (int)editor.IoComponentSize.x), pge->ScreenHeight() / 2 + (10 + InputCenteringYOffset) * (int)editor.IoComponentSize.y};

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

        editor.CircuitTrees.push_back(UserInput1);
        editor.CircuitTrees.push_back(UserInput2);
        editor.CircuitTrees.push_back(UserInput3);
        editor.CircuitTrees.push_back(UserInput4);
        editor.CircuitTrees.push_back(UserInput5);
        editor.CircuitTrees.push_back(UserInput6);
        editor.CircuitTrees.push_back(UserOutput1);
        editor.CircuitTrees.push_back(UserOutput2);
        editor.CircuitTrees.push_back(UserOutput3);
        editor.CircuitTrees.push_back(UserOutput4);
        editor.CircuitTrees.push_back(UserOutput5);

        Inputs.push_back({Input1, UserInput1});
        Inputs.push_back({Input2, UserInput2});
        Inputs.push_back({Input3, UserInput3});
        Inputs.push_back({Input4, UserInput4});
        Inputs.push_back({Input5, UserInput5});
        Inputs.push_back({Input6, UserInput6});
        Outputs.push_back({Output1, UserOutput1});
        Outputs.push_back({Output2, UserOutput2});
        Outputs.push_back({Output3, UserOutput3});
        Outputs.push_back({Output4, UserOutput4});
        Outputs.push_back({Output5, UserOutput5});
    }
};

#endif //LEVEL_Medium2
