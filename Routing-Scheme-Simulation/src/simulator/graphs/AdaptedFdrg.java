package simulator.graphs;

import java.util.ArrayList;
import java.util.Random;

import org.jgrapht.Graph;
import org.jgrapht.graph.DefaultEdge;
import org.jgrapht.graph.SimpleGraph;
import simulator.ComputerNode;

/**
 * AdaptedFDRG - Random Power Log Graph.
 * This is an implementation of definition 3.2 in the article.
 * this graph model is based on the Fixed Degree Random Graph model in definition 3.1.
 * n = num of vertexes in the graph.
 */
public class AdaptedFdrg implements GraphWithCore {
    private final int nodeNum;
    private static final double EPSILON = 1E-12; // = 0.000000000001
    private double gamaPrime;
    private final double tau;
    ArrayList<Double> expectedDegreeSequence;
    private double normFactor;   // normalization factor
    private final Random random;
    private Graph<ComputerNode, DefaultEdge> graph;

    public void temporaryPrintThreshold() {
        System.out.println("Threshold: " + Math.pow(nodeNum, gamaPrime) * 2);
    }

    /**
     * Generates a random power-law graph using the adapted FDRG model specified in the paper.
     *
     * @param numOfNodesInGraph N
     * @param tau               power-law exponent
     */
    public AdaptedFdrg(int numOfNodesInGraph, double tau) {
        nodeNum = numOfNodesInGraph;
        this.tau = tau;
        random = new Random();
        initExpectedDegreeSequenceAndNormFactor();
        generateGraph();
    }
    // Todo - add nudes number threshold

    private void computeGamaAndGamaPrime() {
        double gama = ((tau - 2) / ((2 * tau) - 3)) + EPSILON;
        gamaPrime = (1 - gama) / (tau - 1);
    }

    /**
     * Building the graphs core by iterating on all the graphs vertexes and looking
     * for vertexes that have a degree higher than the core degree threshold.
     * Every vertex that have a degree higher than the threshold will be inserted to the core.
     * <p>
     * The degree threshold is calculated using the gama prim constant.
     */
    public ArrayList<ComputerNode> getCore() {
        ArrayList<ComputerNode> core = new ArrayList<>();
        computeGamaAndGamaPrime();
        double coreDegreeThreshold = Math.pow(nodeNum, gamaPrime) / 4;
        for (ComputerNode node : graph.vertexSet()) {
            if (node.getDegree() > coreDegreeThreshold) {
                core.add(node);
            }
        }
        return core;
    }

    /**
     * Generates a random power-law graph using the adapted FDRG model specified in the paper.
     *
     * @param numOfNodesInGraph N
     * @param tau               power-law exponent
     * @param seed              seed for the random generation
     */
    public AdaptedFdrg(int numOfNodesInGraph, double tau, long seed) {
        nodeNum = numOfNodesInGraph;
        this.tau = tau;
        random = new Random(seed);
        initExpectedDegreeSequenceAndNormFactor();
        generateGraph();
    }

    private double getExpectedDegree(int nodeIndex) {
        return Math.pow(nodeNum / (double) nodeIndex, 1.0 / (tau - 1));
    }

    private void initExpectedDegreeSequenceAndNormFactor() {
        expectedDegreeSequence = new ArrayList<>();
        normFactor = 0.0;
        for (int i = 1; i <= nodeNum; ++i) {
            double expectedDegree = getExpectedDegree(i);
            expectedDegreeSequence.add(expectedDegree);
            normFactor += expectedDegree;
        }
        normFactor = 1.0 / normFactor;
    }

    private void makeGraphConnected() {
        Object[] nodes = this.graph.vertexSet().toArray();
        for (int i = 0; i < nodeNum - 1; ++i) {
            if (!graph.containsEdge((ComputerNode) nodes[i], (ComputerNode) nodes[i + 1])) {
                graph.addEdge((ComputerNode) nodes[i], (ComputerNode) nodes[i + 1]);
            }
        }
    }

    private void generateGraph() {
        graph = new SimpleGraph<>(DefaultEdge.class);
        ArrayList<ComputerNode> nodes = new ArrayList<>();
        for (int i = 0; i < nodeNum; ++i) {
            nodes.add(new ComputerNode(i));
            graph.addVertex(nodes.get(i));
        }
        for (int i = 0; i < nodeNum - 1; ++i) {
            for (int j = i + 1; j < nodeNum; ++j) {
                double probability = Math.min(expectedDegreeSequence.get(i) * expectedDegreeSequence.get(j)
                        * normFactor, 1.0);
                if (random.nextDouble() < probability
                        && !graph.containsEdge(nodes.get(i), nodes.get(j))) {    // should we keep this check?
                    graph.addEdge(nodes.get(i), nodes.get(j));
                }
            }
        }
    }

    //region <Getters>
    public Graph<ComputerNode, DefaultEdge> getGraph() {
        return graph;
    }

    public int getNodeNum() {
        return nodeNum;
    }

    public double getTau() {
        return tau;
    }

    public ArrayList<Double> getExpectedDegreeSequence() {
        return expectedDegreeSequence;
    }

    public Random getRandom() {
        return random;
    }

    //endregion
}
