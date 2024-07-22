# Compact Routing Scheme Experiment
This project was conducted by Ohad Heines and Ori Katzir, under the supervision of Liam Roditty and Ariel Sapir.

In the realm of network communications, efficient routing and distance querying are fundamental challenges. In the article "A Compact Routing Scheme and Approximate Distance Oracle for Power-Law Graphs" by Wei Chen, Christian Sommer, Shang-Hua Teng, and Yajun Wang [2012] an innovative approach is presented to address these challenges for power-law graphs. We implemented their algorithm, and recreated their experiments on randomly generated graphs.

## Installtion
This repository with all of its data can be cloned with:
```
$ git clone 'https://github.com/bananabajanana/Routing-Scheme.git'
```

## Directory Overview
This directory contains all of the code, the data, and the results of our experiments. The file tree should look as follows:
```
ROUTING-SCHEME
|   Original-Paper.pdf
|   Report.pdf
|   Data.zip
|
└───Results
|   |   Data-Graphs.xlsx
|   |
|   └───Graph-Images
|       |   ...
|
└───Simulator-src
|   |   ...
|
└───Graph-Generator-src
|   |   ...
```
We'll now go into each of those in detail
<details><summary>Original-Paper.pdf</summary>
The paper by Wei Chen, Christian Sommer, Shang-Hua Teng, and Yajun Wang [2012], of which we tried to recreate their experiements.
</details>
<details><summary>Report.pdf</summary>
This report contains an extended summary of the previous paper (sections 1-3), and goes how we programmed the experiment and our results (section 4). <b>It is highly recommended to read this pdf for a better understanding of this repository</b>
</details>
<details><summary>Data.zip</summary>

This zip-ed folder contains all of the graphs we tested on in our experiments. It is zipped since it takes a lot of space, but we added it for completness sake. Once unzipped each file in it is of the form `N_[size]_beta_[tau]_[i].xml` where
* size - the amount of nodes in the graph
* tau - the tau parameter of the power-law distribution
* i - an identifier for the graph (they are numbered from 0 to 30)

So for example, the fifth graph with 10,000 nodes, that was generated with tau=2.5 would have a file with the name `N_10000_beta_2.5_4.xml`
</details>
<details><summary>Results</summary>

The results folder contains all of the information that came out of our experiments. All of the graphs we generated are sorted in the `Graph-Images` subfolder. In addition, all of the raw data and an interactive environment for the graph viewing is available in the excel sheet provided `Data-Graphs.xlsx`.

The excel file contains 5 sheets - Viewer, ThresholdGraphs, Data, ThresholdData, Viewer-Helper. Each one of those contain as follows:
* Viewer - An interactive environment where you can set parameters N and Tau, and pick which data-sets to plot, and then you get interactive plots with those parameters.
* ThresholdGraphs - This sheet contains all of the graphs we used to reach our proposed threshold function. For further details read Report.pdf
* Data - This sheet contains all of the raw data for the expected value and variance of tbl sizes and distance stretches across all of our experiments.
ThresholdData - This sheet contains all of the raw data from the calculations we did to find the optimal threshold for each graph
* Viewer-Helper - This sheet **should not be touched!** It only contains no real data, and only enables the dynamic functionality of the Viewer.
</details>
<details><summary>Simulator-src</summary>

This is a folder containing all of our source code for the experiments. We programmed all of our code in Java, using jgrapht, and recommend opening the folder using intellij, as we left the idea meta-data for easier access. In-case you want to run your own experiments, there are many helpful functions in `utils.java` that should make this code more accessible, so you should check it out. An example is provided in the main function on how to run an experiment.

In further detail, our code relies on an internal library we made called `simulator`, which creates the environment at which the simulation runs (i.e. this library manages ports, messages, and nodes). The `RoutingProcedure.java` class recieves an environment that is created by `simulator/RoutingGraphBuilder.java` and simulates the message forwarding procedure while collecting data on the way. Finally, the `Utils.java` has many shorcuts and utils which should help whith standard experiments (for example, running the experiment on multiple files with the same parameters).
</details>
<details><summary>Graph-Generator-src</summary>

For completeness sake, this folder contains a copy of the open-source Brady and Cowen power-law graph generator [2006]. This code is written in cpp, and after running it all output files should be immediately ready for the helper functions in our `Utils.java` class mentioned earlier.

## Authors

* [Ohad Heines](https://github.com/bananabajanana)
* [Ori Katzir](https://github.com/oriKatzir)