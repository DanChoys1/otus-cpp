#ifndef IDOCUMENT_EXPORTER_H
#define IDOCUMENT_EXPORTER_H

#include "idocument.h"

class IDocumentExporter
{
public:
    virtual ~IDocumentExporter() = default;

    virtual std::unique_ptr<IDocument> create() = 0;
    virtual void save(const std::string& name, const IDocument& doc) = 0;
    virtual std::unique_ptr<IDocument> import(const std::string& name) = 0;
};

#endif