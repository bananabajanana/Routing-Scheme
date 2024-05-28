package simulator.graphs;

import java.util.Random;
import org.jgrapht.Graph;
import org.jgrapht.graph.DefaultEdge;
import org.jgrapht.graph.SimpleGraph;
import simulator.ComputerNode;
import simulator.RoutingGraphBuilder;

import java.util.ArrayList;
/**
 * AdaptedFDRG - Random Power Log Graph.
 * This is an implementation of definition 3.2 in the article.
 * this graph model is based on the Fixed Degree Random Graph model in definition 3.1.
 *  n = num of vertexes in the graph.
 */
public class AdaptedFDRG implements RPLG {
    private final int nodeNum;
    private final double tau;
    ArrayList<Double> expectedDegreeSequence;
    private double p;   // normalization factor
    private final Random random;
    private Graph<ComputerNode, DefaultEdge> graph;

    public AdaptedFDRG(int numOfNodesInGraph, double tau){
        nodeNum = numOfNodesInGraph;
        this.tau = tau;
        random = new Random();
        initExpectedDegreeSequenceAndNormFactor();
        generateGraph();
    }
    // Todo - add nudes number threshold

    public AdaptedFDRG(int numOfNodesInGraph, double tau, long seed){
        nodeNum = numOfNodesInGraph;
        this.tau = tau;
        random = new Random(seed);
        initExpectedDegreeSequenceAndNormFactor();
        generateGraph();
    }

    private double getExpectedDegree(int nodeIndex) {
        return Math.pow( nodeNum / (double) nodeIndex, 1.0 / (tau - 1));
    }

    private void initExpectedDegreeSequenceAndNormFactor(){
        expectedDegreeSequence = new ArrayList<>();
        p = 0.0;
        for(int i = 1; i <= nodeNum; ++i){
            double expectedDegree = getExpectedDegree(i);
            expectedDegreeSequence.add(expectedDegree);
            p += expectedDegree;
        }
        p = 1.0 / p;
    }

    private void generateGraph(){
        graph = new SimpleGraph<>(DefaultEdge.class);
        ArrayList<ComputerNode> nodes = new ArrayList<>();
        for (int i = 0; i < nodeNum; ++i){
            nodes.add(new ComputerNode(i));
            graph.addVertex(nodes.get(i));
        }
        for (int i = 0; i < nodeNum - 1; ++i){
            for (int j = i + 1; j < nodeNum; ++j){
                double probability = Math.min(expectedDegreeSequence.get(i) * expectedDegreeSequence.get(j) * p, 1.0);
                if (random.nextDouble() < probability
                    && !graph.containsEdge(nodes.get(i), nodes.get(j))){    // should we keep this check?
                    graph.addEdge(nodes.get(i), nodes.get(j));
                    nodes.get(i).connectPort(nodes.get(j));
                    nodes.get(j).connectPort(nodes.get(i));
                }
            }
        }
    }

    //region <Getters>
    public Graph<ComputerNode, DefaultEdge> getGraph(){
        return graph;
    }

    public int getNodeNum() { return nodeNum; }

    public double getTau() {
        return tau;
    }

    public ArrayList<Double> getExpectedDegreeSequence() {
        return expectedDegreeSequence;
    }

    public Random getRandom() { return random; }

    //endregion

    public static void main(String[] args) {
        // Creating a simple undirected graph
        Graph<String, DefaultEdge> graph = new SimpleGraph<>(DefaultEdge.class);

        // Adding vertices
        graph.addVertex("A");
        graph.addVertex("B");
        graph.addVertex("C");
        graph.addVertex("D");


        // Adding edges
        graph.addEdge("A", "B");
        graph.addEdge("A", "C");
        graph.addEdge("B", "D");

        AdaptedFDRG g = new AdaptedFDRG(10000, 2.3);

        RoutingGraphBuilder builder = new RoutingGraphBuilder(g);

        builder.process();

    }
}
