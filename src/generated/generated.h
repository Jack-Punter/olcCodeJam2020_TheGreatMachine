#ifndef GENERATED_GENERATED_H
#define GENERATED_GENERATED_H
// @StringArray 
typedef enum LogicComponent
{
    LOGIC_XOR,
    LOGIC_OR,
    LOGIC_AND,
    LOGIC_BUFFER,
    LOGIC_XNOR,
    LOGIC_NOR,
    LOGIC_NAND,
    LOGIC_NOT,
    LOGIC_NONE,
}
LogicComponent;

const char *LogicComponentStrings[]
{
		"LOGIC_XOR",
	"LOGIC_OR",
	"LOGIC_AND",
	"LOGIC_BUFFER",
	"LOGIC_XNOR",
	"LOGIC_NOR",
	"LOGIC_NAND",
	"LOGIC_NOT",
	"LOGIC_NONE",
};

// @StringArray 
typedef enum IoComponent
{
    IO_BIT,
    IO_LED,
    IO_NONE,
}
IoComponent;

const char *IoComponentStrings[]
{
		"IO_BIT",
	"IO_LED",
	"IO_NONE",
};

// @StringArray 
typedef enum ControlMode
{
    PLACE_COMPONENTS,
    PLACE_CONNECTIONS,
    INTERACT,
    DELETE_TREES,
    DELETE_NODES,
}
ControlMode;

const char *ControlModeStrings[]
{
		"PLACE_COMPONENTS",
	"PLACE_CONNECTIONS",
	"INTERACT",
	"DELETE_TREES",
	"DELETE_NODES",
};

#endif //GENERATED_GENERATED_H