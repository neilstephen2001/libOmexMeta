#include "omexmeta/OmexMetaCApi.h"

using namespace omexmeta;

int main(){

    const char* sbml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<model xmlns=\"http://www.cellml.org/cellml/1.1#\" xmlns:cmeta=\"http://www.cellml.org/metadata/1.0#\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" xmlns:bqs=\"http://www.cellml.org/bqs/1.0#\" xmlns:semsim=\"http://bime.uw.edu/semsim/#\" xmlns:dc=\"https://dublincore.org/specifications/dublin-core/dcmi-terms/\" xmlns:vCard=\"http://www.w3.org/2001/vcard-rdf/3.0#\" name=\"annotation_examples\" cmeta:id=\"annExamples\">\n"
                       "  <component name=\"main\">\n"
                       "    <variable cmeta:id=\"main.Volume\" initial_value=\"100\" name=\"Volume\" units=\"dimensionless\" />\n"
                       "    <variable cmeta:id=\"main.MembraneVoltage\" initial_value=\"-80\" name=\"MembraneVoltage\" units=\"dimensionless\" />\n"
                       "    <variable cmeta:id=\"main.ReactionRate\" initial_value=\"1\" name=\"ReactionRate\" units=\"dimensionless\" />\n"
                       "    <variable cmeta:id=\"main.entity1\" initial_value=\"1\" name=\"entity1\" units=\"dimensionless\" />\n"
                       "    <variable cmeta:id=\"main.entity2\" initial_value=\"1\" name=\"entity2\" units=\"dimensionless\" />\n"
                       "    <variable cmeta:id=\"main.entity3\" initial_value=\"1\" name=\"entity3\" units=\"dimensionless\" />\n"
                       "  </component>\n"
                       "</model>";

    RDF* rdf = RDF_new();

    Editor* editor = RDF_toEditor(rdf, sbml, true, false);

    EnergyDiff* energyDiff = Editor_newEnergyDiff(editor);
    EnergyDiff_addSink(energyDiff, "entity1", LOCAL_URI);
    EnergyDiff_addSource(energyDiff, "entity2", LOCAL_URI);
    EnergyDiff_hasPropertyisVersionOf(energyDiff, "opb:OPB_00592");

    Editor_addEnergyDiff(editor, energyDiff);
    char* rdf_string = RDF_toString(rdf, "turtle");
    printf("%s\n", rdf_string);

    free(rdf_string);
    EnergyDiff_delete(energyDiff);
    Editor_delete(editor);
    RDF_delete(rdf);
    return 0;
}