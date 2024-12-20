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
3. **Presentation**: A concise 10-slide presentation summarizing key findings.

## Usage

1. Clone the repository:  
   ```
   git clone https://github.com/yourusername/aloha-network-simulator.git
   ```
2. Follow the instructions in the documentation to configure and run the simulator.

## License

This project is licensed under the MIT License.

## Acknowledgments

This project is part of a course project to evaluate the performance of the Pure ALOHA protocol. Special thanks to the course instructors for their guidance and resources.
