package simulator.graphs;

import java.util.ArrayList;
import org.jgrapht.Graph;
import org.jgrapht.graph.DefaultEdge;
import simulator.ComputerNode;

/**
 * A simple graph with pre-defined core.
 * This class lets the user define a graph, and the core, in
 * a hardcoded fashion.
 */
public class ManualGraphCore implements GraphWithCore {

  private int nodesNum;
  Graph<ComputerNode, DefaultEdge> graph;
  ArrayList<ComputerNode> core;

  /**
   * Constructor for a graph with core.

   * @param graph input graph
   * @param core subset of nodes
   */
  public ManualGraphCore(Graph<ComputerNode, DefaultEdge> graph, ArrayList<ComputerNode> core) {
    this.core = core;
    this.graph = graph;
    this.nodesNum = graph.vertexSet().size();
  }

  @Override
  public Graph<ComputerNode, DefaultEdge> getGraph() {
    return graph;
  }

  @Override
  public ArrayList<ComputerNode> getCore() {
    return core;
  }

  @Override
  public int getNodeNum() {
    return nodesNum;
  }
}
