#pragma once

struct Variable
{
    float min;
    float max;

    char symbol;
    unsigned char mappingCode;
    /*
        Determines how to render this dimension in the plotter.

        To impliment a new mapping, impliment a method for translating
        output vectors to the appropriate form for that map in 
        Function.GetForm(). Then add the mapping code here.

        0: (default)
            Variable is an abstract parameter and maps to no
            physical quality of the plot. 

            Any number of abstract variables can exist.

            Requires that the function is not over/under perscribed.

            All local parameters are abstract.

        1: 
            Variable maps to a spacial dimension.

            Any number of spacial dimensions can exist. 

            A spacial dimension contributes to the basis array, added in
            the order that the variable was created. 

        2: 
            Variable maps to hue component of HSV color. 

            Only one hue dimension can exist. 

        3: 
            Variable maps to saturation component of HSV color. 

            Only one saturation dimension can exist. 

        3: 
            Varaible maps to value component of HSV color. 

            Only one value dimension can exist.

            Ideally, only 2 of the 3 HSV components are utalized
            as to maximise readablity of the plot. Value is the 
            least suibtable component to use. 

    */
};
