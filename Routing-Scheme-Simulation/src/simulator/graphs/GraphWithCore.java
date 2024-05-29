package simulator.graphs;

import java.util.ArrayList;
import org.jgrapht.Graph;
import org.jgrapht.graph.DefaultEdge;
import simulator.ComputerNode;

/**
 * Interface for a graph with a pre-defined 'core'.
 * The core is a subset of nodes, which ideally is small and close to all other nodes.
 */
public interface GraphWithCore {
  Graph<ComputerNode, DefaultEdge> getGraph();

  ArrayList<ComputerNode> getCore();

  int getNodeNum();
}
