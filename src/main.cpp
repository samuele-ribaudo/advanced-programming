#include <iostream>
#include "Workflow.h"

int main() {
    
    Workflow workflow;
    workflow.runMainLoop();
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
}