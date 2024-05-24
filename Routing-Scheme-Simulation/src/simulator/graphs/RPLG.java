package simulator.graphs;

import org.jgrapht.Graph;
import org.jgrapht.graph.DefaultEdge;
import simulator.ComputerNode;

public interface RPLG {
     Graph<ComputerNode, DefaultEdge> getGraph();

     double getTau();

     int getNodeNum();
}
