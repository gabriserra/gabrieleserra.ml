---
title: Scheduling Replica Voting in Fixed-Priority Real-Time Systems
authors: Pietro Fara, <strong>Gabriele Serra</strong>, Alessandro Biondi, Ciro Donnarumma 
authors_affiliations: 1, 1, 1, 2
institutions:
  - Scuola Superiore Sant'Anna, Pisa, Italy
  - Rete Ferroviaria Italiana, Italy
date: 2021-06-30
custom_date: Jun 30, 2021
publication_name: Leibniz International Proceedings in Informatics (LIPIcs) of the 33rd Euromicro Conference on Real-Time Systems (ECRTS 2021) 
book:
pages: 13:1--13:21
editor: Schloss Dagstuhl -- Leibniz-Zentrum fur Informatik
doi: https://doi.org/10.4230/LIPIcs.ECRTS.2021.13
description: Reliability and safety are mandatory requirements for safety-critical embedded systems. The design of a fault-tolerant system is required in many fields (e.g., railway, automotive, avionics) and redundancy helps in achieving this goal. Redundant systems typically leverage voting techniques applied to the outputs produced by tasks to detect and even tolerate failures.
This paper studies the integration of distributed voting protocols in fixed-priority real-time systems from a scheduling perspective. It analyzes two scheduling strategies for implementing voting. One is attractive and friendly for software developers and based on suspending the task execution until the replica provides the data to be voted. The other one is inspired by the Logical Execution Time (LET) paradigm and requires introducing additional tasks in the system to accomplish voting-related activities. Queuing and delays introduced by inter-replica communication interfaces are also analyzed.
Experimental results are finally presented to compare the two strategies, showing that LET-inspired voting is much more predictable and hence more suitable than the other strategy for fixed-priority real-time systems.
paper_link: /assets/documents/papers/fara-ecrts2021.pdf
award: 
award_link: 
language: eng
youtube:
---
