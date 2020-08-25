//
// Created by Ciaran on 4/22/2020.
//

#include "omexmeta/PhysicalForce.h"

#include <utility>

#include <utility>


namespace omexmeta {

    PhysicalForce::PhysicalForce(librdf_model *model, std::string model_uri,std::string local_uri, PhysicalProperty physicalProperty,
                                 Sources sources,
                                 Sinks sinks)
            : PhysicalPhenomenon(model, model_uri, local_uri, std::move(physicalProperty), PHYSICAL_PROCESS),
              sources_(std::move(sources)), sinks_(std::move(sinks)) {

    }

    const std::vector<SourceParticipant> &PhysicalForce::getSources() const {
        return sources_;
    }

    const std::vector<SinkParticipant> &PhysicalForce::getSinks() const {
        return sinks_;
    }

    std::string PhysicalForce::createMetaId() {
        return generateMetaId("PhysicalForce");
    }

    Triples PhysicalForce::toTriples() {
        if (getAbout().empty()) {
            throw AnnotationBuilderException(
                    "PhysicalForce::toTriples(): Cannot create"
                    " triples because the \"about\" information is not set. "
                    "Use the setAbout() method."
            );
        }
        if (getPhysicalProperty().getResourceStr().empty()) {
            throw AnnotationBuilderException(
                    "PhysicalForce::toTriples(): Cannot create"
                    " triples because the \"physical_property resource\" information is not set. "
                    "Use the setPhysicalProperty() method."
            );
        }

        LOG_DEBUG("here");

        std::string force_id = generateMetaId("PhysicalForce");
        LOG_DEBUG("here");
        force_id = OmexMetaUtils::concatMetaIdAndUri(force_id, getLocalUri());

        LOG_DEBUG("here");
        Triples triples = physical_property_.toTriples(force_id);
        LOG_DEBUG("here");

        for (auto &source : sources_) {
        LOG_DEBUG("here");
            for (auto &triple : source.toTriples(force_id, new_metaid_exclusion_list_)) {
        LOG_DEBUG("here");
                triples.move_back(triple);
        LOG_DEBUG("here");
            }
        }
        LOG_DEBUG("here");
        for (auto &sink : sinks_) {
        LOG_DEBUG("here");
            for (auto &triple : sink.toTriples(force_id, new_metaid_exclusion_list_)) {
        LOG_DEBUG("here");
                triples.move_back(triple);
        LOG_DEBUG("here");
            }
        }
        LOG_DEBUG("here");
        return triples;
    }

    PhysicalForce &PhysicalForce::setPhysicalProperty(PhysicalProperty physicalProperty) {
        physical_property_ = std::move(physicalProperty);
        return (*this);
    }

    PhysicalForce &PhysicalForce::setPhysicalProperty(std::string subject_metaid, std::string physical_property) {
        if (!OmexMetaUtils::startsWith(subject_metaid, "http")){
            subject_metaid = OmexMetaUtils::concatMetaIdAndUri(subject_metaid, getModelUri());
        }
        physical_property_ = PhysicalProperty(std::move(subject_metaid), std::move(physical_property), getModelUri());
        return *this;
    }

    PhysicalForce &PhysicalForce::addSource(double multiplier, const std::string &physical_entity_reference) {
        sources_.push_back(
                std::move(SourceParticipant(
                        model_, multiplier, physical_entity_reference, getModelUri(), getLocalUri()
                ))
        );
        return (*this);
    }

    PhysicalForce &PhysicalForce::addSink(double multiplier, const std::string &physical_entity_reference) {
        sinks_.push_back(
                SinkParticipant(model_, multiplier, physical_entity_reference, getModelUri(), getLocalUri())
        );
        return (*this);
    }

    PhysicalForce::PhysicalForce(librdf_model *model)
            : PhysicalPhenomenon(model) {}

    PhysicalForce::PhysicalForce(librdf_model *model, const std::string& model_uri, const std::string& local_uri)
            : PhysicalPhenomenon(model, model_uri, local_uri) {}

    int PhysicalForce::getNumSources() {
        return sources_.size();
    }

    int PhysicalForce::getNumSinks() {
        return sinks_.size();
    }

    void PhysicalForce::free() {

        for (auto &i : sources_) {
            i.free();
        }
        for (auto &i : sinks_) {
            i.free();
        }
    }

    bool PhysicalForce::operator==(const PhysicalForce &rhs) const {
        return static_cast<const omexmeta::PhysicalPhenomenon &>(*this) ==
               static_cast<const omexmeta::PhysicalPhenomenon &>(rhs) &&
               sources_ == rhs.sources_ &&
               sinks_ == rhs.sinks_;
    }

    bool PhysicalForce::operator!=(const PhysicalForce &rhs) const {
        return !(rhs == *this);
    }

}