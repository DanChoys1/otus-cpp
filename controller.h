#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>

#include "iview.h"
#include "idocument_exporter.h"
#include "circle_primitive.h"
#include "line_primitive.h"
#include "rect_primitive.h"

class Controller
{
public:
    Controller(std::unique_ptr<IView> view, std::unique_ptr<IDocumentExporter> docExp) :
        _view(std::move(view)), _docExp(std::move(docExp))
    {}

    void createNewDoc()
    {
        _doc = _docExp->create();
    }

    void importDoc(const std::string& name)
    {
        if (_doc && _view->showDialog("Remove current document changes?"))
            _doc = _docExp->import(name);
    }

    void saveDoc(const std::string& name)
    {
        _docExp->save(name, *_doc);
    }

    void addCircle(Circle circle)
    {
        std::cout << "Add " << circle << std::endl;
        _doc->setPrimitive(std::make_unique<CirclePrimitive>(circle));
    }

    void addLine(Line line)
    {
        std::cout << "Add " << line << std::endl;
        _doc->setPrimitive(std::make_unique<LinePrimitive>(line));
    }

    void addRect(Rect rect)
    {
        std::cout << "Add " << rect << std::endl;
        _doc->setPrimitive(std::make_unique<RectPrimitive>(rect));
    }

    void removePrimitive(int index)
    {
        std::cout << "Remove primitive index = " << index << std::endl;
        _doc->removePrimitive(index);
    }

    void update()
    {
        _view->display(*_doc);
    }

private:
    std::unique_ptr<IView> _view;
    std::unique_ptr<IDocument> _doc;
    std::unique_ptr<IDocumentExporter> _docExp;

};

#endif