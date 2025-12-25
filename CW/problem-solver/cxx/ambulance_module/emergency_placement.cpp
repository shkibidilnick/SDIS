#include "emergency_placement_module.hpp"
#include "agents/village_distance_calculator.hpp"
#include "agents/graph_center_locator.hpp"
#include "agents/station_location_optimizer.hpp" 
#include "agents/accessibility_analyzer.hpp"
#include "keynodes/emergency_service_concepts.hpp"

using namespace ambulance_module;

SC_MODULE_REGISTER(AmbulanceModule)
    ->Agent<CalculateDistancesAgent>()
    ->Agent<FindCenterAgent>()
    ->Agent<FindOptimalAgent>()
    ->Agent<FindProblemZonesAgent>();
