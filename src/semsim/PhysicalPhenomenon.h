//
// Created by Ciaran on 4/22/2020.
//

#ifndef LIBSEMSIM_PhysicalPhenomenon_H
#define LIBSEMSIM_PhysicalPhenomenon_H

#include "semsim/Subject.h"
#include "semsim/PhysicalPropertyResource.h"
#include "semsim/AnnotationType.h"
#include "semsim/Triple.h"
#include "semsim/Triples.h"
#include "semsim/MetaID.h"
#include "semsim/Error.h"
#include "semsim/Query.h"
#include "RedlandAPIWrapper.h"
#include "HERE.h"
using namespace redland;

namespace semsim {
    class PhysicalPhenomenon {
    protected:
        PhysicalPhenomenon(librdf_model *model, AnnotationType type);

        librdf_model *model_ = nullptr; // should be cleaned up by the LibrdfModel inside RDF.
        Subject about;
        PhysicalPropertyResource physical_property_;
        AnnotationType type_;

        std::string generateMetaId(std::string id_base) const;

    public:
        PhysicalPhenomenon() = delete;

        ~PhysicalPhenomenon();

        PhysicalPhenomenon(const PhysicalPhenomenon &phenomenon);

        PhysicalPhenomenon(PhysicalPhenomenon &&phenomenon) noexcept;

        PhysicalPhenomenon &operator=(const PhysicalPhenomenon &phenomenon);

        PhysicalPhenomenon &operator=(PhysicalPhenomenon &&phenomenon) noexcept;

        PhysicalPhenomenon(librdf_model* model);

        PhysicalPhenomenon(librdf_model* model, Subject metaid,
                           PhysicalPropertyResource propertyResource,
                           AnnotationType type);

        Subject getAbout() const;

        Subject getSubject() const;

        AnnotationType getType() const;

        PhysicalPropertyResource getPhysicalProperty() const;

        virtual Triples toTriples() const;


    };

    typedef std::shared_ptr<PhysicalPhenomenon> PhysicalPhenomenonPtr;


}

#endif //LIBSEMSIM_PhysicalPhenomenon_H
