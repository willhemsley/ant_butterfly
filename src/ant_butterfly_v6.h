
#ifndef ANT_BUTTERFLY_V6_H_
#define ANT_BUTTERFLY_V6_H_

#include "biodynamo.h"
#include "ant.h"
#include "butterfly.h"
#include "core/util/timing.h"
#include "behavior.h"
//#include "physical_bond.h"

namespace bdm {

// List substance(s) to be used in the simulation
enum Substances { kSubstanceSugar }; //kDepth may be added

inline int Simulate(int argc, const char** argv) {
  auto set_param = [](Param* param) {
    // Create an artificial bound for the simulation space
    param->bound_space = Param::BoundSpaceMode::kClosed;  // set domain as closed space
    param->min_bound = 0;
    param->max_bound = 500; // cube of 500*500*500
    param->unschedule_default_operations = {"mechanical forces"};
    param->remove_output_dir_contents = true; // remove the old output files
  };

  Simulation simulation(argc, argv, set_param);
  auto* param = simulation.GetParam(); // param holds simulation parameters
  auto* rm = simulation.GetResourceManager(); // create ResourceManager
  auto* myrand = simulation.GetRandom(); // random number

  size_t num_larvae = 100; // set the number of larvae cells
  size_t num_ant = 1000; // set the number of ant cells
  double x_coord, y_coord, z_coord {0}; // initialise coordinates

// Parralise the following block of code
//#pragma omp parallel

  myrand->SetSeed(4321); // Set random seed so reproducible results

  /**********************************************************************
  / Defining Substances
  // Order: substance id, substance_name, diffusion_coefficient, decay_constant,
  // resolution of diffusion grid
  ***********************************************************************/

  ModelInitializer::DefineSubstance(kSubstanceSugar, "Substance_Sugar", 0.5, 0.1, 20);
  //ModelInitializer::DefineSubstance(kDepth, "Depth", 0.5, 0.1, 20);

  /**********************************************************************
  / Initialise Substance
  // Order: substance id, function
  ***********************************************************************/

  //ModelInitializer::InitializeSubstance(kDepth, GaussianBand(250, 500, Axis::YAxis));

  /**********************************************************************
  / Creating the Ants
  ***********************************************************************/

  for (int i = 0; i < num_ant; ++i) {
    x_coord = myrand->Uniform(param->min_bound, param->max_bound);
    y_coord = myrand->Uniform(150, 500); // set overground as above 150
    z_coord = myrand->Uniform(param->min_bound, param->max_bound);

    Ant* ant = new Ant({x_coord, y_coord, z_coord});
    ant->SetDiameter(10);
    ant->AddBehavior(new Chemotaxis("Substance_Sugar", 3)); // Chemotaxis(substance, speed)
    //ant->AddBehavior(new Chemotaxis("Substance", 1));
    //ant->AddBehavior(new RandomMovement(0.5)); // RandomMovement(speed)
    //ant->AddBehavior(new YMovement_Und(-0.5)); // MovementX(speed)
    // ant->SetAgentType(1);
    // ant->SetAntColour(1);
    //ant->AddBehavior(new Bond_Species());

    rm->AddAgent(ant);
  }

  /**********************************************************************
  / Creating the Larvae
  ***********************************************************************/

  for (int j = 0; j < num_larvae; ++j) {
    x_coord = myrand->Uniform(param->min_bound, param->max_bound); // set starting boundary in x
    y_coord = myrand->Uniform(150, 500); // set overground => above 150
    z_coord = myrand->Uniform(param->min_bound, param->max_bound);

    Butterfly* larvae = new Butterfly({x_coord, y_coord, z_coord});
    larvae->SetDiameter(10);
    larvae->AddBehavior(new Secretion("Substance_Sugar")); // Add secretion behaviour, i.e secrete sugar
    //larvae->AddBehavior(new Chemotaxis("Substance", 1));
    //larvae->AddBehavior(new RandomMovement());
    //larvae->AddBehavior(new YMovement_Und(1));
    // larvae->SetAgentType(-1);
    // larvae->SetButterflyColour(-1);
    //larvae->AddBehavior(new Bond_Species());
    larvae->AddBehavior(new Fix_Nearby_Ant());
    // larvae->AddBehavior(new Carry_Larva());

    rm->AddAgent(larvae);
  }

  /**********************************************************************
  / Running the Simulation
  ***********************************************************************/

  auto start = Timing::Timestamp(); // get start time

  simulation.GetScheduler()->Simulate(500);

  std::cout << "Simulation completed successfully!" << std::endl;
  auto stop = Timing::Timestamp(); // get end time
  std::cout << "RUNTIME: " << (stop - start) << std::endl;

  return 0;

}

}  // namespace bdm

#endif  // ANT_BUTTERFLY_V6_H_
