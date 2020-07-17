//
// Created by Ciaran on 4/14/2020.
//

#include "OmexMetaXmlAssistant.h"
#include "HERE.h"

namespace omexmeta {
    OmexMetaXmlAssistant::OmexMetaXmlAssistant(std::string xml, std::string metaid_base, int metaid_num_digits,
                                               bool generate_new_metaids) :
            xml_(std::move(xml)), metaid_base_(std::move(metaid_base)), metaid_num_digits_(metaid_num_digits),
            generate_new_metaids_(generate_new_metaids) {
    }

    std::vector<std::string> OmexMetaXmlAssistant::getValidElements() const {
        return std::vector<std::string>({"Any"});
    }

    void OmexMetaXmlAssistant::generateMetaId(std::vector<std::string> &seen_metaids, long count,
                                              const MetaID &metaid_gen,
                                              std::string &id) {
        id = metaid_gen.generate(count);
        if (std::find(seen_metaids.begin(), seen_metaids.end(), id) != seen_metaids.end()) {
            count += 1;
            generateMetaId(seen_metaids, count, metaid_gen, id); // recursion
        }
    }

    void OmexMetaXmlAssistant::addMetaIdsRecursion(xmlNode *a_node, std::vector<std::string> &seen_metaids) {
        //todo make OmexMetaId sting
        MetaID metaId(getMetaidBase(), 0, getMetaidNumDigits());
        xmlNode *cur_node;
        cur_node = a_node;
        long count = 0;
        for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
            // isolate element nodes
            if (cur_node->type == XML_ELEMENT_NODE) {
                const std::vector<std::string> &valid_elements = getValidElements();
                // if the node name is in our list of valid elements or if valid_elements_ = ["All"]
                if (std::find(valid_elements.begin(), valid_elements.end(),
                              std::string((const char *) cur_node->name)) != valid_elements.end()
                    || (valid_elements.size() == 1 && valid_elements[0] == "Any")) {
                    // test to see whether the element has the metaid attribute
                    bool has_meta_id_value = xmlHasProp(cur_node, (const xmlChar *) metaIdTagName().c_str());
                    if (!has_meta_id_value) {
                        // next we check to see whether the user wants us to add a metaid for them
                        // By collect only, we meet, "we collect the metaids but do not add new ones"
                        if (generateNewMetaids()) {
                            // If we don't already have metaid and user wants us to add one, we generate a unique id
                            std::string id;
                            OmexMetaXmlAssistant::generateMetaId(seen_metaids, count, metaId, id);
                            xmlNewProp(cur_node, (const xmlChar *) metaIdTagName().c_str(),
                                       (const xmlChar *) id.c_str());
                            seen_metaids.push_back(id);
                            count += 1;
                        }
                    } else {
                        // if so, we take note by adding it to seen_metaids.
                        xmlChar *id = xmlGetProp(cur_node, (const xmlChar *) metaIdTagName().c_str());
                        seen_metaids.emplace_back((const char *) id);
                        xmlFree(id);
                    }
                }
            }
            // recursion, we do this for every node
            addMetaIdsRecursion(cur_node->children, seen_metaids);
        }
    }


    std::pair<std::string, std::vector<std::string>> OmexMetaXmlAssistant::addMetaIds() {
        LIBXML_TEST_VERSION;
        xmlDocPtr doc; /* the resulting document tree */
        doc = xmlParseDoc((const xmlChar *) xml_.c_str());
        if (doc == nullptr) {
            throw NullPointerException("NullPointerException:  OmexMetaXmlAssistant::addMetaIds(): doc");
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

    std::string OmexMetaXmlAssistant::metaIdTagName() const {
        return "metaid";
    }

    bool OmexMetaXmlAssistant::generateNewMetaids() const {
        return generate_new_metaids_;
    }

    const std::string &OmexMetaXmlAssistant::getMetaidBase() const {
        return metaid_base_;
    }

    int OmexMetaXmlAssistant::getMetaidNumDigits() const {
        return metaid_num_digits_;
    }

    std::vector<std::string> SBMLAssistant::getValidElements() const {
        std::vector<std::string> valid_elements_ = {
                "model",
                "unit",
                "compartment",
                "species",
                "reaction",
                "kineticLaw",
                "localParameter",
        };
        return valid_elements_;
    }

    std::string SBMLAssistant::metaIdTagName() const {
        return "metaid";
    }

    std::vector<std::string> CellMLAssistant::getValidElements() const {
        std::vector<std::string> valid_elements_ = {
                "model",
                "unit",
                "compartment",
                "species",
                "reaction",
                "kineticLaw",
                "localParameter",
        };
        return valid_elements_;
    }

    std::string CellMLAssistant::metaIdTagName() const {
        return "cmeta";
    }

    XmlAssistantPtr
    SemsimXmlAssistantFactory::generate(const std::string &xml, OmexMetaXmlType type, bool generate_new_metaids,
                                        std::string metaid_base, int metaid_num_digits) {
        switch (type) {
            case OMEXMETA_TYPE_SBML: {
                SBMLAssistant sbmlAssistant(xml, metaid_base, metaid_num_digits, generate_new_metaids);
                return std::make_unique<SBMLAssistant>(sbmlAssistant);
            }
            case OMEXMETA_TYPE_CELLML: {
                CellMLAssistant cellMlAssistant(xml, metaid_base, metaid_num_digits, generate_new_metaids);
                return std::make_unique<CellMLAssistant>(cellMlAssistant);
            }
            case OMEXMETA_TYPE_OTHER: {
                OmexMetaXmlAssistant xmlAssistant(xml, metaid_base, metaid_num_digits, generate_new_metaids);
                return std::make_unique<OmexMetaXmlAssistant>(xmlAssistant);
            }
            default:
                throw std::invalid_argument("Not a correct type");
        }
    }

}