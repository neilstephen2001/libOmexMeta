//
// Created by Ciaran on 4/14/2020.
//

#include "SemsimXmlAssistant.h"

namespace semsim {
    SemsimXmlAssistant::SemsimXmlAssistant(std::string xml, std::string base, int metaid_num_digits) :
            xml_(std::move(xml)), metaid_base(std::move(base)), metaid_num_digits_(metaid_num_digits) {
    }

    const std::vector<std::string> &SemsimXmlAssistant::getValidElements() const {
        return valid_elements_;
    }

    void SemsimXmlAssistant::generateMetaId(std::vector<std::string> &seen_metaids, long count,
                                                    const MetaID &metaid_gen,
                                                    std::string &id) {
        id = metaid_gen.generate(count);

        if (std::find(seen_metaids.begin(), seen_metaids.end(), id) != seen_metaids.end()) {
            count += 1;
            generateMetaId(seen_metaids, count, metaid_gen, id); // recursion
        }
    }

    void SemsimXmlAssistant::addMetaIdsRecursion(xmlNode *a_node, std::vector<std::string> &seen_metaids) {
        MetaID metaId("SemsimMetaid", 0, 4);
        xmlNode *cur_node = nullptr;
        long count = 0;
        for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
            // isolate element nodes
            if (cur_node->type == XML_ELEMENT_NODE) {
                // if the node name is in our list of valid elements or if valid_elements_ = ["All"]
                if (std::find(getValidElements().begin(), getValidElements().end(),
                              std::string((const char *) cur_node->name)) != getValidElements().end()
                    ||
                    (getValidElements().size() == 1 &&
                     strcmp(getValidElements()[0].c_str(), (const char *) "All") != 0)) {
                    // test to see whether the element has the metaid attribute
                    bool has_meta_id = xmlHasProp(cur_node, (const xmlChar *) "metaid");
                    if (!has_meta_id) {
                        // if not, we add one and give it a unique value
                        std::string id;
                        SemsimXmlAssistant::generateMetaId(seen_metaids, count, metaId, id);
                        xmlNewProp(cur_node, (const xmlChar *) "metaid", (const xmlChar *) id.c_str());
                        seen_metaids.push_back(id);
                        count += 1;
                    } else {
                        // if so, we take note by adding it to seen_metaids.
                        xmlChar *id = xmlGetProp(cur_node, (const xmlChar *) "metaid");
                        seen_metaids.emplace_back((const char *) id);
                    }

                }
            }
            // recursion, we do this for every node
            addMetaIdsRecursion(cur_node->children, seen_metaids);
        }
    }


    std::pair<std::string, std::vector<std::string>> SemsimXmlAssistant::addMetaIds() {
        LIBXML_TEST_VERSION;

        xmlDocPtr doc; /* the resulting document tree */
        doc = xmlParseDoc((const xmlChar *) xml_.c_str());
        if (doc == nullptr) {
            throw std::bad_alloc();
        }

        xmlNodePtr root_element = xmlDocGetRootElement(doc);

        std::vector<std::string> seen_metaids = {};
        addMetaIdsRecursion(root_element, seen_metaids);

        xmlChar *s;
        int size;
        xmlDocDumpMemory(doc, &s, &size);
        if (s == nullptr)
            throw std::bad_alloc();
        std::string x = std::string((const char *) s);
        xmlFree(s);

        xmlFreeDoc(doc);
        xmlCleanupParser();
        std::pair<std::string, std::vector<std::string>> sbml_with_metaid(x, seen_metaids);
        return sbml_with_metaid;
    }

    const std::vector<std::string> &SBMLAssistant::getValidElements() const {
        return valid_elements_;
    }

    const std::vector<std::string> &CellMLAssistant::getValidElements() const {
        return valid_elements_;
    }

    XmlAssistantPtr XmlAssistantFactory::generate(const std::string &xml, SemsimXmlType type) {
        switch (type) {
            case SEMSIM_TYPE_SBML: {
                SBMLAssistant sbmlAssistant(xml);
                return std::make_unique<SBMLAssistant>(sbmlAssistant);
            }
            case SEMSIM_TYPE_CELLML: {
                CellMLAssistant cellMlAssistant(xml);
                return std::make_unique<CellMLAssistant>(cellMlAssistant);
            }
            case SEMSIM_TYPE_OTHER: {
                SemsimXmlAssistant xmlAssistant(xml);
                return std::make_unique<SemsimXmlAssistant>(xmlAssistant);
            }
            default:
                throw std::invalid_argument("Not a correct type");
        }
    }

}