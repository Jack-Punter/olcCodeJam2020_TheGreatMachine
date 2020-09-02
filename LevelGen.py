import sys;
def generate(inputs, outputs, level_code, validate_filename):
    with open(validate_filename, 'w') as file:

        file.write(f"#ifndef LEVEL_{level_code}\n")
        file.write(f"#define LEVEL_{level_code}\n")
        file.write("\n")

        file.write(f"struct BlackBox{level_code} : public BlackBoxCircuitNode")
        file.write(" {\n")
        file.write(f"    /* Number of inputs: {inputs} */\n")
        file.write(f"    /* Number of outputs: {outputs} */\n\n")
        file.write("    bool Evaluate(CircuitNode * callingParent) override {\n")
        file.write("        /* return SafeEval(Input[0], this) && SafeEval(Input[1], this); */\n")
        file.write("    }\n")
        file.write("};\n")
        file.write("\n")
        file.write(f"struct {level_code}Level : public Level ")
        file.write("{\n")
        file.write("    void OnUserCreate(olc::PixelGameEngine *_pge) {\n")
        file.write("        Level::OnUserCreate(_pge);\n")
        file.write("        editor.OnUserCreate(_pge);\n")
        file.write(f'        LevelName = "Level {inputs} - {outputs}";\n')
        file.write(f'        LevelCompleteName = "Level {inputs} - {outputs}\\n{level_code}";\n')
        file.write("\n")


        if inputs < outputs:
            file.write(f"        int InputCenteringYOffset = {outputs-inputs};\n")
            file.write("        int OutputCenteringYOffset = 0;\n")
        elif inputs > outputs:
            file.write("        int InputCenteringYOffset = 0;\n")
            file.write(f"        int OutputCenteringYOffset = {inputs-outputs};\n")
        else:
            file.write("        int InputCenteringYOffset = 0;\n")
            file.write("        int OutputCenteringYOffset = 0;\n")
        file.write("\n")


        # Draw black box inputs
        for i in range(inputs):

            file.write(f"        CircuitNodeBIT *Input{i+1} = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);\n")
            file.write(f"        Input{i+1}->Held = false;\n")
            file.write(f"        Input{i+1}->IsStatic = true;\n")
            file.write(f"        Input{i+1}->pos = (2 * editor.IoComponentSize) + ")
            file.write("olc::vi2d{0, (int)editor.IoComponentSize.y * ")
            file.write(f"({2*i} + InputCenteringYOffset)")
            file.write("};\n")
            file.write("\n")

        # Draw black box

        file.write(f"        CircuitNode *BlackBox = new BlackBox{level_code}();\n")
        file.write("        BlackBox->SpriteIndex = (int)LOGIC_NONE;\n")
        file.write("        BlackBox->IsStatic = true;\n")
        file.write("        BlackBox->pge = pge;\n")
        file.write("        BlackBox->pos = {((pge->ScreenWidth() - 300) / 2 - (int)editor.LogicComponentSize.x), " + 
				  f"(Input1->pos.y + (Input{inputs}->pos.y + 2 * (int)Input{inputs}->SpriteSize.y)) / 2 - (int)editor.LogicComponentSize.y" + "};\n")
        file.write("        BlackBox->SpriteSheet = editor.LogicComponentRenderable;\n")
        file.write("        BlackBox->SpriteSize = editor.LogicComponentSize;\n")
        file.write("        BlackBox->Held = false;\n")
        file.write("\n")


        # Draw input connections
        for i in range(inputs):
            file.write(f"        BlackBox->ConnectChild(Input{i+1});\n")
        file.write("\n")


        # Draw black box outputs
        for o in range(outputs):

            file.write(f"        CircuitNode *Output{o+1} = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);\n")
            file.write(f"        Output{o+1}->Held = false;\n")
            file.write(f"        Output{o+1}->IsStatic = true;\n")

            file.write(f"        Output{o+1}->pos = ")
            file.write("{pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, (int)editor.IoComponentSize.y * ")
            file.write(f"({2*(o+1)} + OutputCenteringYOffset)")
            file.write("};\n")
            file.write("\n")

            file.write(f"        Output{o+1}->ConnectChild(BlackBox);\n")
            file.write(f"        editor.CircuitTrees.push_back(Output{o+1});\n")
            file.write("\n")

        # bottom inputs
        for i in range(inputs):
            file.write(f"        CircuitNodeBIT *UserInput{i+1} = (CircuitNodeBIT *)CreateHeldNode<IoCircuitNode>(_pge, IO_BIT, editor.IoComponentSize, editor.IoComponentRenderable);\n")
            file.write(f"        UserInput{i+1}->Held = false;\n")
            file.write(f"        UserInput{i+1}->IsStatic = true;\n")
            file.write(f"        UserInput{i+1}->pos = ")
            file.write("{(2 * (int)editor.IoComponentSize.x), pge->ScreenHeight() / 2 + ")
            file.write(f"({2*i} + InputCenteringYOffset) * (int)editor.IoComponentSize.y")
            file.write("};\n")
            file.write("\n")


        # bottom outputs
        for o in range(outputs):
            file.write(f"        CircuitNode *UserOutput{o+1} = CreateHeldNode<IoCircuitNode>(_pge, IO_LED, editor.IoComponentSize, editor.IoComponentRenderable);\n")
            file.write(f"        UserOutput{o+1}->Held = false;\n")
            file.write(f"        UserOutput{o+1}->IsStatic = true;\n")


            file.write(f"        UserOutput{o+1}->pos = ")
            file.write("{pge->ScreenWidth() - 300 - 4 * (int)editor.IoComponentSize.x, pge->ScreenHeight() / 2 + ")
            file.write(f"({2*o} + OutputCenteringYOffset) * (int)editor.IoComponentSize.y")
            file.write("};\n")
            file.write("\n")

        for i in range(inputs):
            file.write(f"        editor.CircuitTrees.push_back(UserInput{i+1});\n")
        for o in range(outputs):
            file.write(f"        editor.CircuitTrees.push_back(UserOutput{o+1});\n")

        file.write(f"\n")

        for i in range(inputs):
            file.write("        Inputs.push_back({")
            file.write(f"Input{i+1}, UserInput{i+1}")
            file.write("});\n")
        for o in range(outputs):
            file.write("        Outputs.push_back({")
            file.write(f"Output{o+1}, UserOutput{o+1}")
            file.write("});\n")

        file.write("    }\n")

        file.write("};\n")
        file.write("\n")
        file.write(f"#endif //LEVEL_{level_code}\n")

if __name__ == "__main__":
    print(sys.argv)
    if (len(sys.argv) < 3 or len(sys.argv) > 4) or (not sys.argv[1].isdigit() or not sys.argv[2].isdigit()):
        print("Usage python LevelGen.py [NumInputs] [NumOutputs] <optionalLevelName>")
    else:
        iCount = int(sys.argv[1]);
        oCount = int(sys.argv[2]);
        if len(sys.argv) == 4:
            LEVELName = sys.argv[3]
        else:
            LEVELName = "TEST"
        
        fileName = f"src/generated/{LEVELName}Level.h"
        print(f"Generating Level: {fileName}")
        generate(iCount, oCount, LEVELName, fileName);
        print("Level Generated")
