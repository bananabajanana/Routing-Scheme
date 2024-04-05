package simulator;

import java.util.HashMap;
import org.jgrapht.graph.DefaultEdge;

/**
 * Additional data each node saves in the graph.
 */
public class ComputerNode {
  private Address address;
  private DefaultEdge[] ports;
  private HashMap<ComputerNode, Integer> tbl;
}
