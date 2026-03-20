# ALOHA Network Simulator

This repository contains the implementation and analysis of a **Pure ALOHA Network Simulation**. The project simulates communication between multiple stations and a central hub station over a shared wireless channel, evaluating key metrics such as **network throughput** and **message delay** under various configurations.

## Project Overview

In a pure ALOHA network:
- **N stations** share a wireless channel to communicate with a central hub.
- Each station generates messages of fixed size **S bytes** at random intervals (**T seconds**, following an exponential distribution).
- Collisions occur when multiple stations transmit simultaneously, resulting in failed transmissions.
- A backoff mechanism determines retransmission timing after failed attempts.
- Successfully decoded messages are re-broadcast to all stations over a dedicated, collision-free channel.

The simulator evaluates the network's performance by analyzing throughput and message delay for different station counts (**N**) and values of time intervals (**T**).

## Features

- Simulates message transmission, collision detection, and retransmission logic.
- Incorporates random backoff times for handling collisions.
- Evaluates **throughput** and **message delay** for various network configurations.
- Supports customizable parameters such as message size (**S**), channel capacity (**C**), station counts (**N**) and values of exponential time intervals (**T**).

## Scenarios Evaluated

The following scenarios are included in the simulation:
- **Incremental Count** of the number of stations (**N**).
- **Exponential distribution** of message generation intervals (**T**).

## Deliverables

This repository includes:
1. **Simulator Code**: A fully-functional implementation of the Pure ALOHA network.
2. **Documentation**: Detailed explanation of the simulation design, methodology, and results.

## Execution Instructions

### Prerequisites
Ensure you have **OMNeT++ 6** installed on your system. For installation and setup instructions, visit the [OMNeT++ official site](https://omnetpp.org).

### Running the Simulation

1. Clone this repository:
   ```bash
   https://github.com/0m0D1NGR0NALD/Pure-ALOHA.git
   ```
2. Open **OMNeT++ 6** and import the project.

3. Locate the `omnetpp.ini` configuration file in the project directory.

4. Run the simulation by selecting the following experiment in OMNeT++:
   ```
   [Experiment_varyN_T]
   description = "various values of N, various values of T"
   repeat = 1
   seed-set = 20
   Aloha.N = ${N=5,10,15,20,25,30} # vary N
   Aloha.station[*].T = exponential(${T=2,4,6..30 step 2}s) # vary T
   ```

### Parameters Explanation

- **`Aloha.N`**: Number of stations in the network. The experiment varies this parameter to evaluate performance for different station counts (e.g., 5, 10, 15, etc.).
- **`Aloha.station[*].T`**: Message generation interval for each station, following an exponential distribution. The experiment varies the mean of `T` across a range of values (e.g., 2s to 30s in steps of 2s) to simulate different traffic conditions:
  - Overload conditions (small `T` values)
  - Optimal traffic conditions (moderate `T` values)
  - Low traffic conditions (large `T` values)

### Viewing Results

After running the simulation:
1. Access the output files generated in the **results folder** of the project.
2. Use the OMNeT++ analysis tool to visualize throughput, message delay, and other metrics.

This experiment provides insights into the behavior of the Pure ALOHA protocol under varying traffic and station configurations.

## License

This project is licensed under the MIT License.

## Acknowledgments

This project is part of a course project to evaluate the performance of the Pure ALOHA protocol. Special thanks to the course instructors at the UUniversity of Pisa for their guidance and resources.
