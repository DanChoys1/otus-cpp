#include <iostream>
#include <vector>

#include "controller.h"
#include "view.h"
#include "document_exporter.h"

using namespace std;

int main()
{
    Controller control { std::make_unique<View>(), std::make_unique<DocumentExporter>() };
    control.createNewDoc();
    control.addCircle({{0,0}, 8});
    control.addLine({{0,0}, {10,10}});
    control.addRect({{-5,-5}, {5,5}});
    control.update();
    control.removePrimitive(1);
    control.update();
    control.importDoc("Other Doc");
    
    return 0;
}
