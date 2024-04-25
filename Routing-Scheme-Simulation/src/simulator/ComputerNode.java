package simulator;

import java.util.ArrayList;
import java.util.HashMap;
import org.jgrapht.graph.DefaultEdge;

/**
 * Additional data each node saves in the graph.
 */
public class ComputerNode {
  private final int NodeIndex;
  private Address address;
  private final ArrayList<ComputerNode> ports;
  private final HashMap<ComputerNode, Integer> tbl;

  /**
   * Empty Computer Node constructor.
   * Initiating a node with only its index number in the graph,
   * all other parameters are empty.
   * @param index a number representing the order degree in the nodes creation process of a graph
   */
  public ComputerNode(int index){
    NodeIndex = index;
    ports = new ArrayList<>();
    tbl = new HashMap<>();
  }
}
