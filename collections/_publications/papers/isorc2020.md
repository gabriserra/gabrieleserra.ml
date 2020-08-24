---
title: An Architecture for Declarative Real-Time Scheduling on Linux 
authors: <strong>Gabriele Serra</strong>, Gabriele Ara, Pietro Fara, Tommaso Cucinotta 
authors_affiliations: 1, 1, 1, 1
institutions:
  - Scuola Superiore Sant'Anna, Pisa, Italy
date: 2020-05-19
custom_date: May 19-21, 2020
publication_name: Proceedings of the 23rd International Symposium on Real-Time Distributed Computing (ISORC 2020), Nashville, TN, USA 
book:
pages: pp. 20-28
editor: IEEE
doi: https://doi.org/10.1109/ISORC49007.2020.00013
description: This paper proposes a novel framework and programming model for real-time applications supporting a declarative access to real-time CPU scheduling features that are available on an operating system. The core idea is to let applications declare their temporal characteristics and/or requirements on the CPU allocation, where, for example, some of them may require real-time POSIX priorities, whilst others might need resource reservations through SCHED_DEADLINE. The framework can properly handle such a set of heterogeneous requirements configuring an underlying multi-core platform so to exploit the various scheduling disciplines that are available in the kernel, matching applications requirements. The framework is realized as a modular architecture in which different plugins handle independently certain real-time scheduling features within the underlying kernel, easing the customization of its behavior to support other schedulers or operating systems by adding further plugins.
paper_link: /assets/documents/papers/serra-isorc2020.pdf
award: 
award_link: 
language: eng
youtube: <iframe width="560" height="315" src="https://www.youtube.com/embed/9Y0KXTPXL14" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
---
