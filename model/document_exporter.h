#ifndef DOCUMENT_EXPORTER_H
#define DOCUMENT_EXPORTER_H

#include <iostream>

#include "document.h"
#include "idocument_exporter.h"

class DocumentExporter final : public IDocumentExporter
{
public:
    std::unique_ptr<IDocument> create() override
    {
        std::cout << "Create document" << std::endl;
        return std::make_unique<Document>();
    }

    void save(const std::string& name, const IDocument& doc) override
    {
        std::cout << "Save document. Name:  " << name  << ", Shape: " << std::endl
                  << doc.data();
    }

    std::unique_ptr<IDocument> import(const std::string& name) override
    {
        std::cout << "Import document. Name:  " << name << std::endl;
        return std::make_unique<Document>();
    }
};

#endif