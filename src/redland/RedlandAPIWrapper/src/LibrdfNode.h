//
// Created by Ciaran on 5/17/2020.
//

#ifndef LIBSEMSIM_LIBRDFNODE_H
#define LIBSEMSIM_LIBRDFNODE_H

//
#include <librdf.h>
#include <raptor2.h>
#include <memory>
#include <sstream>
//#include "semsim/HERE.h"
#include "LibrdfUri.h"
#include "World.h"
#include "LibrdfException.h"


/*
 * Do I need to mirror the underlying structure of
 * the librdf_node* object?
 */

namespace semsim {

    typedef std::unique_ptr<librdf_node> librdf_node_ptr;

    class LibrdfNode {

//        raptor_term_value value;

        struct deleter {
            void operator()(librdf_node *node);
        };

    private:
        std::unique_ptr<librdf_node, deleter> node_;

    public:
        LibrdfNode() = default;

        explicit LibrdfNode(librdf_node *node);

        [[nodiscard]] librdf_node *get() const;

        static LibrdfNode fromUriString(const std::string &uri_string);

        static LibrdfNode fromBlank(const std::string &blank);

        static LibrdfNode fromLiteral(const std::string &literal, const std::string &xml_language = std::string(),
                                      std::string literal_datatype_uri = "string");

        raptor_term_type getRaptorTermType();

        [[nodiscard]] std::string str() const;

        LibrdfUri getLiteralDatatype();

        std::string getLiteralLanguage();

        std::string getBlankIdentifier();

        LibrdfUri getUri();
    };
}


#endif //LIBSEMSIM_LIBRDFNODE_H