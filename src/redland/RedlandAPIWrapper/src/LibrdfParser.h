//
// Created by Ciaran on 5/20/2020.
//

#ifndef LIBSEMSIM_LIBRDFPARSER_H
#define LIBSEMSIM_LIBRDFPARSER_H


#include <librdf.h>
#include <memory>

#include "World.h"
#include "LibrdfNode.h"
#include "LibrdfUri.h"
#include "LibrdfModel.h"

namespace semsim {

    class LibrdfParser {

        struct deleter {
            void operator()(librdf_parser *parser);
        };

        std::unique_ptr<librdf_parser, deleter> parser_;
    public:
        LibrdfParser() = default;

        explicit LibrdfParser(librdf_parser *parser);

        explicit LibrdfParser(const char *name, const char *mime_type = nullptr,
                              const char *type_uri = nullptr);

        librdf_parser *get() const;

        void setFeature(std::string feature_uri, LibrdfNode node) const;

        [[nodiscard]] int numNamespacesSeen() const;

        [[nodiscard]] std::string getNamespacesSeenUri(int index) const;

        void
        parseString(const std::string &rdf_string, const semsim::LibrdfModel &model, const LibrdfUri &base_uri) const;

        std::string getNamespacesSeenPrefix(int index) const;

        void parseUriIntoModel(const LibrdfUri &uri, const LibrdfUri &base_uri, const LibrdfModel &model) const;

        void parseFilenameUriIntoModel(const LibrdfUri &filename_uri, const LibrdfUri &base_uri,
                                       const LibrdfModel &model) const;
    };
}


#endif //LIBSEMSIM_LIBRDFPARSER_H