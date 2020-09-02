#include <stdio.h>
#include "..\data_desk\source\data_desk.h"

static FILE *global_header_file = 0;

DATA_DESK_FUNC void
DataDeskCustomInitCallback(void)
{
	global_header_file = fopen("generated\\generated.h", "w");
    fprintf(global_header_file, "#ifndef GENERATED_GENERATED_H\n");
    fprintf(global_header_file, "#define GENERATED_GENERATED_H\n");
    
}

DATA_DESK_FUNC void
DataDeskCustomParseCallback(DataDeskNode *root, char *filename)
{
	DataDeskFWriteGraphAsC(global_header_file, root);
    
    if(root->type == DataDeskNodeType_EnumDeclaration && DataDeskNodeHasTag(root, "StringArray"))
	{
        fprintf(global_header_file, "\nconst char *%sStrings[]\n{\n\t", root->string);
        for(DataDeskNode *node = root->children_list_head; node; node = node->next)
        {
            fprintf(global_header_file, "\t\"%s\",\n", node->string);
        }
        fprintf(global_header_file, "};\n\n");
    }
}

DATA_DESK_FUNC void
DataDeskCustomCleanUpCallback(void)
{
    fprintf(global_header_file, "#endif //GENERATED_GENERATED_H");
    fclose(global_header_file);
}